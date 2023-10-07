#ifndef __TEST016_H__
#define __TEST016_H__

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

enum test016_qux::Bar {
    test016_qux::Bar_Baz = 0,
    test016_qux::Bar_Bum = 1,
    test016_qux::Bar_Qux = 2,
};


/** structs */

typedef struct test016_qux::Foo_t test016_qux::Foo_t;



/** functions */

int test016_qux::Foo_take_enum(test016_qux::Foo_t const* _this, int64_t b);

int test016_qux::Foo_return_enum(test016_qux::Foo_t* _this, int64_t* _result);

int test016_qux::Foo_ctor(test016_qux::Foo_t** _result);

int test016_qux::Foo_dtor(test016_qux::Foo_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
