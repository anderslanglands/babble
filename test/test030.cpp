#include "babble"


namespace OCIO_NAMESPACE {

struct Transform
{
    void getValues(double (&values)[3]) const;
    void setValues(const double (&values)[3]);
};

}

// Test bind kinds and constructors
BBL_MODULE(test030) {
    bbl::Class<OCIO_NAMESPACE::Transform>()
        .m(&OCIO_NAMESPACE::Transform::getValues)
        .m(&OCIO_NAMESPACE::Transform::setValues)
        ;

}
