#include "bbl-c.hpp"
#include "bbl_detail.h"
#include "spdlog/spdlog.h"
#include <exception>

extern "C" {

using bbl_capi_t = bbl::C_API*;
using bbl_context_t = bbl::Context*;

using bbl_capi_module_t = bbl::C_Module const*;
using bbl_capi_struct_t = bbl::C_Struct const*;
using bbl_capi_function_t = bbl::C_Function const*;
using bbl_capi_stdfunction_t = bbl::C_StdFunction const*;
using bbl_capi_enum_t = bbl::C_Enum const*;
using bbl_capi_field_t = bbl::C_Field const*;
using bbl_class_t = bbl::Class const*;

bbl_result_t bbl_capi_create(bbl_context_t cpp_context, bbl_capi_t* capi) {
    try {
        *capi = new bbl::C_API(*cpp_context);
        return BBL_RESULT_Success;
    } catch (std::exception& e) {
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

bbl_result_t bbl_capi_get_num_modules(bbl_capi_t capi, size_t* num_modules) {
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

bbl_result_t bbl_capi_get_num_structs(bbl_capi_t capi, size_t* num_structs) {
    if (capi == nullptr) {
        *num_structs = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_structs = capi->get_structs().size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_struct(bbl_capi_t capi, size_t index, bbl_capi_struct_t* strct) {
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

bbl_result_t bbl_capi_get_function(bbl_capi_t capi, size_t index, bbl_capi_function_t* function) {
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

bbl_result_t bbl_capi_get_num_stdfunctions(bbl_capi_t capi, size_t* num_stdfunctions) {
    if (capi == nullptr) {
        *num_stdfunctions = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_stdfunctions = capi->get_stdfunctions().size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_stdfunction(bbl_capi_t capi, size_t index, bbl_capi_stdfunction_t* stdfunction) {
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

bbl_result_t bbl_capi_get_num_enums(bbl_capi_t capi, size_t* num_enums) {
    if (capi == nullptr) {
        *num_enums = 0;
        return BBL_RESULT_ArgumentIsNull;
    }

    *num_enums = capi->get_enums().size();
    return BBL_RESULT_Success;
}

bbl_result_t bbl_capi_get_enum(bbl_capi_t capi, size_t index, bbl_capi_enum_t* enm) {
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


}