#ifndef __TEST011_H__
#define __TEST011_H__

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

typedef struct test011_FooFloat_t test011_FooFloat_t;




/** functions */

int test011_FooFloat_bar(test011_FooFloat_t* _this, float const* a, float* _result);

int test011_FooFloat_ctor(test011_FooFloat_t** _result);

int test011_FooFloat_with_float(float* f, test011_FooFloat_t** _result);

int test011_FooFloat_dtor(test011_FooFloat_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
