#ifndef __TEST007_H__
#define __TEST007_H__

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

struct BBL_ALIGN(4) test007_qux::Bar_t {
    int f1;
    float f2;
};




/** functions */

int test007_qux::Bar_bar(test007_qux::Bar_t const* _this, int* _result);

int test007_qux::Bar_baz();

int test007_qux::Bar_bum();

/**
 * function foo
 */
int myfoo(test007_qux::Bar_t* bar, size_t* _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
