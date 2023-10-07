#ifndef __TEST023_H__
#define __TEST023_H__

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

typedef struct BBL_ALIGN(4) test0023_qux::Foo_t {
    float x;
    float y;
    float z;
} test0023_qux::Foo_t;




/** functions */

int test0023_qux::Foo_length(test0023_qux::Foo_t* _this, float* _result);

int test0023_qux::Foo_normalize(test0023_qux::Foo_t* _this, float* _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
