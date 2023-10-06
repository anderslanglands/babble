#include <vcruntime.h>
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

    bbl::Class<PXR_NS::SdfPath>("Path")
        .opaque_ptr()
        .ctor(bbl::Ctor<PXR_NS::SdfPath>(), "new")
        .m(&PXR_NS::SdfPath::GetPathElementCount)
        .m(&PXR_NS::SdfPath::IsAbsolutePath)
        .m(&PXR_NS::SdfPath::IsAbsoluteRootPath)
        .m(&PXR_NS::SdfPath::IsPrimPath)
        .m(&PXR_NS::SdfPath::IsAbsoluteRootOrPrimPath)
        .m(&PXR_NS::SdfPath::IsRootPrimPath)
        .m(&PXR_NS::SdfPath::IsPropertyPath)
        .m(&PXR_NS::SdfPath::IsPrimPropertyPath)
        .m(&PXR_NS::SdfPath::IsNamespacedPropertyPath)
        .m(&PXR_NS::SdfPath::IsPrimVariantSelectionPath)
        .m(&PXR_NS::SdfPath::IsPrimOrPrimVariantSelectionPath)
        .m(&PXR_NS::SdfPath::ContainsPrimVariantSelection)
        .m(&PXR_NS::SdfPath::ContainsPropertyElements)
        .m(&PXR_NS::SdfPath::ContainsTargetPath)
        .m(&PXR_NS::SdfPath::IsRelationalAttributePath)
        .m(&PXR_NS::SdfPath::IsTargetPath)
        .m(&PXR_NS::SdfPath::IsMapperPath)
        .m(&PXR_NS::SdfPath::IsMapperArgPath)
        .m(&PXR_NS::SdfPath::IsExpressionPath)
        .m(&PXR_NS::SdfPath::IsEmpty)
        .m(&PXR_NS::SdfPath::GetAsToken)
        .m(&PXR_NS::SdfPath::GetAsString)
        .m(&PXR_NS::SdfPath::GetString)
        .m(&PXR_NS::SdfPath::GetText)
        .m((PXR_NS::SdfPathVector (PXR_NS::SdfPath::*)() const)
            &PXR_NS::SdfPath::GetPrefixes)
        .m((PXR_NS::SdfPathVector (PXR_NS::SdfPath::*)(size_t) const)
            &PXR_NS::SdfPath::GetPrefixes, "GetPrefixes_up_to")
        .m((void (PXR_NS::SdfPath::*)(PXR_NS::SdfPathVector*) const)
            &PXR_NS::SdfPath::GetPrefixes, "GetPrefixes_in")
        .m((void (PXR_NS::SdfPath::*)(PXR_NS::SdfPathVector*, size_t) const)
            &PXR_NS::SdfPath::GetPrefixes, "GetPrefixes_in_up_to")
        // GetAncestorsRange
        .m(&PXR_NS::SdfPath::GetName)
        .m(&PXR_NS::SdfPath::GetNameToken)
        .m(&PXR_NS::SdfPath::GetElementString)
        .m(&PXR_NS::SdfPath::GetElementToken)
        .m(&PXR_NS::SdfPath::ReplaceName)
        .m(&PXR_NS::SdfPath::GetTargetPath)
        .m(&PXR_NS::SdfPath::GetAllTargetPathsRecursively)
        // GetVariantSelection
        .m(&PXR_NS::SdfPath::HasPrefix)
        .m(&PXR_NS::SdfPath::GetParentPath)
        .m(&PXR_NS::SdfPath::GetPrimPath)
        .m(&PXR_NS::SdfPath::GetPrimOrPrimVariantSelectionPath)
        .m(&PXR_NS::SdfPath::GetAbsoluteRootOrPrimPath)
        .m(&PXR_NS::SdfPath::StripAllVariantSelections)
        .m(&PXR_NS::SdfPath::AppendPath)
        .m(&PXR_NS::SdfPath::AppendChild)
        .m(&PXR_NS::SdfPath::AppendProperty)
        .m(&PXR_NS::SdfPath::AppendVariantSelection)
        .m(&PXR_NS::SdfPath::AppendTarget)
        .m(&PXR_NS::SdfPath::AppendRelationalAttribute)
        .m(&PXR_NS::SdfPath::ReplaceTargetPath)
        .m(&PXR_NS::SdfPath::AppendMapper)
        .m(&PXR_NS::SdfPath::AppendMapperArg)
        .m(&PXR_NS::SdfPath::AppendExpression)
        .m(&PXR_NS::SdfPath::AppendElementString)
        .m(&PXR_NS::SdfPath::AppendElementToken)
        .m(&PXR_NS::SdfPath::ReplacePrefix)
        .m(&PXR_NS::SdfPath::GetCommonPrefix)
        // .m(&PXR_NS::SdfPath::RemoveCommonSuffix)
        .m(&PXR_NS::SdfPath::MakeAbsolutePath)
        .m(&PXR_NS::SdfPath::MakeRelativePath)
        .m(&PXR_NS::SdfPath::EmptyPath)
        .m(&PXR_NS::SdfPath::AbsoluteRootPath)
        .m(&PXR_NS::SdfPath::ReflexiveRelativePath)
        .m(&PXR_NS::SdfPath::IsValidIdentifier)
        .m(&PXR_NS::SdfPath::IsValidNamespacedIdentifier)
        .m(&PXR_NS::SdfPath::TokenizeIdentifier)
        .m(&PXR_NS::SdfPath::TokenizeIdentifierAsTokens)
        .m(&PXR_NS::SdfPath::IsValidPathString)
        .m(&PXR_NS::SdfPath::GetConciseRelativePaths)
        .m(&PXR_NS::SdfPath::RemoveDescendentPaths)
        .m(&PXR_NS::SdfPath::RemoveAncestorPaths)
        ;

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
