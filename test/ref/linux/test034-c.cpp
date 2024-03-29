

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


namespace bblext {

auto Complete_create() -> foo::Complete* {
            return foo::Complete::create();
        }
}


using test034_Incomplete_t = foo::Incomplete;
using test034_Complete_t = foo::Complete;

extern "C" {
int test034_Complete_hello(test034_Complete_t const* _this, char const* msg) {
    try {
        _this->hello(msg);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test034_Complete_dtor(test034_Complete_t* _this) {
    delete _this;
    return 0;
}

int test034_Complete_create(test034_Complete_t** _result) {
    try {
        *_result = bblext::Complete_create();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

} // extern "C"
