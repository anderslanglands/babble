#ifndef __TEST042_C_H__
#define __TEST042_C_H__

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

typedef struct test042_Abstract_t test042_Abstract_t;

typedef struct test042_Base1_t test042_Base1_t;

typedef struct test042_Base2_t test042_Base2_t;




/** functions */

int test042_Abstract_do_pure_thing(test042_Abstract_t* _this);

int test042_Abstract_public_method(test042_Abstract_t* _this);

int test042_Abstract_dtor(test042_Abstract_t* _this);

int test042_Base1_do_pure_thing(test042_Base1_t* _this);

int test042_Base1_public_method(test042_Base1_t* _this);

int test042_Base1_dtor(test042_Base1_t* _this);

int test042_Base2_do_pure_thing(test042_Base2_t* _this);

int test042_Base2_public_method(test042_Base2_t* _this);

int test042_Base2_dtor(test042_Base2_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
