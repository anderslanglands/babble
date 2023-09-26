#if defined(__clang__)

#include "babble"
#include "babble-std"

#include <pxr/usd/sdf/path.h>
#include <pxr/usd/sdf/primSpec.h>


BBL_MODULE(sdf) {
    bbl::rename_namespace("PXR_NS::", "pxr");
    bbl::prepend_module_name(true);

    bbl::Class<PXR_NS::SdfPrimSpecHandle>("PrimSpecHandle");
    bbl::Enum<PXR_NS::SdfSpecifier>("Specifier");

    bbl::Class<PXR_NS::SdfPrimSpecHandleVector>("PrimSpecHandleVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::SdfPrimSpecHandle);

    bbl::Class<std::pair<PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset>>(
        "PrimSpecHandleOffsetPair");
    // BBL_STD_PAIR_FUNCTIONS(PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset,
    //                        "PrimSpecHandleOffsetPair");

    bbl::Class<std::vector<
        std::pair<PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset>>>(
        "PrimSpecHandleOffsetPairVector")
        BBL_STD_VECTOR_METHODS(
            (std::pair<PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset>));

    bbl::Class<PXR_NS::SdfLayerOffset>("LayerOffset");

    bbl::Enum<PXR_NS::SdfVariability>("Variability");

    bbl::Class<PXR_NS::SdfPath>("Path").opaque_ptr();

    bbl::Class<PXR_NS::SdfPathVector>("PathVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::SdfPath);

    bbl::Class<PXR_NS::SdfValueTypeName>("ValueTypeName").opaque_bytes();
}

#endif
