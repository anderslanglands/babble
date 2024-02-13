
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

auto Foo_do_thing(const foo::Foo &_this, int a) -> float {
    return _this.f;
}

void normal_thing() {}
}


using test046_Foo_t = foo::Foo;

extern "C" {
int test046_Foo_dtor(test046_Foo_t* _this) {
    delete _this;
    return 0;
}

int test046_Foo_do_thing(test046_Foo_t const* _this, int a, float* _result) {
    try {
        *_result = bblext::Foo_do_thing(*_this, a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test046_normal_thing() {
    try {
        bblext::normal_thing();
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

} // extern "C"
