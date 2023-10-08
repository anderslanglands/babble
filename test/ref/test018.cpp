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

using test018_qux::Foo_t = qux::Foo;

int test018_qux::Foo_do_foo(test018_qux::Foo_t const* _this, bool(*)(test018_qux::Foo_t const*)* fun) {
    _this->do_foo(*fun);
    return 0;
}

int test018_qux::Foo_ctor(int a, float b, test018_qux::Foo_t** _result) {
    *_result = new qux::Foo(a, b);
    return 0;
}

int test018_qux::Foo_dtor(test018_qux::Foo_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
