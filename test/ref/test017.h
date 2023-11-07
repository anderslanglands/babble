#ifndef __TEST017_H__
#define __TEST017_H__

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

typedef struct BBL_ALIGN(4) test017_Vec3f_t {
    float x;
    float y;
    float z;
} test017_Vec3f_t;

typedef struct test017_Foo_t test017_Foo_t;

typedef struct test017_Bar_t test017_Bar_t;




/** functions */

int test017_Vec3f_length(test017_Vec3f_t const* _this, float* _result);

int test017_Vec3f_dist(test017_Vec3f_t const* _this, test017_Vec3f_t const* other, float* _result);

int test017_Vec3f_ctor(test017_Vec3f_t* _result);

int test017_Foo_do_foo(test017_Foo_t const* _this, test017_Foo_t const* other);

int test017_Foo_do_foo2(test017_Foo_t* _this, test017_Foo_t* other);

int test017_Foo_ctor(int a, float b, test017_Foo_t** _result);

int test017_Foo_dtor(test017_Foo_t* _this);

int test017_Bar_ctor(int a, float b, test017_Bar_t** _result);

int test017_Bar_dtor(test017_Bar_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
