
#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test0023_qux::Foo_t = qux::Foo;
struct BBL_ALIGN(4) test0023_qux::Foo_t_bbl_size_check {
    float x;
    float y;
    float z;
};
static_assert(sizeof(test0023_qux::Foo_t_bbl_size_check) == sizeof(qux::Foo), "size of value type does not match");
static_assert(alignof(test0023_qux::Foo_t_bbl_size_check) == alignof(qux::Foo), "align of value type does not match");


int test0023_qux::Foo_length(test0023_qux::Foo_t* _this, float* _result) {
    *_result = _this->length();
    return 0;
}

int test0023_qux::Foo_normalize(test0023_qux::Foo_t* _this, float* _result) {
    *_result = _this->normalize();
    return 0;
}

} // extern "C"
