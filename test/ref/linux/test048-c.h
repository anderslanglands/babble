#ifndef __TEST048_C_H__
#define __TEST048_C_H__

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

enum test048_Bar {
    test048_Bar_Baz = 0,
    test048_Bar_Bum = 1,
    test048_Bar_Qux = 2,
};


/** structs */

typedef struct test048_IntBarPair_t test048_IntBarPair_t;




/** functions */

int test048_IntBarPair_dtor(test048_IntBarPair_t* _this);

int test048_IntBarPair_first(test048_IntBarPair_t* p, int** _result);

int test048_IntBarPair_second(test048_IntBarPair_t* p, unsigned int** _result);

int test048_IntBarPair_first_const(test048_IntBarPair_t const* p, int const** _result);

int test048_IntBarPair_second_const(test048_IntBarPair_t const* p, unsigned int const** _result);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
