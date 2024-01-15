#ifndef __TEST040_C_H__
#define __TEST040_C_H__

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

typedef struct test040_Base_t test040_Base_t;

typedef struct test040_Derived_t test040_Derived_t;

typedef struct test040_string_t test040_string_t;




/** functions */

int test040_Base_hello(test040_Base_t const* _this, test040_string_t* str, int* a, float* b, float* _result);

int test040_Base_hello2(test040_Base_t const* _this, test040_string_t* str, int* _result);

int test040_Base_nonvirtual(test040_Base_t* _this);

int test040_Base_default(test040_Base_t** _result);

int test040_Base_from_str(test040_string_t* foo, test040_string_t* foo_ref, test040_string_t* foo_ptr, test040_Base_t** _result);

int test040_Base_from_int(int foo, int* foo_ref, int* foo_ptr, test040_Base_t** _result);

int test040_Base_dtor(test040_Base_t* _this);

int test040_Derived_hello(test040_Derived_t const* _this, test040_string_t* str, int* a, float* b, float* _result);

int test040_Derived_hello2(test040_Derived_t const* _this, test040_string_t* str, int* _result);

int test040_Derived_nonvirtual(test040_Derived_t* _this);

int test040_Derived_dtor(test040_Derived_t* _this);

int test040_string_dtor(test040_string_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
