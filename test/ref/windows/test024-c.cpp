
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

static thread_local std::string _bbl_error_message;

// This is necessary for some type definitions in lambdas defined in binding macros
namespace bbl { namespace detail {

template <typename T> struct argument_type;
template <typename T, typename U> struct argument_type<T(U)> {
    typedef U type;
};

} }



using test0024_Bar_t = qux::Bar;
struct BBL_ALIGN(4) test0024_Bar_t_bbl_size_check {
    float data[3];
};
static_assert(sizeof(test0024_Bar_t_bbl_size_check) == sizeof(qux::Bar), "size of value type does not match");
static_assert(alignof(test0024_Bar_t_bbl_size_check) == alignof(qux::Bar), "align of value type does not match");


extern "C" {
int test0024_Bar_set_data(test0024_Bar_t* _this, float* d) {
    try {
        _this->set_data(d);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

} // extern "C"
