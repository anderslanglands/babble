#ifndef __TEST009_H__
#define __TEST009_H__

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** enums */


/** structs */

struct test_std_pair_StdPairIntFloat_t;



/** functions */

int test_std_pair_StdPairIntFloat_dtor(test_std_pair_StdPairIntFloat_t* _this);

int StdPairIntFloat_get0(test_std_pair_StdPairIntFloat_t* _Pr, int* _result);

int StdPairIntFloat_get0_const(test_std_pair_StdPairIntFloat_t const* _Pr, int* _result);

int StdPairIntFloat_get1(test_std_pair_StdPairIntFloat_t* _Pr, float* _result);

int StdPairIntFloat_get1_const(test_std_pair_StdPairIntFloat_t const* _Pr, float* _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
