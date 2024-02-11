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

// This is necessary for some type definitions in lambdas defined in binding macros
namespace bbl { namespace detail {

template <typename T> struct argument_type;
template <typename T, typename U> struct argument_type<T(U)> {
    typedef U type;
};

} }



using test039_Base_t = foo::Base;
using test039_string_t = std::string;

class test039_Base_Subclass_t;
typedef int (*test039_Base_Subclass_t_hello)(test039_Base_t const*, test039_string_t*, int*, float*, float*, void*);
typedef int (*test039_Base_Subclass_t_hello2)(test039_Base_t const*, test039_string_t*, int*, void*);
typedef int (*test039_Base_Subclass_dtor)(test039_Base_Subclass_t*, void*);

class test039_Base_Subclass_t : public foo::Base {
    test039_Base_Subclass_t_hello _fn_hello;
    void* _fn_hello_user_data;
    test039_Base_Subclass_t_hello2 _fn_hello2;
    void* _fn_hello2_user_data;
    test039_Base_Subclass_dtor _fn_dtor;
    void* _fn_dtor_user_data;

public:
    test039_Base_Subclass_t(test039_Base_Subclass_t_hello fn_hello, void* fn_hello_user_data, test039_Base_Subclass_t_hello2 fn_hello2, void* fn_hello2_user_data, test039_Base_Subclass_dtor fn_dtor, void* fn_dtor_user_data)
    : Base(),
    _fn_hello(fn_hello),
    _fn_hello_user_data(fn_hello_user_data),
    _fn_hello2(fn_hello2),
    _fn_hello2_user_data(fn_hello2_user_data),
    _fn_dtor(fn_dtor),
    _fn_dtor_user_data(fn_dtor_user_data)
    {}


    test039_Base_Subclass_t(std::string foo, std::string& foo_ref, std::string* foo_ptr, test039_Base_Subclass_t_hello fn_hello, void* fn_hello_user_data, test039_Base_Subclass_t_hello2 fn_hello2, void* fn_hello2_user_data, test039_Base_Subclass_dtor fn_dtor, void* fn_dtor_user_data)
    : Base(foo, foo_ref, foo_ptr),
    _fn_hello(fn_hello),
    _fn_hello_user_data(fn_hello_user_data),
    _fn_hello2(fn_hello2),
    _fn_hello2_user_data(fn_hello2_user_data),
    _fn_dtor(fn_dtor),
    _fn_dtor_user_data(fn_dtor_user_data)
    {}


    test039_Base_Subclass_t(int foo, int& foo_ref, int* foo_ptr, test039_Base_Subclass_t_hello fn_hello, void* fn_hello_user_data, test039_Base_Subclass_t_hello2 fn_hello2, void* fn_hello2_user_data, test039_Base_Subclass_dtor fn_dtor, void* fn_dtor_user_data)
    : Base(foo, foo_ref, foo_ptr),
    _fn_hello(fn_hello),
    _fn_hello_user_data(fn_hello_user_data),
    _fn_hello2(fn_hello2),
    _fn_hello2_user_data(fn_hello2_user_data),
    _fn_dtor(fn_dtor),
    _fn_dtor_user_data(fn_dtor_user_data)
    {}


    virtual ~test039_Base_Subclass_t() {
        if (_fn_dtor) {
            _fn_dtor(this, _fn_dtor_user_data);
        }
    }
    virtual auto hello(std::string& str, int& a, float* b) const -> float override {
        if (_fn_hello == nullptr) {
            return foo::Base::hello(str, a, b);
        }

        float _result;
        _fn_hello(this, &str, &a, b, &_result, _fn_hello_user_data);
        return _result;
    }

    virtual auto hello2(std::string& str) const -> int override {
        if (_fn_hello2 == nullptr) {
            return foo::Base::hello2(str);
        }

        int _result;
        _fn_hello2(this, &str, &_result, _fn_hello2_user_data);
        return _result;
    }

};

extern "C" int test039_Base_Subclass_default(test039_Base_Subclass_t** _result, test039_Base_Subclass_t_hello fn_hello, void* fn_hello_user_data, test039_Base_Subclass_t_hello2 fn_hello2, void* fn_hello2_user_data, test039_Base_Subclass_dtor fn_dtor, void* fn_dtor_user_data) {
    try {
        *_result = new test039_Base_Subclass_t(fn_hello, fn_hello_user_data, fn_hello2, fn_hello2_user_data, fn_dtor, fn_dtor_user_data);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

extern "C" int test039_Base_Subclass_from_str(test039_string_t* foo, test039_string_t* foo_ref, test039_string_t* foo_ptr, test039_Base_Subclass_t** _result, test039_Base_Subclass_t_hello fn_hello, void* fn_hello_user_data, test039_Base_Subclass_t_hello2 fn_hello2, void* fn_hello2_user_data, test039_Base_Subclass_dtor fn_dtor, void* fn_dtor_user_data) {
    try {
        *_result = new test039_Base_Subclass_t(*foo, *foo_ref, foo_ptr, fn_hello, fn_hello_user_data, fn_hello2, fn_hello2_user_data, fn_dtor, fn_dtor_user_data);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

extern "C" int test039_Base_Subclass_from_int(int foo, int* foo_ref, int* foo_ptr, test039_Base_Subclass_t** _result, test039_Base_Subclass_t_hello fn_hello, void* fn_hello_user_data, test039_Base_Subclass_t_hello2 fn_hello2, void* fn_hello2_user_data, test039_Base_Subclass_dtor fn_dtor, void* fn_dtor_user_data) {
    try {
        *_result = new test039_Base_Subclass_t(foo, *foo_ref, foo_ptr, fn_hello, fn_hello_user_data, fn_hello2, fn_hello2_user_data, fn_dtor, fn_dtor_user_data);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

extern "C" {
int test039_Base_hello(test039_Base_t const* _this, test039_string_t* str, int* a, float* b, float* _result) {
    try {
        *_result = _this->hello(*str, *a, b);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test039_Base_hello2(test039_Base_t const* _this, test039_string_t* str, int* _result) {
    try {
        *_result = _this->hello2(*str);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test039_Base_nonvirtual(test039_Base_t* _this) {
    try {
        _this->nonvirtual();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test039_Base_default(test039_Base_t** _result) {
    try {
        *_result = new foo::Base();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test039_Base_from_str(test039_string_t* foo, test039_string_t* foo_ref, test039_string_t* foo_ptr, test039_Base_t** _result) {
    try {
        *_result = new foo::Base(*foo, *foo_ref, foo_ptr);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test039_Base_from_int(int foo, int* foo_ref, int* foo_ptr, test039_Base_t** _result) {
    try {
        *_result = new foo::Base(foo, *foo_ref, foo_ptr);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test039_Base_dtor(test039_Base_t* _this) {
    delete _this;
    return 0;
}

int test039_string_dtor(test039_string_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
