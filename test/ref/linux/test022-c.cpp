
#include <string>

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>
#include <exception>
#include <thread>
#include <string>

static thread_local std::string _bbl_error_message;

namespace bblext {

static float custom_foo(Foo const& foo) {
    return foo.get<float>();
}
}


using test0022_Foo_t = Foo;

extern "C" {
int test0022_Foo_dtor(test0022_Foo_t* _this) {
    delete _this;
    return 0;
}

int test0022_custom_foo(test0022_Foo_t const* foo, float* _result) {
    try {
        *_result = bblext::custom_foo(*foo);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

} // extern "C"
