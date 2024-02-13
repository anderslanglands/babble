#ifndef __TEST047_C_H__
#define __TEST047_C_H__

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

typedef struct test047_FloatIntPair_t test047_FloatIntPair_t;




/** functions */

int test047_FloatIntPair_dtor(test047_FloatIntPair_t* _this);

int test047_FloatIntPair_first(test047_FloatIntPair_t* p, float** _result);

int test047_FloatIntPair_second(test047_FloatIntPair_t* p, int** _result);

int test047_FloatIntPair_first_const(test047_FloatIntPair_t const* p, float const** _result);

int test047_FloatIntPair_second_const(test047_FloatIntPair_t const* p, int const** _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
