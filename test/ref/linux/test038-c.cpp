
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

// This is necessary for some type definitions in lambdas defined in binding macros
namespace bbl { namespace detail {

template <typename T> struct argument_type;
template <typename T, typename U> struct argument_type<T(U)> {
    typedef U type;
};

} }



using test037_Bar_t = foo::Bar;

extern "C" {
int test037_Bar_get_int(test037_Bar_t* _this, int** _result) {
    try {
        *_result = &_this->get_int();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test037_Bar_get_bar(test037_Bar_t* _this, test037_Bar_t const** _result) {
    try {
        *_result = &_this->get_bar();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test037_Bar_dtor(test037_Bar_t* _this) {
    delete _this;
    return 0;
}

int test037_get_float(float** _result) {
    try {
        *_result = &foo::get_float();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test037_get_bar_ref(test037_Bar_t const** _result) {
    try {
        *_result = &foo::get_bar_ref();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

} // extern "C"
