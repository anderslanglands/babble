#ifndef __TEST049_C_H__
#define __TEST049_C_H__

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

typedef struct test049_String_t test049_String_t;

typedef struct test049_StringSet_t test049_StringSet_t;

typedef struct test049_StringSetIterator_t test049_StringSetIterator_t;




/** functions */

int test049_String_dtor(test049_String_t* _this);

int test049_StringSet_empty(test049_StringSet_t const* _this, bool* _result);

int test049_StringSet_size(test049_StringSet_t const* _this, size_t* _result);

int test049_StringSet_begin(test049_StringSet_t const* _this, test049_StringSetIterator_t** _result);

int test049_StringSet_end(test049_StringSet_t const* _this, test049_StringSetIterator_t** _result);

int test049_StringSet_find(test049_StringSet_t* _this, test049_String_t const* __x, test049_StringSetIterator_t** _result);

int test049_StringSet_dtor(test049_StringSet_t* _this);

int test049_StringSetIterator_op_inc(test049_StringSetIterator_t* _this, test049_StringSetIterator_t** _result);

int test049_StringSetIterator_op_mul(test049_StringSetIterator_t const* _this, test049_String_t const** _result);

int test049_StringSetIterator_dtor(test049_StringSetIterator_t* _this);

int test049_StringSetIterator_op_eq(test049_StringSetIterator_t const* l, test049_StringSetIterator_t const* r, bool* _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
