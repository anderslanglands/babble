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


using test042_Abstract_t = foo::Abstract;
using test042_Base1_t = foo::Base1;
using test042_Base2_t = foo::Base2;

extern "C" {
int test042_Abstract_do_pure_thing(test042_Abstract_t* _this) {
    try {
        _this->do_pure_thing();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test042_Abstract_public_method(test042_Abstract_t* _this) {
    try {
        _this->public_method();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test042_Abstract_dtor(test042_Abstract_t* _this) {
    delete _this;
    return 0;
}

int test042_Base1_do_pure_thing(test042_Base1_t* _this) {
    try {
        _this->do_pure_thing();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test042_Base1_public_method(test042_Base1_t* _this) {
    try {
        _this->public_method();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test042_Base1_dtor(test042_Base1_t* _this) {
    delete _this;
    return 0;
}

int test042_Base2_do_pure_thing(test042_Base2_t* _this) {
    try {
        _this->do_pure_thing();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test042_Base2_public_method(test042_Base2_t* _this) {
    try {
        _this->public_method();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test042_Base2_dtor(test042_Base2_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
