#include <functional>

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


using bar_Enum = unsigned int;
using baz_Enum = unsigned int;

using bar_Foo_t = QUX_NS::BarFoo;
using baz_Foo_t = QUX_NS::BazFoo;

extern "C" {
int bar_Foo_ctor(int a, float b, bar_Foo_t** _result) {
    try {
        *_result = new QUX_NS::BarFoo(a, b);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int bar_Foo_dtor(bar_Foo_t* _this) {
    delete _this;
    return 0;
}

int bar_bar_fn(bar_Foo_t const* a, unsigned int b, unsigned int* c) {
    try {
        QUX_NS::bar_fn(*a, static_cast<QUX_NS::BarEnum>(b), reinterpret_cast<QUX_NS::BarEnum*>(c));
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int baz_Foo_ctor(int a, float b, baz_Foo_t** _result) {
    try {
        *_result = new QUX_NS::BazFoo(a, b);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int baz_Foo_dtor(baz_Foo_t* _this) {
    delete _this;
    return 0;
}

int baz_baz_fn(baz_Foo_t const* a, unsigned int b, unsigned int* c) {
    try {
        QUX_NS::baz_fn(*a, static_cast<QUX_NS::BazEnum>(b), *reinterpret_cast<QUX_NS::BazEnum*>(c));
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

} // extern "C"
