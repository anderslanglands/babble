
#include <utility>

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

auto StdPairIntFloat_first(bbl::detail::argument_type<void (std::pair<int, float>)>::type &p) -> bbl::detail::argument_type<void (std::pair<int, float>)>::type::first_type & {
    return p.first;
}

auto StdPairIntFloat_second(bbl::detail::argument_type<void (std::pair<int, float>)>::type &p) -> bbl::detail::argument_type<void (std::pair<int, float>)>::type::second_type & {
    return p.second;
}

auto StdPairIntFloat_first_const(const bbl::detail::argument_type<void (std::pair<int, float>)>::type &p) -> const bbl::detail::argument_type<void (std::pair<int, float>)>::type::first_type & {
    return p.first;
}

auto StdPairIntFloat_second_const(const bbl::detail::argument_type<void (std::pair<int, float>)>::type &p) -> const bbl::detail::argument_type<void (std::pair<int, float>)>::type::second_type & {
    return p.second;
}

}


using test_std_pair_StdPairIntFloat_t = bbl::detail::argument_type<void (std::pair<int, float>)>::type;

extern "C" {
int test_std_pair_StdPairIntFloat_dtor(test_std_pair_StdPairIntFloat_t* _this) {
    delete _this;
    return 0;
}

int test_std_pair_StdPairIntFloat_first(test_std_pair_StdPairIntFloat_t* p, int** _result) {
    try {
        *_result = &bblext::StdPairIntFloat_first(*p);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test_std_pair_StdPairIntFloat_second(test_std_pair_StdPairIntFloat_t* p, float** _result) {
    try {
        *_result = &bblext::StdPairIntFloat_second(*p);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test_std_pair_StdPairIntFloat_first_const(test_std_pair_StdPairIntFloat_t const* p, int const** _result) {
    try {
        *_result = &bblext::StdPairIntFloat_first_const(*p);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test_std_pair_StdPairIntFloat_second_const(test_std_pair_StdPairIntFloat_t const* p, float const** _result) {
    try {
        *_result = &bblext::StdPairIntFloat_second_const(*p);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test_std_pair_foo(int a) {
    try {
        baz::foo(a);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

} // extern "C"
