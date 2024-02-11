#include <stdint.h>

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


using test016_Bar = int64_t;

using test016_Foo_t = qux::Foo;

extern "C" {
int test016_Foo_take_enum(test016_Foo_t const* _this, int64_t b) {
    try {
        _this->take_enum(static_cast<qux::Bar>(b));
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test016_Foo_take_enum_constref(test016_Foo_t const* _this, int64_t const* b) {
    try {
        _this->take_enum_constref(*reinterpret_cast<qux::Bar const*>(b));
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test016_Foo_return_enum(test016_Foo_t* _this, int64_t* _result) {
    try {
        *_result = static_cast<int64_t>(_this->return_enum());
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test016_Foo_return_enum_by_ref(test016_Foo_t* _this, int64_t** _result) {
    try {
        *_result = reinterpret_cast<int64_t*>(&_this->return_enum_by_ref());
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test016_Foo_ctor(test016_Foo_t** _result) {
    *_result = new qux::Foo();
    return 0;
}

int test016_Foo_dtor(test016_Foo_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
