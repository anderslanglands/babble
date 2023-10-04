
#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test01_Foo_t = qux::Foo;

int test01_Foo_bar(test01_Foo_t* _this, float a, int* _result) {
    *_result = _this->bar(a);
    return 0;
}

int test01_Foo_dtor(test01_Foo_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
