#if defined(__clang__)

#include "babble"
#include "babble-std"

#include <pxr/usd/pcp/primIndex.h>

BBL_MODULE(pcp) {
    bbl::rename_namespace("PXR_NS::", "pxr");
    bbl::prepend_module_name(true);

    bbl::Class<PXR_NS::PcpPrimIndex>("PrimIndex");
}


#endif

