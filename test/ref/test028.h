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

typedef struct test028_Foo_t test028_Foo_t;
typedef struct test028_Bar_t test028_Bar_t;



/** functions */

int test028_Foo_dtor(test028_Foo_t* _this);

int test028_Bar_adjust_foo(void (*fn)(test028_Foo_t* param00, test028_Foo_t** _result));

int test028_Bar_dtor(test028_Bar_t* _this);

int test028_free_adjust_foo(void (*fn)(test028_Foo_t* param00, test028_Foo_t** _result), int* _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
