#ifndef __TEST029_C_H__
#define __TEST029_C_H__

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

typedef struct BBL_ALIGN(8) test029_GradingRGBM_t {
    double m_red;
    double m_green;
    double m_blue;
    double m_master;
} test029_GradingRGBM_t;




/** functions */



#ifdef __cplusplus
}
#endif

#undef BBL_ALIGN


#endif
