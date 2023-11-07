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

static thread_local std::string __bbl_error_message;

extern "C" {

using test018_Foo_t = qux::Foo;

int test018_Foo_do_foo(test018_Foo_t const* _this, void (*fun)(test018_Foo_t const* param00, int param01, float param02, test018_Foo_t** _result)) {
    try {
        std::function<qux::Foo (qux::Foo const&, int, float)> fun_wrapper = [&](qux::Foo const& param00, int param01, float param02) {
        qux::Foo _result;
        qux::Foo* _result_ptr;
        fun(&param00, param01, param02, &_result_ptr);
        _result = std::move(*_result_ptr);
        delete _result_ptr;
        return _result;
    };
        _this->do_foo(fun_wrapper);
        return 0;
    } catch (std::exception& e) {
        __bbl_error_message = e.what();
        return 1;
    }
}

int test018_Foo_do_foo2(test018_Foo_t const* _this, void (*fun)(test018_Foo_t const* param00, int param01, float param02, test018_Foo_t const** _result)) {
    try {
        std::function<qux::Foo const& (qux::Foo const&, int, float)> fun_wrapper = [&](qux::Foo const& param00, int param01, float param02) {
        qux::Foo const* _result;
        fun(&param00, param01, param02, &_result);
        return *_result;
    };
        _this->do_foo2(fun_wrapper);
        return 0;
    } catch (std::exception& e) {
        __bbl_error_message = e.what();
        return 1;
    }
}

int test018_Foo_ctor(int a, float b, test018_Foo_t** _result) {
    try {
        *_result = new qux::Foo(a, b);
        return 0;
    } catch (std::exception& e) {
        __bbl_error_message = e.what();
        return 1;
    }
}

int test018_Foo_dtor(test018_Foo_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
