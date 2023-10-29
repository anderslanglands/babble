#ifndef __TEST028_H__
#define __TEST028_H__

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
typedef struct test027_Bar_t test027_Bar_t;



/** functions */

int test027_Foo_dtor(test027_Foo_t* _this);

int test027_Bar_adjust_foo(void (*fn)(test027_Foo_t* param00, test027_Foo_t** _result));

int test027_Bar_dtor(test027_Bar_t* _this);

int test027_free_adjust_foo(void (*fn)(test027_Foo_t* param00, test027_Foo_t** _result), int* _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
