#include "bbl-c.hpp"
#include "bbl_detail.h"
#include "bblfmt.hpp"

#include <memory>
#include <optional>
#include <variant>

namespace bbl {

static auto replace_namespace_prefix(std::string const& spelling,
                                     std::string const& mod_name)
    -> std::string {
    size_t pos = spelling.find_last_of(':');
    if (pos == std::string::npos) {
        // return fmt::format("{}_{}", mod_name, spelling);
        return spelling;
    } else {
        // return fmt::format("{}_{}_{}", spelling.substr(0, pos - 1), mod_name,
        //                    spelling.substr(pos + 1));
        return fmt::format("{}_{}", spelling.substr(0, pos - 1),
                           spelling.substr(pos + 1));
    }
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
        std::vector<std::string> mod_enums;

        for (auto const& cpp_class_id : cpp_mod.classes) {
            auto const* cpp_cls = _cpp_ctx.get_class(cpp_class_id);
            assert(cls);

            // Construct the struct name from the name of the module, and the
            // spelling or rename string
            std::string struct_namespace = replace_namespace_prefix(
                cpp_cls->rename.empty() ? cpp_cls->spelling : cpp_cls->rename,
                cpp_mod.name);

            std::string struct_name = fmt::format("{}_t", struct_namespace);

            // Translate all fields
            std::vector<C_Field> fields;
            try {
                for (auto const& field : cpp_cls->fields) {
                    fields.emplace_back(C_Field{
                        _translate_qtype(field.type),
                        field.name,
                    });
                }
            } catch (MissingTypeBindingException& e) {
                SPDLOG_ERROR("could not translate a field of {}. Class will be "
                             "ignored.\n{}",
                             cpp_cls->spelling, e.what());
                continue;
            }

            // Push the struct into the main list and the module list
            _structs.emplace(cpp_class_id,
                             C_Struct{*cpp_cls, struct_name, fields});
            mod_structs.push_back(cpp_class_id);

            // Translate all methods to functions
            for (auto const& method_id : cpp_cls->methods) {
                Method const* method = _cpp_ctx.get_method(method_id);
                assert(method);

                try {
                    C_Function c_fun = _translate_method(
                        method, struct_namespace, cpp_class_id);

                    _functions.emplace(method_id, std::move(c_fun));
                    mod_functions.push_back(method_id);
                } catch (MissingTypeBindingException& e) {
                    SPDLOG_ERROR("could not translate method {} of class {}. "
                                 "Method will be ignored.\n{}",
                                 method->function.name, cpp_cls->spelling,
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
                                 cpp_cls->spelling, e.what());
                }
            }

            // If the class is opaque ptr then add a dtor function
            // XXX: what do we do about other kinds? if it's safe to bind a type
            // as opaque bytes or value type then it should be safe to just let
            // them fall off the stack too...
            if (cpp_cls->bind_kind == BindKind::OpaquePtr) {
                C_Function c_dtor =
                    _generate_destructor(struct_namespace, cpp_class_id);
                std::string dtor_id = cpp_class_id + "_dtor";
                _functions.emplace(dtor_id, std::move(c_dtor));
                mod_functions.push_back(dtor_id);
            }
        }

        // Translate all enums
        for (auto const& cpp_enum_id : cpp_mod.enums) {
            auto const* cpp_enum = _cpp_ctx.get_enum(cpp_enum_id);
            std::string enum_namespace = replace_namespace_prefix(
                cpp_enum->rename.empty() ? cpp_enum->spelling
                                         : cpp_enum->rename,
                cpp_mod.name);

            std::string enum_name = fmt::format("{}", enum_namespace);
            bbl_builtin_t integer_type = get_builtin(cpp_enum->integer_type);

            _enums.emplace(cpp_enum_id, C_Enum{
                                            enum_name,
                                            cpp_enum->variants,
                                            integer_type,
                                        });
            mod_enums.push_back(cpp_enum_id);
        }

        // Translate functions
        for (auto const& cpp_fun_id : cpp_mod.functions) {
            auto const* cpp_fun = _cpp_ctx.get_function(cpp_fun_id);

            try {
                C_Function c_fun = _translate_function(cpp_fun, "");
                _functions.emplace(cpp_fun_id, std::move(c_fun));
                mod_functions.push_back(cpp_fun_id);

            } catch (MissingTypeBindingException& e) {
                SPDLOG_ERROR("could not translate function {}. "
                             "Function will be ignored.\n{}",
                             cpp_fun->spelling, e.what());
            }
        }

        SourceFile const* source_file =
            _cpp_ctx.get_source_file(cpp_mod.source_file);
        assert(source_file);

        // add the source inclusions to the main list
        for (Inclusion const& inclusion : source_file->inclusions) {
            if (!seen_includes.contains(inclusion.filename)) {
                _inclusions.push_back(inclusion);
                seen_includes.insert(inclusion.filename);
            }
        }

        _modules.emplace_back(C_Module{
            cpp_id,
            cpp_mod.name,
            source_file->inclusions,
            std::move(mod_structs),
            std::move(mod_functions),
        });
    }

    // translate all std functions
    for (auto const& [cpp_stdfun_id, cpp_stdfun] : _cpp_ctx.stdfunctions()) {
        C_QType result_type = _translate_qtype(cpp_stdfun.return_type);

        std::vector<C_QType> c_params;
        for (auto const& param : cpp_stdfun.params) {
            c_params.emplace_back(_translate_qtype(param));
        }

        _stdfunctions.emplace(cpp_stdfun_id, C_StdFunction{
                                                 result_type,
                                                 std::move(c_params),
                                             });
    }
}

auto C_API::_get_c_qtype_as_string(C_QType const& qt) const -> std::string {
    char const* s_const = qt.is_const ? " const" : "";

    if (std::holds_alternative<C_Type>(qt.type)) {
        C_Type const& c_type = std::get<C_Type>(qt.type);

        if (std::holds_alternative<bbl_builtin_t>(c_type.kind)) {
            bbl_builtin_t builtin = std::get<bbl_builtin_t>(c_type.kind);
            return fmt::format("{}{}", builtin, s_const);
        } else if (std::holds_alternative<C_StructId>(c_type.kind)) {
            C_StructId struct_id = std::get<C_StructId>(c_type.kind);
            C_Struct const& c_struct = _structs.at(struct_id.id);
            return fmt::format("{}{}", c_struct.name, s_const);
        } else if (std::holds_alternative<C_EnumId>(c_type.kind)) {
            C_EnumId enum_id = std::get<C_EnumId>(c_type.kind);
            C_Enum const& c_enum = _enums.at(enum_id.id);
            return fmt::format("{}", c_enum.integer_type);
        } else if (std::holds_alternative<C_StdFunctionId>(c_type.kind)) {
            C_StdFunctionId fun_id = std::get<C_StdFunctionId>(c_type.kind);
            C_StdFunction const& fun = _stdfunctions.at(fun_id.id);

            std::string result =
                fmt::format("{}(*)(", _get_c_qtype_as_string(fun.return_type));
            bool first = true;
            for (auto const& param : fun.params) {
                if (first) {
                    first = false;
                } else {
                    result = fmt::format("{}, ", result);
                }

                result =
                    fmt::format("{}{}", result, _get_c_qtype_as_string(param));
            }
            return fmt::format("{})", result);
        } else {
            BBL_THROW("unhandled variant");
        }

    } else if (std::holds_alternative<C_Pointer>(qt.type)) {
        C_Pointer const& c_pointer = std::get<C_Pointer>(qt.type);
        return fmt::format("{}*{}", _get_c_qtype_as_string(*c_pointer.pointee),
                           s_const);
    } else {
        BBL_THROW("unhandled variant");
    }
}

// Returns true if the given QType is an l-value reference
static bool is_lvalue_reference(QType const& qt) {
    if (std::holds_alternative<LValueReference>(qt.type)) {
        return true;
    } else {
        return false;
    }
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

// Returns true if the given QType is not a pointer or reference type
static bool is_by_value(QType const& qt) {
    return std::holds_alternative<Type>(qt.type);
}

static bool is_opaqueptr(QType const& qt, Context const& ctx) {
    if (std::holds_alternative<Type>(qt.type)) {
        Type const& type = std::get<Type>(qt.type);
        if (std::holds_alternative<ClassId>(type.kind)) {
            ClassId const& id = std::get<ClassId>(type.kind);
            if (ctx.get_class(id.id)->bind_kind == BindKind::OpaquePtr) {
                return true;
            }
        } else if (std::holds_alternative<ClassTemplateSpecializationId>(
                       type.kind)) {
            ClassTemplateSpecializationId const& id =
                std::get<ClassTemplateSpecializationId>(type.kind);
            if (ctx.get_class(id.id)->bind_kind == BindKind::OpaquePtr) {
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

static bool is_reference_to_opaqueptr(QType const& qt, Context const& ctx) {
    if (std::holds_alternative<LValueReference>(qt.type)) {
        LValueReference const& ref = std::get<LValueReference>(qt.type);
        return is_opaqueptr(*ref.pointee, ctx);
    }

    return false;
}

// Returns true if this QType is an opaque ptr-bound value
static bool is_value_opaque_ptr(QType const& qt, Context& cpp_ctx) {
    if (std::holds_alternative<Type>(qt.type)) {
        Type const& type = std::get<Type>(qt.type);
        if (std::holds_alternative<ClassId>(type.kind)) {
            ClassId const& id = std::get<ClassId>(type.kind);
            return cpp_ctx.get_class(id.id)->bind_kind == BindKind::OpaquePtr;
        } else if (std::holds_alternative<ClassTemplateSpecializationId>(
                       type.kind)) {
            ClassTemplateSpecializationId const& id =
                std::get<ClassTemplateSpecializationId>(type.kind);
            return cpp_ctx.get_class(id.id)->bind_kind == BindKind::OpaquePtr;
        }
    }

    return false;
}

// Returns true is the given QType directly holds a Class with bind kind of
// opaque ptr
static bool is_opaque_ptr(QType const& qt, Context const& ctx) {
    if (std::holds_alternative<Type>(qt.type)) {
        Type const& type = std::get<Type>(qt.type);
        if (std::holds_alternative<ClassId>(type.kind)) {
            ClassId const& id = std::get<ClassId>(type.kind);
            Class const* cls = ctx.get_class(id.id);
            assert(cls);
            return cls->bind_kind == BindKind::OpaquePtr;
        } else if (std::holds_alternative<ClassTemplateSpecializationId>(
                       type.kind)) {
            ClassTemplateSpecializationId const& id =
                std::get<ClassTemplateSpecializationId>(type.kind);
            Class const* cls = ctx.get_class(id.id);
            assert(cls);
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

auto C_API::_translate_method(Method const* method,
                              std::string const& function_prefix,
                              std::string const& class_id) -> C_Function {
    // Function name is the name of the struct followed by either the rename or
    // original method name
    std::string function_name =
        fmt::format("{}_{}", function_prefix,
                    method->function.rename.empty() ? method->function.name
                                                    : method->function.rename);

    // For return values:
    //
    // + opaque ptr
    //   o if the result is by value
    //     - wrap the param in a pointer, and assign into it
    //       *result = call()
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
    bool result_is_reference = is_lvalue_reference(method->function.return_type);
    bool result_is_reference_to_opaqueptr =
        is_reference_to_opaqueptr(method->function.return_type, _cpp_ctx);
    bool result_is_reference_to_bytes_or_value =
        is_reference_to_bytes_or_value(method->function.return_type, _cpp_ctx);
    C_QType return_type = _translate_qtype(method->function.return_type);
    if (!is_void(return_type)) {
        if (result_is_reference_to_bytes_or_value) {
            remove_const_on_pointee(return_type);
            result = C_Param{
                std::move(return_type),
                "_result",
            };
        } else {
            result = C_Param{
                C_QType{nullptr, false,
                        C_Pointer{
                            std::make_shared<C_QType>(std::move(return_type))}},
                "_result",
            };
        }
    }

    std::vector<C_Param> c_params;

    // if the method is not static, create _this param, which will be a (maybe
    // const) pointer to a struct of this class_id
    std::optional<C_Param> receiver;
    if (!method->is_static) {
        bool pointee_is_const = method->is_const;

        receiver = C_Param{
            C_QType{
                    nullptr, false,
                    C_Pointer{std::unique_ptr<C_QType>(new C_QType{
                    nullptr, pointee_is_const, C_Type{C_StructId{class_id}}})},
                    },
            "_this",
        };
    }

    std::vector<ExprPtr> body;
    std::vector<ExprPtr> expr_params;

    _translate_parameter_list(method->function.params, c_params, expr_params);

    ExprPtr expr_call =
        ExprCall::create(method->function.name + method->function.template_call, std::move(expr_params));

    Class const* cls = _cpp_ctx.get_class(class_id);
    assert(cls);

    ExprPtr receiving_call =
        receiver.has_value()
            ? ExprArrow::create(ExprToken::create(receiver.value().name),
                                std::move(expr_call))
            : ExprNamespaceRef::create(cls->spelling, std::move(expr_call));

    if (result.has_value()) {
        // if the function returns a reference, we need to wrap the call in an
        // address operator in order to convert it back to a pointer
        if (result_is_reference_to_opaqueptr) {
            receiving_call = ExprAddress::create(std::move(receiving_call));
        }

        body.emplace_back(ExprAssign::create(
            ExprDeref::create(ExprToken::create(result.value().name)),
            std::move(receiving_call)));
    } else {
        body.emplace_back(std::move(receiving_call));
    }

    body.emplace_back(ExprReturn::create(ExprToken::create("0")));

    return C_Function{
        method,
        function_name,
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

    // For return values:
    //
    // + opaque ptr
    //   o if the result is by value
    //     - wrap the param in a pointer, and assign into it
    //       *result = call()
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
    bool result_is_reference = is_lvalue_reference(function->return_type);
    bool result_is_reference_to_opaqueptr =
        is_reference_to_opaqueptr(function->return_type, _cpp_ctx);
    bool result_is_reference_to_bytes_or_value =
        is_reference_to_bytes_or_value(function->return_type, _cpp_ctx);
    C_QType return_type = _translate_qtype(function->return_type);
    if (!is_void(return_type)) {
        if (result_is_reference_to_bytes_or_value) {
            result = C_Param{
                std::move(return_type),
                "_result",
            };
        } else {
            result = C_Param{
                C_QType{nullptr, false,
                        C_Pointer{
                            std::make_shared<C_QType>(std::move(return_type))}},
                "_result",
            };
        }
    }

    std::vector<C_Param> c_params;

    std::vector<ExprPtr> body;
    std::vector<ExprPtr> expr_params;

    _translate_parameter_list(function->params, c_params, expr_params);

    ExprPtr expr_call =
        ExprCall::create(function->name + function->template_call, std::move(expr_params));

    if (result.has_value()) {
        // if the function returns a reference, we need to wrap the call in an
        // address operator in order to convert it back to a pointer when
        // returning an opaqueptr
        if (result_is_reference_to_opaqueptr) {
            expr_call = ExprAddress::create(std::move(expr_call));
        }

        body.emplace_back(ExprAssign::create(
            ExprDeref::create(ExprToken::create(result.value().name)),
            std::move(expr_call)));
    } else {
        body.emplace_back(std::move(expr_call));
    }

    body.emplace_back(ExprReturn::create(ExprToken::create("0")));

    return C_Function{
        function, function_name,       std::move(result),
        {},       std::move(c_params), std::move(body),
    };
}

auto C_API::_translate_parameter_list(std::vector<Param> const& params,
                                      std::vector<C_Param>& c_params,
                                      std::vector<ExprPtr>& expr_params)
    -> void {
    int param_number = 0;
    for (auto const& param : params) {
        C_QType param_type = _translate_qtype(param.type);

        // if the param is pass-by-value and the type is opaque ptr, then wrap
        // it in an extra pointer
        if (is_by_value(param.type) && is_opaque_ptr(param.type, _cpp_ctx)) {
            param_type = C_QType{
                nullptr,
                false,
                C_Pointer{std::make_shared<C_QType>(std::move(param_type))},
            };
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

        if (is_lvalue_reference(param.type) ||
            (is_by_value(param.type) && is_opaque_ptr(param.type, _cpp_ctx))) {
            expr_params.emplace_back(
                ExprDeref::create(ExprToken::create(param_name)));
        } else if (is_enum(param.type)) {
            // enums need to be static_cast<>() from their underlying type to
            // the enum explicitly when passing to C++
            EnumId enum_id =
                std::get<EnumId>(std::get<Type>(param.type.type).kind);
            Enum const* enm = _cpp_ctx.get_enum(enum_id.id);
            if (enm == nullptr) {
                BBL_THROW(
                    "could not get enum {} while translating param \"{}\"",
                    enum_id.id, param_name);
            }
            expr_params.emplace_back(
                ExprStaticCast::create(ExprToken::create(enm->spelling),
                                       ExprToken::create(param_name)));
        } else {
            expr_params.emplace_back(ExprToken::create(param_name));
        }

        param_number++;
    }
}

auto wrap_in_pointer(C_QType&& qt) -> C_QType {
    return C_QType{nullptr, false,
                   C_Pointer{std::make_shared<C_QType>(std::move(qt))}};
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
        nullptr, false,
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

    _translate_parameter_list(ctor->params, c_params, expr_params);

    ExprPtr expr_call = ExprCall::create(cls->spelling, std::move(expr_params));

    // if it's opaque ptr, do a new, assigning the result to the deref'd result
    // ptr
    if (cls->bind_kind == BindKind::OpaquePtr) {
        ExprPtr expr_new = ExprNew::create(std::move(expr_call));
        body.emplace_back(ExprAssign::create(
            ExprDeref::create(ExprToken::create(result.name)),
            std::move(expr_new)));
    } else {
        // otherwise, do placement new straight into the result *
        body.emplace_back(ExprPlacementNew::create(
            ExprToken::create(result.name), std::move(expr_call)));
    }

    body.emplace_back(ExprReturn::create(ExprToken::create("0")));

    return C_Function{
        ctor, function_name,       std::move(result),
        {},   std::move(c_params), std::move(body),
    };
}

auto C_API::_generate_destructor(std::string const& function_prefix,
                                 std::string const& class_id) -> C_Function {
    Class const* cls = _cpp_ctx.get_class(class_id);
    assert(cls);

    // destructor takes a single argument of pointer to the class
    C_QType this_type = wrap_in_pointer(C_QType{
        nullptr, false,
        C_Type{
               C_StructId{class_id},
               }
    });

    C_Param this_param{this_type, "_this"};

    std::string function_name = fmt::format("{}_dtor", function_prefix);

    std::vector<ExprPtr> body;

    body.emplace_back(ExprDelete::create(ExprToken::create(this_param.name)));
    body.emplace_back(ExprReturn::create(ExprToken::create("0")));

    return C_Function{
        Generated{}, function_name,           {},
        {},          std::vector{this_param}, std::move(body),
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

        return C_QType{&qt, qt.is_const,
                       C_Pointer{std::unique_ptr<C_QType>(new C_QType(
                           std::move(_translate_qtype(*pointer.pointee))))}};
    } else if (std::holds_alternative<LValueReference>(qt.type)) {
        LValueReference const& pointer = std::get<LValueReference>(qt.type);

        return C_QType{&qt, qt.is_const,
                       C_Pointer{std::unique_ptr<C_QType>(new C_QType(
                           std::move(_translate_qtype(*pointer.pointee))))}};
    } else if (std::holds_alternative<RValueReference>(qt.type)) {
        RValueReference const& pointer = std::get<RValueReference>(qt.type);

        return C_QType{&qt, qt.is_const,
                       C_Pointer{std::unique_ptr<C_QType>(new C_QType(
                           std::move(_translate_qtype(*pointer.pointee))))}};
    }

    BBL_THROW("unreachable");
}

std::string C_API::get_header() const {
    std::string result = R"(#ifndef SOMEHEADERGUARD_H
#define SOMEHEADERGUARD_H

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#ifdef __cplusplus
extern "C" {
#endif

)";
    result = fmt::format("{}/** enums */\n\n", result);
    for (auto const& [c_enum_id, c_enum] : _enums) {
        result = fmt::format("{}enum {} {{\n", result, c_enum.name);

        for (auto const& [var_name, var_value] : c_enum.variants) {
            result =
                fmt::format("{}    {} = {},\n", result, var_name, var_value);
        }

        result = fmt::format("{}}};\n", result);
    }
    result = fmt::format("{}\n", result);

    result = fmt::format("{}/** structs */\n\n", result);
    for (auto const& [c_struct_id, c_struct] : _structs) {

        if (c_struct.cls.bind_kind == BindKind::OpaquePtr) {
            result = fmt::format("{}struct {};\n", result, c_struct.name);
        } else if (c_struct.cls.bind_kind == BindKind::OpaqueBytes) {
            result =
                fmt::format("{}struct BBL_ALIGN({}) {} {{\n", result,
                            c_struct.cls.layout.align_bytes, c_struct.name);

            result = fmt::format("{}    char _bbl_opaque[{}];\n", result,
                                 c_struct.cls.layout.size_bytes);
            result = fmt::format("{}}};\n", result);
            result = fmt::format("{}\n", result);
        } else {
            result =
                fmt::format("{}struct BBL_ALIGN({}) {} {{\n", result,
                            c_struct.cls.layout.align_bytes, c_struct.name);

            for (C_Field const& field : c_struct.fields) {
                result =
                    fmt::format("{}    {} {};\n", result,
                                _get_c_qtype_as_string(field.type), field.name);
            }

            result = fmt::format("{}}};\n", result);
            result = fmt::format("{}\n", result);
        }
    }
    result = fmt::format("{}\n", result);

    result = fmt::format("{}\n\n/** functions */\n\n", result);
    for (auto const& [function_id, c_fun] : _functions) {
        result =
            fmt::format("{}{};\n", result, _get_function_declaration(c_fun));
    }
    result = fmt::format("{}\n", result);

    result += R"(
#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN

#endif
)";

    return result;
}

auto C_API::_get_function_declaration(C_Function const& c_fun) const
    -> std::string {
    std::string result = fmt::format("int {}(", c_fun.name);
    bool first = true;

    // If the method has a receiver (i.e. it's not static), bund the receiver in
    // as the first parameter
    if (c_fun.receiver.has_value()) {
        result =
            fmt::format("{}{} {}", result,
                        _get_c_qtype_as_string(c_fun.receiver.value().type),
                        c_fun.receiver.value().name);

        first = false;
    }

    // Then do the actual parameters
    for (C_Param const& c_param : c_fun.params) {
        if (first) {
            first = false;
        } else {
            result = fmt::format("{}, ", result);
        }

        std::string type_string = _get_c_qtype_as_string(c_param.type);
        result = fmt::format("{}{} {}", result, type_string, c_param.name);
    }

    // Finally, we convert the return type (if it's not void) to an out
    // parameter at the end of the parameter list, as the actual return type of
    // the C function will be int to do error reporting
    if (c_fun.result.has_value()) {
        if (!first) {
            result = fmt::format("{}, ", result);
        }
        std::string type_string =
            _get_c_qtype_as_string(c_fun.result.value().type);
        result = fmt::format("{}{} {}", result, type_string,
                             c_fun.result.value().name);
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

    result = fmt::format("{}{}", result,
                         R"(#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

)");

    result = fmt::format("{}extern \"C\" {{\n", result);

    // we expose enum parameters as their underlying integer type in the API, so
    // we write aliases for them in the source rather than an enum definition
    for (auto const& [enum_id, c_enum] : _enums) {
        result = fmt::format("{}using {} = {};\n", result, c_enum.name,
                             c_enum.integer_type);
    }
    result = fmt::format("{}\n", result);

    // the struct definitions are just aliases to the corresponding C++ type
    for (auto const& [struct_id, c_struct] : _structs) {
        result = fmt::format("{}using {} = {};\n", result, c_struct.name,
                             c_struct.cls.spelling);

        // if the type is opaque bytes, put a simple size check in
        if (c_struct.cls.bind_kind == BindKind::OpaqueBytes) {
            result = fmt::format("{0}static_assert(sizeof({1}) == {2}, \"size "
                                 "of {1} and {3} do not match\");\n",
                                 result, c_struct.cls.spelling,
                                 c_struct.cls.layout.size_bytes, c_struct.name);

            result =
                fmt::format("{0}static_assert(alignof({1}) == {2}, \"align "
                            "of {1} and {3} do not match\");\n",
                            result, c_struct.cls.spelling,
                            c_struct.cls.layout.align_bytes, c_struct.name);

            result = fmt::format("{}\n", result);
        } else if (c_struct.cls.bind_kind == BindKind::ValueType) {
            // for value type, make a little size-checking type
            result = fmt::format(
                "{}struct BBL_ALIGN({}) {}_bbl_size_check {{\n", result,
                c_struct.cls.layout.align_bytes, c_struct.name);

            for (C_Field const& field : c_struct.fields) {
                result =
                    fmt::format("{}    {} {};\n", result,
                                _get_c_qtype_as_string(field.type), field.name);
            }

            result = fmt::format("{}}};\n", result);

            result = fmt::format(
                "{}static_assert(sizeof({}_bbl_size_check) == sizeof({}), "
                "\"size of value type does not match\");\n",
                result, c_struct.name, c_struct.cls.spelling);

            result = fmt::format(
                "{}static_assert(alignof({}_bbl_size_check) == alignof({}), "
                "\"align of value type does not match\");\n",
                result, c_struct.name, c_struct.cls.spelling);

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

} // namespace bbl