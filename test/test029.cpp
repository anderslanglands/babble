#include "babble"


namespace OCIO_NAMESPACE {

struct GradingRGBM
{
    GradingRGBM() = default;
    GradingRGBM(double red, double green, double blue, double master)
        : m_red(red)
        , m_green(green)
        , m_blue(blue)
        , m_master(master)
    {
    }
    GradingRGBM(const double(&rgbm)[4])
        : m_red(rgbm[0])
        , m_green(rgbm[1])
        , m_blue(rgbm[2])
        , m_master(rgbm[3])
    {
    }
    double m_red{ 0. };
    double m_green{ 0. };
    double m_blue{ 0. };
    double m_master{ 0. };
};

}

// Test bind kinds and constructors
BBL_MODULE(test029) {
    bbl::Class<OCIO_NAMESPACE::GradingRGBM>()
        .value_type()
        .f(&OCIO_NAMESPACE::GradingRGBM::m_red)
        .f(&OCIO_NAMESPACE::GradingRGBM::m_green)
        .f(&OCIO_NAMESPACE::GradingRGBM::m_blue)
        .f(&OCIO_NAMESPACE::GradingRGBM::m_master)
        ;

}
