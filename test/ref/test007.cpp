#include <string>
#include <vector>

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test007_Bar_t = qux::Bar;
struct BBL_ALIGN(4) test007_Bar_t_bbl_size_check {
    int f1;
    float f2;
};
static_assert(sizeof(test007_Bar_t_bbl_size_check) == sizeof(qux::Bar), "size of value type does not match");
static_assert(alignof(test007_Bar_t_bbl_size_check) == alignof(qux::Bar), "align of value type does not match");


int test007_Bar_bar(test007_Bar_t const* _this, int* _result) {
    *_result = _this->bar();
    return 0;
}

int test007_Bar_baz() {
    qux::Bar::baz();
    return 0;
}

int test007_Bar_bum() {
    qux::Bar::bum();
    return 0;
}

} // extern "C"
