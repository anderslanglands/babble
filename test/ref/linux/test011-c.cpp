#include <string>

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



using test011_FooFloat_t = qux::Foo<float>;

extern "C" {
int test011_FooFloat_bar(test011_FooFloat_t* _this, float const* a, float* _result) {
    try {
        *_result = _this->bar(*a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test011_FooFloat_ctor(test011_FooFloat_t** _result) {
    *_result = new qux::Foo<float>();
    return 0;
}

int test011_FooFloat_with_float(float* f, test011_FooFloat_t** _result) {
    try {
        *_result = new qux::Foo<float>(*f);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test011_FooFloat_dtor(test011_FooFloat_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
