
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



using test035_ImageCache_t = OIIO::ImageCache;

extern "C" {
int test035_ImageCache_hello(test035_ImageCache_t* _this) {
    try {
        _this->hello();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test035_ImageCache_op_assign(test035_ImageCache_t* _this, test035_ImageCache_t const* param00, test035_ImageCache_t** _result) {
    try {
        *_result = &_this->operator=(*param00);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test035_ImageCache_dtor(test035_ImageCache_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
