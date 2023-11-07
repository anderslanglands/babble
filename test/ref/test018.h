#ifndef __TEST018_H__
#define __TEST018_H__

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

typedef struct test018_Foo_t test018_Foo_t;




/** functions */

int test018_Foo_do_foo(test018_Foo_t const* _this, void (*fun)(test018_Foo_t const* param00, int param01, float param02, test018_Foo_t** _result));

int test018_Foo_do_foo2(test018_Foo_t const* _this, void (*fun)(test018_Foo_t const* param00, int param01, float param02, test018_Foo_t const** _result));

int test018_Foo_ctor(int a, float b, test018_Foo_t** _result);

int test018_Foo_dtor(test018_Foo_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
