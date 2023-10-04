#ifndef __TEST012_H__
#define __TEST012_H__

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

struct test012_qux::Base_t;
struct test012_qux::Foo_t;



/** functions */

int test012_qux::Base_bar(test012_qux::Base_t* _this);

int test012_qux::Base_dtor(test012_qux::Base_t* _this);

int test012_qux::Foo_bar(test012_qux::Foo_t* _this);

int test012_qux::Foo_ctor(test012_qux::Foo_t** _result);

int test012_qux::Foo_dtor(test012_qux::Foo_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
