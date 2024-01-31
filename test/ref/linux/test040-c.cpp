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


using test040_Base_t = foo::Base;
using test040_Derived_t = foo::Derived;
using test040_string_t = std::string;

extern "C" {
int test040_Base_hello(test040_Base_t const* _this, test040_string_t* str, int* a, float* b, float* _result) {
    try {
        *_result = _this->hello(*str, *a, b);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test040_Base_hello2(test040_Base_t const* _this, test040_string_t* str, int* _result) {
    try {
        *_result = _this->hello2(*str);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test040_Base_nonvirtual(test040_Base_t* _this) {
    try {
        _this->nonvirtual();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test040_Base_default(test040_Base_t** _result) {
    try {
        *_result = new foo::Base();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test040_Base_from_str(test040_string_t* foo, test040_string_t* foo_ref, test040_string_t* foo_ptr, test040_Base_t** _result) {
    try {
        *_result = new foo::Base(*foo, *foo_ref, foo_ptr);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test040_Base_from_int(int foo, int* foo_ref, int* foo_ptr, test040_Base_t** _result) {
    try {
        *_result = new foo::Base(foo, *foo_ref, foo_ptr);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test040_Base_dtor(test040_Base_t* _this) {
    delete _this;
    return 0;
}

int test040_Derived_hello(test040_Derived_t const* _this, test040_string_t* str, int* a, float* b, float* _result) {
    try {
        *_result = _this->hello(*str, *a, b);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test040_Derived_hello2(test040_Derived_t const* _this, test040_string_t* str, int* _result) {
    try {
        *_result = _this->hello2(*str);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test040_Derived_nonvirtual(test040_Derived_t* _this) {
    try {
        _this->nonvirtual();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test040_Derived_dtor(test040_Derived_t* _this) {
    delete _this;
    return 0;
}

int test040_string_dtor(test040_string_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
