#if defined(__clang__)

#include "babble"
#include "babble-std"

#include <pxr/base/vt/value.h>

BBL_MODULE(vt) {
    bbl::rename_namespace("PXR_NS::", "pxr");
    bbl::prepend_module_name(true);


    // clang-format off

    bbl::Class<PXR_NS::VtValue>("Value")
        ;
}


#endif

