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

extern "C" {

using test0021_Foo_t = Foo;
using test0021_BarShort_t = Bar<short>;

int test0021_Foo_get_int(test0021_Foo_t const* _this, int* _result) {
    try {
        *_result = _this->get();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test0021_Foo_is_int(test0021_Foo_t const* _this, bool* _result) {
    try {
        *_result = _this->is();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test0021_Foo_get_float(test0021_Foo_t const* _this, float* _result) {
    try {
        *_result = _this->get();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test0021_Foo_is_float(test0021_Foo_t const* _this, bool* _result) {
    try {
        *_result = _this->is();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test0021_Foo_from_int(int value, test0021_Foo_t** _result) {
    try {
        *_result = new Foo(value);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test0021_Foo_from_float(float value, test0021_Foo_t** _result) {
    try {
        *_result = new Foo(value);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
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
