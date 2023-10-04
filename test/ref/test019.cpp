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
using bar_Enum = int;
using baz_Enum = int;

using bar_Foo_t = QUX_NS::BarFoo;
using baz_Foo_t = QUX_NS::BazFoo;

int bar_Foo_ctor(int a, float b, bar_Foo_t** _result) {
    *_result = new QUX_NS::BarFoo(a, b);
    return 0;
}

int bar_Foo_dtor(bar_Foo_t* _this) {
    delete _this;
    return 0;
}

int bar_fn(bar_Foo_t const* a, int b) {
    bar_fn(*a, static_cast<QUX_NS::BarEnum>(b));
    return 0;
}

int baz_Foo_ctor(int a, float b, baz_Foo_t** _result) {
    *_result = new QUX_NS::BazFoo(a, b);
    return 0;
}

int baz_Foo_dtor(baz_Foo_t* _this) {
    delete _this;
    return 0;
}

int baz_fn(baz_Foo_t const* a, int b) {
    baz_fn(*a, static_cast<QUX_NS::BazEnum>(b));
    return 0;
}

} // extern "C"
