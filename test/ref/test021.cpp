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

using test0021_Foo_t = Foo;
using test0021_BarShort_t = Bar<short>;

int test0021_Foo_get_float(test0021_Foo_t const* _this, float* _result) {
    *_result = _this->get<float>();
    return 0;
}

int test0021_Foo_get_int(test0021_Foo_t const* _this, int* _result) {
    *_result = _this->get<int>();
    return 0;
}

int test0021_Foo_get_bar_short(test0021_Foo_t const* _this, test0021_BarShort_t* _result) {
    *_result = _this->get<Bar<short>>();
    return 0;
}

int test0021_Foo_dtor(test0021_Foo_t* _this) {
    delete _this;
    return 0;
}

int test0021_BarShort_dtor(test0021_BarShort_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
