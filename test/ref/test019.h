#ifndef __TEST019_H__
#define __TEST019_H__

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

enum bar_Enum {
    BarEnumZero = 0,
    BarEnumOne = 1,
    BarEnumTwo = 2,
};

enum baz_Enum {
    BAZ_BazEnumZero = 0,
    BAZ_BazEnumOne = 1,
    BAZ_BazEnumTwo = 2,
};


/** structs */

typedef struct bar_Foo_t bar_Foo_t;

typedef struct baz_Foo_t baz_Foo_t;




/** functions */

int bar_Foo_ctor(int a, float b, bar_Foo_t** _result);

int bar_Foo_dtor(bar_Foo_t* _this);

int bar_bar_fn(bar_Foo_t const* a, int b, int* c);

int baz_Foo_ctor(int a, float b, baz_Foo_t** _result);

int baz_Foo_dtor(baz_Foo_t* _this);

int baz_baz_fn(baz_Foo_t const* a, int b, int* c);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
