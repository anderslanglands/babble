
#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {
using test016_Bar = int64_t;

using test016_Foo_t = qux::Foo;

int test016_Foo_take_enum(test016_Foo_t const* _this, int64_t b) {
    _this->take_enum(static_cast<qux::Bar>(b));
    return 0;
}

int test016_Foo_return_enum(test016_Foo_t* _this, int64_t* _result) {
    *_result = static_cast<int64_t>(_this->return_enum());
    return 0;
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
