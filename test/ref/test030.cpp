
#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

extern "C" {

using test030_Transform_t = OCIO_NAMESPACE::Transform;

int test030_Transform_getValues(test030_Transform_t const* _this, double (*values)[3]) {
    _this->getValues(*values);
    return 0;
}

int test030_Transform_setValues(test030_Transform_t* _this, double const (*values)[3]) {
    _this->setValues(*values);
    return 0;
}

int test030_Transform_dtor(test030_Transform_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
