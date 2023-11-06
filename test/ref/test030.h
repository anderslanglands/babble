#ifndef __TEST030_H__
#define __TEST030_H__

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

typedef struct test030_Transform_t test030_Transform_t;



/** functions */

int test030_Transform_getValues(test030_Transform_t const* _this, double (*values)[3]);

int test030_Transform_setValues(test030_Transform_t* _this, double const (*values)[3]);

int test030_Transform_dtor(test030_Transform_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
