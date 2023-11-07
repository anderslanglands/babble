#ifndef __TEST012_H__
#define __TEST012_H__

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

typedef struct test012_Base_t test012_Base_t;

typedef struct test012_Foo_t test012_Foo_t;




/** functions */

int test012_Base_bar(test012_Base_t* _this);

int test012_Base_dtor(test012_Base_t* _this);

int test012_Foo_bar(test012_Foo_t* _this);

int test012_Foo_ctor(test012_Foo_t** _result);

int test012_Foo_dtor(test012_Foo_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
