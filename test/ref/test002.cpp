
#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test002_FooFloat_t = Foo<float>;
using test002_FooInt_t = FooInt;

int test002_FooFloat_bar(test002_FooFloat_t* _this, float const* a, float* _result) {
    *_result = _this->bar(*a);
    return 0;
}

int test002_FooFloat_dtor(test002_FooFloat_t* _this) {
    delete _this;
    return 0;
}

int test002_FooInt_bar(test002_FooInt_t* _this, int const* a, int* _result) {
    *_result = _this->bar(*a);
    return 0;
}

int test002_FooInt_dtor(test002_FooInt_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
