#include "../capi.hpp"
#include "bbl-detail.h"
#include "spdlog/spdlog.h"
#include <exception>
#include <variant>

extern "C" {

using bbl_capi_t = bbl::C_API*;
using bbl_context_t = bbl::Context*;
using bbl_method_t = bbl::Method const*;
using bbl_constructor_t = bbl::Constructor const*;
using bbl_function_t = bbl::Function const*;

using bbl_capi_module_t = bbl::C_Module const*;
using bbl_capi_struct_t = bbl::C_Struct const*;
using bbl_capi_function_t = bbl::C_Function const*;
using bbl_capi_stdfunction_t = bbl::C_StdFunction const*;
using bbl_capi_enum_t = bbl::C_Enum const*;
using bbl_capi_field_t = bbl::C_Field const*;
using bbl_capi_qtype_t = bbl::C_QType const*;
using bbl_capi_param_t = bbl::C_Param const*;
using bbl_capi_inclusion_t = bbl::Inclusion const*;

using bbl_capi_structid_t = std::string const*;
using bbl_capi_enumid_t = std::string const*;
using bbl_capi_functionid_t = std::string const*;
using bbl_capi_stdfunctionid_t = std::string const*;


using bbl_class_t = bbl::Class const*;

bbl_result_t bbl_capi_create(bbl_context_t cpp_context, bbl_capi_t* capi) {
    try {
        *capi = new bbl::C_API(*cpp_context);
        return BBL_RESULT_Success;
    } catch (std::exception& e) {
        SPDLOG_ERROR("{}", e.what());
        *capi = nullptr;
        return BBL_RESULT_UnknownError;
    }
}

bbl_result_t bbl_capi_destroy(bbl_capi_t capi) {
    delete capi;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_header(bbl_capi_t capi, char const** str, size_t* len) {
    static std::string result_str;
    try {
        result_str = capi->get_header();
        *str = result_str.c_str();
        *len = result_str.size();
        return BBL_RESULT_Success;
    } catch (std::exception& e) {
        *str = "";
        *len = 0;
        SPDLOG_ERROR("{}", e.what());
        return BBL_RESULT_UnknownError;
    }
}

bbl_result_t bbl_capi_get_source(bbl_capi_t capi, char const** str, size_t* len) {
    static std::string result_str;
    try {
        result_str = capi->get_source();
        *str = result_str.c_str();
        *len = result_str.size();
        return BBL_RESULT_Success;
    } catch (std::exception& e) {
        *str = "";
        *len = 0;
        SPDLOG_ERROR("{}", e.what());
        return BBL_RESULT_UnknownError;
    }
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_num_modules(bbl_capi_t capi, size_t* num_modules) {
    if (capi == nullptr) {
        *num_modules = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_modules = capi->get_modules().size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_module(bbl_capi_t capi, size_t index, bbl_capi_module_t* module) {
    *module = nullptr;

    if (capi == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= capi->get_modules().size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *module = &capi->get_modules()[index];
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_module_get_name(bbl_capi_module_t module, char const** ptr, size_t* len) {
    *ptr = nullptr;
    *len = 0;

    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = module->name.c_str();
    *len = module->name.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_module_get_num_inclusions(bbl_capi_module_t module, size_t* num_inclusions) {
    *num_inclusions = 0;

    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_inclusions = module->inclusions.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_module_get_inclusion(bbl_capi_module_t module, size_t index, bbl_capi_inclusion_t* inclusion) {
    *inclusion = nullptr;

    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= module->inclusions.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *inclusion = &module->inclusions[index];
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_module_get_num_structs(bbl_capi_module_t module, size_t* num_structs) {
    *num_structs = 0;

    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_structs = module->structs.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_module_get_struct(bbl_capi_module_t module, size_t index, bbl_capi_structid_t* structid) {
    *structid = nullptr;

    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= module->structs.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *structid = &module->structs[index];
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_module_get_num_functions(bbl_capi_module_t module, size_t* num_functions) {
    *num_functions = 0;

    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_functions = module->functions.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_module_get_function(bbl_capi_module_t module, size_t index, bbl_capi_functionid_t* functionid) {
    *functionid = nullptr;

    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= module->functions.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *functionid = &module->functions[index];
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_module_get_num_stdfunctions(bbl_capi_module_t module, size_t* num_stdfunctions) {
    *num_stdfunctions = 0;

    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_stdfunctions = module->stdfunctions.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_module_get_stdfunction(bbl_capi_module_t module, size_t index, bbl_capi_stdfunctionid_t* stdfunctionid) {
    *stdfunctionid = nullptr;

    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= module->stdfunctions.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *stdfunctionid = &module->stdfunctions[index];
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_module_get_num_enums(bbl_capi_module_t module, size_t* num_enums) {
    *num_enums = 0;

    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_enums = module->enums.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_module_get_enum(bbl_capi_module_t module, size_t index, bbl_capi_enumid_t* enumid) {
    *enumid = nullptr;

    if (module == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= module->enums.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *enumid = &module->enums[index];
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_inclusion_get_directive(bbl_capi_inclusion_t inclusion, char const** ptr, size_t* len) {
    *ptr = nullptr;
    *len = 0;

    if (inclusion == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = inclusion->directive.c_str();
    *len = inclusion->directive.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_inclusion_get_filename(bbl_capi_inclusion_t inclusion, char const** ptr, size_t* len) {
    *ptr = nullptr;
    *len = 0;

    if (inclusion == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = inclusion->filename.c_str();
    *len = inclusion->filename.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_inclusion_is_local(bbl_capi_inclusion_t inclusion, bool* is_local) {
    if (inclusion == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_local = inclusion->is_local;
    return BBL_RESULT_Success;
}


bbl_result_t bbl_capi_get_num_structs(bbl_capi_t capi, size_t* num_structs) {
    if (capi == nullptr) {
        *num_structs = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_structs = capi->get_structs().size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_struct(bbl_capi_t capi, bbl_capi_structid_t id, bbl_capi_struct_t* strct) {
    *strct = nullptr;

    if (capi == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    try {
        *strct = &capi->get_structs().at(*id);
    } catch (std::exception& e) {
        SPDLOG_ERROR("struct id {} not found", *id);
        return BBL_RESULT_NotFound;
    }

    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_struct_at_index(bbl_capi_t capi, size_t index, bbl_capi_struct_t* strct) {
    *strct = nullptr;

    if (capi == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= capi->get_structs().size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *strct = &capi->get_structs().values()[index].second;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_num_functions(bbl_capi_t capi, size_t* num_functions) {
    if (capi == nullptr) {
        *num_functions = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_functions = capi->get_functions().size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_function_at_index(bbl_capi_t capi, size_t index, bbl_capi_function_t* function) {
    *function = nullptr;

    if (capi == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= capi->get_functions().size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *function = &capi->get_functions().values()[index].second;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_function(bbl_capi_t capi, bbl_capi_functionid_t id, bbl_capi_function_t* function) {
    *function = nullptr;

    if (capi == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    try {
        *function = &capi->get_functions().at(*id);
    } catch (std::exception& e) {
        SPDLOG_ERROR("function id {} not found", *id);
        return BBL_RESULT_NotFound;
    }

    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_num_stdfunctions(bbl_capi_t capi, size_t* num_stdfunctions) {
    if (capi == nullptr) {
        *num_stdfunctions = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_stdfunctions = capi->get_stdfunctions().size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_stdfunction_at_index(bbl_capi_t capi, size_t index, bbl_capi_stdfunction_t* stdfunction) {
    *stdfunction = nullptr;

    if (capi == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= capi->get_stdfunctions().size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *stdfunction = &capi->get_stdfunctions().values()[index].second;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_stdfunction(bbl_capi_t capi, bbl_capi_stdfunctionid_t id, bbl_capi_stdfunction_t* stdfunction) {
    *stdfunction = nullptr;

    if (capi == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    try {
        *stdfunction = &capi->get_stdfunctions().at(*id);
    } catch (std::exception& e) {
        SPDLOG_ERROR("stdfunction {} not found", *id);
        return BBL_RESULT_NotFound;
    }
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_num_enums(bbl_capi_t capi, size_t* num_enums) {
    if (capi == nullptr) {
        *num_enums = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_enums = capi->get_enums().size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_enum(bbl_capi_t capi, bbl_capi_enumid_t id, bbl_capi_enum_t* enm) {
    *enm = nullptr;

    if (capi == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    try {
        *enm = &capi->get_enums().at(*id);
    } catch (std::exception& e) {
        SPDLOG_ERROR("enum {} not found", *id);
        return BBL_RESULT_NotFound;
    }
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_enum_at_index(bbl_capi_t capi, size_t index, bbl_capi_enum_t* enm) {
    *enm = nullptr;

    if (capi == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= capi->get_enums().size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *enm = &capi->get_enums().values()[index].second;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_struct_get_cpp(bbl_capi_struct_t strct, bbl_class_t* cls) {
    *cls = nullptr;

    if (strct == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *cls = &strct->cls;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_struct_get_bind_kind(bbl_capi_struct_t strct, bbl_bind_kind_t* bind_kind) {
    if (strct == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    switch (strct->cls.bind_kind) {
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

bbl_result_t bbl_capi_struct_get_name(bbl_capi_struct_t strct, char const** ptr, size_t* len) {
    *ptr = nullptr;
    *len = 0;

    if (strct == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = strct->name.c_str();
    *len = strct->name.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_struct_get_num_fields(bbl_capi_struct_t strct, size_t* num_fields) {
    *num_fields = 0;

    if (strct == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_fields = strct->fields.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_struct_get_field(bbl_capi_struct_t strct, size_t index, bbl_capi_field_t* field) {
    *field = nullptr;

    if (strct == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= strct->fields.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *field = &strct->fields[index];
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_field_get_type(bbl_capi_field_t field, bbl_capi_qtype_t* type) {
    *type = nullptr;

    if (field == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *type = &field->type;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_field_get_name(bbl_capi_field_t field, char const** ptr, size_t* len) {
    *ptr = nullptr;
    *len = 0;

    if (field == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = field->name.c_str();
    *len = field->name.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_param_get_type(bbl_capi_param_t param, bbl_capi_qtype_t* type) {
    *type = nullptr;

    if (param == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *type = &param->type;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_param_get_name(bbl_capi_param_t param, char const** ptr, size_t* len) {
    *ptr = nullptr;
    *len = 0;

    if (param == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = param->name.c_str();
    *len = param->name.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_function_get_source(bbl_capi_function_t fun, bbl_function_source_t* source) {
    if (fun == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::Method const*>(fun->method_or_function)) {
        *source = bbl_function_source_Method;
    } else if (std::holds_alternative<bbl::Function const*>(fun->method_or_function)) {
        *source = bbl_function_source_Function;
    } else if (std::holds_alternative<bbl::Constructor const*>(fun->method_or_function)) {
        *source = bbl_function_source_Constructor;
    } else if (std::holds_alternative<bbl::Generated>(fun->method_or_function)) {
        *source = bbl_function_source_Generated;
    } else {
        return BBL_RESULT_UnknownKind;
    }

    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_function_get_cpp_method(bbl_capi_function_t fun, bbl_method_t* cpp_method) {
    *cpp_method = nullptr;

    if (fun == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::Method const*>(fun->method_or_function)) {
        *cpp_method = std::get<bbl::Method const*>(fun->method_or_function);
        return BBL_RESULT_Success;
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_capi_function_get_cpp_constructor(bbl_capi_function_t fun, bbl_constructor_t* cpp_ctor) {
    *cpp_ctor = nullptr;

    if (fun == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::Constructor const*>(fun->method_or_function)) {
        *cpp_ctor = std::get<bbl::Constructor const*>(fun->method_or_function);
        return BBL_RESULT_Success;
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_capi_function_get_cpp_function(bbl_capi_function_t fun, bbl_function_t* cpp_fun) {
    *cpp_fun = nullptr;

    if (fun == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::Function const*>(fun->method_or_function)) {
        *cpp_fun = std::get<bbl::Function const*>(fun->method_or_function);
        return BBL_RESULT_Success;
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_capi_function_get_name(bbl_capi_function_t function, char const** ptr, size_t* len) {
    *ptr = nullptr;
    *len = 0;

    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = function->name.c_str();
    *len = function->name.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_function_get_result_param(bbl_capi_function_t function, bbl_capi_param_t* result_param) {
    *result_param = nullptr;

    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (function->result.has_value()) {
        *result_param = &*function->result;
        return BBL_RESULT_Success;
    } else {
        return BBL_RESULT_NotFound;
    }
}

bbl_result_t bbl_capi_function_get_receiver_param(bbl_capi_function_t function, bbl_capi_param_t* receiver_param) {
    *receiver_param = nullptr;

    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::C_Param>(function->receiver)) {
        auto const& param = std::get<bbl::C_Param>(function->receiver);
        *receiver_param = &param;
        return BBL_RESULT_Success;
    } else if (std::holds_alternative<bbl::C_SmartPtr>(function->receiver)) {
        auto const& param = std::get<bbl::C_SmartPtr>(function->receiver);
        *receiver_param = &param.smartptr;
        return BBL_RESULT_Success;
    } else {
        return BBL_RESULT_NotFound;
    }
}

bbl_result_t bbl_capi_function_get_num_params(bbl_capi_function_t function, size_t* num_params) {
    *num_params = 0;

    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_params = function->params.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_function_get_param(bbl_capi_function_t function, size_t index, bbl_capi_param_t* param) {
    *param = nullptr;

    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= function->params.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *param = &function->params[index];
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_function_get_body(bbl_capi_function_t function, char const** ptr, size_t* len) {
    static std::string result_str;

    *ptr = nullptr;
    *len = 0;

    if (function == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    result_str = function->body->to_string(1);

    *ptr = result_str.c_str();
    *len = result_str.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_stdfunction_get_name(bbl_capi_stdfunction_t stdfunction, char const** ptr, size_t* len) {
    *ptr = nullptr;
    *len = 0;

    if (stdfunction == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = stdfunction->rename.c_str();
    *len = stdfunction->rename.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_stdfunction_get_result_param(bbl_capi_stdfunction_t stdfunction, bbl_capi_param_t* result_param) {
    *result_param = nullptr;

    if (stdfunction == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (stdfunction->return_type.has_value()) {
        *result_param = &*stdfunction->return_type;
        return BBL_RESULT_Success;
    } else {
        return BBL_RESULT_NotFound;
    }
}

bbl_result_t bbl_capi_stdfunction_get_num_params(bbl_capi_stdfunction_t stdfunction, size_t* num_params) {
    *num_params = 0;

    if (stdfunction == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_params = stdfunction->params.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_stdfunction_get_param(bbl_capi_stdfunction_t stdfunction, size_t index, bbl_capi_param_t* param) {
    *param = nullptr;

    if (stdfunction == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= stdfunction->params.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *param = &stdfunction->params[index];
    return BBL_RESULT_Success;
}


bbl_result_t bbl_capi_enum_get_name(bbl_capi_enum_t enm, char const** ptr, size_t* len) {
    *ptr = nullptr;
    *len = 0;

    if (enm == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *ptr = enm->name.c_str();
    *len = enm->name.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_enum_get_num_variants(bbl_capi_enum_t enm,
                                            size_t* num_variants) {
    *num_variants = 0;

    if (enm == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_variants = enm->variants.size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_enum_get_variant(bbl_capi_enum_t enm,
                                       size_t index,
                                       char const** name,
                                       size_t* name_len,
                                       char const** value,
                                       size_t* value_len) {
    *name = nullptr;
    *name_len = 0;
    *value = nullptr;
    *value_len = 0;

    if (enm == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= enm->variants.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *name = enm->variants[index].first.c_str();
    *name_len = enm->variants[index].first.size();
    *value = enm->variants[index].second.c_str();
    *value_len = enm->variants[index].second.size();

    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_enum_get_original_variant(bbl_capi_enum_t enm,
                                       size_t index,
                                       char const** name,
                                       size_t* name_len,
                                       char const** value,
                                       size_t* value_len) {
    *name = nullptr;
    *name_len = 0;
    *value = nullptr;
    *value_len = 0;

    if (enm == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (index >= enm->original_variants.size()) {
        return BBL_RESULT_ArgumentOutOfRange;
    }

    *name = enm->original_variants[index].first.c_str();
    *name_len = enm->original_variants[index].first.size();
    *value = enm->original_variants[index].second.c_str();
    *value_len = enm->original_variants[index].second.size();

    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_enum_get_underlying_type(bbl_capi_enum_t enm, bbl_builtin_t* type) {
    if (enm == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *type = enm->integer_type;

    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_qtype_is_const(bbl_capi_qtype_t qtype, bool* is_const) {
    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    *is_const = qtype->is_const;
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_qtype_get_type_kind(bbl_capi_qtype_t qtype, bbl_capi_type_kind_t* type_kind) {
    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::C_Type>(qtype->type)) {
        bbl::C_Type const& type = std::get<bbl::C_Type>(qtype->type);
        if (std::holds_alternative<bbl::C_StructId>(type.kind)) {
            *type_kind = bbl_capi_typekind_Struct;
            return BBL_RESULT_Success;
        } else if (std::holds_alternative<bbl::C_EnumId>(type.kind)) {
            *type_kind = bbl_capi_typekind_Enum;
            return BBL_RESULT_Success;
        } else if (std::holds_alternative<bbl::C_StdFunctionId>(type.kind)) {
            *type_kind = bbl_capi_typekind_StdFunction;
            return BBL_RESULT_Success;
        } else if (std::holds_alternative<bbl_builtin_t>(type.kind)) {
            *type_kind = bbl_capi_typekind_Builtin;
            return BBL_RESULT_Success;
        }

    } else if (std::holds_alternative<bbl::C_Array>(qtype->type)) {
        *type_kind = bbl_capi_typekind_Array;
        return BBL_RESULT_Success;
    } else if (std::holds_alternative<bbl::C_Pointer>(qtype->type)) {
        *type_kind = bbl_capi_typekind_Pointer;
        return BBL_RESULT_Success;
    }

    return BBL_RESULT_UnknownKind;
}

bbl_result_t bbl_capi_qtype_get_as_builtin(bbl_capi_qtype_t qtype, bbl_builtin_t* builtin) {
    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::C_Type>(qtype->type)) {
        bbl::C_Type const& type = std::get<bbl::C_Type>(qtype->type);
        if (std::holds_alternative<bbl_builtin_t>(type.kind)) {
            *builtin = std::get<bbl_builtin_t>(type.kind);
            return BBL_RESULT_Success;
        }
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_capi_qtype_get_as_structid(bbl_capi_qtype_t qtype, bbl_capi_structid_t* structid) {
    *structid = nullptr;

    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::C_Type>(qtype->type)) {
        bbl::C_Type const& type = std::get<bbl::C_Type>(qtype->type);
        if (std::holds_alternative<bbl::C_StructId>(type.kind)) {
            *structid = &std::get<bbl::C_StructId>(type.kind).id;
            return BBL_RESULT_Success;
        }
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_capi_qtype_get_as_stdfunctionid(bbl_capi_qtype_t qtype, bbl_capi_stdfunctionid_t* stdfunctionid) {
    *stdfunctionid = nullptr;

    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::C_Type>(qtype->type)) {
        bbl::C_Type const& type = std::get<bbl::C_Type>(qtype->type);
        if (std::holds_alternative<bbl::C_StdFunctionId>(type.kind)) {
            *stdfunctionid = &std::get<bbl::C_StdFunctionId>(type.kind).id;
            return BBL_RESULT_Success;
        }
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_capi_qtype_get_as_enumid(bbl_capi_qtype_t qtype, bbl_capi_enumid_t* enumid) {
    *enumid = nullptr;

    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::C_Type>(qtype->type)) {
        bbl::C_Type const& type = std::get<bbl::C_Type>(qtype->type);
        if (std::holds_alternative<bbl::C_EnumId>(type.kind)) {
            *enumid = &std::get<bbl::C_EnumId>(type.kind).id;
            return BBL_RESULT_Success;
        }
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_capi_qtype_get_pointee_type(bbl_capi_qtype_t qtype, bbl_capi_qtype_t* pointee_type) {
    *pointee_type = nullptr;

    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::C_Pointer>(qtype->type)) {
        bbl::C_Pointer const& pointer = std::get<bbl::C_Pointer>(qtype->type);
        *pointee_type = pointer.pointee.get();
        return BBL_RESULT_Success;
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_capi_qtype_get_array_element_type(bbl_capi_qtype_t qtype, bbl_capi_qtype_t* element_type) {
    *element_type = nullptr;

    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::C_Array>(qtype->type)) {
        bbl::C_Array const& array = std::get<bbl::C_Array>(qtype->type);
        *element_type = array.element_type.get();
        return BBL_RESULT_Success;
    }

    return BBL_RESULT_WrongKind;
}

bbl_result_t bbl_capi_qtype_get_array_size(bbl_capi_qtype_t qtype, size_t* size) {
    *size = 0;

    if (qtype == nullptr) {
        return BBL_RESULT_ArgumentIsNull;
    }

    if (std::holds_alternative<bbl::C_Array>(qtype->type)) {
        bbl::C_Array const& array = std::get<bbl::C_Array>(qtype->type);
        *size = array.size;
        return BBL_RESULT_Success;
    }

    return BBL_RESULT_WrongKind;
}

}