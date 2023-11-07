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

static thread_local std::string __bbl_error_message;

extern "C" {

using test011_FooFloat_t = qux::Foo<float>;

int test011_FooFloat_bar(test011_FooFloat_t* _this, float const* a, float* _result) {
    try {
        *_result = _this->bar(*a);
        return 0;
    } catch (std::exception& e) {
        __bbl_error_message = e.what();
        return 1;
    }
}

int test011_FooFloat_ctor(test011_FooFloat_t** _result) {
    *_result = new qux::Foo<float>();
    return 0;
}

int test011_FooFloat_with_float(float* f, test011_FooFloat_t** _result) {
    try {
        *_result = new qux::Foo<float>(*f);
        return 0;
    } catch (std::exception& e) {
        __bbl_error_message = e.what();
        return 1;
    }
}

int test011_FooFloat_dtor(test011_FooFloat_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
