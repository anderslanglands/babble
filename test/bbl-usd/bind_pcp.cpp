#include <pxr/usd/pcp/types.h>
#if defined(__clang__)

#include "babble"
#include "babble-std"

#include <pxr/usd/pcp/primIndex.h>

BBL_MODULE(pcp) {
    bbl::Class<PXR_NS::PcpPrimIndex>("PrimIndex");

    bbl::Class<PXR_NS::PcpVariantFallbackMap>("VariantFallbackMap")
        ;
}


#endif

