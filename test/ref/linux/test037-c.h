#ifndef __TEST037_C_H__
#define __TEST037_C_H__

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

typedef struct test037_Bar_t test037_Bar_t;




/** functions */

int test037_Bar_baz(test037_Bar_t const* _this, int b);

int test037_Bar_dtor(test037_Bar_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
