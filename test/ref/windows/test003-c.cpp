
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

using test003_FooFloat_t = Foo<float>;
using test003_FooInt_t = FooInt;
struct BBL_ALIGN(4) test003_FooInt_t_bbl_size_check {
};
static_assert(sizeof(test003_FooInt_t_bbl_size_check) == sizeof(FooInt), "size of value type does not match");
static_assert(alignof(test003_FooInt_t_bbl_size_check) == alignof(FooInt), "align of value type does not match");

using test003_Baz_t = Baz;

int test003_FooFloat_bar(test003_FooFloat_t* _this, float const* a, float* _result) {
    try {
        *_result = _this->bar(*a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test003_FooFloat_baz(test003_FooFloat_t* _this, test003_Baz_t* b) {
    try {
        _this->baz(b);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test003_FooFloat_dtor(test003_FooFloat_t* _this) {
    delete _this;
    return 0;
}

int test003_FooInt_bar(test003_FooInt_t* _this, int const* a, int* _result) {
    try {
        *_result = _this->bar(*a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test003_FooInt_baz(test003_FooInt_t* _this, test003_Baz_t* b) {
    try {
        _this->baz(b);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test003_Baz_dtor(test003_Baz_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
