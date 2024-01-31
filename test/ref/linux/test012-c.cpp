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


using test012_Base_t = qux::Base;
using test012_Foo_t = qux::Foo;

extern "C" {
int test012_Base_bar(test012_Base_t* _this) {
    try {
        _this->bar();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test012_Base_dtor(test012_Base_t* _this) {
    delete _this;
    return 0;
}

int test012_Foo_bar(test012_Foo_t* _this) {
    try {
        _this->bar();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test012_Foo_ctor(test012_Foo_t** _result) {
    *_result = new qux::Foo();
    return 0;
}

int test012_Foo_dtor(test012_Foo_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
