#ifndef __TEST034_C_H__
#define __TEST034_C_H__

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

typedef struct test034_Incomplete_t test034_Incomplete_t;

typedef struct test034_Complete_t test034_Complete_t;




/** functions */

int test034_Complete_hello(test034_Complete_t const* _this, char const* msg);

int test034_Complete_dtor(test034_Complete_t* _this);

int test034_Complete_create(test034_Complete_t** _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
