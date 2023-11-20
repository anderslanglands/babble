#ifndef __TEST024_C_H__
#define __TEST024_C_H__

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

typedef struct BBL_ALIGN(4) test0024_Bar_t {
    float data[3];
} test0024_Bar_t;




/** functions */

int test0024_Bar_set_data(test0024_Bar_t* _this, float* d);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
