#ifndef BBL_BBL_C_H
#define BBL_BBL_C_H

#include "bbl-context.h"
#include "bbl-detail.h"

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
bbl_result_t
bbl_capi_get_header(bbl_capi_t capi, char const** str, size_t* len);

/**
 * @brief Get the complete C source for the given API
 *
 * @param capi
 * @param str
 * @param len
 * @return bbl_result_t
 */
bbl_result_t
bbl_capi_get_source(bbl_capi_t capi, char const** str, size_t* len);

/**
 * @brief Get the number of modules in the C API
 *
 * @param capi
 * @param num_modules
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_num_modules(bbl_capi_t capi, size_t* num_modules);

/**
 * @brief Get the index'th module in the C API
 *
 * @param capi
 * @param index
 * @param module
 * @return bbl_result_t
 */
bbl_result_t
bbl_capi_get_module(bbl_capi_t capi, size_t index, bbl_capi_module_t* module);

/**
 * @brief Get the name of the given module and return BBL_RESULT_Success.
 *
 * If the given module is NULL, then str is set to NULL, len is set to 0 and
 * BBL_RESULT_ArgumentIsNull is returned
 *
 * @param module
 * @param str
 * @param len
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_module_get_name(bbl_capi_module_t module,
                                      char const** str,
                                      size_t* len);

/**
 * @brief Get the number of inclusions in this module
 * 
 * @param module 
 * @param num_inclusions 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_module_get_num_inclusions(bbl_capi_module_t module,
                                                size_t* num_inclusions);

/**
 * @brief Get the id of the index'th inclusion from this module
 * 
 * @param module 
 * @param index 
 * @param inclusion 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_module_get_inclusion(bbl_capi_module_t module,
                                           size_t index,
                                           bbl_capi_inclusion_t* inclusion);

/**
 * @brief Get the number of structs in this module
 * 
 * @param module 
 * @param num_structs 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_module_get_num_structs(bbl_capi_module_t module,
                                             size_t* num_structs);

/**
 * @brief Get the id of the index'th struct from this module
 * 
 * @param module 
 * @param index 
 * @param structid 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_module_get_struct(bbl_capi_module_t module,
                                        size_t index,
                                        bbl_capi_structid_t* structid);

/**
 * @brief Get the number of functions in this module
 * 
 * @param module 
 * @param num_functions 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_module_get_num_functions(bbl_capi_module_t module,
                                               size_t* num_functions);

/**
 * @brief Get the id of the index'th function from this module
 * 
 * @param module 
 * @param index 
 * @param funid 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_module_get_function(bbl_capi_module_t module,
                                          size_t index,
                                          bbl_capi_functionid_t* funid);

/**
 * @brief Get the number of stdfunctions in this module
 * 
 * @param module 
 * @param num_stdfunctions 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_module_get_num_stdfunctions(bbl_capi_module_t module,
                                                  size_t* num_stdfunctions);

/**
 * @brief Get the id of the index'th stdfunction in this module
 * 
 * @param module 
 * @param index 
 * @param funid 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_module_get_stdfunction(bbl_capi_module_t module,
                                             size_t index,
                                             bbl_capi_stdfunctionid_t* funid);

/**
 * @brief Get the number of enums in this module
 * 
 * @param module 
 * @param num_enums 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_module_get_num_enums(bbl_capi_module_t module,
                                           size_t* num_enums);

/**
 * @brief Get the id of the index'th enum in this module
 * 
 * @param module 
 * @param index 
 * @param enumid 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_module_get_enum(bbl_capi_module_t module,
                                      size_t index,
                                      bbl_capi_enumid_t* enumid);

/**
 * @brief Get the full include directive of this inclusion, i.e. "#include <foo/foo.h>"
 * 
 * @param inclusion 
 * @param str 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_inclusion_get_directive(bbl_capi_inclusion_t inclusion,
                                              char const** str,
                                              size_t* len);

/**
 * @brief Get the filename of this inclusion, i.e. "foo/foo.h"
 * 
 * @param inclusion 
 * @param str 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_inclusion_get_filename(bbl_capi_inclusion_t inclusion,
                                             char const** str,
                                             size_t* len);

/**
 * @brief Get whether this inclusion is local or not, i.e. whether it is included
 * with '"' as opposed to '<'
 * 
 * @param inclusion 
 * @param is_local 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_inclusion_is_local(bbl_capi_inclusion_t inclusion,
                                         bool* is_local);

/**
 * @brief Get the total number of structs in the C API (across all modules)
 *
 * @param capi
 * @param num_structs
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_get_num_structs(bbl_capi_t capi, size_t* num_structs);

/**
 * @brief Write the struct with id `id` into strct and return
 * BBL_RESULT_Success.
 *
 * If capi is NULL, strct is set to NULL and BBL_RESULT_ArgumentIsNull is
 * returned If id does not refer to a valid struct, strct is set to NULL and
 * BBL_RESULT_NotFound is returned
 *
 * @param capi
 * @param index
 * @param strct
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_get_struct(bbl_capi_t capi,
                                 bbl_capi_structid_t id,
                                 bbl_capi_struct_t* strct);

/**
 * @brief Get the index'th struct in the C API (across all modules)
 *
 * @param capi
 * @param index
 * @param strct
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_get_struct_at_index(bbl_capi_t capi,
                                          size_t index,
                                          bbl_capi_struct_t* strct);

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
bbl_result_t bbl_capi_get_function_at_index(bbl_capi_t capi,
                                            size_t index,
                                            bbl_capi_function_t* function);

/**
 * @brief Get the function with id `id`
 *
 * @param capi
 * @param id
 * @param function
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_get_function(bbl_capi_t capi,
                                   bbl_capi_functionid_t id,
                                   bbl_capi_function_t* function);

/**
 * @brief Get the total number of stdfunctions in the C API (across all modules)
 *
 * @param capi
 * @param num_stdfunctions
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_get_num_stdfunctions(bbl_capi_t capi,
                                           size_t* num_stdfunctions);

/**
 * @brief Write the stdfunction with id `id` into fun and return
 * BBL_RESULT_Success.
 *
 * If capi is NULL, fun is set to NULL and BBL_RESULT_ArgumentIsNull is returned
 * If id does not refer to a valid stdfunction, fun is set to NULL and
 * BBL_RESULT_NotFound is returned
 *
 * @param capi
 * @param index
 * @param fun
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_get_stdfunction(bbl_capi_t capi,
                                      bbl_capi_stdfunctionid_t id,
                                      bbl_capi_stdfunction_t* fun);

/**
 * @brief Get the index'th stdfunction in the C API (across all modules)
 *
 * @param capi
 * @param index
 * @param stdfunction
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_get_stdfunction_at_index(
    bbl_capi_t capi, size_t index, bbl_capi_stdfunction_t* stdfunction);

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
 * If id does not refer to a valid enum, enm is set to NULL and
 * BBL_RESULT_NotFound is returned
 *
 * @param capi
 * @param index
 * @param enm
 * @return bbl_result_t
 */
bbl_result_t
bbl_capi_get_enum(bbl_capi_t capi, bbl_capi_enumid_t id, bbl_capi_enum_t* enm);

/**
 * @brief Get the index'th enum in the C API (across all modules)
 *
 * @param capi
 * @param index
 * @param enm
 * @return bbl_result_t
 */
bbl_result_t
bbl_capi_get_enum_at_index(bbl_capi_t capi, size_t index, bbl_capi_enum_t* enm);

/**
 * @brief get the bbl_class_t that is the C++ representation that this struct
 * was derived from
 *
 * @param strct
 * @param cls
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_struct_get_cpp(bbl_capi_struct_t strct, bbl_class_t* cls);

/**
 * @brief get the bind kind of this struct
 *
 * @param strct
 * @param bind_kind
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_struct_get_bind_kind(bbl_capi_struct_t strct,
                                           bbl_bind_kind_t* bind_kind);

/**
 * @brief Get the name of the struct
 *
 * @param strct
 * @param ptr
 * @param len
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_struct_get_name(bbl_capi_struct_t strct,
                                      char const** ptr,
                                      size_t* len);

/**
 * @brief Get the number of fields in the struct
 *
 * @param strct
 * @param num_fields
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_struct_get_num_fields(bbl_capi_struct_t strct,
                                            size_t* num_fields);

/**
 * @brief Get the index'th field in the struct
 *
 * @param strct
 * @param index
 * @param field
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_struct_get_field(bbl_capi_struct_t strct,
                                       size_t index,
                                       bbl_capi_field_t* field);

/**
 * @brief Get the type of the field
 *
 * @param field
 * @param type
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_field_get_type(bbl_capi_field_t field,
                                     bbl_capi_qtype_t* type);

/**
 * @brief Get the name of the field
 *
 * @param field
 * @param ptr
 * @param len
 * @return bbl_result_t
 */
bbl_result_t
bbl_capi_field_get_name(bbl_capi_field_t field, char const** ptr, size_t* len);

/**
 * @brief Get the type of the param
 *
 * @param param
 * @param type
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_param_get_type(bbl_capi_param_t param,
                                     bbl_capi_qtype_t* type);

/**
 * @brief Get the name of the param
 *
 * @param param
 * @param ptr
 * @param len
 * @return bbl_result_t
 */
bbl_result_t
bbl_capi_param_get_name(bbl_capi_param_t param, char const** ptr, size_t* len);

/**
 * @brief Get the source where this function definition came from
 *
 * @param fun
 * @param source
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_function_get_source(bbl_capi_function_t fun,
                                          bbl_function_source_t* source);

/**
 * @brief If the function came from a C++ method, get that method
 *
 * @param fun
 * @param cpp_method
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_function_get_cpp_method(bbl_capi_function_t fun,
                                              bbl_method_t* cpp_method);

/**
 * @brief If the function came from a C++ constructor, get that constructor
 *
 * @param fun
 * @param cpp_ctor
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_function_get_cpp_constructor(bbl_capi_function_t fun,
                                                   bbl_constructor_t* cpp_ctor);

/**
 * @brief If the function came from a C++ free function, get that function
 *
 * @param fun
 * @param cpp_fun
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_function_get_cpp_function(bbl_capi_function_t fun,
                                                bbl_function_t* cpp_fun);

/**
 * @brief Get the name of the function
 *
 * @param function
 * @param ptr
 * @param len
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_function_get_name(bbl_capi_function_t function,
                                        char const** ptr,
                                        size_t* len);

/**
 * @brief Get the result out parameter (i.e. the original function's return type
 * moved to an out paramter), if it exists.
 *
 * @param function
 * @param result_param
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_function_get_result_param(bbl_capi_function_t function,
                                                bbl_capi_param_t* result_param);

/**
 * @brief Get the receiver (i.e. this) parameter, if it exists
 *
 * @param function
 * @param receiver_param
 * @return bbl_result_t
 */
bbl_result_t
bbl_capi_function_get_receiver_param(bbl_capi_function_t function,
                                     bbl_capi_param_t* receiver_param);

/**
 * @brief Get the number of parameters to this function, not including result
 * and receiver parameters
 *
 * @param function
 * @param num_params
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_function_get_num_params(bbl_capi_function_t function,
                                              size_t* num_params);

/**
 * @brief Get the index'th parameter of this function
 *
 * @param function
 * @param index
 * @param param
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_function_get_param(bbl_capi_function_t function,
                                         size_t index,
                                         bbl_capi_param_t* param);

/**
 * @brief Get the generated function body as a string
 *
 * @param function
 * @param ptr
 * @param len
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_function_get_body(bbl_capi_function_t function,
                                        char const** ptr,
                                        size_t* len);

/**
 * @brief Get the name of the stdfunction
 *
 * @param stdfunction
 * @param ptr
 * @param len
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_stdfunction_get_name(bbl_capi_stdfunction_t stdfunction,
                                        char const** ptr,
                                        size_t* len);

/**
 * @brief Get the result out parameter (i.e. the original stdfunction's return type
 * moved to an out paramter), if it exists.
 *
 * @param stdfunction
 * @param result_param
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_stdfunction_get_result_param(bbl_capi_stdfunction_t stdfunction,
                                                bbl_capi_param_t* result_param);

/**
 * @brief Get the number of parameters to this stdfunction, not including result
 * and receiver parameters
 *
 * @param stdfunction
 * @param num_params
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_stdfunction_get_num_params(bbl_capi_stdfunction_t stdfunction,
                                              size_t* num_params);

/**
 * @brief Get the index'th parameter of this stdfunction
 *
 * @param stdfunction
 * @param index
 * @param param
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_stdfunction_get_param(bbl_capi_stdfunction_t stdfunction,
                                         size_t index,
                                         bbl_capi_param_t* param);

/**
 * @brief Get the name of the enum
 *
 * @param strct
 * @param ptr
 * @param len
 * @return bbl_result_t
 */
bbl_result_t
bbl_capi_enum_get_name(bbl_capi_enum_t enm, char const** ptr, size_t* len);

/**
 * @brief Get the number of variants in this enum
 * 
 * @param enm 
 * @param num_variants 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_enum_get_num_variants(bbl_capi_enum_t enm,
                                            size_t* num_variants);

/**
 * @brief Get the index'th variant in this enum
 * 
 * @param enm 
 * @param index 
 * @param name 
 * @param name_len 
 * @param value 
 * @param value_len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_enum_get_variant(bbl_capi_enum_t enm,
                                       size_t index,
                                       char const** name,
                                       size_t* name_len,
                                       char const** value,
                                       size_t* value_len);

/**
 * @brief Get the index'th original variant in this enum
 * 
 * The original variants are the names from the C++ declaration without any
 * prefixing
 * 
 * @param enm 
 * @param index 
 * @param name 
 * @param name_len 
 * @param value 
 * @param value_len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_enum_get_original_variant(bbl_capi_enum_t enm,
                                       size_t index,
                                       char const** name,
                                       size_t* name_len,
                                       char const** value,
                                       size_t* value_len);

/**
 * @brief Get the underlying integer type of this enum
 * 
 * @param enm 
 * @param type 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_enum_get_underlying_type(bbl_capi_enum_t enm, bbl_builtin_t* type);

/**
 * @brief Get whether this qtype is const-qualified
 * 
 * @param qtype 
 * @param is_const will be assigned true if `qtype` is const, false otherwise
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_qtype_is_const(bbl_capi_qtype_t qtype, bool* is_const);

/**
 * @brief Get the kind of this qtype
 * 
 * @param qtype 
 * @param type_kind 
 * @return bbl_result_t 
 */
bbl_result_t bbl_capi_qtype_get_type_kind(bbl_capi_qtype_t qtype,
                                          bbl_capi_type_kind_t* type_kind);

/**
 * @brief Get the builtin this qtype contains, if it does so
 * 
 * @param qtype 
 * @param builtin Will be assigned the value of the builtin this qtype contains, 
 *        if the return value is BBL_RESULT_Success
 * @return bbl_result_t - BBL_RESULT_Success if `qtype` is valid and contains a builtin
 * - BBL_RESULT_ArgumentIsNull if `qtype` is NULL
 * - BBL_RESULT_WrongKind otherwise
 */
bbl_result_t bbl_capi_qtype_get_as_builtin(bbl_capi_qtype_t qtype,
                                           bbl_builtin_t* builtin);

/**
 * @brief Get the id of the struct this qtype contains, if it does so
 * 
 * @param qtype 
 * @param structid Will be assigned the id of the struct this qtype contains
 * @return bbl_result_t - BBL_RESULT_Success if `qtype` is valid and contains a struct
 * - BBL_RESULT_ArgumentIsNull if `qtype` is NULL
 * - BBL_RESULT_WrongKind otherwise
 */
bbl_result_t bbl_capi_qtype_get_as_structid(bbl_capi_qtype_t qtype,
                                            bbl_capi_structid_t* structid);

/**
 * @brief Get the id of the stdfunction this qtype contains, if it does so
 * 
 * @param qtype 
 * @param stdfunctionid Will be assigned the id of the stdfunction this qtype contains
 * @return bbl_result_t - BBL_RESULT_Success if `qtype` is valid and contains a stdfunction
 * - BBL_RESULT_ArgumentIsNull if `qtype` is NULL
 * - BBL_RESULT_WrongKind otherwise
 */
bbl_result_t
bbl_capi_qtype_get_as_stdfunctionid(bbl_capi_qtype_t qtype,
                                    bbl_capi_stdfunctionid_t* stdfunctionid);

/**
 * @brief Get the id of the enum this qtype contains, if it does so
 * 
 * @param qtype 
 * @param enumid  Will be assigned the id of the enum this qtype contains
 * @return bbl_result_t - BBL_RESULT_Success if `qtype` is valid and contains a stdfunction
 * - BBL_RESULT_ArgumentIsNull if `qtype` is NULL
 * - BBL_RESULT_WrongKind otherwise
 */
bbl_result_t bbl_capi_qtype_get_as_enumid(bbl_capi_qtype_t qtype,
                                          bbl_capi_enumid_t* enumid);

/**
 * @brief If the given qtype is a pointer, sets pointee_type to the type of the
 * pointee and return BBL_RESULT_Success
 *
 * If the given qtype is not a pointer, pointee_type will be set to NULL and
 * BBL_RESULT_WrongKind wil be returned. If the given qtype is NULL,
 * pointee_type will be set to NULL and BBL_RESULT_ArgumentIsNull will be
 * returned.
 *
 * @param qtype
 * @param pointee_type
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_qtype_get_pointee_type(bbl_capi_qtype_t qtype,
                                             bbl_capi_qtype_t* pointee_type);

/**
 * @brief If the given qtype is an array, sets element_type to the type of the
 * array element and return BBL_RESULT_Success
 *
 * If the given qtype is not an array, element_type will be set to NULL and
 * BBL_RESULT_WrongKind wil be returned. If the given qtype is NULL,
 * element_type will be set to NULL and BBL_RESULT_ArgumentIsNull will be
 * returned.
 *
 * @param qtype
 * @param element_type
 * @return bbl_result_t
 */
bbl_result_t
bbl_capi_qtype_get_array_element_type(bbl_capi_qtype_t qtype,
                                      bbl_capi_qtype_t* element_type);

/**
 * @brief If the given qtype is an array, sets size to the size of the array
 * return BBL_RESULT_Success
 *
 * If the given qtype is not an array, size will be set to 0 and
 * BBL_RESULT_WrongKind wil be returned. If the given qtype is NULL, size will
 * be set to 0 and BBL_RESULT_ArgumentIsNull will be returned.
 *
 *
 * @param qtype
 * @param size
 * @return bbl_result_t
 */
bbl_result_t bbl_capi_qtype_get_array_size(bbl_capi_qtype_t qtype,
                                           size_t* size);

#ifdef __cplusplus
}
#endif

#endif