#ifndef __TEST046_C_H__
#define __TEST046_C_H__

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

typedef struct test046_Foo_t test046_Foo_t;




/** functions */

int test046_Foo_dtor(test046_Foo_t* _this);

int test046_Foo_do_thing(test046_Foo_t const* _this, int a, float* _result);

int test046_normal_thing();



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
