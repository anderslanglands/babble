#include "bbl-c.hpp"
#include "bbl_detail.h"
#include "spdlog/spdlog.h"
#include <exception>

extern "C" {

using bbl_capi_t = bbl::C_API*;
using bbl_context_t = bbl::Context*;

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


}