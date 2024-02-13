
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



using test030_Transform_t = OCIO_NAMESPACE::Transform;

extern "C" {
int test030_Transform_getValues(test030_Transform_t const* _this, double (*values)[3]) {
    try {
        _this->getValues(*values);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test030_Transform_setValues(test030_Transform_t* _this, double const (*values)[3]) {
    try {
        _this->setValues(*values);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test030_Transform_dtor(test030_Transform_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
