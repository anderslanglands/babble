#ifndef __TEST002_H__
#define __TEST002_H__

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** enums */


/** structs */

struct test002_FooFloat_t;
struct test002_FooInt_t;



/** functions */

int test002_FooFloat_bar(test002_FooFloat_t* _this, float const* a, float* _result);

int test002_FooFloat_dtor(test002_FooFloat_t* _this);

int test002_FooInt_bar(test002_FooInt_t* _this, int const* a, int* _result);

int test002_FooInt_dtor(test002_FooInt_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
