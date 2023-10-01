#if defined(__clang__)

#include "babble"
#include "babble-std"

#include <pxr/base/vt/array.h>
#include <pxr/usd/sdf/assetPath.h>
#include <pxr/usd/sdf/attributeSpec.h>
#include <pxr/usd/sdf/declareHandles.h>
#include <pxr/usd/sdf/layer.h>
#include <pxr/usd/sdf/listEditorProxy.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/sdf/payload.h>
#include <pxr/usd/sdf/primSpec.h>
#include <pxr/usd/sdf/propertySpec.h>
#include <pxr/usd/sdf/proxyTypes.h>
#include <pxr/usd/sdf/reference.h>
#include <pxr/usd/sdf/relationshipSpec.h>
#include <pxr/usd/sdf/timeCode.h>
#include <pxr/usd/sdf/types.h>

BBL_MODULE(sdf) {
    bbl::Class<PXR_NS::SdfAssetPath>("AssetPath");

    bbl::Class<PXR_NS::VtArray<PXR_NS::SdfAssetPath>>("AssetPathArray");

    bbl::Class<PXR_NS::SdfAttributeSpecHandle>("AttributeSpecHandle");

    bbl::Class<PXR_NS::SdfAttributeSpecHandleVector>(
        "AttributeSpecHandleVector");

    bbl::Class<PXR_NS::SdfLayer::FileFormatArguments>(
        "LayerFileFormatArguments");

    bbl::Class<PXR_NS::SdfLayerHandle>("LayerHandle");

    bbl::Class<PXR_NS::SdfLayerHandleVector>("LayerHandleVector");

    bbl::Class<PXR_NS::SdfLayerOffset>("LayerOffset");

    bbl::Class<PXR_NS::SdfLayerRefPtr>("LayerRefPtr");

    bbl::Class<PXR_NS::SdfListOp<std::string>>("StringListOp");

    bbl::Class<PXR_NS::SdfNameEditorProxy>("NameEditorProxy");

    bbl::Class<PXR_NS::SdfPath>("Path").opaque_ptr();

    bbl::Class<PXR_NS::SdfPathEditorProxy>("PathEditorProxy");

    bbl::Class<PXR_NS::SdfPathSet>("PathSet");

    bbl::Class<PXR_NS::SdfPathVector>("PathVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::SdfPath);

    bbl::Class<PXR_NS::SdfPayload>("Payload");

    bbl::Class<PXR_NS::SdfPayloadEditorProxy>("PayloadEditorProxy");

    bbl::Class<PXR_NS::SdfPayloadVector>("PayloadVector");

    bbl::Class<PXR_NS::SdfPrimSpecHandle>("PrimSpecHandle");

    bbl::Class<PXR_NS::SdfPrimSpecHandleVector>("PrimSpecHandleVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::SdfPrimSpecHandle);

    bbl::Class<std::pair<PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset>>(
        "PrimSpecHandleOffsetPair");

    bbl::Class<std::vector<
        std::pair<PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset>>>(
        "PrimSpecHandleOffsetPairVector")
        BBL_STD_VECTOR_METHODS(
            (std::pair<PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset>));

    bbl::Class<PXR_NS::SdfPropertySpecHandle>("PropertySpecHandle");

    bbl::Class<PXR_NS::SdfPropertySpecHandleVector>("PropertySpecHandleVector");

    bbl::Class<
        std::pair<PXR_NS::SdfPropertySpecHandle, PXR_NS::SdfLayerOffset>>(
        "PropertySpecHandleOffsetPair");

    bbl::Class<std::vector<
        std::pair<PXR_NS::SdfPropertySpecHandle, PXR_NS::SdfLayerOffset>>>(
        "PropertySpecHandleOffsetPairVector")
        BBL_STD_VECTOR_METHODS(
            (std::pair<PXR_NS::SdfPropertySpecHandle, PXR_NS::SdfLayerOffset>));

    bbl::Class<PXR_NS::SdfReference>("Reference");

    bbl::Class<PXR_NS::SdfReferenceEditorProxy>("ReferenceEditorProxy");

    bbl::Class<PXR_NS::SdfReferenceVector>("ReferenceVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::SdfReference)
    ;

    bbl::Class<PXR_NS::SdfRelationshipSpecHandle>("RelationshipSpecHandle");

    bbl::Class<PXR_NS::SdfRelationshipSpecHandleVector>(
        "RelationshipSpecHandleVector");

    bbl::Class<
        std::pair<PXR_NS::SdfRelationshipSpecHandle, PXR_NS::SdfLayerOffset>>(
        "RelationshipSpecHandleOffsetPair");

    bbl::Class<std::vector<
        std::pair<PXR_NS::SdfRelationshipSpecHandle, PXR_NS::SdfLayerOffset>>>(
        "RelationshipSpecHandleOffsetPairVector")
        BBL_STD_VECTOR_METHODS((std::pair<PXR_NS::SdfRelationshipSpecHandle,
                                          PXR_NS::SdfLayerOffset>));

    bbl::Enum<PXR_NS::SdfSpecifier>("Specifier");

    bbl::Enum<PXR_NS::SdfSpecType>("SpecType");

    bbl::Class<PXR_NS::SdfSpecHandle>("SpecHandle");

    bbl::Class<PXR_NS::SdfTimeCode>("TimeCode")
        .ctor(bbl::Ctor<PXR_NS::SdfTimeCode, double>(), "from_time")
        .m(&PXR_NS::SdfTimeCode::GetValue);

    bbl::Class<PXR_NS::SdfValueTypeName>("ValueTypeName").opaque_bytes();

    bbl::Enum<PXR_NS::SdfVariability>("Variability");
}

#endif
