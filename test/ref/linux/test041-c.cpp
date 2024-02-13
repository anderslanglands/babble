
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


namespace bblext {

class Base : public foo::Abstract {
public:
    Base() {}

    virtual void do_protected_thing() {
        foo::Abstract::do_protected_thing();
    }

    void do_protected_thing_base() {
        foo::Abstract::do_protected_thing();
    }
};
}


using test041_Abstract_t = foo::Abstract;
using test041_Base_t = bblext::Base;

class test041_Base_Subclass_t;
typedef int (*test041_Base_Subclass_t_do_protected_thing)(test041_Base_t*, void*);
typedef int (*test041_Base_Subclass_t_do_pure_thing)(test041_Base_t*, void*);
typedef int (*test041_Base_Subclass_dtor)(test041_Base_Subclass_t*, void*);

class test041_Base_Subclass_t : public bblext::Base {
    test041_Base_Subclass_t_do_protected_thing _fn_do_protected_thing;
    void* _fn_do_protected_thing_user_data;
    test041_Base_Subclass_t_do_pure_thing _fn_do_pure_thing;
    void* _fn_do_pure_thing_user_data;
    test041_Base_Subclass_dtor _fn_dtor;
    void* _fn_dtor_user_data;

public:
    test041_Base_Subclass_t(test041_Base_Subclass_t_do_protected_thing fn_do_protected_thing, void* fn_do_protected_thing_user_data, test041_Base_Subclass_t_do_pure_thing fn_do_pure_thing, void* fn_do_pure_thing_user_data, test041_Base_Subclass_dtor fn_dtor, void* fn_dtor_user_data)
    : Base(),
    _fn_do_protected_thing(fn_do_protected_thing),
    _fn_do_protected_thing_user_data(fn_do_protected_thing_user_data),
    _fn_do_pure_thing(fn_do_pure_thing),
    _fn_do_pure_thing_user_data(fn_do_pure_thing_user_data),
    _fn_dtor(fn_dtor),
    _fn_dtor_user_data(fn_dtor_user_data)
    {}


    virtual ~test041_Base_Subclass_t() {
        if (_fn_dtor) {
            _fn_dtor(this, _fn_dtor_user_data);
        }
    }
    virtual auto do_protected_thing() -> void override {
        if (_fn_do_protected_thing == nullptr) {
            return bblext::Base::do_protected_thing();
        }

        _fn_do_protected_thing(this, _fn_do_protected_thing_user_data);
    }

    virtual auto do_pure_thing() -> void override {
        if (_fn_do_pure_thing == nullptr) {
            fprintf(stderr, "method foo::Abstract::do_pure_thing is pure virtual, but no implementation was provided in the subclass");
            std::terminate();
        }

        _fn_do_pure_thing(this, _fn_do_pure_thing_user_data);
    }

};

extern "C" int test041_Base_Subclass_default(test041_Base_Subclass_t** _result, test041_Base_Subclass_t_do_protected_thing fn_do_protected_thing, void* fn_do_protected_thing_user_data, test041_Base_Subclass_t_do_pure_thing fn_do_pure_thing, void* fn_do_pure_thing_user_data, test041_Base_Subclass_dtor fn_dtor, void* fn_dtor_user_data) {
    try {
        *_result = new test041_Base_Subclass_t(fn_do_protected_thing, fn_do_protected_thing_user_data, fn_do_pure_thing, fn_do_pure_thing_user_data, fn_dtor, fn_dtor_user_data);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

extern "C" {
int test041_Abstract_do_pure_thing(test041_Abstract_t* _this) {
    try {
        _this->do_pure_thing();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test041_Abstract_public_method(test041_Abstract_t* _this) {
    try {
        _this->public_method();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test041_Abstract_dtor(test041_Abstract_t* _this) {
    delete _this;
    return 0;
}

int test041_Base_do_protected_thing(test041_Base_t* _this) {
    try {
        _this->do_protected_thing();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test041_Base_do_protected_thing_base(test041_Base_t* _this) {
    try {
        _this->do_protected_thing_base();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test041_Base_do_pure_thing(test041_Base_t* _this) {
    try {
        _this->do_pure_thing();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test041_Base_public_method(test041_Base_t* _this) {
    try {
        _this->public_method();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test041_Base_dtor(test041_Base_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
