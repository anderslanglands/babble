
#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>
#include <exception>
#include <thread>
#include <string>

static thread_local std::string __bbl_error_message;

extern "C" {

using test029_GradingRGBM_t = OCIO_NAMESPACE::GradingRGBM;
struct BBL_ALIGN(8) test029_GradingRGBM_t_bbl_size_check {
    double m_red;
    double m_green;
    double m_blue;
    double m_master;
};
static_assert(sizeof(test029_GradingRGBM_t_bbl_size_check) == sizeof(OCIO_NAMESPACE::GradingRGBM), "size of value type does not match");
static_assert(alignof(test029_GradingRGBM_t_bbl_size_check) == alignof(OCIO_NAMESPACE::GradingRGBM), "align of value type does not match");


} // extern "C"
