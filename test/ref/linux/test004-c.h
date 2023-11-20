#ifndef __TEST004_C_H__
#define __TEST004_C_H__

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

typedef struct test004_FooFloat_t test004_FooFloat_t;

typedef struct test004_FooInt_t test004_FooInt_t;




/** functions */

int test004_FooFloat_baz_int(test004_FooFloat_t* _this, size_t a);

int test004_FooFloat_baz_float(test004_FooFloat_t* _this, float a);

int test004_FooFloat_dtor(test004_FooFloat_t* _this);

int test004_FooInt_baz_int(test004_FooInt_t* _this, size_t a);

int test004_FooInt_baz_float(test004_FooInt_t* _this, float a);

int test004_FooInt_dtor(test004_FooInt_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
