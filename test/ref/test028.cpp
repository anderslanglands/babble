#include <functional>

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test027_Foo_t = tst::Foo;
using test027_Bar_t = tst::Bar;

int test027_Foo_dtor(test027_Foo_t* _this) {
    delete _this;
    return 0;
}

int test027_Bar_adjust_foo(void (*fn)(test027_Foo_t* param00, test027_Foo_t** _result)) {
    auto fn_wrapper = [&](tst::Foo param00) {
        tst::Foo _result;
        tst::Foo* _result_ptr;
        fn(&param00, &_result_ptr);
        _result = std::move(*_result_ptr);
        delete _result_ptr;
        return _result;
    };
    tst::Bar::adjust_foo(fn_wrapper);
    return 0;
}

int test027_Bar_dtor(test027_Bar_t* _this) {
    delete _this;
    return 0;
}

int test027_free_adjust_foo(void (*fn)(test027_Foo_t* param00, test027_Foo_t** _result), int* _result) {
    auto fn_wrapper = [&](tst::Foo param00) {
        tst::Foo _result;
        tst::Foo* _result_ptr;
        fn(&param00, &_result_ptr);
        _result = std::move(*_result_ptr);
        delete _result_ptr;
        return _result;
    };
    *_result = tst::free_adjust_foo(fn_wrapper);
    return 0;
}

} // extern "C"
