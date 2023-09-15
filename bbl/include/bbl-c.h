#ifndef BBL_BBL_C_H
#define BBL_BBL_C_H

#include "bbl.h"
#include "bbl_detail.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bbl_capi_instance_t* bbl_capi_t;

/**
 * @brief Create a C API from the C++ context.
 *
 * This translates the C++ API to C. If this function returns successfully, the
 * C header and source can then be queried using `bbl_capi_get_header()` and
 * `bbl_capi_get_source()`
 *
 * The given cpp_context object must outlive the returned capi object
 *
 * @param cpp_context
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_create(bbl_context_t cpp_context, bbl_capi_t* capi);

/**
 * @brief Destroy a previously created C API object
 *
 * @param capi
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_destroy(bbl_capi_t capi);

bbl_result_t bbl_capi_get_header(bbl_capi_t capi, char const** str, size_t* len);
bbl_result_t bbl_capi_get_source(bbl_capi_t capi, char const** str, size_t* len);

#ifdef __cplusplus
}
#endif

#endif