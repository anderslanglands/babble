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

using test011_FooFloat_t = qux::Foo<float>;

int test011_FooFloat_bar(test011_FooFloat_t* _this, float const* a, float* _result) {
    *_result = _this->bar(*a);
    return 0;
}

int test011_FooFloat_ctor(test011_FooFloat_t** _result) {
    *_result = new qux::Foo<float>();
    return 0;
}

int test011_FooFloat_with_float(float* f, test011_FooFloat_t** _result) {
    *_result = new qux::Foo<float>(*f);
    return 0;
}

int test011_FooFloat_dtor(test011_FooFloat_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
