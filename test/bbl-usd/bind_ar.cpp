#if defined(__clang__)

#include "babble"
#include "babble-std"

#include <pxr/usd/ar/resolverContext.h>

BBL_MODULE(ar) {
    bbl::rename_namespace("PXR_NS::", "pxr");
    bbl::prepend_module_name(true);

    bbl::Class<PXR_NS::ArResolverContext>("ResolverContext")
        ;
}


#endif

