#ifndef __TEST022_H__
#define __TEST022_H__

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

typedef struct test0022_Foo_t test0022_Foo_t;



/** functions */

int test0022_Foo_dtor(test0022_Foo_t* _this);

int test0022_custom_foo(test0022_Foo_t const* foo, float* _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
