#ifndef __TEST015_H__
#define __TEST015_H__

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

typedef struct test015_Bar_t test015_Bar_t;

typedef struct test015_Foo_t test015_Foo_t;




/** functions */

int test015_Bar_baz(test015_Bar_t const* _this, char const** s, float* const f, int** _result);

int test015_Bar_ctor(test015_Bar_t** _result);

int test015_Bar_dtor(test015_Bar_t* _this);

int test015_Foo_foo(test015_Foo_t const* _this, test015_Bar_t* b, int** i, test015_Foo_t const** _result);

int test015_Foo_foo2(test015_Foo_t* _this, test015_Bar_t* b);

int test015_Foo_foo3(test015_Bar_t const* b, int* _result);

int test015_Foo_ctor(test015_Foo_t** _result);

int test015_Foo_dtor(test015_Foo_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
