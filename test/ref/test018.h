#ifndef __TEST018_H__
#define __TEST018_H__

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

struct BBL_ALIGN(4) test018_qux::Foo_t {
    char _bbl_opaque[8];
};




/** functions */

int test018_qux::Foo_do_foo(test018_qux::Foo_t const* _this, bool(*)(test018_qux::Foo_t const*)* fun);

int test018_qux::Foo_ctor(int a, float b, test018_qux::Foo_t* _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
