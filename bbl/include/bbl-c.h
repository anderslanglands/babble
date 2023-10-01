#ifndef BBL_BBL_C_H
#define BBL_BBL_C_H

#include "bbl.h"
#include "bbl_detail.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bbl_capi_instance_t* bbl_capi_t;
typedef struct bbl_capi_module_instance* bbl_capi_module_t;
typedef struct bbl_capi_struct_instance* bbl_capi_struct_t;
typedef struct bbl_capi_function_instance* bbl_capi_function_t;
typedef struct bbl_capi_stdfunction_instance* bbl_capi_stdfunction_t;
typedef struct bbl_capi_enum_instance* bbl_capi_enum_t;
typedef struct bbl_capi_field_instance* bbl_capi_field_t;

/**
 * @brief Create a C API from the C++ context.
 *
 * This translates the C++ API to C. If this function returns successfully, the
 * C header and source can then be generated using `bbl_capi_get_header()` and
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

/**
 * @brief Get the complete C header for the given API
 * 
 * @param capi 
 * @param str 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_header(bbl_capi_t capi, char const** str, size_t* len);

/**
 * @brief Get the complete C source for the given API
 * 
 * @param capi 
 * @param str 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_source(bbl_capi_t capi, char const** str, size_t* len);

bbl_result_t bbl_capi_get_num_modules(bbl_capi_t capi, size_t* num_modules);
bbl_result_t bbl_capi_get_module(bbl_capi_t capi, size_t index, bbl_capi_module_t* module);

bbl_result_t bbl_capi_get_num_structs(bbl_capi_t capi, size_t* num_structs);
bbl_result_t bbl_capi_get_struct(bbl_capi_t capi, size_t index, bbl_capi_struct_t* strct);

bbl_result_t bbl_capi_get_num_functions(bbl_capi_t capi, size_t* num_functions);
bbl_result_t bbl_capi_get_function(bbl_capi_t capi, size_t index, bbl_capi_function_t* function);

bbl_result_t bbl_capi_get_num_stdfunctions(bbl_capi_t capi, size_t* num_stdfunctions);
bbl_result_t bbl_capi_get_stdfunction(bbl_capi_t capi, size_t index, bbl_capi_stdfunction_t* stdfunction);

bbl_result_t bbl_capi_get_num_enums(bbl_capi_t capi, size_t* num_enums);
bbl_result_t bbl_capi_get_enum(bbl_capi_t capi, size_t index, bbl_capi_enum_t* enm);

bbl_result_t bbl_capi_struct_get_cpp(bbl_capi_struct_t strct, bbl_class_t* cls);
bbl_result_t bbl_capi_struct_get_name(bbl_capi_struct_t strct, char const** ptr, size_t* len);
bbl_result_t bbl_capi_struct_get_num_fields(bbl_capi_struct_t strct, size_t* num_fields);
bbl_result_t bbl_capi_struct_get_field(bbl_capi_struct_t strct, size_t index, bbl_capi_field_t* field);

#ifdef __cplusplus
}
#endif

#endif