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
typedef struct bbl_capi_qtype_instance* bbl_capi_qtype_t;
typedef struct bbl_capi_param_instance* bbl_capi_param_t;
typedef struct bbl_capi_inclusion_instance* bbl_capi_inclusion_t;

typedef struct bbl_capi_structid_instance* bbl_capi_structid_t;
typedef struct bbl_capi_enumid_instance* bbl_capi_enumid_t;
typedef struct bbl_capi_functionid_instance* bbl_capi_functionid_t;
typedef struct bbl_capi_stdfunctionid_instance* bbl_capi_stdfunctionid_t;

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

/**
 * @brief Get the number of modules in the C API
 * 
 * @param capi 
 * @param num_modules 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_num_modules(bbl_capi_t capi, size_t* num_modules);

/**
 * @brief Get the index'th module in the C API
 * 
 * @param capi 
 * @param index 
 * @param module 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_module(bbl_capi_t capi, size_t index, bbl_capi_module_t* module);

/**
 * @brief Get the name of the given module and return BBL_RESULT_Success. 
 *
 * If the given module is NULL, then str is set to NULL, len is set to 0 and BBL_RESULT_ArgumentIsNull is returned
 * 
 * @param module 
 * @param str 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_module_get_name(bbl_capi_module_t module, char const** str, size_t* len);

bbl_result_t bbl_capi_module_get_num_inclusions(bbl_capi_module_t module, size_t* num_inclusions);
bbl_result_t bbl_capi_module_get_inclusion(bbl_capi_module_t module, size_t index, bbl_capi_inclusion_t* inclusion);
bbl_result_t bbl_capi_module_get_num_structs(bbl_capi_module_t module, size_t* num_structs);
bbl_result_t bbl_capi_module_get_struct(bbl_capi_module_t module, size_t index, bbl_capi_structid_t* structid);
bbl_result_t bbl_capi_module_get_num_functions(bbl_capi_module_t module, size_t* num_functions);
bbl_result_t bbl_capi_module_get_function(bbl_capi_module_t module, size_t index, bbl_capi_functionid_t* funid);
bbl_result_t bbl_capi_module_get_num_stdfunctions(bbl_capi_module_t module, size_t* num_stdfunctions);
bbl_result_t bbl_capi_module_get_stdfunction(bbl_capi_module_t module, size_t index, bbl_capi_stdfunctionid_t* funid);
bbl_result_t bbl_capi_module_get_num_enums(bbl_capi_module_t module, size_t* num_enums);
bbl_result_t bbl_capi_module_get_enum(bbl_capi_module_t module, size_t index, bbl_capi_enumid_t* enumid);

bbl_result_t bbl_capi_inclusion_get_directive(bbl_capi_inclusion_t inclusion, char const** str, size_t* len);
bbl_result_t bbl_capi_inclusion_get_filename(bbl_capi_inclusion_t inclusion, char const** str, size_t* len);
bbl_result_t bbl_capi_inclusion_is_local(bbl_capi_inclusion_t inclusion, bool* is_local);


/**
 * @brief Get the total number of structs in the C API (across all modules)
 * 
 * @param capi 
 * @param num_structs 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_num_structs(bbl_capi_t capi, size_t* num_structs);

/**
 * @brief Write the struct with id `id` into strct and return BBL_RESULT_Success.
 *
 * If capi is NULL, strct is set to NULL and BBL_RESULT_ArgumentIsNull is returned
 * If id does not refer to a valid struct, strct is set to NULL and BBL_RESULT_NotFound is returned
 * 
 * @param capi 
 * @param index 
 * @param strct 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_struct(bbl_capi_t capi, bbl_capi_structid_t id, bbl_capi_struct_t* strct);

/**
 * @brief Get the index'th struct in the C API (across all modules)
 * 
 * @param capi 
 * @param index 
 * @param strct 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_struct_at_index(bbl_capi_t capi, size_t index, bbl_capi_struct_t* strct);

/**
 * @brief Get the total number of functions in the C API (across all modules)

 * 
 * @param capi 
 * @param num_functions 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_num_functions(bbl_capi_t capi, size_t* num_functions);

/**
 * @brief  Get the index'th function in the C API (across all modules)
 * 
 * @param capi 
 * @param index 
 * @param function 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_function_at_index(bbl_capi_t capi, size_t index, bbl_capi_function_t* function);

/**
 * @brief Get the total number of stdfunctions in the C API (across all modules)
 * 
 * @param capi 
 * @param num_stdfunctions 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_num_stdfunctions(bbl_capi_t capi, size_t* num_stdfunctions);

/**
 * @brief Write the stdfunction with id `id` into fun and return BBL_RESULT_Success.
 *
 * If capi is NULL, fun is set to NULL and BBL_RESULT_ArgumentIsNull is returned
 * If id does not refer to a valid stdfunction, fun is set to NULL and BBL_RESULT_NotFound is returned
 * 
 * @param capi 
 * @param index 
 * @param fun 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_stdfunction(bbl_capi_t capi, bbl_capi_stdfunctionid_t id, bbl_capi_stdfunction_t* fun);

/**
 * @brief Get the index'th stdfunction in the C API (across all modules)
 * 
 * @param capi 
 * @param index 
 * @param stdfunction 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_stdfunction_at_index(bbl_capi_t capi, size_t index, bbl_capi_stdfunction_t* stdfunction);

/**
 * @brief Get the total number of enums in the C API (across all modules)
 * 
 * @param capi 
 * @param num_enums 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_num_enums(bbl_capi_t capi, size_t* num_enums);

/**
 * @brief Write the enum with id `id` into enm and return BBL_RESULT_Success.
 *
 * If capi is NULL, enm is set to NULL and BBL_RESULT_ArgumentIsNull is returned
 * If id does not refer to a valid enum, enm is set to NULL and BBL_RESULT_NotFound is returned
 * 
 * @param capi 
 * @param index 
 * @param enm 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_enum(bbl_capi_t capi, bbl_capi_enumid_t id, bbl_capi_enum_t* enm);

/**
 * @brief Get the index'th enum in the C API (across all modules)
 * 
 * @param capi 
 * @param index 
 * @param enm 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_get_enum_at_index(bbl_capi_t capi, size_t index, bbl_capi_enum_t* enm);

/**
 * @brief get the bbl_class_t that is the C++ representation that this struct was derived from
 * 
 * @param strct 
 * @param cls 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_struct_get_cpp(bbl_capi_struct_t strct, bbl_class_t* cls);

/**
 * @brief Get the name of the struct
 * 
 * @param strct 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_struct_get_name(bbl_capi_struct_t strct, char const** ptr, size_t* len);

/**
 * @brief Get the number of fields in the struct
 * 
 * @param strct 
 * @param num_fields 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_struct_get_num_fields(bbl_capi_struct_t strct, size_t* num_fields);

/**
 * @brief Get the index'th field in the struct
 * 
 * @param strct 
 * @param index 
 * @param field 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_struct_get_field(bbl_capi_struct_t strct, size_t index, bbl_capi_field_t* field);

/**
 * @brief Get the type of the field
 * 
 * @param field 
 * @param type 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_field_get_type(bbl_capi_field_t field, bbl_capi_qtype_t* type);

/**
 * @brief Get the name of the field
 * 
 * @param field 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_field_get_name(bbl_capi_field_t field, char const** ptr, size_t* len);

/**
 * @brief Get the type of the param
 * 
 * @param param 
 * @param type 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_param_get_type(bbl_capi_param_t param, bbl_capi_qtype_t* type);

/**
 * @brief Get the name of the param
 * 
 * @param param 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_param_get_name(bbl_capi_param_t param, char const** ptr, size_t* len);

/**
 * @brief Get the source where this function definition came from
 * 
 * @param fun 
 * @param source 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_function_get_source(bbl_capi_function_t fun, bbl_function_source_t* source);

/**
 * @brief If the function came from a C++ method, get that method
 * 
 * @param fun 
 * @param cpp_method 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_function_get_cpp_method(bbl_capi_function_t fun, bbl_method_t* cpp_method);

/**
 * @brief If the function came from a C++ constructor, get that constructor
 * 
 * @param fun 
 * @param cpp_ctor 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_function_get_cpp_constructor(bbl_capi_function_t fun, bbl_constructor_t* cpp_ctor);

/**
 * @brief If the function came from a C++ free function, get that function
 * 
 * @param fun 
 * @param cpp_fun 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_function_get_cpp_function(bbl_capi_function_t fun, bbl_function_t* cpp_fun);

/**
 * @brief Get the name of the function
 * 
 * @param function 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_function_get_name(bbl_capi_function_t function, char const** ptr, size_t* len);

/**
 * @brief Get the result out parameter (i.e. the original function's return type moved to an out paramter), if it exists.
 * 
 * @param function 
 * @param result_param 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_function_get_result_param(bbl_capi_function_t function, bbl_capi_param_t* result_param);

/**
 * @brief Get the receiver (i.e. this) parameter, if it exists
 * 
 * @param function 
 * @param receiver_param 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_function_get_receiver_param(bbl_capi_function_t function, bbl_capi_param_t* receiver_param);

/**
 * @brief Get the number of parameters to this function, not including result and receiver parameters
 * 
 * @param function 
 * @param num_params 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_function_get_num_params(bbl_capi_function_t function, size_t* num_params);

/**
 * @brief Get the index'th parameter of this function
 * 
 * @param function 
 * @param index 
 * @param param 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_function_get_param(bbl_capi_function_t function, size_t index, bbl_capi_param_t* param);

/**
 * @brief Get the generated function body as a string
 * 
 * @param function 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_function_get_body(bbl_capi_function_t function, char const** ptr, size_t* len);

bbl_result_t bbl_capi_qtype_is_const(bbl_capi_qtype_t qtype, bool* is_const);
bbl_result_t bbl_capi_qtype_get_type_kind(bbl_capi_qtype_t qtype, bbl_capi_type_kind_t* type_kind);
bbl_result_t bbl_capi_qtype_get_as_builtin(bbl_capi_qtype_t qtype, bbl_builtin_t* builtin);
bbl_result_t bbl_capi_qtype_get_as_structid(bbl_capi_qtype_t qtype, bbl_capi_structid_t* structid);
bbl_result_t bbl_capi_qtype_get_as_stdfunctionid(bbl_capi_qtype_t qtype, bbl_capi_stdfunctionid_t* stdfunctionid);
bbl_result_t bbl_capi_qtype_get_as_enumid(bbl_capi_qtype_t qtype, bbl_capi_enumid_t* enumid);

/**
 * @brief If the given qtype is a pointer, sets pointee_type to the type of the pointee and return BBL_RESULT_Success
 *
 * If the given qtype is not a pointer, pointee_type will be set to NULL and BBL_RESULT_WrongKind wil be returned. 
 * If the given qtype is NULL, pointee_type will be set to NULL and BBL_RESULT_ArgumentIsNull will be returned.
 * 
 * @param qtype 
 * @param pointee_type 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_qtype_get_pointee_type(bbl_capi_qtype_t qtype, bbl_capi_qtype_t* pointee_type);

/**
 * @brief If the given qtype is an array, sets element_type to the type of the array element and return BBL_RESULT_Success
 *
 * If the given qtype is not an array, element_type will be set to NULL and BBL_RESULT_WrongKind wil be returned. 
 * If the given qtype is NULL, element_type will be set to NULL and BBL_RESULT_ArgumentIsNull will be returned.
 * 
 * @param qtype 
 * @param element_type 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_qtype_get_array_element_type(bbl_capi_qtype_t qtype, bbl_capi_qtype_t* element_type);

/**
 * @brief If the given qtype is an array, sets size to the size of the array return BBL_RESULT_Success
 *
 * If the given qtype is not an array, size will be set to 0 and BBL_RESULT_WrongKind wil be returned. 
 * If the given qtype is NULL, size will be set to 0 and BBL_RESULT_ArgumentIsNull will be returned.
 * 
 * 
 * @param qtype 
 * @param size 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_qtype_get_array_element_size(bbl_capi_qtype_t qtype, size_t* size);

#ifdef __cplusplus
}
#endif

#endif