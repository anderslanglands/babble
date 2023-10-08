#ifndef __TEST003_H__
#define __TEST003_H__

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

typedef struct test003_FooFloat_t test003_FooFloat_t;
typedef struct BBL_ALIGN(4) test003_FooInt_t {
} test003_FooInt_t;

typedef struct test003_Baz_t test003_Baz_t;



/** functions */

int test003_FooFloat_bar(test003_FooFloat_t* _this, float const* a, float* _result);

int test003_FooFloat_baz(test003_FooFloat_t* _this, test003_Baz_t* b);

int test003_FooFloat_dtor(test003_FooFloat_t* _this);

int test003_FooInt_bar(test003_FooInt_t* _this, int const* a, int* _result);

int test003_FooInt_baz(test003_FooInt_t* _this, test003_Baz_t* b);

int test003_Baz_dtor(test003_Baz_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
