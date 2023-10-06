#include <utility>

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test_std_pair_StdPairIntFloat_t = std::pair<int, float>;

int test_std_pair_StdPairIntFloat_dtor(test_std_pair_StdPairIntFloat_t* _this) {
    delete _this;
    return 0;
}

int test_std_pair_StdPairIntFloat_get0(test_std_pair_StdPairIntFloat_t* _Pr, int* _result) {
    *_result = get<0, bbl::detail::argument_type<void(TY0)>::type, bbl::detail::argument_type<void(TY1)>::type>(*_Pr);
    return 0;
}

int test_std_pair_StdPairIntFloat_get0_const(test_std_pair_StdPairIntFloat_t const* _Pr, int* _result) {
    *_result = get<0, bbl::detail::argument_type<void(TY0)>::type, bbl::detail::argument_type<void(TY1)>::type>(*_Pr);
    return 0;
}

int test_std_pair_StdPairIntFloat_get1(test_std_pair_StdPairIntFloat_t* _Pr, float* _result) {
    *_result = get<1, bbl::detail::argument_type<void(TY0)>::type, bbl::detail::argument_type<void(TY1)>::type>(*_Pr);
    return 0;
}

int test_std_pair_StdPairIntFloat_get1_const(test_std_pair_StdPairIntFloat_t const* _Pr, float* _result) {
    *_result = get<1, bbl::detail::argument_type<void(TY0)>::type, bbl::detail::argument_type<void(TY1)>::type>(*_Pr);
    return 0;
}

} // extern "C"
