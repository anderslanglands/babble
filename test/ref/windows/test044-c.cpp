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


using test044_Foo_t = foo::Foo;

extern "C" {
int test044_Foo_foo1(test044_Foo_t const* _this, int* _result) {
    try {
        *_result = _this->foo1();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test044_Foo_dtor(test044_Foo_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
