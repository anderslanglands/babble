#ifndef __TEST007_H__
#define __TEST007_H__

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

typedef struct BBL_ALIGN(4) test007_Bar_t {
    int f1;
    float f2;
} test007_Bar_t;




/** functions */

int test007_Bar_bar(test007_Bar_t const* _this, int* _result);

int test007_Bar_baz();

int test007_Bar_bum();



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
