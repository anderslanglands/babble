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

extern "C" {

using test028_Foo_t = tst::Foo;
using test028_Bar_t = tst::Bar;

int test028_Foo_dtor(test028_Foo_t* _this) {
    delete _this;
    return 0;
}

int test028_Bar_adjust_foo(void (*fn)(test028_Foo_t* param00, test028_Foo_t** _result)) {
    try {
        std::function<tst::Foo (tst::Foo)> fn_wrapper = [&](tst::Foo param00) {
        tst::Foo _result;
        tst::Foo* _result_ptr;
        fn(&param00, &_result_ptr);
        _result = std::move(*_result_ptr);
        delete _result_ptr;
        return _result;
    };
        tst::Bar::adjust_foo(fn_wrapper);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test028_Bar_dtor(test028_Bar_t* _this) {
    delete _this;
    return 0;
}

int test028_free_adjust_foo(void (*fn)(test028_Foo_t* param00, test028_Foo_t** _result), int* _result) {
    try {
        std::function<tst::Foo (tst::Foo)> fn_wrapper = [&](tst::Foo param00) {
        tst::Foo _result;
        tst::Foo* _result_ptr;
        fn(&param00, &_result_ptr);
        _result = std::move(*_result_ptr);
        delete _result_ptr;
        return _result;
    };
        *_result = tst::free_adjust_foo(fn_wrapper);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

} // extern "C"
