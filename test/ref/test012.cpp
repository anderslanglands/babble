#include <string>

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test012_Base_t = qux::Base;
using test012_Foo_t = qux::Foo;

int test012_Base_bar(test012_Base_t* _this) {
    _this->bar();
    return 0;
}

int test012_Base_dtor(test012_Base_t* _this) {
    delete _this;
    return 0;
}

int test012_Foo_bar(test012_Foo_t* _this) {
    _this->bar();
    return 0;
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
