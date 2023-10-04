
#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test004_FooFloat_t = Foo<float>;
using test004_FooInt_t = FooInt;

int test004_FooFloat_baz_int(test004_FooFloat_t* _this, size_t a) {
    _this->baz(a);
    return 0;
}

int test004_FooFloat_baz_float(test004_FooFloat_t* _this, float a) {
    _this->baz(a);
    return 0;
}

int test004_FooFloat_dtor(test004_FooFloat_t* _this) {
    delete _this;
    return 0;
}

int test004_FooInt_baz_int(test004_FooInt_t* _this, size_t a) {
    _this->baz(a);
    return 0;
}

int test004_FooInt_baz_float(test004_FooInt_t* _this, float a) {
    _this->baz(a);
    return 0;
}

int test004_FooInt_dtor(test004_FooInt_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
