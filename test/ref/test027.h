#ifndef __TEST027_H__
#define __TEST027_H__

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

typedef struct test027_Foo_t test027_Foo_t;
typedef struct test027_FooPtr_t test027_FooPtr_t;



/** functions */

int test027_Foo_create(test027_FooPtr_t** _result);

int test027_Foo_get_foo(test027_Foo_t const* _this, int* _result);

int test027_Foo_dtor(test027_Foo_t* _this);

int test027_FooPtr_new(test027_FooPtr_t** _result);

int test027_FooPtr_dtor(test027_FooPtr_t* _this);

int test027_FooPtr_get_foo(test027_FooPtr_t const* _this, int* _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
