#ifndef __TEST026_H__
#define __TEST026_H__

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

typedef struct test0021_Foo_t test0021_Foo_t;

typedef struct test0021_BarShort_t test0021_BarShort_t;




/** functions */

int test0021_Foo_get_int(test0021_Foo_t const* _this, int* _result);

int test0021_Foo_is_int(test0021_Foo_t const* _this, bool* _result);

int test0021_Foo_get_float(test0021_Foo_t const* _this, float* _result);

int test0021_Foo_is_float(test0021_Foo_t const* _this, bool* _result);

int test0021_Foo_from_int(int value, test0021_Foo_t** _result);

int test0021_Foo_from_float(float value, test0021_Foo_t** _result);

int test0021_Foo_dtor(test0021_Foo_t* _this);

int test0021_BarShort_dtor(test0021_BarShort_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
