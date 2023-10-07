#ifndef __TEST005_H__
#define __TEST005_H__

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

typedef struct test005_FooFloat_t test005_FooFloat_t;



/** functions */

int test005_FooFloat_bar(test005_FooFloat_t* _this, float const* a, size_t* _result);

int test005_FooFloat_baz(test005_FooFloat_t* _this, test005_FooFloat_t* b, size_t* _result);

int test005_FooFloat_dtor(test005_FooFloat_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
