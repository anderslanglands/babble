
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



using test01_Foo_t = qux::Foo;
using test01_Bum_t = qux::Bum;

extern "C" {
int test01_Foo_bar(test01_Foo_t* _this, float a, int* _result) {
    try {
        *_result = _this->bar(a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test01_Foo_dtor(test01_Foo_t* _this) {
    delete _this;
    return 0;
}

int test01_Bum_baz(test01_Bum_t* _this, int b) {
    try {
        _this->baz(b);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test01_Bum_bum(test01_Bum_t* _this, short d, char* _result) {
    try {
        *_result = _this->bum(d);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test01_Bum_boo(test01_Bum_t* _this, float f, float* _result) {
    try {
        *_result = _this->boo(f);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test01_Bum_dtor(test01_Bum_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
