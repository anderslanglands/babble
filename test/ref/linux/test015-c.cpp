
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

extern "C" {

using test015_Bar_t = qux::Bar;
using test015_Foo_t = qux::Foo;

int test015_Bar_baz(test015_Bar_t const* _this, char const** s, float* const f, int** _result) {
    try {
        *_result = _this->baz(s, f);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test015_Bar_ctor(test015_Bar_t** _result) {
    try {
        *_result = new qux::Bar();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test015_Bar_dtor(test015_Bar_t* _this) {
    delete _this;
    return 0;
}

int test015_Foo_foo(test015_Foo_t const* _this, test015_Bar_t* b, int** i, test015_Foo_t const** _result) {
    try {
        *_result = &_this->foo(*b, *i);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test015_Foo_foo2(test015_Foo_t* _this, test015_Bar_t* b) {
    _this->foo2(*b);
    return 0;
}

int test015_Foo_foo3(test015_Bar_t const* b, int* _result) {
    *_result = qux::Foo::foo3(*b);
    return 0;
}

int test015_Foo_ctor(test015_Foo_t** _result) {
    try {
        *_result = new qux::Foo();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test015_Foo_dtor(test015_Foo_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
