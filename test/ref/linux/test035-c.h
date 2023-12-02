#ifndef __TEST035_C_H__
#define __TEST035_C_H__

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

typedef struct test035_ImageCache_t test035_ImageCache_t;




/** functions */

int test035_ImageCache_hello(test035_ImageCache_t* _this);

int test035_ImageCache_op_assign(test035_ImageCache_t* _this, test035_ImageCache_t const* param00, test035_ImageCache_t** _result);

int test035_ImageCache_dtor(test035_ImageCache_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
