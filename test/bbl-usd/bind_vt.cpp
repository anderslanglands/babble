#if defined(__clang__)

#include "babble"
#include "babble-std"

#include <pxr/base/vt/value.h>

BBL_MODULE(vt) {
    // clang-format off

    bbl::Class<PXR_NS::VtValue>("Value")
        ;
}


#endif

