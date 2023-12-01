#ifndef __TEST033_C_H__
#define __TEST033_C_H__

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

typedef struct test033_Foo_t test033_Foo_t;




/** functions */

int test033_Foo_dtor(test033_Foo_t* _this);

int test033_get_name_wrapped(test033_Foo_t* foo, char const* name, int* _result);

int test033_get_name_wrapped(char const** chars, size_t* len);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
