
#include <string>

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

namespace bblext {

static float custom_foo(Foo const& foo) {
    return foo.get<float>();
}
}

extern "C" {

using test0022_Foo_t = Foo;

int test0022_Foo_dtor(test0022_Foo_t* _this) {
    delete _this;
    return 0;
}

int test0022_custom_foo(test0022_Foo_t const* foo, float* _result) {
    *_result = bblext::custom_foo(*foo);
    return 0;
}

} // extern "C"
