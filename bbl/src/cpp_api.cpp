#include "bbl_detail.h"

#include "bbl.hpp"

#include <spdlog/spdlog.h>
#include <variant>

extern "C" {

using bbl_context_t = bbl::Context*;
using bbl_module_t = bbl::Module const*;
using bbl_class_t = bbl::Class const*;
using bbl_classid_t = std::string const*;
using bbl_function_t = bbl::Function const*;
using bbl_functionid_t = std::string const*;
using bbl_method_t = bbl::Method const*;
using bbl_constructor_t = bbl::Constructor const*;
using bbl_stdfunction_t = bbl::StdFunction const*;
using bbl_stdfunctionid_t = std::string const*;
using bbl_enum_t = bbl::Enum const*;
using bbl_enumid_t = std::string const*;
using bbl_qtype_t = bbl::QType const*;
using bbl_param_t = bbl::Param const*;

bbl_result_t bbl_context_create(bbl_context_t* ctx) {
    *ctx = new bbl::Context();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_context_destroy(bbl_context_t ctx) {
    delete ctx;
    return bbl_result_t::BBL_RESULT_Success;
}

bbl_result_t bbl_context_compile_and_extract(bbl_context_t ctx, int argc,
                                             char const** argv) {
    if (ctx == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    try {
        ctx->compile_and_extract(argc, argv);
        return BBL_RESULT_Success;
    } catch (std::exception& e) {
        SPDLOG_ERROR(e.what());
        return BBL_RESULT_Success;
    }
}

bbl_result_t bbl_context_num_modules(bbl_context_t ctx, size_t* num_modules) {
    if (ctx == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    *num_modules = ctx->num_modules();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_context_get_module(bbl_context_t ctx, size_t index,
                                    bbl_module_t* module) {
    if (ctx == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    if (index >= ctx->num_modules()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *module = &ctx->modules()[index].second;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_context_get_class(bbl_context_t ctx, bbl_classid_t id,
                                   bbl_class_t* cls) {
    if (ctx == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *cls = ctx->get_class(*id);
    if (*cls == nullptr) {
        return BBL_RESULT_NotFound;
    }

    return BBL_RESULT_Success;
}

bbl_result_t bbl_context_get_function(bbl_context_t ctx, bbl_functionid_t id,
                                      bbl_function_t* fun) {
    if (ctx == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *fun = ctx->get_function(*id);
    if (*fun == nullptr) {
        return BBL_RESULT_NotFound;
    }

    return BBL_RESULT_Success;
}

bbl_result_t bbl_context_get_stdfunction(bbl_context_t ctx,
                                         bbl_stdfunctionid_t id,
                                         bbl_stdfunction_t* fun) {
    if (ctx == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *fun = ctx->get_stdfunction(*id);
    if (*fun == nullptr) {
        return BBL_RESULT_NotFound;
    }

    return BBL_RESULT_Success;
}

bbl_result_t bbl_context_get_enum(bbl_context_t ctx, bbl_enumid_t id,
                                  bbl_enum_t* enm) {
    if (ctx == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *enm = ctx->get_enum(*id);
    if (*enm == nullptr) {
        return BBL_RESULT_NotFound;
    }

    return BBL_RESULT_Success;
}

bbl_result_t bbl_module_get_num_source_files(bbl_module_t module, size_t* num_source_files) {
    if (module == nullptr) {
        *num_source_files = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_source_files = module->source_files.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_module_get_source_file(bbl_module_t module, size_t index, char const** ptr,
                                        size_t* len) {
    if (module == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= module->source_files.size()) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *ptr = module->source_files[index].c_str();
    *len = module->source_files[index].size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_module_get_name(bbl_module_t module, char const** ptr,
                                 size_t* len) {
    if (module == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }
    *ptr = module->name.c_str();
    *len = module->name.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_module_num_classes(bbl_module_t module, size_t* num_classes) {
    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    *num_classes = module->classes.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_module_get_classid(bbl_module_t module, size_t index,
                                    bbl_classid_t* cls) {
    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= module->classes.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *cls = &module->classes[index];

    return BBL_RESULT_Success;
}

bbl_result_t bbl_module_num_functions(bbl_module_t module,
                                      size_t* num_functions) {
    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_functions = module->functions.size();

    return BBL_RESULT_Success;
}

bbl_result_t bbl_module_get_functionid(bbl_module_t module, size_t index,
                                       bbl_functionid_t* fun) {
    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    if (fun == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= module->functions.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *fun = &module->functions[index];

    return BBL_RESULT_Success;
}

bbl_result_t bbl_module_num_stdfunctions(bbl_module_t module,
                                         size_t* num_stdfunctions) {
    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_stdfunctions = module->stdfunctions.size();

    return BBL_RESULT_Success;
}

bbl_result_t bbl_module_get_stdfunctionid(bbl_module_t module, size_t index,
                                          bbl_stdfunctionid_t* fun) {
    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    if (fun == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= module->stdfunctions.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *fun = &module->stdfunctions[index];

    return BBL_RESULT_Success;
}

bbl_result_t bbl_module_num_enums(bbl_module_t module, size_t* num_enums) {
    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_enums = module->enums.size();

    return BBL_RESULT_Success;
}

bbl_result_t bbl_module_get_enumid(bbl_module_t module, size_t index,
                                 bbl_enumid_t* enm) {
    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    if (enm == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= module->enums.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *enm = &module->enums[index];

    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_get_qualified_name(bbl_class_t cls, char const** ptr,
                                          size_t* len) {
    if (cls == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = cls->qualified_name.c_str();
    *len = cls->qualified_name.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_get_spelling(bbl_class_t cls, char const** ptr,
                                    size_t* len) {
    if (cls == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = cls->spelling.c_str();
    *len = cls->spelling.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_get_name(bbl_class_t cls, char const** ptr,
                                size_t* len) {
    if (cls == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = cls->name.c_str();
    *len = cls->name.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_get_rename(bbl_class_t cls, char const** ptr,
                                  size_t* len) {
    if (cls == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = cls->rename.c_str();
    *len = cls->rename.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_get_layout(bbl_class_t cls, bbl_layout_t* layout) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    layout->size_bytes = cls->layout.size_bytes;
    layout->align_bytes = cls->layout.align_bytes;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_get_bind_kind(bbl_class_t cls,
                                     bbl_bind_kind_t* bind_kind) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    switch (cls->bind_kind) {
    case bbl::BindKind::OpaquePtr:
        *bind_kind = BBL_BIND_KIND_OpaquePtr;
        break;
    case bbl::BindKind::OpaqueBytes:
        *bind_kind = BBL_BIND_KIND_OpaqueBytes;
        break;
    case bbl::BindKind::ValueType:
        *bind_kind = BBL_BIND_KIND_ValueType;
        break;
    }

    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_num_methods(bbl_class_t cls, size_t* num_methods) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_methods = cls->methods.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_get_method(bbl_class_t cls, size_t index,
                                  bbl_method_t* method) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= cls->methods.size()) {
        method = nullptr;
        return BBL_RESULT_ArgumentOutOfRange;
    }

    std::string const& method_id = cls->methods[index];
    *method = cls->ctx->get_method(method_id);

    if (*method == nullptr) {
        return BBL_RESULT_NotFound;
    } else {
        return BBL_RESULT_Success;
    }
}


bbl_result_t bbl_class_is_copy_constructible(bbl_class_t cls, bool* is_copy_constructible) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_copy_constructible = cls->rule_of_seven.is_copy_constructible;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_is_nothrow_copy_constructible(bbl_class_t cls, bool* is_nothrow_copy_constructible) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_nothrow_copy_constructible = cls->rule_of_seven.is_nothrow_copy_constructible;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_is_move_constructible(bbl_class_t cls, bool* is_move_constructible) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_move_constructible = cls->rule_of_seven.is_move_constructible;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_is_nothrow_move_constructible(bbl_class_t cls, bool* is_nothrow_move_constructible) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_nothrow_move_constructible = cls->rule_of_seven.is_nothrow_move_constructible;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_is_copy_assignable(bbl_class_t cls, bool* is_copy_assignable) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_copy_assignable = cls->rule_of_seven.is_copy_assignable;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_is_nothrow_copy_assignable(bbl_class_t cls, bool* is_nothrow_copy_assignable) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_nothrow_copy_assignable = cls->rule_of_seven.is_nothrow_copy_assignable;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_is_move_assignable(bbl_class_t cls, bool* is_move_assignable) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_move_assignable = cls->rule_of_seven.is_move_assignable;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_is_nothrow_move_assignable(bbl_class_t cls, bool* is_nothrow_move_assignable) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_nothrow_move_assignable = cls->rule_of_seven.is_nothrow_move_assignable;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_is_destructible(bbl_class_t cls, bool* is_destructible) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_destructible = cls->rule_of_seven.is_destructible;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_has_virtual_destructor(bbl_class_t cls, bool* has_virtual_destructor) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *has_virtual_destructor = cls->rule_of_seven.has_virtual_destructor;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_is_abstract(bbl_class_t cls, bool* is_abstract) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_abstract = cls->is_abstract;
    return BBL_RESULT_Success;
}


bbl_result_t bbl_class_num_constructors(bbl_class_t cls, size_t* num_constructors) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_constructors = cls->constructors.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_class_get_constructor(bbl_class_t cls, size_t index,
                                  bbl_constructor_t* constructor) {
    if (cls == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= cls->constructors.size()) {
        constructor = nullptr;
        return BBL_RESULT_ArgumentOutOfRange;
    }

    std::string const& ctor_id = cls->constructors[index];
    *constructor = cls->ctx->get_constructor(ctor_id);

    if (*constructor == nullptr) {
        return BBL_RESULT_NotFound;
    } else {
        return BBL_RESULT_Success;
    }

    return BBL_RESULT_Success;
}


bbl_result_t bbl_constructor_get_is_noexcept(bbl_constructor_t constructor,
                                          bool* is_noexcept) {
    if (constructor == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_noexcept = constructor->is_noexcept;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_constructor_get_rename(bbl_constructor_t constructor, char const** ptr,
                                     size_t* len) {
    if (constructor == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = constructor->rename.c_str();
    *len = constructor->rename.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_constructor_num_params(bbl_constructor_t constructor,
                                     size_t* num_params) {
    if (constructor == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_params = constructor->params.size();

    return BBL_RESULT_Success;
}

bbl_result_t bbl_constructor_get_param(bbl_constructor_t constructor, size_t index,
                                    bbl_param_t* param) {
    if (constructor == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= constructor->params.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *param = &constructor->params[index];

    return BBL_RESULT_Success;
}

bbl_result_t bbl_function_get_qualified_name(bbl_function_t function,
                                             char const** ptr, size_t* len) {
    if (function == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = function->qualified_name.c_str();
    *len = function->qualified_name.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_function_get_name(bbl_function_t function, char const** ptr,
                                   size_t* len) {
    if (function == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = function->name.c_str();
    *len = function->name.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_function_get_is_noexcept(bbl_function_t function,
                                          bool* is_noexcept) {
    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_noexcept = function->is_noexcept;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_function_get_rename(bbl_function_t function, char const** ptr,
                                     size_t* len) {
    if (function == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = function->rename.c_str();
    *len = function->rename.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_function_get_return_type(bbl_function_t function,
                                          bbl_qtype_t* return_type) {
    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    *return_type = &function->return_type;

    return BBL_RESULT_Success;
}

bbl_result_t bbl_function_num_params(bbl_function_t function,
                                     size_t* num_params) {
    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_params = function->params.size();

    return BBL_RESULT_Success;
}

bbl_result_t bbl_function_get_param(bbl_function_t function, size_t index,
                                    bbl_param_t* param) {
    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= function->params.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *param = &function->params[index];

    return BBL_RESULT_Success;
}

bbl_result_t bbl_stdfunction_get_spelling(bbl_stdfunction_t function, char const** ptr,
                                     size_t* len) {
    if (function == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = function->spelling.c_str();
    *len = function->spelling.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_stdfunction_get_return_type(bbl_stdfunction_t function,
                                          bbl_qtype_t* return_type) {
    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    *return_type = &function->return_type;

    return BBL_RESULT_Success;
}

bbl_result_t bbl_stdfunction_num_params(bbl_stdfunction_t function,
                                     size_t* num_params) {
    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_params = function->params.size();

    return BBL_RESULT_Success;
}

bbl_result_t bbl_stdfunction_get_param(bbl_stdfunction_t function, size_t index,
                                    bbl_qtype_t* param) {
    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= function->params.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *param = &function->params[index];

    return BBL_RESULT_Success;
}

bbl_result_t bbl_method_get_qualified_name(bbl_method_t method,
                                           char const** ptr, size_t* len) {
    if (method == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }
    return bbl_function_get_qualified_name(&method->function, ptr, len);
}

bbl_result_t bbl_method_get_name(bbl_method_t method, char const** ptr,
                                 size_t* len) {
    if (method == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }
    return bbl_function_get_name(&method->function, ptr, len);
}

bbl_result_t bbl_method_get_rename(bbl_method_t method, char const** ptr,
                                   size_t* len) {
    if (method == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }
    return bbl_function_get_rename(&method->function, ptr, len);
}

bbl_result_t bbl_method_get_is_noexcept(bbl_method_t method,
                                        bool* is_noexcept) {
    if (method == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    return bbl_function_get_is_noexcept(&method->function, is_noexcept);
}

bbl_result_t bbl_method_get_return_type(bbl_method_t method,
                                        bbl_qtype_t* return_type) {
    if (method == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    return bbl_function_get_return_type(&method->function, return_type);
}

bbl_result_t bbl_method_num_params(bbl_method_t method, size_t* num_params) {
    if (method == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    return bbl_function_num_params(&method->function, num_params);
}

bbl_result_t bbl_method_get_param(bbl_method_t method, size_t index,
                                  bbl_param_t* param) {
    if (method == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    return bbl_function_get_param(&method->function, index, param);
}

bbl_result_t bbl_method_get_is_const(bbl_method_t method, bool* is_const) {
    if (method == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_const = method->is_const;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_method_get_is_static(bbl_method_t method, bool* is_static) {
    if (method == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_static = method->is_static;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_method_get_is_virtual(bbl_method_t method, bool* is_virtual) {
    if (method == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_virtual = method->is_virtual;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_method_get_is_pure(bbl_method_t method, bool* is_pure) {
    if (method == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_pure = method->is_pure;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_method_get_function(bbl_method_t method, bbl_function_t* function) {
    if (method == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *function = &method->function;

    return BBL_RESULT_Success;
}

bbl_result_t bbl_enum_get_spelling(bbl_enum_t enm, char const** ptr,
                                   size_t* len) {
    if (enm == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = enm->spelling.c_str();
    *len = enm->spelling.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_enum_get_rename(bbl_enum_t enm, char const** ptr,
                                 size_t* len) {
    if (enm == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = enm->rename.c_str();
    *len = enm->rename.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_enum_num_variants(bbl_enum_t enm, size_t* num_variants) {
    if (enm == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_variants = enm->variants.size();

    return BBL_RESULT_Success;
}

bbl_result_t bbl_enum_get_variant(bbl_enum_t enm, size_t index,
                                  char const** name_ptr, size_t* name_len,
                                  char const** value_ptr, size_t* value_len) {
    if (enm == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= enm->variants.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    auto const& var = enm->variants[index];

    *name_ptr = var.first.c_str();
    *name_len = var.first.size();
    *value_ptr = var.second.c_str();
    *value_len = var.second.size();

    return BBL_RESULT_Success;
}

bbl_result_t bbl_enum_get_type(bbl_enum_t enm, bbl_qtype_t* type) {
    if (enm == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *type = &enm->integer_type;

    return BBL_RESULT_Success;
}

bbl_result_t bbl_param_get_name(bbl_param_t param, char const** ptr,
                                size_t* len) {
    if (param == nullptr) {
        *ptr = nullptr;
        *len = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = param->name.c_str();
    *len = param->name.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_param_get_type(bbl_param_t param, bbl_qtype_t* type) {
    if (param == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *type = &param->type;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_qtype_is_const(bbl_qtype_t qtype, bool* is_const) {
    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }
    *is_const = qtype->is_const;

    return BBL_RESULT_Success;
}

bbl_result_t bbl_qtype_get_type_kind(bbl_qtype_t qtype,
                                     bbl_type_kind_t* type_kind) {
    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::Type>(qtype->type)) {
        auto const& ty = std::get<bbl::Type>(qtype->type);
        if (std::holds_alternative<bbl_builtin_t>(ty.kind)) {
            *type_kind = BBL_TYPEKIND_Builtin;
        } else if (std::holds_alternative<bbl::ClassId>(ty.kind)) {
            *type_kind = BBL_TYPEKIND_Class;
        } else if (std::holds_alternative<bbl::ClassTemplateSpecializationId>(
                       ty.kind)) {
            *type_kind = BBL_TYPEKIND_ClassTemplateSpecialization;
        } else if (std::holds_alternative<bbl::EnumId>(ty.kind)) {
            *type_kind = BBL_TYPEKIND_Enum;
        } else if (std::holds_alternative<bbl::StdFunctionId>(ty.kind)) {
            *type_kind = BBL_TYPEKIND_StdFunction;
        } else {
            return BBL_RESULT_UnknownKind;
        }
    } else if (std::holds_alternative<bbl::Pointer>(qtype->type)) {
        *type_kind = BBL_TYPEKIND_Pointer;
    } else if (std::holds_alternative<bbl::LValueReference>(qtype->type)) {
        *type_kind = BBL_TYPEKIND_LValueReference;
    } else if (std::holds_alternative<bbl::RValueReference>(qtype->type)) {
        *type_kind = BBL_TYPEKIND_RValueReference;
    } else if (std::holds_alternative<bbl::Array>(qtype->type)) {
        *type_kind = BBL_TYPEKIND_Array;
    } else {
        return BBL_RESULT_UnknownKind;
    }

    return BBL_RESULT_Success;
}

bbl_result_t bbl_qtype_get_as_builtin(bbl_qtype_t qtype,
                                      bbl_builtin_t* builtin) {
    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::Type>(qtype->type)) {
        auto const& ty = std::get<bbl::Type>(qtype->type);
        if (std::holds_alternative<bbl_builtin_t>(ty.kind)) {
            *builtin = std::get<bbl_builtin_t>(ty.kind);
            return BBL_RESULT_Success;
        }
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_qtype_get_as_classid(bbl_qtype_t qtype,
                                      bbl_classid_t* classid) {
    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::Type>(qtype->type)) {
        auto const& ty = std::get<bbl::Type>(qtype->type);
        if (std::holds_alternative<bbl::ClassId>(ty.kind) ||
            std::holds_alternative<bbl::ClassTemplateSpecializationId>(
                ty.kind)) {
            *classid = &std::get<bbl::ClassId>(ty.kind).id;
            return BBL_RESULT_Success;
        }
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t
bbl_qtype_get_as_stdfunctionid(bbl_qtype_t qtype,
                               bbl_stdfunctionid_t* stdfunctionid) {
    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::Type>(qtype->type)) {
        auto const& ty = std::get<bbl::Type>(qtype->type);
        if (std::holds_alternative<bbl::StdFunctionId>(ty.kind)) {
            *stdfunctionid = &std::get<bbl::StdFunctionId>(ty.kind).id;
            return BBL_RESULT_Success;
        }
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_qtype_get_as_enumid(bbl_qtype_t qtype, bbl_enumid_t* enumid) {
    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::Type>(qtype->type)) {
        auto const& ty = std::get<bbl::Type>(qtype->type);
        if (std::holds_alternative<bbl::EnumId>(ty.kind)) {
            *enumid = &std::get<bbl::EnumId>(ty.kind).id;
            return BBL_RESULT_Success;
        }
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_qtype_get_pointee_type(bbl_qtype_t qtype,
                                        bbl_qtype_t* pointee_type) {
    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::Pointer>(qtype->type)) {
        *pointee_type = std::get<bbl::Pointer>(qtype->type).pointee.get();
        return BBL_RESULT_Success;
    } else if (std::holds_alternative<bbl::LValueReference>(qtype->type)) {
        *pointee_type =
            std::get<bbl::LValueReference>(qtype->type).pointee.get();
        return BBL_RESULT_Success;
    } else if (std::holds_alternative<bbl::RValueReference>(qtype->type)) {
        *pointee_type =
            std::get<bbl::RValueReference>(qtype->type).pointee.get();
        return BBL_RESULT_Success;
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_qtype_get_array_element_type(bbl_qtype_t qtype,
                                        bbl_qtype_t* element_type) {
    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::Array>(qtype->type)) {
        *element_type = std::get<bbl::Array>(qtype->type).element_type.get();
        return BBL_RESULT_Success;
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_qtype_get_array_size(bbl_qtype_t qtype, size_t* size) {
    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::Array>(qtype->type)) {
        *size = std::get<bbl::Array>(qtype->type).size;
        return BBL_RESULT_Success;
    }

    return BBL_RESULT_WrongKind;
}

}