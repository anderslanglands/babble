
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

extern "C" {

using test004_FooFloat_t = Foo<float>;
using test004_FooInt_t = FooInt;

int test004_FooFloat_baz_int(test004_FooFloat_t* _this, size_t a) {
    try {
        _this->baz(a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test004_FooFloat_baz_float(test004_FooFloat_t* _this, float a) {
    try {
        _this->baz(a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test004_FooFloat_dtor(test004_FooFloat_t* _this) {
    delete _this;
    return 0;
}

int test004_FooInt_baz_int(test004_FooInt_t* _this, size_t a) {
    try {
        _this->baz(a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test004_FooInt_baz_float(test004_FooInt_t* _this, float a) {
    try {
        _this->baz(a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test004_FooInt_dtor(test004_FooInt_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
