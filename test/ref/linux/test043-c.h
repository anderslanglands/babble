#ifndef __TEST043_C_H__
#define __TEST043_C_H__

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

typedef struct test043_Foo_t test043_Foo_t;




/** functions */

int test043_Foo_foo1(test043_Foo_t const* _this, int* _result);

int test043_Foo_foo1(test043_Foo_t* _this, int* _result);

int test043_Foo_dtor(test043_Foo_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
