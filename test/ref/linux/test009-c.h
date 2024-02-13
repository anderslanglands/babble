#ifndef __TEST009_C_H__
#define __TEST009_C_H__

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

typedef struct test_std_pair_StdPairIntFloat_t test_std_pair_StdPairIntFloat_t;




/** functions */

int test_std_pair_StdPairIntFloat_dtor(test_std_pair_StdPairIntFloat_t* _this);

int test_std_pair_StdPairIntFloat_first(test_std_pair_StdPairIntFloat_t* p, int** _result);

int test_std_pair_StdPairIntFloat_second(test_std_pair_StdPairIntFloat_t* p, float** _result);

int test_std_pair_StdPairIntFloat_first_const(test_std_pair_StdPairIntFloat_t const* p, int const** _result);

int test_std_pair_StdPairIntFloat_second_const(test_std_pair_StdPairIntFloat_t const* p, float const** _result);

int test_std_pair_foo(int a);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
