
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



using test036_ustring_t = OIIO::ustring;

extern "C" {
int test036_ustring_op_lt(test036_ustring_t const* _this, test036_ustring_t const* x, bool* _result) {
    *_result = _this->operator<(*x);
    return 0;
}

int test036_ustring_dtor(test036_ustring_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
