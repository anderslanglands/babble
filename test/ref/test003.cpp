
#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test003_Baz_t = Baz;
using test003_FooFloat_t = Foo<float>;
static_assert(sizeof(Foo<float>) == 4, "size of Foo<float> and test003_FooFloat_t do not match");
static_assert(alignof(Foo<float>) == 4, "align of Foo<float> and test003_FooFloat_t do not match");

using test003_FooInt_t = FooInt;
struct BBL_ALIGN(4) test003_FooInt_t_bbl_size_check {
};
static_assert(sizeof(test003_FooInt_t_bbl_size_check) == sizeof(FooInt), "size of value type does not match");
static_assert(alignof(test003_FooInt_t_bbl_size_check) == alignof(FooInt), "align of value type does not match");


int test003_Baz_dtor(test003_Baz_t* _this) {
    delete _this;
    return 0;
}

int test003_FooFloat_bar(test003_FooFloat_t* _this, float const* a, float* _result) {
    *_result = _this->bar(*a);
    return 0;
}

int test003_FooFloat_baz(test003_FooFloat_t* _this, test003_Baz_t* b) {
    _this->baz(b);
    return 0;
}

int test003_FooInt_bar(test003_FooInt_t* _this, int const* a, int* _result) {
    *_result = _this->bar(*a);
    return 0;
}

int test003_FooInt_baz(test003_FooInt_t* _this, test003_Baz_t* b) {
    _this->baz(b);
    return 0;
}

} // extern "C"
