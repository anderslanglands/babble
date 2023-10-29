
#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test0024_Bar_t = qux::Bar;
struct BBL_ALIGN(4) test0024_Bar_t_bbl_size_check {
    float data[3];
};
static_assert(sizeof(test0024_Bar_t_bbl_size_check) == sizeof(qux::Bar), "size of value type does not match");
static_assert(alignof(test0024_Bar_t_bbl_size_check) == alignof(qux::Bar), "align of value type does not match");


int test0024_Bar_set_data(test0024_Bar_t* _this, float* d) {
    _this->set_data(d);
    return 0;
}

} // extern "C"
