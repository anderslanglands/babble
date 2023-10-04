#ifndef __TEST001_H__
#define __TEST001_H__

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

/**
 * A doc comment on the Foo class - brief
 * 
 * More detailed description here
 * ```
 *     example code
 * ```
 * and some more detail
 */
struct test01_Foo_t;



/** functions */

/**
 * Comment on the method
 */
int test01_Foo_bar(test01_Foo_t* _this, float a, int* _result);

int test01_Foo_dtor(test01_Foo_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
