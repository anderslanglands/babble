#ifndef BBL_BBL_H
#define BBL_BBL_H

#include "bbl-detail.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque context type representing the babble context object.
 *
 * The context object is responsible for parsing the binding sources and
 * extracting information about the bound classes and functions.
 *
 * Once parsing has completed successfully, the context can then be queried.
 */
typedef struct bbl_context_instance_t* bbl_context_t;

/**
 * @brief Create a new context object
 *
 * @param ctx Pointer to a context object to initialize
 * @return bbl_result_t Error for any error
 */
bbl_result_t bbl_context_create(bbl_context_t* ctx);

/**
 * @brief Destroy a context object
 *
 * @param ctx The context object to destroy
 * @return bbl_result_t
 */
bbl_result_t bbl_context_destroy(bbl_context_t ctx);

/**
 * @brief Compile a set of C++ binding sources and extract information about the
 * bound classes and functions.
 *
 * The arguments list is essentially a clang command line, but a little wonky.
 * Note the double dash, this is not optional.
 *
 * <cmd> file01.cpp file02.cpp -- -I<include_dir>
 *
 * FIXME: We should pass in a dedicated parameter struct here instead of the raw
 * clang command line
 *
 * @param ctx
 * @param argc Number of arguments
 * @param argv Arguments
 * @return bbl_result_t
 */
bbl_result_t bbl_context_compile_and_extract(bbl_context_t ctx, int argc,
                                             char const** argv);

/**
 * @brief Information about a Module, as declared by the BBL_MODULE(name) function
 * 
 */
typedef struct bbl_module_instance_t* bbl_module_t;

/**
 * @brief Information about a Class, as bound by the `bbl::Class<X>()` constructor
 * 
 */
typedef struct bbl_class_instance_t* bbl_class_t;

/**
 * @brief Opaque identifier to refer to a Class instance
 * 
 */
typedef struct bbl_classid_instance_t* bbl_classid_t;

/**
 * @brief Information about a Method, as bound by the `.m()` member on bbl::Class
 * 
 */
typedef struct bbl_method_instance_t* bbl_method_t;

/**
 * @brief Information about a Method, as bound by the `.m()` member on bbl::Class
 * 
 */
typedef struct bbl_constructor_instance_t* bbl_constructor_t;

/**
 * @brief Information about a free Function, as bound by the `bbl::fn()` call
 * 
 */
typedef struct bbl_function_instance_t* bbl_function_t;

/**
 * @brief Opaque identifier to refer to a Function instance
 * 
 */
typedef struct bbl_functionid_instance_t* bbl_functionid_t;

/**
 * @brief Information about a StdFunction (std::function<>), as bound by the `bbl::Class()` call
 * 
 * These require special handling in translation, which is why they're a separate type in the bindings
 */
typedef struct bbl_stdfunction_instance_t* bbl_stdfunction_t;

/**
 * @brief Opaque identifier to refer to a StdFunction instance
 * 
 */
typedef struct bbl_stdfunctionid_instance_t* bbl_stdfunctionid_t;

/**
 * @brief Information about an Enum, as bound by the `bbl::Enum()` constructor
 * 
 */
typedef struct bbl_enum_instance_t* bbl_enum_t;

/**
 * @brief Opaque identifier to refer to an Enum instance
 * 
 */
typedef struct bbl_enumid_instance_t* bbl_enumid_t;

/**
 * @brief Represents a (const-) qualified type
 * 
 */
typedef struct bbl_qtype_instance_t* bbl_qtype_t;

/**
 * @brief Represents a function parameter, i.e. a pair of name and type
 * 
 */
typedef struct bbl_param_instance_t* bbl_param_t;

/**
 * @brief Returns the number of Modules stored on the Context
 * 
 * @param ctx 
 * @param num_modules 
 * @return bbl_result_t 
 */
bbl_result_t bbl_context_num_modules(bbl_context_t ctx, size_t* num_modules);

/**
 * @brief Get the index'th Module on this Context
 * 
 * @param ctx 
 * @param index 
 * @param module 
 * @return bbl_result_t
 */
bbl_result_t bbl_context_get_module(bbl_context_t ctx, size_t index, bbl_module_t* module);

/**
 * @brief Get the bbl_class_t with the given `id`
 *
 * The ids can be retrieved from the modules
 * 
 * @param ctx 
 * @param id 
 * @param cls 
 * @return bbl_result_t 
 */
bbl_result_t bbl_context_get_class(bbl_context_t ctx, bbl_classid_t id, bbl_class_t* cls);

/**
 * @brief Get the bbl_function_t with the given `id`
 *
 * The ids can be retrieved from the modules
 * 
 * @param ctx 
 * @param id 
 * @param cls 
 * @return bbl_result_t 
 */
bbl_result_t bbl_context_get_function(bbl_context_t ctx, bbl_functionid_t id, bbl_function_t* cls);

/**
 * @brief Get the bbl_stdfunction_t with the given `id`
 *
 * The ids can be retrieved from the modules
 * 
 * @param ctx 
 * @param id 
 * @param cls 
 * @return bbl_result_t 
 */
bbl_result_t bbl_context_get_stdfunction(bbl_context_t ctx, bbl_stdfunctionid_t id, bbl_stdfunction_t* cls);

/**
 * @brief Get the bbl_enum_t with the given `id`
 *
 * The ids can be retrieved from the modules
 * 
 * @param ctx 
 * @param id 
 * @param cls 
 * @return bbl_result_t 
 */
bbl_result_t bbl_context_get_enum(bbl_context_t ctx, bbl_enumid_t id, bbl_enum_t* cls);

/**
 * @brief Get the number of source files that this module was created in
 * 
 * @param module 
 * @param num_source_files 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_get_num_source_files(bbl_module_t module, size_t* num_source_files);

/**
 * @brief Get the index'th source file for this module
 * 
 * @param module 
 * @param index 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_get_source_file(bbl_module_t module, size_t index, char const** ptr, size_t* len);

/**
 * @brief Get the number of function impls that this module contains
 * 
 * @param module 
 * @param num_function_impls 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_get_num_function_impls(bbl_module_t module, size_t* num_function_impls);

/**
 * @brief Get the index'th function impl for this module
 * 
 * @param module 
 * @param index 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_get_function_impl(bbl_module_t module, size_t index, char const** ptr, size_t* len);

/**
 * @brief Get the name of this module, i.e. "X" in BBL_MODULE(X)
 * 
 * @param module 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_get_name(bbl_module_t module, char const** ptr, size_t* len);

/**
 * @brief Get the number of classes bound in this module
 * 
 * @param module 
 * @param num_classes 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_num_classes(bbl_module_t module, size_t* num_classes);

/**
 * @brief Get the bbl_classid_t for the index'th bbl_class_t bound in this module. 
 *
 * The returned class id can then be used to retrieve to bbl_class_t from the context
 * 
 * @param module 
 * @param index 
 * @param cls 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_get_classid(bbl_module_t module, size_t index, bbl_classid_t* cls);

/**
 * @brief Get the number of functions bound in this module
 * 
 * @param module 
 * @param num_functions 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_num_functions(bbl_module_t module, size_t* num_functions);

/**
 * @brief Get the bbl_functionid_t for the index'th bbl_function_t bound in this module. 
 *
 * The returned function id can then be used to retrieve to bbl_function_t from the context
 * 
 * @param module 
 * @param index 
 * @param fun 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_get_functionid(bbl_module_t module, size_t index, bbl_functionid_t* fun);

/**
 * @brief Get the number of stdfunctions bound in this module
 * 
 * @param module 
 * @param num_stdfunctions 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_num_stdfunctions(bbl_module_t module, size_t* num_stdfunctions);

/**
 * @brief Get the id of the index'th stdfunction in this module
 * 
 * @param module 
 * @param index 
 * @param fun 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_get_stdfunctionid(bbl_module_t module, size_t index, bbl_stdfunctionid_t* fun);

/**
 * @brief Get the number of enums bound in this module
 * 
 * @param module 
 * @param num_enums 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_num_enums(bbl_module_t module, size_t* num_enums);

/**
 * @brief Get the id of the index'th enum in this module
 * 
 * @param module 
 * @param index 
 * @param enm 
 * @return bbl_result_t 
 */
bbl_result_t bbl_module_get_enumid(bbl_module_t module, size_t index, bbl_enumid_t* enm);

/**
 * @brief Get the qualified name of this class, i.e. foo::Bar
 * 
 * @param cls 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_get_qualified_name(bbl_class_t cls, char const** ptr, size_t* len);

/**
 * @brief Get the spelling of this class. 
 *
 * This is not actually how it was spelt by the user, but how clang prints it
 * 
 * @param cls 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_get_spelling(bbl_class_t cls, char const** ptr, size_t* len);

/**
 * @brief Get the name of this class, i.e. "Bar" for "foo::Bar"
 * 
 * @param cls 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_get_name(bbl_class_t cls, char const** ptr, size_t* len);

/**
 * @brief Get the rename string provided for this class by the user, if any
 * 
 * @param cls 
 * @param ptr Pointer to a char* to receive the string. Will be set to NULL if no rename string provided
 * @param len Length of the string, provided for convenience
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_get_rename(bbl_class_t cls, char const** ptr, size_t* len);

/**
 * @brief Get the doc comment attached to this class if there is one
 * 
 * @param cls 
 * @param ptr Pointer to a char* to receive the comment string. Will be set to NULL if no comment string provided
 * @param len Length of the string, provided for convenience
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_get_comment(bbl_class_t cls, char const** ptr, size_t* len);

/**
 * @brief Get the memory layout of this class, i.e. its size and alignment
 * 
 * @param cls 
 * @param layout 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_get_layout(bbl_class_t cls, bbl_layout_t* layout);

/**
 * @brief Get the bind kind of this class
 * 
 * @param cls 
 * @param bind_kind 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_get_bind_kind(bbl_class_t cls, bbl_bind_kind_t* bind_kind);

/**
 * @brief Get the number of methods bound on this class
 * 
 * @param cls 
 * @param num_methods 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_num_methods(bbl_class_t cls, size_t* num_methods);

/**
 * @brief Get the index'th method bound on this class
 * 
 * @param cls 
 * @param index 
 * @param method 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_get_method(bbl_class_t cls, size_t index, bbl_method_t* method);

/**
 * @brief Get the number of constructors on this class
 * 
 * @param cls 
 * @param num_constructors 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_num_constructors(bbl_class_t cls, size_t* num_constructors);

/**
 * @brief Get the index'th constructor on this class
 * 
 * @param cls 
 * @param index 
 * @param constructor 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_get_constructor(bbl_class_t cls, size_t index, bbl_constructor_t* constructor);

/**
 * @brief Query whether this class is copy constructible
 * 
 * @param cls 
 * @param is_copy_constructible 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_is_copy_constructible(bbl_class_t cls, bool* is_copy_constructible);

/**
 * @brief Query whether this class's copy constructor is noexcept
 * 
 * @param cls 
 * @param is_nothrow_copy_constructible 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_is_nothrow_copy_constructible(bbl_class_t cls, bool* is_nothrow_copy_constructible);

/**
 * @brief Query whether this class is move-constructible
 * 
 * @param cls 
 * @param is_move_constructible 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_is_move_constructible(bbl_class_t cls, bool* is_move_constructible);

/**
 * @brief Query whether this class's move constructor is noexcept
 * 
 * @param cls 
 * @param is_nothrow_move_constructible 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_is_nothrow_move_constructible(bbl_class_t cls, bool* is_nothrow_move_constructible);

/**
 * @brief Query whether this class is copy assignable
 * 
 * @param cls 
 * @param is_copy_assignable 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_is_copy_assignable(bbl_class_t cls, bool* is_copy_assignable);

/**
 * @brief Query whether this class's copy assignment operator is noexcept
 * 
 * @param cls 
 * @param is_nothrow_copy_assignable 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_is_nothrow_copy_assignable(bbl_class_t cls, bool* is_nothrow_copy_assignable);

/**
 * @brief Query whether this class is move assignable
 * 
 * @param cls 
 * @param is_move_assignable 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_is_move_assignable(bbl_class_t cls, bool* is_move_assignable);

/**
 * @brief Query whether this class's move assignment operator is noexcept
 * 
 * @param cls 
 * @param is_nothrow_move_assignable 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_is_nothrow_move_assignable(bbl_class_t cls, bool* is_nothrow_move_assignable);

/**
 * @brief Query whether this class has a destructor
 * 
 * @param cls 
 * @param is_destructible 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_is_destructible(bbl_class_t cls, bool* is_destructible);

/**
 * @brief Query whether this class has a virtual destructor
 * 
 * @param cls 
 * @param has_virtual_destructor 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_has_virtual_destructor(bbl_class_t cls, bool* has_virtual_destructor);

/**
 * @brief Query whether this class is abstract
 * 
 * @param cls 
 * @param is_abstract 
 * @return bbl_result_t 
 */
bbl_result_t bbl_class_is_abstract(bbl_class_t cls, bool* is_abstract);

/**
 * @brief Get this method's qualified name, i.e. "foo::Bar::baz"
 * 
 * @param method 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_qualified_name(bbl_method_t method, char const** ptr, size_t* len);

/**
 * @brief Get this method's name, i.e. for "foo::Bar::baz" return "baz"
 * 
 * @param method 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_name(bbl_method_t method, char const** ptr, size_t* len);

/**
 * @brief Get this method's rename string, if one was provided
 * 
 * @param method 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_rename(bbl_method_t method, char const** ptr, size_t* len);

/**
 * @brief Get the template argument string required for a call to this method
 *
 * For foo::do_thing<int, float>() this would return "<int, float>"
 * 
 * @param method 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_template_call(bbl_method_t method, char const** ptr, size_t* len);

/**
 * @brief Get the doc comment on this method, if there is one
 * 
 * @param method 
 * @param ptr 
 * @param len 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_comment(bbl_method_t method, char const** ptr, size_t* len);

/**
 * @brief Query whether this method is noexcept
 * 
 * @param method 
 * @param is_noexcept 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_is_noexcept(bbl_method_t method, bool* is_noexcept);

/**
 * @brief Get this method's return type
 * 
 * @param method 
 * @param return_type 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_return_type(bbl_method_t method, bbl_qtype_t* return_type);

/**
 * @brief Get the number of parameters on this method
 * 
 * @param method 
 * @param num_params 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_num_params(bbl_method_t method, size_t* num_params);

/**
 * @brief Get the index'th parameter on this method
 * 
 * @param method 
 * @param index 
 * @param param 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_param(bbl_method_t method, size_t index, bbl_param_t* param);

/**
 * @brief Query whether this method is const
 * 
 * @param method 
 * @param is_const 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_is_const(bbl_method_t method, bool* is_const);

/**
 * @brief Query whether this method is static
 * 
 * @param method 
 * @param is_static 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_is_static(bbl_method_t method, bool* is_static);

/**
 * @brief Get the underlying function description from this method
 * 
 * @param method 
 * @param function 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_function(bbl_method_t method, bbl_function_t* function);

/**
 * @brief Query whether this method is virtual
 * 
 * @param method 
 * @param is_virtual 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_is_virtual(bbl_method_t method, bool* is_virtual);

/**
 * @brief Query whether this method is pure
 * 
 * @param method 
 * @param is_pure 
 * @return bbl_result_t 
 */
bbl_result_t bbl_method_get_is_pure(bbl_method_t method, bool* is_pure);

bbl_result_t bbl_constructor_get_rename(bbl_constructor_t constructor, char const** ptr, size_t* len);
bbl_result_t bbl_constructor_get_comment(bbl_constructor_t constructor, char const** ptr, size_t* len);
bbl_result_t bbl_constructor_get_is_noexcept(bbl_constructor_t constructor, bool* is_noexcept);
bbl_result_t bbl_constructor_num_params(bbl_constructor_t constructor, size_t* num_params);
bbl_result_t bbl_constructor_get_param(bbl_constructor_t constructor, size_t index, bbl_param_t* param);

bbl_result_t bbl_function_get_qualified_name(bbl_function_t function, char const** ptr, size_t* len);
bbl_result_t bbl_function_get_name(bbl_function_t function, char const** ptr, size_t* len);
bbl_result_t bbl_function_get_rename(bbl_function_t function, char const** ptr, size_t* len);
bbl_result_t bbl_function_get_comment(bbl_function_t function, char const** ptr, size_t* len);
bbl_result_t bbl_function_get_template_call(bbl_function_t function, char const** ptr, size_t* len);
bbl_result_t bbl_function_get_is_noexcept(bbl_function_t function, bool* is_noexcept);
bbl_result_t bbl_function_get_return_type(bbl_function_t function, bbl_qtype_t* return_type);
bbl_result_t bbl_function_num_params(bbl_function_t function, size_t* num_params);
bbl_result_t bbl_function_get_param(bbl_function_t function, size_t index, bbl_param_t* param);

bbl_result_t bbl_stdfunction_get_spelling(bbl_stdfunction_t function, char const** ptr, size_t* len);
bbl_result_t bbl_stdfunction_get_comment(bbl_stdfunction_t function, char const** ptr, size_t* len);
bbl_result_t bbl_stdfunction_get_return_type(bbl_stdfunction_t function, bbl_qtype_t* return_type);
bbl_result_t bbl_stdfunction_num_params(bbl_stdfunction_t function, size_t* num_params);
bbl_result_t bbl_stdfunction_get_param(bbl_stdfunction_t function, size_t index, bbl_qtype_t* param);

bbl_result_t bbl_enum_get_spelling(bbl_enum_t enm, char const** ptr, size_t* len);
bbl_result_t bbl_enum_get_rename(bbl_enum_t enm, char const** ptr, size_t* len);
bbl_result_t bbl_enum_get_comment(bbl_enum_t enm, char const** ptr, size_t* len);
bbl_result_t bbl_enum_num_variants(bbl_enum_t enm, size_t* num_variants);
bbl_result_t bbl_enum_get_variant(bbl_enum_t enm, size_t index, char const** name_ptr, size_t* name_len, char const** value_ptr, size_t* value_len);
bbl_result_t bbl_enum_get_type(bbl_enum_t enm, bbl_qtype_t* type);

bbl_result_t bbl_param_get_name(bbl_param_t param, char const** ptr, size_t* len);
bbl_result_t bbl_param_get_type(bbl_param_t param, bbl_qtype_t* type);

bbl_result_t bbl_qtype_is_const(bbl_qtype_t qtype, bool* is_const);
bbl_result_t bbl_qtype_get_type_kind(bbl_qtype_t qtype, bbl_type_kind_t* type_kind);
bbl_result_t bbl_qtype_get_as_builtin(bbl_qtype_t qtype, bbl_builtin_t* builtin);
bbl_result_t bbl_qtype_get_as_classid(bbl_qtype_t qtype, bbl_classid_t* classid);
bbl_result_t bbl_qtype_get_as_stdfunctionid(bbl_qtype_t qtype, bbl_stdfunctionid_t* stdfunctionid);
bbl_result_t bbl_qtype_get_as_enumid(bbl_qtype_t qtype, bbl_enumid_t* enumid);
bbl_result_t bbl_qtype_get_pointee_type(bbl_qtype_t qtype, bbl_qtype_t* pointee_type);

/**
 * @brief Get the element type if `qtype` is an array, leaving `element_type` untouched otherwise
 * 
 * @param qtype 
 * @param element_type 
 * @return bbl_result_t 
 */
bbl_result_t bbl_qtype_get_array_element_type(bbl_qtype_t qtype, bbl_qtype_t* element_type);

bbl_result_t bbl_qtype_get_array_size(bbl_qtype_t qtype, size_t* size);

void bbl_hello(char const* msg);

#ifdef __cplusplus
}
#endif

#endif