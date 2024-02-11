
#include <stdint.h>

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

auto IntBarPair_first(bbl::detail::argument_type<void (std::pair<int, qux::Bar>)>::type &p) -> bbl::detail::argument_type<void (std::pair<int, qux::Bar>)>::type::first_type & {
    return p.first;
}

auto IntBarPair_second(bbl::detail::argument_type<void (std::pair<int, qux::Bar>)>::type &p) -> bbl::detail::argument_type<void (std::pair<int, qux::Bar>)>::type::second_type & {
    return p.second;
}

auto IntBarPair_first_const(const bbl::detail::argument_type<void (std::pair<int, qux::Bar>)>::type &p) -> const bbl::detail::argument_type<void (std::pair<int, qux::Bar>)>::type::first_type & {
    return p.first;
}

auto IntBarPair_second_const(const bbl::detail::argument_type<void (std::pair<int, qux::Bar>)>::type &p) -> const bbl::detail::argument_type<void (std::pair<int, qux::Bar>)>::type::second_type & {
    return p.second;
}

}

using test048_Bar = int;

using test048_IntBarPair_t = bbl::detail::argument_type<void (std::pair<int, qux::Bar>)>::type;

extern "C" {
int test048_IntBarPair_dtor(test048_IntBarPair_t* _this) {
    delete _this;
    return 0;
}

int test048_IntBarPair_first(test048_IntBarPair_t* p, int** _result) {
    try {
        *_result = &bblext::IntBarPair_first(*p);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test048_IntBarPair_second(test048_IntBarPair_t* p, int** _result) {
    try {
        *_result = reinterpret_cast<int*>(&bblext::IntBarPair_second(*p));
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test048_IntBarPair_first_const(test048_IntBarPair_t const* p, int const** _result) {
    try {
        *_result = &bblext::IntBarPair_first_const(*p);
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

int test048_IntBarPair_second_const(test048_IntBarPair_t const* p, int const** _result) {
    try {
        *_result = reinterpret_cast<int const*>(&bblext::IntBarPair_second_const(*p));
        return 0;
    } catch (std::exception& e) {
        _bbl_error_message = e.what();
        return 1;
    }
}

} // extern "C"
