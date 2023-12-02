#ifndef __TEST036_C_H__
#define __TEST036_C_H__

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

typedef struct test036_ustring_t test036_ustring_t;




/** functions */

int test036_ustring_op_lt(test036_ustring_t const* _this, test036_ustring_t const* x, bool* _result);

int test036_ustring_dtor(test036_ustring_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
