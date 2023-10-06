#ifndef __TEST008_H__
#define __TEST008_H__

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

struct test_std_vector_StdVectorInt_t;
struct test_std_vector_StdPairIntFloat_t;



/** functions */

int test_std_vector_StdVectorInt_data(test_std_vector_StdVectorInt_t* _this, test_std_vector_StdPairIntFloat_t** _result);

int test_std_vector_StdVectorInt_data_const(test_std_vector_StdVectorInt_t const* _this, test_std_vector_StdPairIntFloat_t const** _result);

int test_std_vector_StdVectorInt_empty(test_std_vector_StdVectorInt_t const* _this, bool* _result);

int test_std_vector_StdVectorInt_size(test_std_vector_StdVectorInt_t const* _this, size_t* _result);

int test_std_vector_StdVectorInt_max_size(test_std_vector_StdVectorInt_t const* _this, size_t* _result);

int test_std_vector_StdVectorInt_reserve(test_std_vector_StdVectorInt_t* _this, size_t const _Newcapacity);

int test_std_vector_StdVectorInt_capacity(test_std_vector_StdVectorInt_t const* _this, size_t* _result);

int test_std_vector_StdVectorInt_clear(test_std_vector_StdVectorInt_t* _this);

int test_std_vector_StdVectorInt_push_back(test_std_vector_StdVectorInt_t* _this, test_std_vector_StdPairIntFloat_t const* _Val);

int test_std_vector_StdVectorInt_pop_back(test_std_vector_StdVectorInt_t* _this);

int test_std_vector_StdVectorInt_resize(test_std_vector_StdVectorInt_t* _this, size_t const _Newsize);

int test_std_vector_StdVectorInt_resize_with(test_std_vector_StdVectorInt_t* _this, size_t const _Newsize, test_std_vector_StdPairIntFloat_t const* _Val);

int test_std_vector_StdVectorInt_op_index(test_std_vector_StdVectorInt_t const* _this, size_t const _Pos, test_std_vector_StdPairIntFloat_t const** _result);

int test_std_vector_StdVectorInt_default(test_std_vector_StdVectorInt_t** _result);

int test_std_vector_StdVectorInt_dtor(test_std_vector_StdVectorInt_t* _this);

int test_std_vector_StdPairIntFloat_dtor(test_std_vector_StdPairIntFloat_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
