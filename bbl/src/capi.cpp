#include "capi.hpp"
#include "bbl_detail.h"
#include "bblfmt.hpp"
#include "spdlog/spdlog.h"

#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>

namespace bbl {

static auto replace_namespace_prefix(std::string const& spelling,
                                     std::string const& mod_name)
    -> std::string {
    std::string result = fmt::format("{}_{}", mod_name, spelling);
    // XXX: sanitize any bad characters
    return result;
}

static auto is_void(C_QType const& qt) -> bool {
    if (std::holds_alternative<C_Type>(qt.type)) {
        C_Type const& c_type = std::get<C_Type>(qt.type);
        if (std::holds_alternative<bbl_builtin_t>(c_type.kind)) {
            bbl_builtin_t builtin = std::get<bbl_builtin_t>(c_type.kind);
            if (builtin == BBL_BUILTIN_Void) {
                return true;
            }
        }
    }

    return false;
}

auto is_stdfunction(QType const& qt) -> bool {
    if (std::holds_alternative<Type>(qt.type)) {
        Type const& type = std::get<Type>(qt.type);
        if (std::holds_alternative<StdFunctionId>(type.kind)) {
            return true;
        }
    }

    return false;
}

auto get_stdfunction_id(QType const& qt) -> std::optional<std::string> {
    if (std::holds_alternative<Type>(qt.type)) {
        Type const& type = std::get<Type>(qt.type);
        if (std::holds_alternative<StdFunctionId>(type.kind)) {
            return std::get<StdFunctionId>(type.kind).id;
        } else {
            return {};
        }
    } else if (std::holds_alternative<LValueReference>(qt.type)) {
        auto const& lvref = std::get<LValueReference>(qt.type);
        return get_stdfunction_id(*lvref.pointee);
    } else {
        return {};
    }
}

auto is_stdfunction(C_QType const& qt) -> bool {
    if (std::holds_alternative<C_Type>(qt.type)) {
        auto const& type = std::get<C_Type>(qt.type);
        if (std::holds_alternative<C_StdFunctionId>(type.kind)) {
            return true;
        }
    }

    return false;
}

// Return the builtin that this QType holds, if it does. Will throw otherwise
static auto get_builtin(QType const& qt) -> bbl_builtin_t {
    Type const& type = std::get<Type>(qt.type);
    return std::get<bbl_builtin_t>(type.kind);
}

C_API::C_API(Context const& cpp_ctx) : _cpp_ctx(cpp_ctx) {
    ankerl::unordered_dense::set<std::string> seen_includes;

    for (auto const& [cpp_id, cpp_mod] : _cpp_ctx.modules()) {
        std::vector<std::string> mod_structs;
        std::vector<std::string> mod_functions;
        std::vector<std::string> mod_stdfunctions;
        std::vector<std::string> mod_enums;

        // Translate all enums
        for (auto const& cpp_enum_id : cpp_mod.enums) {
            auto const* cpp_enum = _cpp_ctx.get_enum(cpp_enum_id);
            std::string enum_namespace = replace_namespace_prefix(
                cpp_enum->rename.empty() ? cpp_enum->name : cpp_enum->rename,
                cpp_mod.name);

            std::string enum_name = fmt::format("{}", enum_namespace);
            bbl_builtin_t integer_type = get_builtin(cpp_enum->integer_type);

            std::string variant_prefix;
            if (cpp_enum->prefix.has_value()) {
                std::string const& prefix = cpp_enum->prefix.value();
                if (prefix.empty()) {
                    variant_prefix = "";
                } else {
                    variant_prefix = fmt::format("{}_", prefix);
                }
            } else {
                variant_prefix = fmt::format("{}_", enum_name);
            }

            std::vector<EnumVariant> c_variants;
            for (auto const& var : cpp_enum->variants) {
                c_variants.emplace_back(
                    fmt::format("{}{}", variant_prefix, var.first), var.second);
            }

            _enums.emplace(cpp_enum_id,
                           C_Enum{
                               enum_name,
                               cpp_enum->comment,
                               std::move(c_variants),
                               integer_type,
                           });
            mod_enums.push_back(cpp_enum_id);
        }

        // translate all classes
        for (auto const& cpp_class_id : cpp_mod.classes) {
            auto const* cpp_cls = _cpp_ctx.get_class(cpp_class_id);
            assert(cpp_cls);

            // Construct the struct name from the name of the module, and the
            // spelling or rename string
            std::string struct_namespace = replace_namespace_prefix(
                cpp_cls->rename.empty() ? cpp_cls->name : cpp_cls->rename,
                cpp_mod.name);

            std::string struct_name = fmt::format("{}_t", struct_namespace);

            // Translate all fields
            std::vector<C_Field> fields;
            try {
                for (auto const& field : cpp_cls->fields) {
                    fields.emplace_back(C_Field{
                        _translate_qtype(field.type),
                        field.name,
                        field.comment,
                    });
                }
            } catch (MissingTypeBindingException& e) {
                SPDLOG_ERROR("could not translate a field of {}. Class will be "
                             "ignored.\n{}",
                             cpp_cls->spelling,
                             e.what());
                continue;
            }

            // Push the struct into the main list and the module list
            _structs.emplace(
                cpp_class_id,
                C_Struct{*cpp_cls, struct_name, cpp_cls->comment, fields});
            mod_structs.push_back(cpp_class_id);
        }

        // Translate stdfunctions
        for (auto const& cpp_fun_id : cpp_mod.stdfunctions) {
            auto const* cpp_fun = _cpp_ctx.get_stdfunction(cpp_fun_id);

            try {
                C_StdFunction c_fun =
                    _translate_stdfunction(cpp_fun, cpp_mod.name);
                _stdfunctions.emplace(cpp_fun_id, std::move(c_fun));
                mod_stdfunctions.push_back(cpp_fun_id);
            } catch (MissingTypeBindingException& e) {
                SPDLOG_ERROR("could not translate stdfunction {}. "
                             "Function will be ignored.\n{}",
                             cpp_fun->spelling,
                             e.what());
            }
        }

        // now translate class methods (after stdfunctions)
        for (auto const& cpp_class_id : cpp_mod.classes) {
            auto const* cpp_cls = _cpp_ctx.get_class(cpp_class_id);
            assert(cpp_cls);

            // Construct the struct name from the name of the module, and the
            // spelling or rename string
            std::string struct_namespace = replace_namespace_prefix(
                cpp_cls->rename.empty() ? cpp_cls->name : cpp_cls->rename,
                cpp_mod.name);

            std::string struct_name = fmt::format("{}_t", struct_namespace);

            // Translate all methods to functions
            for (auto const& method_id : cpp_cls->methods) {
                Method const* method = _cpp_ctx.get_method(method_id);
                assert(method);

                try {
                    C_Function c_fun = _translate_method(
                        method, struct_namespace, cpp_class_id);

                    // we can have multiple instances of the "same" method
                    // through inheritance, so make a new function id including
                    // the class id
                    std::string function_id =
                        fmt::format("{}/{}", method_id, cpp_class_id);

                    _functions.emplace(function_id, std::move(c_fun));
                    mod_functions.push_back(function_id);
                } catch (MissingTypeBindingException& e) {
                    SPDLOG_ERROR("could not translate method {} of class {}. "
                                 "Method will be ignored.\n{}",
                                 method->function.name,
                                 cpp_cls->spelling,
                                 e.what());
                }
            }

            // Translate constructors
            for (auto const& ctor_id : cpp_cls->constructors) {
                Constructor const* ctor = _cpp_ctx.get_constructor(ctor_id);
                assert(ctor);

                try {
                    C_Function c_fun = _translate_constructor(
                        ctor, struct_namespace, cpp_class_id);
                    _functions.emplace(ctor_id, std::move(c_fun));
                    mod_functions.push_back(ctor_id);
                } catch (MissingTypeBindingException& e) {
                    SPDLOG_ERROR("could not translate constructor of class {}. "
                                 "Constructor will be ignored.\n{}",
                                 cpp_cls->spelling,
                                 e.what());
                }
            }

            // If the class is opaque ptr then add a dtor function
            // XXX: what do we do about other kinds? if it's safe to bind a type
            // as opaque bytes or value type then it should be safe to just let
            // them fall off the stack too...
            if (cpp_cls->bind_kind == BindKind::OpaquePtr &&
                cpp_cls->rule_of_seven.is_destructible) {
                C_Function c_dtor =
                    _generate_destructor(struct_namespace, cpp_class_id);
                std::string dtor_id = cpp_class_id + "_dtor";
                _functions.emplace(dtor_id, std::move(c_dtor));
                mod_functions.push_back(dtor_id);
            }

            // If this class is a smart pointer to another class, then add the
            // pointee's non-static methods to it
            if (cpp_cls->smartptr_to.has_value()) {
                std::string pointee_class_id = *cpp_cls->smartptr_to;
                std::string const& smartptr_class_id = cpp_class_id;
                Class const* pointee_cls = cpp_ctx.get_class(pointee_class_id);
                assert(pointee_cls);

                for (auto const& method_id : pointee_cls->methods) {
                    Method const* method = _cpp_ctx.get_method(method_id);
                    assert(method);

                    if (method->is_static) {
                        continue;
                    }

                    if (cpp_cls->smartptr_is_const && !method->is_const) {
                        continue;
                    }

                    try {
                        C_Function c_fun = _translate_method(
                            method, struct_namespace, smartptr_class_id);

                        // we can have multiple instances of the "same" method
                        // through inheritance, so make a new function id
                        // including the class id
                        std::string function_id =
                            fmt::format("{}/{}", method_id, smartptr_class_id);

                        _functions.emplace(function_id, std::move(c_fun));
                        mod_functions.push_back(function_id);
                    } catch (MissingTypeBindingException& e) {
                        SPDLOG_ERROR(
                            "could not translate method {} of class {}. "
                            "Method will be ignored.\n{}",
                            method->function.name,
                            pointee_cls->spelling,
                            e.what());
                    } catch (std::runtime_error& e) {
                        BBL_RETHROW(e,
                                    "could not translate method {} of smartptr "
                                    "{} to {}",
                                    method->function.qualified_name,
                                    smartptr_class_id,
                                    pointee_class_id);
                    }
                }
            }
        }

        // Translate functions
        for (auto const& cpp_fun_id : cpp_mod.functions) {
            auto const* cpp_fun = _cpp_ctx.get_function(cpp_fun_id);

            try {
                C_Function c_fun = _translate_function(cpp_fun, cpp_mod.name);
                _functions.emplace(cpp_fun_id, std::move(c_fun));
                mod_functions.push_back(cpp_fun_id);

            } catch (MissingTypeBindingException& e) {
                SPDLOG_ERROR("could not translate function {}. "
                             "Function will be ignored.\n{}",
                             cpp_fun->spelling,
                             e.what());
            }
        }

        std::vector<Inclusion> mod_inclusions;
        for (auto const& source_filename : cpp_mod.source_files) {
            SourceFile const* source_file =
                _cpp_ctx.get_source_file(source_filename);
            assert(source_file);

            // add the source inclusions to the main list
            for (Inclusion const& inclusion : source_file->inclusions) {
                if (!seen_includes.contains(inclusion.filename)) {
                    _inclusions.push_back(inclusion);
                    mod_inclusions.push_back(inclusion);
                    seen_includes.insert(inclusion.filename);
                }
            }
        }

        _modules.emplace_back(C_Module{cpp_id,
                                       cpp_mod.name,
                                       mod_inclusions,
                                       std::move(mod_structs),
                                       std::move(mod_functions),
                                       std::move(mod_stdfunctions),
                                       std::move(mod_enums),
                                       cpp_mod.function_impls});
    }
}

auto C_API::_get_c_qtype_as_string(C_QType const& qt,
                                   std::string const& name) const
    -> std::string {
    char const* s_const = qt.is_const ? " const" : "";

    std::string name_with_space = name.empty() ? "" : fmt::format(" {}", name);

    if (std::holds_alternative<C_Type>(qt.type)) {
        C_Type const& c_type = std::get<C_Type>(qt.type);

        if (std::holds_alternative<bbl_builtin_t>(c_type.kind)) {
            bbl_builtin_t builtin = std::get<bbl_builtin_t>(c_type.kind);
            return fmt::format("{}{}{}", builtin, s_const, name_with_space);
        } else if (std::holds_alternative<C_StructId>(c_type.kind)) {
            C_StructId struct_id = std::get<C_StructId>(c_type.kind);
            C_Struct const& c_struct = _structs.at(struct_id.id);
            return fmt::format(
                "{}{}{}", c_struct.name, s_const, name_with_space);
        } else if (std::holds_alternative<C_EnumId>(c_type.kind)) {
            C_EnumId enum_id = std::get<C_EnumId>(c_type.kind);
            C_Enum const& c_enum = _enums.at(enum_id.id);
            return fmt::format("{}{}", c_enum.integer_type, name_with_space);
        } else if (std::holds_alternative<C_StdFunctionId>(c_type.kind)) {
            C_StdFunctionId fun_id = std::get<C_StdFunctionId>(c_type.kind);
            C_StdFunction const& fun = _stdfunctions.at(fun_id.id);
            return _get_stdfunction_type_as_string(fun, name);
        } else {
            BBL_THROW("unhandled variant");
        }

    } else if (std::holds_alternative<C_Pointer>(qt.type)) {
        C_Pointer const& c_pointer = std::get<C_Pointer>(qt.type);

        if (std::holds_alternative<C_Array>(c_pointer.pointee->type)) {
            // If it's a pointer to an array, we need to do handle the paren
            // type specially
            C_Array const& c_array = std::get<C_Array>(c_pointer.pointee->type);
            return fmt::format(
                "{} (*{})[{}]",
                _get_c_qtype_as_string(*c_array.element_type, ""),
                name,
                c_array.size);

        } else {
            return fmt::format("{}*{}{}",
                               _get_c_qtype_as_string(*c_pointer.pointee, ""),
                               s_const,
                               name_with_space);
        }

    } else if (std::holds_alternative<C_Array>(qt.type)) {
        C_Array const& c_array = std::get<C_Array>(qt.type);
        return fmt::format("{} {}[{}]{}",
                           _get_c_qtype_as_string(*c_array.element_type, ""),
                           name,
                           c_array.size,
                           s_const);
    } else {
        BBL_THROW("unhandled variant");
    }
}

auto C_API::_get_stdfunction_type_as_string(C_StdFunction const& fun,
                                            std::string const& name) const
    -> std::string {
    std::string result = fmt::format("void (*{})(", name);
    bool first = true;

    for (auto const& param : fun.params) {
        if (first) {
            first = false;
        } else {
            result = fmt::format("{}, ", result);
        }

        result = fmt::format(
            "{}{}", result, _get_c_qtype_as_string(param.type, param.name));
    }

    if (fun.return_type.has_value()) {
        if (!first) {
            result = fmt::format("{}, ", result);
        }

        result = fmt::format("{}{}",
                             result,
                             _get_c_qtype_as_string(fun.return_type->type,
                                                    fun.return_type->name));
    }

    return fmt::format("{})", result);
}

// Returns true if the given QType is an l-value reference
static bool is_lvalue_reference(QType const& qt) {
    return std::holds_alternative<LValueReference>(qt.type);
}

// Returns true if the given QType is an r-value reference
static bool is_rvalue_reference(QType const& qt) {
    return std::holds_alternative<RValueReference>(qt.type);
}

// Returns true if the given QType is a pointer
static bool is_pointer(QType const& qt) {
    if (std::holds_alternative<Pointer>(qt.type)) {
        return true;
    } else {
        return false;
    }
}

// Returns true if the given C_QType is a pointer
static bool is_pointer(C_QType const& qt) {
    if (std::holds_alternative<C_Pointer>(qt.type)) {
        return true;
    } else {
        return false;
    }
}

// Returns true if the given C_QType directly holds an enum
static bool is_enum(C_QType const& qt) {
    if (std::holds_alternative<C_Type>(qt.type)) {
        C_Type type = std::get<C_Type>(qt.type);
        if (std::holds_alternative<C_EnumId>(type.kind)) {
            return true;
        }
    }

    return false;
}

// Returns true if the given QType directly holds an enum
static bool is_enum(QType const& qt) {
    if (std::holds_alternative<Type>(qt.type)) {
        Type type = std::get<Type>(qt.type);
        if (std::holds_alternative<EnumId>(type.kind)) {
            return true;
        }
    }

    return false;
}

// Returns true if the given QTYpe directly holds a builtin
static bool is_builtin(QType const& qt) {
    if (std::holds_alternative<Type>(qt.type)) {
        Type const& type = std::get<Type>(qt.type);
        if (std::holds_alternative<bbl_builtin_t>(type.kind)) {
            return true;
        }
    }

    return false;
}

static bool is_lvalue_reference_to_enum(QType const& qt) {
    if (std::holds_alternative<LValueReference>(qt.type)) {
        LValueReference const& ref = std::get<LValueReference>(qt.type);
        return is_enum(*ref.pointee);
    }

    return false;
}

static std::optional<EnumId> as_enum(QType const& qt) {
    if (std::holds_alternative<Type>(qt.type)) {
        Type type = std::get<Type>(qt.type);
        if (std::holds_alternative<EnumId>(type.kind)) {
            return std::get<EnumId>(type.kind);
        }
    }

    return {};
}

static std::optional<EnumId> as_lvalue_reference_to_enum(QType const& qt) {
    if (std::holds_alternative<LValueReference>(qt.type)) {
        LValueReference const& ref = std::get<LValueReference>(qt.type);
        return as_enum(*ref.pointee);
    }

    return {};
}

static bool is_pointer_to_enum(QType const& qt) {
    if (std::holds_alternative<Pointer>(qt.type)) {
        Pointer const& ptr = std::get<Pointer>(qt.type);
        return is_enum(*ptr.pointee);
    }

    return false;
}

static std::optional<EnumId> as_pointer_to_enum(QType const& qt) {
    if (std::holds_alternative<Pointer>(qt.type)) {
        Pointer const& ptr = std::get<Pointer>(qt.type);
        return as_enum(*ptr.pointee);
    }

    return {};
}

// Returns true if the given QType is not a pointer or reference type
static bool is_by_value(QType const& qt) {
    return std::holds_alternative<Type>(qt.type);
}

static bool is_opaqueptr(QType const& qt, Context const& ctx) {
    if (std::holds_alternative<Type>(qt.type)) {
        Type const& type = std::get<Type>(qt.type);
        if (std::holds_alternative<ClassId>(type.kind)) {
            ClassId const& id = std::get<ClassId>(type.kind);

            Class const* cls = ctx.get_class(id.id);
            if (cls == nullptr) {
                BBL_THROW("class {} is not bound", id.id);
            }

            if (cls->bind_kind == BindKind::OpaquePtr) {
                return true;
            }
        } else if (std::holds_alternative<ClassTemplateSpecializationId>(
                       type.kind)) {
            ClassTemplateSpecializationId const& id =
                std::get<ClassTemplateSpecializationId>(type.kind);

            Class const* cls = ctx.get_class(id.id);
            if (cls == nullptr) {
                BBL_THROW("class {} is not bound", id.id);
            }

            if (cls->bind_kind == BindKind::OpaquePtr) {
                return true;
            }
        }
    }

    return false;
}

static bool is_reference_to_bytes_or_value(QType const& qt,
                                           Context const& ctx) {
    if (std::holds_alternative<LValueReference>(qt.type)) {
        LValueReference const& ref = std::get<LValueReference>(qt.type);
        return !is_opaqueptr(*ref.pointee, ctx);
    }

    return false;
}

static bool is_opaqueptr_lvalue_reference(QType const& qt, Context const& ctx) {
    if (std::holds_alternative<LValueReference>(qt.type)) {
        LValueReference const& ref = std::get<LValueReference>(qt.type);
        return is_opaqueptr(*ref.pointee, ctx);
    }

    return false;
}

// Returns true is the given QType directly holds a Class with bind kind of
// opaque ptr
static bool is_opaque_ptr_by_value(QType const& qt, Context const& ctx) {
    if (std::holds_alternative<Type>(qt.type)) {
        Type const& type = std::get<Type>(qt.type);
        if (std::holds_alternative<ClassId>(type.kind)) {
            ClassId const& id = std::get<ClassId>(type.kind);
            Class const* cls = ctx.get_class(id.id);
            if (cls == nullptr) {
                BBL_THROW("class {} is not bound", id.id);
            }
            return cls->bind_kind == BindKind::OpaquePtr;
        } else if (std::holds_alternative<ClassTemplateSpecializationId>(
                       type.kind)) {
            ClassTemplateSpecializationId const& id =
                std::get<ClassTemplateSpecializationId>(type.kind);
            Class const* cls = ctx.get_class(id.id);
            if (cls == nullptr) {
                BBL_THROW("class {} is not bound", id.id);
            }
            return cls->bind_kind == BindKind::OpaquePtr;
        }
    }

    return false;
}

static void remove_const_on_pointee(C_QType& qt) {
    if (std::holds_alternative<C_Pointer>(qt.type)) {
        C_Pointer& pointer = std::get<C_Pointer>(qt.type);
        pointer.pointee->is_const = false;
    }
}

auto wrap_in_pointer(C_QType&& qt) -> C_QType {
    return C_QType{
        nullptr, false, C_Pointer{std::make_shared<C_QType>(std::move(qt))}};
}

auto C_API::_translate_return_type(QType const& cpp_return_type)
    -> std::optional<C_Param> {
    // For return values:
    //
    // + opaque ptr
    //   o if the result is by value
    //     - wrap the param in two pointers, and construct into it
    //       *result = new Foo(call())
    //   o if the result is by pointer
    //     - wrap the param in another pointer, and assign the pointer into it
    //       *result = call()
    //   o if the result is by reference
    //     - wrap the param in a pointer, and assign the addres of the result
    //     into it
    //       *result = &call()
    //
    // + opaque bytes/value type
    //   o if the result is by value
    //     - wrap the param in a pointer, assign into it
    //       *result = call()
    //   o if the result is by pointer
    //     - wrap the param in another pointer and assign the pointer into it
    //       *result = call()
    //   o if the result is by reference
    //     - use the existing pointer from the reference conversion, and assign
    //     into that
    //       *result = call()
    std::optional<C_Param> result;

    C_QType return_type = _translate_qtype(cpp_return_type);
    std::string return_name = "_result";

    if (!is_void(return_type)) {
        if (is_reference_to_bytes_or_value(cpp_return_type, _cpp_ctx)) {
            remove_const_on_pointee(return_type);
            result = C_Param{
                std::move(return_type),
                std::move(return_name),
            };
        } else if (is_opaque_ptr_by_value(cpp_return_type, _cpp_ctx)) {
            result = C_Param{
                wrap_in_pointer(wrap_in_pointer(std::move(return_type))),
                std::move(return_name),
            };
        } else {
            result = C_Param{
                C_QType{nullptr,
                        false, C_Pointer{
                            std::make_shared<C_QType>(std::move(return_type))}},
                std::move(return_name),
            };
        }
    }

    return result;
}

auto C_API::_translate_method(Method const* method,
                              std::string const& function_prefix,
                              std::string const& class_id) -> C_Function {
    // Function name is the name of the struct followed by either the rename or
    // original method name
    std::string function_name =
        fmt::format("{}_{}",
                    function_prefix,
                    method->function.rename.empty() ? method->function.name
                                                    : method->function.rename);

    std::optional<C_Param> result =
        _translate_return_type(method->function.return_type);

    std::vector<C_Param> c_params;
    // if the method is not static, create _this param, which will be a (maybe
    // const) pointer to a struct of this class_id
    std::variant<IsStatic, C_Param, C_SmartPtr> receiver = IsStatic{};
    if (!method->is_static) {
        bool pointee_is_const = method->is_const;

        if (method->target_class_id == class_id) {
            // regular method call
            receiver = std::move(C_Param{
                C_QType{
                        nullptr, false,
                        C_Pointer{std::unique_ptr<C_QType>(
                        new C_QType{nullptr,
                                    pointee_is_const,
                                    C_Type{C_StructId{class_id}}})},
                        },
                "_this",
            });
        } else {
            // this method is being delegated from a smart pointer
            receiver = std::move(C_SmartPtr{
                C_Param{
                        C_QType{
                        nullptr,
                        false,
                        C_Pointer{std::unique_ptr<C_QType>(
                            new C_QType{nullptr,
                                        pointee_is_const,
                                        C_Type{C_StructId{class_id}}})},
                    }, "_this",
                        }
            });
        }
    }

    std::vector<ExprPtr> body;
    std::vector<ExprPtr> expr_params;

    _translate_parameter_list(
        method->function.params, c_params, expr_params, body);

    ExprPtr expr_call =
        ex_call(method->function.name + method->function.template_call,
                std::make_unique<ExprParameterList>(std::move(expr_params)));

    Class const* cls = _cpp_ctx.get_class(class_id);
    assert(cls);

    ExprPtr receiving_call;
    if (std::holds_alternative<C_Param>(receiver)) {
        auto const& param = std::get<C_Param>(receiver);
        receiving_call =
            ex_arrow(ExprToken::create(param.name), std::move(expr_call));
    } else if (std::holds_alternative<C_SmartPtr>(receiver)) {
        // we need to put an extra deref around the smart pointer to delegate to
        // the underlying class
        auto const& param = std::get<C_SmartPtr>(receiver);
        receiving_call =
            ex_arrow(ex_paren(ex_deref(ExprToken::create(param.smartptr.name))),
                     std::move(expr_call));
    } else {
        receiving_call = ex_namespace_ref(cls->spelling, std::move(expr_call));
    }

    if (result.has_value()) {
        // if the function returns a reference, we need to wrap the call in an
        // address operator in order to convert it back to a pointer
        if (is_opaqueptr_lvalue_reference(method->function.return_type,
                                          _cpp_ctx)) {
            receiving_call = ex_address(std::move(receiving_call));
        } else if (is_opaque_ptr_by_value(method->function.return_type,
                                          _cpp_ctx)) {
            // need to construct a new object on the heap to return
            std::vector<ExprPtr> params;
            params.emplace_back(std::move(receiving_call));

            std::unique_ptr<ExprParameterList> param_list =
                ex_parameter_list(std::move(params));

            receiving_call = ex_new(ex_call(
                _cpp_ctx.get_qtype_as_string(method->function.return_type),
                std::move(param_list)));
        } else if (is_enum(method->function.return_type)) {
            // enums must be explicitly static_cast to their underlying type on
            // the way back
            EnumId enum_id = std::get<EnumId>(
                std::get<Type>(method->function.return_type.type).kind);
            Enum const* enm = _cpp_ctx.get_enum(enum_id.id);
            if (enm == nullptr) {
                BBL_THROW("could not get enum {} while translating return type",
                          enum_id.id);
            }

            receiving_call =
                ex_static_cast(ExprToken::create(fmt::format(
                                   "{}", get_builtin(enm->integer_type))),
                               std::move(receiving_call));
        }

        body.emplace_back(
            ex_assign(ex_deref(ExprToken::create(result.value().name)),
                      std::move(receiving_call)));
    } else {
        body.emplace_back(std::move(receiving_call));
    }

    body.emplace_back(ex_return(ExprToken::create("0")));

    return C_Function{
        method,
        function_name,
        method->function.comment,
        std::move(result),
        std::move(receiver),
        std::move(c_params),
        std::move(body),
    };
}

auto C_API::_translate_function(Function const* function,
                                std::string const& function_prefix)
    -> C_Function {
    // Function name is the name of the struct followed by either the rename or
    // original method name
    std::string function_name =
        function->rename.empty() ? function->name : function->rename;

    if (!function_prefix.empty()) {
        function_name = fmt::format("{}_{}", function_prefix, function_name);
    }

    std::optional<C_Param> result =
        _translate_return_type(function->return_type);

    std::vector<C_Param> c_params;

    std::vector<ExprPtr> body;
    std::vector<ExprPtr> expr_params;

    _translate_parameter_list(function->params, c_params, expr_params, body);

    ExprPtr expr_call =
        ex_call(function->spelling,
                std::make_unique<ExprParameterList>(std::move(expr_params)));

    if (result.has_value()) {
        // if the function returns a reference, we need to wrap the call in an
        // address operator in order to convert it back to a pointer when
        // returning an opaqueptr
        if (is_opaqueptr_lvalue_reference(function->return_type, _cpp_ctx)) {
            expr_call = ex_address(std::move(expr_call));
        } else if (is_opaque_ptr_by_value(function->return_type, _cpp_ctx)) {
            // need to construct a new object on the heap to return
            std::vector<ExprPtr> params;
            params.emplace_back(std::move(expr_call));

            std::unique_ptr<ExprParameterList> param_list =
                ex_parameter_list(std::move(params));

            expr_call = ex_new(
                ex_call(_cpp_ctx.get_qtype_as_string(function->return_type),
                        std::move(param_list)));
        } else if (is_enum(function->return_type)) {
            // enums must be explicitly static_cast to their underlying type on
            // the way back
            EnumId enum_id = std::get<EnumId>(
                std::get<Type>(function->return_type.type).kind);
            Enum const* enm = _cpp_ctx.get_enum(enum_id.id);
            if (enm == nullptr) {
                BBL_THROW("could not get enum {} while translating return type",
                          enum_id.id);
            }

            expr_call =
                ex_static_cast(ExprToken::create(fmt::format(
                                   "{}", get_builtin(enm->integer_type))),
                               std::move(expr_call));
        }

        body.emplace_back(
            ex_assign(ex_deref(ExprToken::create(result.value().name)),
                      std::move(expr_call)));
    } else {
        body.emplace_back(std::move(expr_call));
    }

    body.emplace_back(ex_return(ExprToken::create("0")));

    return C_Function{
        function,
        function_name,
        function->comment,
        std::move(result),
        {},
        std::move(c_params),
        std::move(body),
    };
}

auto C_API::_translate_stdfunction(StdFunction const* stdfunction,
                                   std::string const& function_prefix)
    -> C_StdFunction {
    // Function name is the name of the struct followed by either the rename or
    // original method name
    std::string function_name = stdfunction->rename.empty()
                                    ? "AUTOGENERATED_NAME"
                                    : stdfunction->rename;

    if (!function_prefix.empty()) {
        function_name = fmt::format("{}_{}", function_prefix, function_name);
    }

    std::optional<C_Param> result =
        _translate_return_type(stdfunction->return_type);
    bool result_is_reference_to_opaqueptr =
        is_opaqueptr_lvalue_reference(stdfunction->return_type, _cpp_ctx);

    std::vector<C_Param> c_params;

    std::vector<ExprPtr> expr_params;
    std::vector<ExprPtr> decls;

    std::vector<Param> named_params;
    int param_index = 0;
    for (auto const& cpp_qt : stdfunction->params) {
        named_params.push_back(
            Param{fmt::format("param{:02}", param_index), clone(cpp_qt)});

        param_index++;
    }

    _translate_parameter_list(named_params, c_params, expr_params, decls);

    return C_StdFunction{
        stdfunction,
        stdfunction->rename,
        result,
        stdfunction->comment,
        std::move(named_params),
        std::move(c_params),
    };
}

auto C_API::_generate_stdfunction_wrapper(std::string const& id,
                                          std::string const& fun_param_name)
    -> ExprPtr {
    C_StdFunction const& c_fun = _stdfunctions.at(id);
    StdFunction const* cpp_fun = c_fun.cpp_fun;

    std::vector<ExprPtr> expr_lambda_params;
    std::vector<ExprPtr> expr_c_call_params;
    for (size_t i = 0; i < cpp_fun->params.size(); ++i) {
        QType const& cpp_param = cpp_fun->params[i];
        std::string const& name = c_fun.params[i].name;

        expr_lambda_params.emplace_back(ex_token(fmt::format(
            "{} {}", _cpp_ctx.get_qtype_as_string(cpp_param), name)));

        // now we need to handle passing the C++ parameter to C
        // if the param is pass-by-value and the type is opaqueptr, or if it is
        // an lvalue reference, then it will be expected as a pointer
        if (is_by_value(cpp_param) && is_opaqueptr(cpp_param, _cpp_ctx) ||
            is_lvalue_reference(cpp_param)) {
            expr_c_call_params.emplace_back(ex_address(ex_token(name)));
        } else {
            expr_c_call_params.emplace_back(ex_token(name));
        }
    }

    std::vector<ExprPtr> expr_lambda_body;

    if (c_fun.return_type.has_value()) {
        C_Param const& c_result = c_fun.return_type.value();
        if (is_opaque_ptr_by_value(cpp_fun->return_type, _cpp_ctx)) {
            // This is a bit nasty. Hopefully there's a better way.
            // Since the return is an opaqueptr by value, the C function will
            // allocate a new object into a pointer that we pass as the out
            // parameter. We need to move that heap allocation to the stack, so
            // we create a temporary to move the heap allocation into an return
            // and delete the heap allocation before returning. This will fail
            // if the return type is not default-constructible.
            std::string result_ptr_name = fmt::format("{}_ptr", c_result.name);

            expr_lambda_body.emplace_back(ex_var_decl(
                ex_token(_cpp_ctx.get_qtype_as_string(cpp_fun->return_type)),
                ex_token(c_result.name)));

            expr_lambda_body.emplace_back(
                ex_var_decl(ex_token(_cpp_ctx.get_qtype_as_string(
                                wrap_in_pointer(cpp_fun->return_type))),
                            ex_token(result_ptr_name)));

            expr_c_call_params.emplace_back(
                ex_address(ex_token(result_ptr_name)));

        } else {
            // add the variable declaration and pass of the return value
            expr_lambda_body.emplace_back(ex_var_decl(
                ex_token(_cpp_ctx.get_qtype_as_string(cpp_fun->return_type)),
                ex_token(c_result.name)));
            expr_c_call_params.emplace_back(
                ex_address(ex_token(c_result.name)));
        }
    }

    // create the call expression from the function pointer name and parameter
    // list and add it to the lambda body
    ExprPtr expr_c_call = ex_call(
        fun_param_name, ex_parameter_list(std::move(expr_c_call_params)));
    expr_lambda_body.emplace_back(std::move(expr_c_call));

    if (c_fun.return_type.has_value()) {
        C_Param const& c_result = c_fun.return_type.value();
        if (is_opaque_ptr_by_value(cpp_fun->return_type, _cpp_ctx)) {
            std::string result_ptr_name = fmt::format("{}_ptr", c_result.name);
            expr_lambda_body.emplace_back(
                ex_assign(ex_token(c_result.name),
                          ex_move(ex_deref(ex_token(result_ptr_name)))));

            expr_lambda_body.emplace_back(ex_delete(ex_token(result_ptr_name)));
        }

        expr_lambda_body.emplace_back(ex_return(ex_token(c_result.name)));
    }

    return ex_token(
        ex_fun_wrapper_lambda(fmt::format("{}_wrapper", fun_param_name),
                              _cpp_ctx.get_stdfunction_as_string(*cpp_fun),
                              std::move(expr_lambda_params),
                              std::move(expr_lambda_body))
            ->to_string(0));
}

auto C_API::_translate_parameter_list(std::vector<Param> const& params,
                                      std::vector<C_Param>& c_params,
                                      std::vector<ExprPtr>& expr_params,
                                      std::vector<ExprPtr>& decls) -> void {
    int param_number = 0;
    for (auto const& param : params) {
        C_QType param_type = _translate_qtype(param.type);

        // if the param is pass-by-value and the type is opaque ptr, then wrap
        // it in a pointer
        if (is_by_value(param.type) &&
            is_opaque_ptr_by_value(param.type, _cpp_ctx)) {
            param_type = wrap_in_pointer(std::move(param_type));
        }

        std::string param_name = param.name.empty()
                                     ? fmt::format("param{:02}", param_number)
                                     : param.name;

        c_params.emplace_back(C_Param{std::move(param_type), param_name});

        // Now we need to decide how to deref the parameter based on its bind
        // kind and the type in the original call. The rules are as follows:
        //
        // + pass by value: Type value
        //   o builtin
        //      value
        //   o value type
        //      value
        //   o opaque ptr
        //      *value
        //   o opaque bytes
        //      value
        //
        // + pass by pointer: Type* value
        //   o builtin
        //      value
        //   o valuetype/opaqueptr/opaquebytes
        //      value
        //
        // + pass by lvalue reference
        //   o builtin
        //      * value
        //   o any other
        //      * value
        //
        // + pass by rvalue reference
        //   this is probably a world of hurt - do we just want to error on
        //   trying to bind functions like this?

        std::optional<std::string> std_function_id =
            get_stdfunction_id(param.type);
        if (std_function_id.has_value()) {
            decls.emplace_back(_generate_stdfunction_wrapper(
                std_function_id.value(), param_name));
            expr_params.emplace_back(
                ExprToken::create(fmt::format("{}_wrapper", param_name)));
        } else if (auto enum_id = as_lvalue_reference_to_enum(param.type);
                   enum_id.has_value()) {
            // we need to static cast a pointer to the enum type then deref
            Enum const* enm = _cpp_ctx.get_enum(enum_id.value().id);
            if (enm == nullptr) {
                BBL_THROW(
                    "could not get enum {} while translating param \"{}\"",
                    enum_id.value().id,
                    param_name);
            }
            expr_params.emplace_back(ex_deref(
                ex_reinterpret_cast(ex_token(fmt::format("{}*", enm->spelling)),
                                    ex_token(param_name))));
        } else if (auto enum_id = as_pointer_to_enum(param.type);
                   enum_id.has_value()) {
            // we need to static cast to the pointer to the enum type
            Enum const* enm = _cpp_ctx.get_enum(enum_id.value().id);
            if (enm == nullptr) {
                BBL_THROW(
                    "could not get enum {} while translating param \"{}\"",
                    enum_id.value().id,
                    param_name);
            }
            expr_params.emplace_back(
                ex_reinterpret_cast(ex_token(fmt::format("{}*", enm->spelling)),
                                    ex_token(param_name)));
        } else if (is_lvalue_reference(param.type) ||
                   (is_by_value(param.type) &&
                    is_opaque_ptr_by_value(param.type, _cpp_ctx))) {
            expr_params.emplace_back(ex_deref(ExprToken::create(param_name)));
        } else if (is_rvalue_reference(param.type)) {
            BBL_THROW("cannot translate function rvalue reference parameter {}",
                      param_name);
        } else if (is_enum(param.type)) {
            // enums need to be static_cast<>() from their underlying type to
            // the enum explicitly when passing to C++
            EnumId enum_id =
                std::get<EnumId>(std::get<Type>(param.type.type).kind);
            Enum const* enm = _cpp_ctx.get_enum(enum_id.id);
            if (enm == nullptr) {
                BBL_THROW(
                    "could not get enum {} while translating param \"{}\"",
                    enum_id.id,
                    param_name);
            }
            expr_params.emplace_back(
                ex_static_cast(ExprToken::create(enm->spelling),
                               ExprToken::create(param_name)));
        } else {
            expr_params.emplace_back(ExprToken::create(param_name));
        }

        param_number++;
    }
}

auto C_API::_translate_constructor(Constructor const* ctor,
                                   std::string const& function_prefix,
                                   std::string const& class_id) -> C_Function {
    Class const* cls = _cpp_ctx.get_class(class_id);
    assert(cls);

    /// XXX: we need to figure out how to uniquify these... or do we?
    std::string function_name = fmt::format(
        "{}_{}", function_prefix, ctor->rename.empty() ? "ctor" : ctor->rename);

    // return type is the class itself. For opaque bytes and
    C_QType return_type = wrap_in_pointer(C_QType{
        nullptr,
        false,
        C_Type{
               C_StructId{class_id},
               }
    });

    // for opaque ptr we need to wrap in an extra * to new into
    if (cls->bind_kind == BindKind::OpaquePtr) {
        return_type = wrap_in_pointer(std::move(return_type));
    }

    C_Param result = C_Param{
        std::move(return_type),
        "_result",
    };

    std::vector<C_Param> c_params;

    std::vector<ExprPtr> body;
    std::vector<ExprPtr> expr_params;
    std::vector<ExprPtr> decls;

    _translate_parameter_list(ctor->params, c_params, expr_params, decls);

    ExprPtr expr_call =
        ex_call(cls->spelling,
                std::make_unique<ExprParameterList>(std::move(expr_params)));

    // if it's opaque ptr, do a new, assigning the result to the deref'd result
    // ptr
    if (cls->bind_kind == BindKind::OpaquePtr) {
        ExprPtr expr_new = ex_new(std::move(expr_call));
        body.emplace_back(ex_assign(ex_deref(ExprToken::create(result.name)),
                                    std::move(expr_new)));
    } else {
        // otherwise, do placement new straight into the result *
        body.emplace_back(ex_placement_new(ExprToken::create(result.name),
                                           std::move(expr_call)));
    }

    body.emplace_back(ex_return(ExprToken::create("0")));

    return C_Function{
        ctor,
        function_name,
        ctor->comment,
        std::move(result),
        {},
        std::move(c_params),
        std::move(body),
    };
}

auto C_API::_generate_destructor(std::string const& function_prefix,
                                 std::string const& class_id) -> C_Function {
    Class const* cls = _cpp_ctx.get_class(class_id);
    assert(cls);

    // destructor takes a single argument of pointer to the class
    C_QType this_type = wrap_in_pointer(C_QType{
        nullptr,
        false,
        C_Type{
               C_StructId{class_id},
               }
    });

    C_Param this_param{this_type, "_this"};

    std::string function_name = fmt::format("{}_dtor", function_prefix);

    std::vector<ExprPtr> body;

    body.emplace_back(ex_delete(ExprToken::create(this_param.name)));
    body.emplace_back(ex_return(ExprToken::create("0")));

    return C_Function{
        Generated{},
        function_name,
        "",
        {},
        {},
        std::vector{this_param},
        std::move(body),
    };
}

auto C_API::_translate_qtype(QType const& qt) -> C_QType {
    if (std::holds_alternative<Type>(qt.type)) {
        Type const& type = std::get<Type>(qt.type);

        if (std::holds_alternative<bbl_builtin_t>(type.kind)) {
            bbl_builtin_t builtin = std::get<bbl_builtin_t>(type.kind);

            return C_QType{
                &qt,
                qt.is_const,
                C_Type{builtin},
            };
        } else if (std::holds_alternative<ClassId>(type.kind)) {
            ClassId const& class_id = std::get<ClassId>(type.kind);

            if (!_cpp_ctx.has_class(class_id.id)) {
                BBL_THROW_MTBE("class {}", class_id.id);
            }

            return C_QType{
                &qt,
                qt.is_const,
                C_Type{C_StructId{class_id.id}},
            };
        } else if (std::holds_alternative<ClassTemplateSpecializationId>(
                       type.kind)) {
            ClassTemplateSpecializationId const& class_id =
                std::get<ClassTemplateSpecializationId>(type.kind);

            if (!_cpp_ctx.has_class(class_id.id)) {
                BBL_THROW_MTBE("class {}", class_id.id);
            }

            return C_QType{
                &qt,
                qt.is_const,
                C_Type{C_StructId{class_id.id}},
            };
        } else if (std::holds_alternative<EnumId>(type.kind)) {
            EnumId const& enum_id = std::get<EnumId>(type.kind);

            if (!_cpp_ctx.has_enum(enum_id.id)) {
                BBL_THROW_MTBE("enum {}", enum_id.id);
            }

            return C_QType{
                &qt,
                qt.is_const,
                C_Type{C_EnumId{enum_id.id}},
            };
        } else {
            StdFunctionId const& fun_id = std::get<StdFunctionId>(type.kind);

            if (!_cpp_ctx.has_stdfunction(fun_id.id)) {
                BBL_THROW_MTBE("{}", fun_id.id);
            }

            return C_QType{
                &qt,
                qt.is_const,
                C_Type{C_StdFunctionId{fun_id.id}},
            };
        }

    } else if (std::holds_alternative<Pointer>(qt.type)) {
        Pointer const& pointer = std::get<Pointer>(qt.type);

        return C_QType{&qt,
                       qt.is_const,
                       C_Pointer{std::unique_ptr<C_QType>(new C_QType(
                           std::move(_translate_qtype(*pointer.pointee))))}};
    } else if (std::holds_alternative<LValueReference>(qt.type)) {
        LValueReference const& pointer = std::get<LValueReference>(qt.type);

        // if it's an l-value reference to a stdfunction, then we *don't* want
        // to convert to a pointer, because we will be converting the function
        // itself to a pointer type
        if (is_stdfunction(*pointer.pointee)) {
            return _translate_qtype(*pointer.pointee);
        }

        return C_QType{&qt,
                       qt.is_const,
                       C_Pointer{std::unique_ptr<C_QType>(new C_QType(
                           std::move(_translate_qtype(*pointer.pointee))))}};
    } else if (std::holds_alternative<RValueReference>(qt.type)) {
        RValueReference const& pointer = std::get<RValueReference>(qt.type);

        return C_QType{&qt,
                       qt.is_const,
                       C_Pointer{std::unique_ptr<C_QType>(new C_QType(
                           std::move(_translate_qtype(*pointer.pointee))))}};
    } else if (std::holds_alternative<Array>(qt.type)) {
        Array const& array = std::get<Array>(qt.type);
        return C_QType{
            &qt,
            qt.is_const,
            C_Array{std::shared_ptr<C_QType>(new C_QType(
                        std::move(_translate_qtype(*array.element_type)))),
                    array.size}
        };
    }

    BBL_THROW("unreachable variant");
}

void replace_all(std::string& str,
                 std::string const& from,
                 std::string const& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos +=
            to.length(); // Handles case where 'to' is a substring of 'from'
    }
}

std::string format_comment(std::string comment) {
#if 0
    replace_all(comment, "\n", "\n * ");
    return fmt::format("/**\n * {}\n */", comment);
#else
    replace_all(comment, "\n", "\n/// ");
    return fmt::format("/// {}", comment);
#endif
}

std::string C_API::get_header() const {
    std::string result = R"(
#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

)";
    result = fmt::format("{}/** enums */\n\n", result);
    for (auto const& [c_enum_id, c_enum] : _enums) {
        if (!c_enum.comment.empty()) {
            result =
                fmt::format("{}{}\n", result, format_comment(c_enum.comment));
        }
        result = fmt::format("{}enum {} {{\n", result, c_enum.name);

        for (auto const& [var_name, var_value] : c_enum.variants) {
            result =
                fmt::format("{}    {} = {},\n", result, var_name, var_value);
        }

        result = fmt::format("{}}};\n", result);
        result = fmt::format("{}\n", result);
    }
    result = fmt::format("{}\n", result);

    result = fmt::format("{}/** structs */\n\n", result);
    for (auto const& [c_struct_id, c_struct] : _structs) {

        if (!c_struct.comment.empty()) {
            result =
                fmt::format("{}{}\n", result, format_comment(c_struct.comment));
        }

        if (c_struct.cls.bind_kind == BindKind::OpaquePtr) {
            result = fmt::format(
                "{0}typedef struct {1} {1};\n", result, c_struct.name);
        } else if (c_struct.cls.bind_kind == BindKind::OpaqueBytes) {
            result = fmt::format("{}typedef struct BBL_ALIGN({}) {} {{\n",
                                 result,
                                 c_struct.cls.layout.align_bytes,
                                 c_struct.name);

            result = fmt::format("{}    char _bbl_opaque[{}];\n",
                                 result,
                                 c_struct.cls.layout.size_bytes);
            result = fmt::format("{}}} {};\n", result, c_struct.name);
            result = fmt::format("{}\n", result);
        } else {
            result = fmt::format("{}typedef struct BBL_ALIGN({}) {} {{\n",
                                 result,
                                 c_struct.cls.layout.align_bytes,
                                 c_struct.name);

            for (C_Field const& field : c_struct.fields) {
                result =
                    fmt::format("{}    {};\n",
                                result,
                                _get_c_qtype_as_string(field.type, field.name));
            }

            result = fmt::format("{}}} {};\n", result, c_struct.name);
            result = fmt::format("{}\n", result);
        }
    }
    result = fmt::format("{}\n", result);

    result = fmt::format("{}\n\n/** functions */\n\n", result);
    for (auto const& [function_id, c_fun] : _functions) {
        if (!c_fun.comment.empty()) {
            result =
                fmt::format("{}{}\n", result, format_comment(c_fun.comment));
        }

        result =
            fmt::format("{}{};\n", result, _get_function_declaration(c_fun));

        result = fmt::format("{}\n", result);
    }
    result = fmt::format("{}\n", result);

    result += R"(
#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN
)";

    return result;
}

auto C_API::_get_function_declaration(C_Function const& c_fun) const
    -> std::string {
    std::string result = fmt::format("int {}(", c_fun.name);
    bool first = true;

    // If the method has a receiver (i.e. it's not static), bind the receiver in
    // as the first parameter
    if (std::holds_alternative<C_Param>(c_fun.receiver)) {
        auto const& receiver = std::get<C_Param>(c_fun.receiver);
        result =
            fmt::format("{}{}",
                        result,
                        _get_c_qtype_as_string(receiver.type, receiver.name));

        first = false;
    } else if (std::holds_alternative<C_SmartPtr>(c_fun.receiver)) {
        auto const& receiver = std::get<C_SmartPtr>(c_fun.receiver);
        result = fmt::format("{}{}",
                             result,
                             _get_c_qtype_as_string(receiver.smartptr.type,
                                                    receiver.smartptr.name));

        first = false;
    }

    // Then do the actual parameters
    for (C_Param const& c_param : c_fun.params) {
        if (first) {
            first = false;
        } else {
            result = fmt::format("{}, ", result);
        }

        std::string type_string =
            _get_c_qtype_as_string(c_param.type, c_param.name);
        result = fmt::format("{}{}", result, type_string);
    }

    // Finally, we convert the return type (if it's not void) to an out
    // parameter at the end of the parameter list, as the actual return type of
    // the C function will be int to do error reporting
    if (c_fun.result.has_value()) {
        if (!first) {
            result = fmt::format("{}, ", result);
        }
        std::string type_string = _get_c_qtype_as_string(
            c_fun.result.value().type, c_fun.result.value().name);
        result = fmt::format("{}{}", result, type_string);
    }

    result = fmt::format("{})", result);

    return result;
}

std::string C_API::get_source() const {
    std::string result;

    // first write all the include statements to the top of the file
    for (Inclusion const& inclusion : _inclusions) {
        // just write the whole directive
        result = fmt::format("{}{}\n", result, inclusion.directive);
    }
    result = fmt::format("{}\n", result);

    result = fmt::format("{}{}",
                         result,
                         R"(#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

)");

    // next do functon impls
    bool did_any_impl = false;
    for (auto const& mod : _modules) {
        for (std::string const& impl : mod.function_impls) {
            result = fmt::format("{}{}\n", result, impl);
            did_any_impl = true;
        }
    }
    if (did_any_impl) {
        result = fmt::format("{}\n", result);
    }

    result = fmt::format("{}extern \"C\" {{\n", result);

    // we expose enum parameters as their underlying integer type in the API, so
    // we write aliases for them in the source rather than an enum definition
    for (auto const& [enum_id, c_enum] : _enums) {
        result = fmt::format(
            "{}using {} = {};\n", result, c_enum.name, c_enum.integer_type);
    }
    result = fmt::format("{}\n", result);

    // the struct definitions are just aliases to the corresponding C++ type
    for (auto const& [struct_id, c_struct] : _structs) {
        result = fmt::format(
            "{}using {} = {};\n", result, c_struct.name, c_struct.cls.spelling);

        // if the type is opaque bytes, put a simple size check in
        if (c_struct.cls.bind_kind == BindKind::OpaqueBytes) {
            result = fmt::format("{0}static_assert(sizeof({1}) == {2}, \"size "
                                 "of {1} and {3} do not match\");\n",
                                 result,
                                 c_struct.cls.spelling,
                                 c_struct.cls.layout.size_bytes,
                                 c_struct.name);

            result =
                fmt::format("{0}static_assert(alignof({1}) == {2}, \"align "
                            "of {1} and {3} do not match\");\n",
                            result,
                            c_struct.cls.spelling,
                            c_struct.cls.layout.align_bytes,
                            c_struct.name);

            result = fmt::format("{}\n", result);
        } else if (c_struct.cls.bind_kind == BindKind::ValueType) {
            // for value type, make a little size-checking type
            result =
                fmt::format("{}struct BBL_ALIGN({}) {}_bbl_size_check {{\n",
                            result,
                            c_struct.cls.layout.align_bytes,
                            c_struct.name);

            for (C_Field const& field : c_struct.fields) {
                result =
                    fmt::format("{}    {};\n",
                                result,
                                _get_c_qtype_as_string(field.type, field.name));
            }

            result = fmt::format("{}}};\n", result);

            result = fmt::format(
                "{}static_assert(sizeof({}_bbl_size_check) == sizeof({}), "
                "\"size of value type does not match\");\n",
                result,
                c_struct.name,
                c_struct.cls.spelling);

            result = fmt::format(
                "{}static_assert(alignof({}_bbl_size_check) == alignof({}), "
                "\"align of value type does not match\");\n",
                result,
                c_struct.name,
                c_struct.cls.spelling);

            result = fmt::format("{}\n", result);
        }
    }
    result = fmt::format("{}\n", result);

    // lastly, all the function definitions
    for (auto const& [function_id, c_fun] : _functions) {
        result =
            fmt::format("{}{} {{\n", result, _get_function_declaration(c_fun));

        result = fmt::format("{}{}", result, c_fun.body.to_string(1));

        result = fmt::format("{}}}\n\n", result);
    }

    result = fmt::format("{}}} // extern \"C\"\n", result);

    return result;
}

auto C_API::get_modules() const -> std::vector<C_Module> const& {
    return _modules;
}

auto C_API::get_inclusions() const -> std::vector<Inclusion> const& {
    return _inclusions;
}

auto C_API::get_structs() const -> C_StructMap const& { return _structs; }

auto C_API::get_functions() const -> C_FunctionMap const& { return _functions; }

auto C_API::get_stdfunctions() const -> C_StdFunctionMap const& {
    return _stdfunctions;
}

auto C_API::get_enums() const -> C_EnumMap const& { return _enums; }

} // namespace bbl