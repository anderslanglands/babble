#ifndef __TEST006_H__
#define __TEST006_H__

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

enum test006a_Foo {
    test006a_Foo_Zero = 0,
    test006a_Foo_One = 1,
    test006a_Foo_Two = 2,
};

enum test006b_Bar {
    test006b_Bar_MinusOne = -1,
    test006b_Bar_Zero = 0,
    test006b_Bar_Two = 2,
    test006b_Bar_Three = 3,
    test006b_Bar_ThreeAsWell = 3,
};


/** structs */




/** functions */



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
