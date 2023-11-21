#include <memory>

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

using test031_Foo_t = Foo;
using test031_FooPtr_t = FooPtr;
using test031_ConstFooPtr_t = ConstFooPtr;
using test031_Bar_t = Bar;
using test031_BarPtr_t = BarPtr;
using test031_ConstBarPtr_t = ConstBarPtr;

int test031_Foo_create(test031_FooPtr_t** _result) {
    try {
        *_result = new FooPtr(Foo::create());
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_Foo_get_foo(test031_Foo_t const* _this, int* _result) {
    try {
        *_result = _this->get_foo();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_Foo_set_foo(test031_Foo_t* _this, int a, int* _result) {
    try {
        *_result = _this->set_foo(a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_Foo_dtor(test031_Foo_t* _this) {
    delete _this;
    return 0;
}

int test031_FooPtr_dtor(test031_FooPtr_t* _this) {
    delete _this;
    return 0;
}

int test031_FooPtr_get_foo(test031_FooPtr_t const* _this, int* _result) {
    try {
        *_result = (*_this)->get_foo();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_FooPtr_set_foo(test031_FooPtr_t* _this, int a, int* _result) {
    try {
        *_result = (*_this)->set_foo(a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_ConstFooPtr_dtor(test031_ConstFooPtr_t* _this) {
    delete _this;
    return 0;
}

int test031_ConstFooPtr_get_foo(test031_ConstFooPtr_t const* _this, int* _result) {
    try {
        *_result = (*_this)->get_foo();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_Bar_create(test031_BarPtr_t** _result) {
    try {
        *_result = new BarPtr(Bar::create());
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_Bar_baz(test031_Bar_t* _this) {
    try {
        _this->baz();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_Bar_dtor(test031_Bar_t* _this) {
    delete _this;
    return 0;
}

int test031_Bar_get_foo(test031_Bar_t const* _this, int* _result) {
    try {
        *_result = _this->get_foo();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_Bar_set_foo(test031_Bar_t* _this, int a, int* _result) {
    try {
        *_result = _this->set_foo(a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_BarPtr_dtor(test031_BarPtr_t* _this) {
    delete _this;
    return 0;
}

int test031_BarPtr_baz(test031_BarPtr_t* _this) {
    try {
        (*_this)->baz();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_BarPtr_get_foo(test031_BarPtr_t const* _this, int* _result) {
    try {
        *_result = (*_this)->get_foo();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_BarPtr_set_foo(test031_BarPtr_t* _this, int a, int* _result) {
    try {
        *_result = (*_this)->set_foo(a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test031_ConstBarPtr_dtor(test031_ConstBarPtr_t* _this) {
    delete _this;
    return 0;
}

int test031_ConstBarPtr_get_foo(test031_ConstBarPtr_t const* _this, int* _result) {
    try {
        *_result = (*_this)->get_foo();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

} // extern "C"
