
#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test015_qux::Bar_t = qux::Bar;
using test015_qux::Foo_t = qux::Foo;

int test015_qux::Bar_baz(test015_qux::Bar_t const* _this, char const** s, float* const f, int** _result) {
    *_result = _this->baz(s, f);
    return 0;
}

int test015_qux::Bar_ctor(test015_qux::Bar_t** _result) {
    *_result = new qux::Bar();
    return 0;
}

int test015_qux::Bar_dtor(test015_qux::Bar_t* _this) {
    delete _this;
    return 0;
}

int test015_qux::Foo_foo(test015_qux::Foo_t const* _this, test015_qux::Bar_t* b, int** i, test015_qux::Foo_t const** _result) {
    *_result = &_this->foo(*b, *i);
    return 0;
}

int test015_qux::Foo_foo2(test015_qux::Foo_t* _this, test015_qux::Bar_t* b) {
    _this->foo2(*b);
    return 0;
}

int test015_qux::Foo_foo3(test015_qux::Bar_t const* b, int* _result) {
    *_result = qux::Foo::foo3(*b);
    return 0;
}

int test015_qux::Foo_ctor(test015_qux::Foo_t** _result) {
    *_result = new qux::Foo();
    return 0;
}

int test015_qux::Foo_dtor(test015_qux::Foo_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
