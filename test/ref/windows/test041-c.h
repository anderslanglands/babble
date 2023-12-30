#ifndef __TEST041_C_H__
#define __TEST041_C_H__

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

typedef struct test041_Abstract_t test041_Abstract_t;

typedef struct test041_Base_t test041_Base_t;


typedef struct test041_Base_Subclass_t test041_Base_Subclass_t;

typedef int (*test041_Base_Subclass_t_do_protected_thing)(test041_Base_t*, void*);
typedef int (*test041_Base_Subclass_t_do_pure_thing)(test041_Base_t*, void*);
typedef int (*test041_Base_Subclass_dtor)(test041_Base_Subclass_t*, void*);

int test041_Base_Subclass_default(test041_Base_Subclass_t** _result, test041_Base_Subclass_t_do_protected_thing fn_do_protected_thing, void* fn_do_protected_thing_user_data, test041_Base_Subclass_t_do_pure_thing fn_do_pure_thing, void* fn_do_pure_thing_user_data, test041_Base_Subclass_dtor fn_dtor, void* fn_dtor_user_data);


/** functions */

int test041_Abstract_do_pure_thing(test041_Abstract_t* _this);

int test041_Abstract_public_method(test041_Abstract_t* _this);

int test041_Abstract_dtor(test041_Abstract_t* _this);

int test041_Base_do_protected_thing(test041_Base_t* _this);

int test041_Base_do_protected_thing_base(test041_Base_t* _this);

int test041_Base_do_pure_thing(test041_Base_t* _this);

int test041_Base_public_method(test041_Base_t* _this);

int test041_Base_dtor(test041_Base_t* _this);



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
