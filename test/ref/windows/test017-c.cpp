
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


using test017_Vec3f_t = qux::Vec3f;
struct BBL_ALIGN(4) test017_Vec3f_t_bbl_size_check {
    float x;
    float y;
    float z;
};
static_assert(sizeof(test017_Vec3f_t_bbl_size_check) == sizeof(qux::Vec3f), "size of value type does not match");
static_assert(alignof(test017_Vec3f_t_bbl_size_check) == alignof(qux::Vec3f), "align of value type does not match");

using test017_Foo_t = qux::Foo;
using test017_Bar_t = qux::Bar;

extern "C" {
int test017_Vec3f_length(test017_Vec3f_t const* _this, float* _result) {
    try {
        *_result = _this->length();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test017_Vec3f_dist(test017_Vec3f_t const* _this, test017_Vec3f_t const* other, float* _result) {
    try {
        *_result = _this->dist(*other);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test017_Vec3f_ctor(test017_Vec3f_t* _result) {
    new (_result) qux::Vec3f();
    return 0;
}

int test017_Foo_do_foo(test017_Foo_t const* _this, test017_Foo_t const* other) {
    try {
        _this->do_foo(*other);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test017_Foo_do_foo2(test017_Foo_t* _this, test017_Foo_t* other) {
    try {
        _this->do_foo2(other);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test017_Foo_ctor(int a, float b, test017_Foo_t** _result) {
    try {
        *_result = new qux::Foo(a, b);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test017_Foo_dtor(test017_Foo_t* _this) {
    delete _this;
    return 0;
}

int test017_Bar_ctor(int a, float b, test017_Bar_t** _result) {
    try {
        *_result = new qux::Bar(a, b);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test017_Bar_dtor(test017_Bar_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
