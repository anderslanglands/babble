#ifndef __TEST038_C_H__
#define __TEST038_C_H__

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

int test037_Bar_get_int(test037_Bar_t* _this, int** _result);

int test037_Bar_get_bar(test037_Bar_t* _this, test037_Bar_t const** _result);

int test037_Bar_dtor(test037_Bar_t* _this);

int test037_get_float(float** _result);

int test037_get_bar_ref(test037_Bar_t const** _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
