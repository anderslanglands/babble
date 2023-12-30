#ifndef __TEST039_C_H__
#define __TEST039_C_H__

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** enums */


/** structs */

typedef struct test039_Base_t test039_Base_t;

typedef struct test039_string_t test039_string_t;


typedef struct test039_Base_Subclass_t test039_Base_Subclass_t;

typedef int (*test039_Base_Subclass_t_hello)(test039_Base_t const*, test039_string_t*, int*, float*, float*, void*);
typedef int (*test039_Base_Subclass_t_hello2)(test039_Base_t const*, test039_string_t*, int*, void*);
typedef int (*test039_Base_Subclass_dtor)(test039_Base_Subclass_t*, void*);

int test039_Base_Subclass_default(test039_Base_Subclass_t** _result, test039_Base_Subclass_t_hello fn_hello, void* fn_hello_user_data, test039_Base_Subclass_t_hello2 fn_hello2, void* fn_hello2_user_data, test039_Base_Subclass_dtor fn_dtor, void* fn_dtor_user_data);
int test039_Base_Subclass_from_str(test039_string_t* foo, test039_string_t* foo_ref, test039_string_t* foo_ptr, test039_Base_Subclass_t** _result, test039_Base_Subclass_t_hello fn_hello, void* fn_hello_user_data, test039_Base_Subclass_t_hello2 fn_hello2, void* fn_hello2_user_data, test039_Base_Subclass_dtor fn_dtor, void* fn_dtor_user_data);
int test039_Base_Subclass_from_int(int foo, int* foo_ref, int* foo_ptr, test039_Base_Subclass_t** _result, test039_Base_Subclass_t_hello fn_hello, void* fn_hello_user_data, test039_Base_Subclass_t_hello2 fn_hello2, void* fn_hello2_user_data, test039_Base_Subclass_dtor fn_dtor, void* fn_dtor_user_data);


/** functions */

int test039_Base_hello(test039_Base_t const* _this, test039_string_t* str, int* a, float* b, float* _result);

int test039_Base_hello2(test039_Base_t const* _this, test039_string_t* str, int* _result);

int test039_Base_nonvirtual(test039_Base_t* _this);

int test039_Base_default(test039_Base_t** _result);

int test039_Base_from_str(test039_string_t* foo, test039_string_t* foo_ref, test039_string_t* foo_ptr, test039_Base_t** _result);

int test039_Base_from_int(int foo, int* foo_ref, int* foo_ptr, test039_Base_t** _result);

int test039_Base_dtor(test039_Base_t* _this);

int test039_string_dtor(test039_string_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
