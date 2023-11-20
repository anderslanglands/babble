#ifndef __TEST031_C_H__
#define __TEST031_C_H__

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

typedef struct test031_Foo_t test031_Foo_t;

typedef struct test031_FooPtr_t test031_FooPtr_t;

typedef struct test031_ConstFooPtr_t test031_ConstFooPtr_t;

typedef struct test031_Bar_t test031_Bar_t;

typedef struct test031_BarPtr_t test031_BarPtr_t;

typedef struct test031_ConstBarPtr_t test031_ConstBarPtr_t;




/** functions */

int test031_Foo_create(test031_FooPtr_t** _result);

int test031_Foo_get_foo(test031_Foo_t const* _this, int* _result);

int test031_Foo_set_foo(test031_Foo_t* _this, int a, int* _result);

int test031_Foo_dtor(test031_Foo_t* _this);

int test031_FooPtr_dtor(test031_FooPtr_t* _this);

int test031_FooPtr_get_foo(test031_FooPtr_t const* _this, int* _result);

int test031_FooPtr_set_foo(test031_FooPtr_t* _this, int a, int* _result);

int test031_ConstFooPtr_dtor(test031_ConstFooPtr_t* _this);

int test031_ConstFooPtr_get_foo(test031_ConstFooPtr_t const* _this, int* _result);

int test031_Bar_create(test031_BarPtr_t** _result);

int test031_Bar_baz(test031_Bar_t* _this);

int test031_Bar_dtor(test031_Bar_t* _this);

int test031_Bar_get_foo(test031_Bar_t const* _this, int* _result);

int test031_Bar_set_foo(test031_Bar_t* _this, int a, int* _result);

int test031_BarPtr_dtor(test031_BarPtr_t* _this);

int test031_BarPtr_baz(test031_BarPtr_t* _this);

int test031_BarPtr_get_foo(test031_BarPtr_t const* _this, int* _result);

int test031_BarPtr_set_foo(test031_BarPtr_t* _this, int a, int* _result);

int test031_ConstBarPtr_dtor(test031_ConstBarPtr_t* _this);

int test031_ConstBarPtr_get_foo(test031_ConstBarPtr_t const* _this, int* _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
