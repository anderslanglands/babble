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


using test_std_pair_StdPairIntFloat_t = std::pair<int, float>;

extern "C" {
int test_std_pair_StdPairIntFloat_dtor(test_std_pair_StdPairIntFloat_t* _this) {
    delete _this;
    return 0;
}

int test_std_pair_StdPairIntFloat_get0(test_std_pair_StdPairIntFloat_t* _Pr, int** _result) {
    *_result = &std::get<0, bbl::detail::argument_type<void (int)>::type, bbl::detail::argument_type<void (float)>::type>(*_Pr);
    return 0;
}

int test_std_pair_StdPairIntFloat_get0_const(test_std_pair_StdPairIntFloat_t const* _Pr, int const** _result) {
    *_result = &std::get<0, bbl::detail::argument_type<void (int)>::type, bbl::detail::argument_type<void (float)>::type>(*_Pr);
    return 0;
}

int test_std_pair_StdPairIntFloat_get1(test_std_pair_StdPairIntFloat_t* _Pr, float** _result) {
    *_result = &std::get<1, bbl::detail::argument_type<void (int)>::type, bbl::detail::argument_type<void (float)>::type>(*_Pr);
    return 0;
}

int test_std_pair_StdPairIntFloat_get1_const(test_std_pair_StdPairIntFloat_t const* _Pr, float const** _result) {
    *_result = &std::get<1, bbl::detail::argument_type<void (int)>::type, bbl::detail::argument_type<void (float)>::type>(*_Pr);
    return 0;
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
