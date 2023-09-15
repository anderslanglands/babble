#include <pxr/usd/usd/common.h>
#include <pxr/usd/usd/stage.h>
#if defined(__clang__)

#include "babble"
#include "babble-std"

#include <pxr/usd/usd/attribute.h>
#include <pxr/usd/usd/editTarget.h>
#include <pxr/usd/usd/inherits.h>
#include <pxr/usd/usd/payloads.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usd/property.h>
#include <pxr/usd/usd/references.h>
#include <pxr/usd/usd/relationship.h>
#include <pxr/usd/usd/resolveTarget.h>
#include <pxr/usd/usd/specializes.h>
#include <pxr/usd/usd/variantSets.h>

#include <pxr/base/tf/token.h>

using Token = PXR_NS::TfToken;

// We define modules that just act as a way to group binding declarations
// together. These would then likely be turned into real modules in languages
// that support them like Rust or Python
BBL_MODULE(usd) {

    bbl::rename_namespace("PXR_NS::", "pxr");
    bbl::prepend_module_name(true);

    // Binding a class looks like this
    using Prim = PXR_NS::UsdPrim;
    bbl::Class<PXR_NS::UsdPrim>("Prim")
        .ctor(bbl::Ctor<Prim>(), "new")
        .opaque_ptr()
        // Binding a (default) constructor looks like this
        // Method bindings just take the member function pointer and all
        // parameters with known types are extracted automatically
        .m(&Prim::GetPrimTypeInfo)
        .m(&Prim::GetPrimDefinition)
        .m(&Prim::GetSpecifier)
        .m(&Prim::SetSpecifier)
        .m(&Prim::GetPrimStack)
        .m(&Prim::GetPrimStackWithLayerOffsets)
        .m(&Prim::GetTypeName)
        .m(&Prim::SetTypeName)
        .m(&Prim::ClearTypeName)
        .m(&Prim::HasAuthoredTypeName)
        .m(&Prim::IsActive)
        .m(&Prim::SetActive)
        .m(&Prim::ClearActive)
        .m(&Prim::HasAuthoredActive)
        .m(&Prim::IsLoaded)
        .m(&Prim::IsModel)
        .m(&Prim::IsGroup)
        .m(&Prim::IsAbstract)
        .m(&Prim::IsDefined)
        .m(&Prim::GetName)
        .m(&Prim::HasDefiningSpecifier)
        .m(&Prim::GetAppliedSchemas)
        // Can't handle std::function properly yet...
        // probably need to create wrapper objects (yuck)
        //   .m(&Prim::GetPropertyNames)
        //   .m(&Prim::GetAuthoredPropertyNames)
        // .m(&Prim::GetProperties)
        // .m(&Prim::GetAuthoredProperties)
        // Overloads require casting the member function pointer to select a
        // particular one
        .m((std::vector<PXR_NS::UsdProperty>(Prim::*)(std::string const&)
                const) &
           Prim::GetPropertiesInNamespace)
        .m((std::vector<PXR_NS::UsdProperty>(Prim::*)(std::string const&)
                const) &
           Prim::GetAuthoredPropertiesInNamespace)
        .m(&Prim::GetPropertyOrder)
        .m(&Prim::SetPropertyOrder)
        .m(&Prim::ClearPropertyOrder)
        .m(&Prim::RemoveProperty)
        .m(&Prim::GetProperty)
        .m(&Prim::HasProperty)
        .m(&Prim::AddAppliedSchema)
        .m(&Prim::RemoveAppliedSchema)

        // IsA
        .m((bool(Prim::*)(PXR_NS::TfToken const&) const) & Prim::IsA)
        // methods can be renamed to give useful names to overloads
        .m((bool(Prim::*)(PXR_NS::TfToken const&, PXR_NS::UsdSchemaVersion)
                const) &
               Prim::IsA,
           "IsA_with_version")

        // IsInFamily
        .m((bool(Prim::*)(PXR_NS::TfToken const&) const) & Prim::IsInFamily)

        .m((bool(Prim::*)(PXR_NS::TfToken const&,
                          PXR_NS::UsdSchemaRegistry::VersionPolicy) const) &
               Prim::IsInFamily,
           "IsInFamily_with_policy")

        .m((bool(Prim::*)(PXR_NS::TfToken const&, PXR_NS::UsdSchemaVersion,
                          PXR_NS::UsdSchemaRegistry::VersionPolicy) const) &
               Prim::IsInFamily,
           "IsInFamily_with_policy_and_version")

        .m((bool(Prim::*)(PXR_NS::TfType const&,
                          PXR_NS::UsdSchemaRegistry::VersionPolicy) const) &
               Prim::IsInFamily,
           "IsInFamily_with_type")

        .m(&Prim::GetVersionIfIsInFamily)

        // HasAPI
        .m((bool(Prim::*)(PXR_NS::TfType const&) const) & Prim::HasAPI)
        .m((bool(Prim::*)(PXR_NS::TfType const&, PXR_NS::TfToken const&)
                const) &
               Prim::HasAPI,
           "HasAPI_with_instance_name")
        .m((bool(Prim::*)(PXR_NS::TfToken const&) const) & Prim::HasAPI,
           "HasAPI_with_schema_identifier")
        .m((bool(Prim::*)(PXR_NS::TfToken const&, PXR_NS::TfToken const&)
                const) &
               Prim::HasAPI,
           "HasAPI_with_instance_name_and_schema_identifier")

        // HasAPIInFamily
        .m((bool(Prim::*)(PXR_NS::TfToken const&) const) & Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&,
        //                              PXR_NS::TfToken const&) const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(
        //        PXR_NS::TfToken const&, PXR_NS::UsdSchemaVersion,
        //        PXR_NS::UsdSchemaRegistry::VersionPolicy) const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&,
        //                              PXR_NS::UsdSchemaVersion,
        //                              PXR_NS::UsdSchemaRegistry::VersionPolicy,
        //                              PXR_NS::TfToken const&) const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(PXR_NS::TfType const&,
        //                              PXR_NS::UsdSchemaRegistry::VersionPolicy)
        //         const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(PXR_NS::TfType const&,
        //                              PXR_NS::UsdSchemaRegistry::VersionPolicy,
        //                              PXR_NS::TfToken const&) const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&,
        //                              PXR_NS::UsdSchemaRegistry::VersionPolicy)
        //         const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&,
        //                              PXR_NS::UsdSchemaRegistry::VersionPolicy,
        //                              PXR_NS::TfToken const&) const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&,
        //                              PXR_NS::UsdSchemaVersion*) const) &
        //    Prim::GetVersionIfHasAPIInFamily)
        .m((bool(Prim::*)(PXR_NS::TfToken const&, PXR_NS::TfToken const&,
                          PXR_NS::UsdSchemaVersion*) const) &
           Prim::GetVersionIfHasAPIInFamily)

        // CanApplyAPI
        .m((bool(Prim::*)(PXR_NS::TfType const&, std::string*) const) &
           Prim::CanApplyAPI)
        // .m((bool(Prim::*)(PXR_NS::TfType const&,
        //                              PXR_NS::TfToken const&, std::string*)
        //         const) &
        //    Prim::CanApplyAPI)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&, std::string*)
        //         const) &
        //    Prim::CanApplyAPI)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&,
        //                              PXR_NS::TfToken const&, std::string*)
        //         const) &
        //    Prim::CanApplyAPI)
        // .m((bool(Prim::*)(
        //        PXR_NS::TfToken const&, PXR_NS::UsdSchemaVersion,
        //        PXR_NS::TfToken const&, std::string*) const) &
        //    Prim::CanApplyAPI)

        // ApplyAPI
        .m((bool(Prim::*)(PXR_NS::TfType const&) const) & Prim::ApplyAPI)
        // .m((bool(Prim::*)(PXR_NS::TfType const&,
        //                              PXR_NS::TfToken const&) const) &
        //    Prim::ApplyAPI)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&) const) &
        //    Prim::ApplyAPI)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&,
        //                              PXR_NS::TfToken const&) const) &
        //    Prim::ApplyAPI)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&,
        //                              PXR_NS::UsdSchemaVersion) const) &
        //    Prim::ApplyAPI)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&,
        //                              PXR_NS::UsdSchemaVersion,
        //                              PXR_NS::TfToken const&) const) &
        //    Prim::ApplyAPI)

        // RemoveAPI
        .m((bool(Prim::*)(PXR_NS::TfType const&) const) & Prim::RemoveAPI)
        // .m((bool(Prim::*)(PXR_NS::TfType const&,
        //                              PXR_NS::TfToken const&) const) &
        //    Prim::RemoveAPI)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&) const) &
        //    Prim::RemoveAPI)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&,
        //                              PXR_NS::TfToken const&) const) &
        //    Prim::RemoveAPI)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&,
        //                              PXR_NS::UsdSchemaVersion) const) &
        //    Prim::RemoveAPI)
        // .m((bool(Prim::*)(PXR_NS::TfToken const&,
        //                              PXR_NS::UsdSchemaVersion,
        //                              PXR_NS::TfToken const&) const) &
        //    Prim::RemoveAPI)

        // Prim Children
        .m(&Prim::GetChild)
        .m(&Prim::GetChildren)
        .m(&Prim::GetAllChildren)
        .m(&Prim::GetFilteredChildren)
        .m(&Prim::GetChildrenNames)
        .m(&Prim::GetFilteredChildrenNames)
        .m(&Prim::GetDescendants)
        .m(&Prim::GetAllDescendants)
        .m(&Prim::GetFilteredDescendants)
        .m(&Prim::GetChildrenReorder)

        // Parent & Stage
        .m(&Prim::GetParent)
        .m(&Prim::GetNextSibling)
        .m(&Prim::GetFilteredNextSibling)
        .m(&Prim::IsPseudoRoot)
        .m(&Prim::GetPrimAtPath)
        .m(&Prim::GetObjectAtPath)
        .m(&Prim::GetPropertyAtPath)
        .m(&Prim::GetAttributeAtPath)
        .m(&Prim::GetRelationshipAtPath)

        // Variants
        .m(&Prim::GetVariantSets)
        .m(&Prim::GetVariantSet)
        .m(&Prim::HasVariantSets)

        // Attributes
        // We'll only do one of these create methods as the rest are for
        // convenience, and we should do convenience on the FFI side
        .m((PXR_NS::UsdAttribute(Prim::*)(PXR_NS::TfToken const&,
                                          PXR_NS::SdfValueTypeName const&, bool,
                                          PXR_NS::SdfVariability) const) &
           Prim::CreateAttribute)
        .m(&Prim::GetAttributes)
        .m(&Prim::GetAuthoredAttributes)
        .m(&Prim::GetAttribute)
        .m(&Prim::HasAttribute)
        // .m(&Prim::FindAllAttributeConnectionPaths)

        // Relationships
        .m((PXR_NS::UsdRelationship(Prim::*)(PXR_NS::TfToken const&, bool)
                const) &
           Prim::CreateRelationship)
        .m(&Prim::GetRelationships)
        .m(&Prim::GetAuthoredRelationships)
        .m(&Prim::GetRelationship)
        .m(&Prim::HasRelationship)
        // .m(&Prim::FindAllRelationshipTargetPaths)

        // Payloads, load and unload
        .m(&Prim::GetPayloads)
        .m(&Prim::HasAuthoredPayloads)
        .m(&Prim::Load)
        .m(&Prim::Unload)

        // References
        .m(&Prim::GetReferences)
        .m(&Prim::HasAuthoredReferences)

        // Inherits
        .m(&Prim::GetInherits)
        .m(&Prim::HasAuthoredInherits)

        // Specializes
        .m(&Prim::GetSpecializes)
        .m(&Prim::HasAuthoredSpecializes)

        // Composition
        .m(&Prim::GetPrimIndex)
        .m(&Prim::ComputeExpandedPrimIndex)
        .m(&Prim::MakeResolveTargetUpToEditTarget)
        .m(&Prim::MakeResolveTargetStrongerThanEditTarget);

    bbl::Class<PXR_NS::UsdObject>("Object");
    bbl::Class<PXR_NS::UsdProperty>("Property");

    bbl::Class<std::vector<PXR_NS::UsdProperty>>("PropertyVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::UsdProperty);

    bbl::Class<PXR_NS::UsdPrimSiblingRange>("PrimSiblingRange")
        .m(&PXR_NS::UsdPrimSiblingRange::begin)
        .m(&PXR_NS::UsdPrimSiblingRange::end);

    bbl::Class<PXR_NS::UsdPrimSiblingIterator>("PrimSiblingIterator")
        .m(&PXR_NS::UsdPrimSiblingIterator::operator*, "deref")
        .m(&PXR_NS::UsdPrimSiblingIterator::operator++, "op_inc");

    bbl::Class<PXR_NS::UsdPrimSubtreeRange>("PrimSubtreeRange");

    bbl::Class<PXR_NS::UsdPrimTypeInfo>("PrimTypeInfo");
    bbl::Class<PXR_NS::UsdPrimDefinition>("PrimDefinition");

    bbl::Class<PXR_NS::Usd_PrimFlagsPredicate>("PrimFlagsPredicate");
    bbl::Class<PXR_NS::UsdPrim::PropertyPredicateFunc>("PropertyPredicateFunc");

    bbl::Enum<PXR_NS::UsdSchemaRegistry::VersionPolicy>(
        "SchemaRegistryVersionPolicy");

    bbl::Class<PXR_NS::UsdVariantSet>("VariantSet");
    bbl::Class<PXR_NS::UsdVariantSets>("VariantSets");

    bbl::Class<PXR_NS::UsdAttribute>("Attribute").opaque_ptr();

    bbl::Class<std::vector<PXR_NS::UsdAttribute>>("AttributeVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::UsdAttribute);

    bbl::Class<PXR_NS::UsdRelationship>("Relationship").opaque_ptr();

    bbl::Class<std::vector<PXR_NS::UsdRelationship>>("RelationshipVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::UsdRelationship);

    bbl::Class<std::function<bool(PXR_NS::UsdAttribute const&)>>(
        "FindAttributePredicate");
    bbl::Class<std::function<bool(PXR_NS::UsdRelationship const&)>>(
        "FindRelationshipPredicate");

    bbl::Class<PXR_NS::UsdPayloads>("Payloads");
    bbl::Class<PXR_NS::UsdReferences>("References");
    bbl::Class<PXR_NS::UsdInherits>("Inherits");
    bbl::Class<PXR_NS::UsdSpecializes>("Specializes");

    bbl::Class<PXR_NS::UsdResolveTarget>("ResolveTarget");
    bbl::Class<PXR_NS::UsdEditTarget>("EditTarget");

    bbl::Enum<PXR_NS::UsdLoadPolicy>("LoadPolicy");

    using Stage = PXR_NS::UsdStage;
    bbl::Class<PXR_NS::UsdStage>("Stage")
        .m((PXR_NS::UsdStageRefPtr(*)(std::string const&,
                                      PXR_NS::UsdStage::InitialLoadSet)) &
           PXR_NS::UsdStage::Open)
        .m(&Stage::GetPseudoRoot);

    using StageRefPtr = PXR_NS::UsdStageRefPtr;
    bbl::Class<PXR_NS::UsdStageRefPtr>("StageRefPtr")
        .ctor(bbl::Ctor<StageRefPtr>(), "ctor")
        .m(&StageRefPtr::operator->, "get");

    using PrimRange = PXR_NS::UsdPrimRange;
    bbl::Class<PXR_NS::UsdPrimRange>("PrimRange")
        .opaque_ptr()
        .ctor(bbl::Ctor<PrimRange, Prim>("start"), "from_prim")
        .m(&PrimRange::begin)
        .m(&PrimRange::end);

    using PrimRangeIterator = PXR_NS::UsdPrimRange::iterator;
    bbl::Class<PXR_NS::UsdPrimRange::iterator>("PrimRangeIterator")
        .opaque_ptr()
        .ctor(bbl::Ctor<PrimRangeIterator>(), "new")
        .m(&PrimRangeIterator::operator++, "op_inc")
        .m((PrimRangeIterator&(PrimRangeIterator::*)(PrimRangeIterator const&))&PrimRangeIterator::operator=, "op_assign")
        .m((bool(PrimRangeIterator::*)(PrimRangeIterator const&) const) &
               PrimRangeIterator::operator==,
           "op_eq")
        .m(&PrimRangeIterator::operator*, "deref");

    bbl::Enum<PXR_NS::UsdStage::InitialLoadSet>("StageInitialLoadSet");
}

BBL_MODULE(pcp) {
    bbl::rename_namespace("PXR_NS::", "pxr");
    bbl::prepend_module_name(true);

    bbl::Class<PXR_NS::PcpPrimIndex>("PrimIndex");
}

BBL_MODULE(std) {
    bbl::prepend_module_name(false);
    bbl::Class<std::string>("String").ctor(
        bbl::Ctor<std::string, char const*>(), "from_char_ptr");

    // We provide macros for automating the binding of common types' methods
    bbl::Class<std::vector<std::string>>("StringVector")
        BBL_STD_VECTOR_METHODS(std::string);
}

BBL_MODULE(tf) {
    bbl::rename_namespace("PXR_NS::", "pxr");
    bbl::prepend_module_name(true);

    bbl::Class<PXR_NS::TfToken>("Token")
        .opaque_ptr()
        .ctor(bbl::Ctor<Token>(), "new")
        .m((Token& (Token::*)(Token const&))&Token::operator=, "op_assign")
        .m(&PXR_NS::TfToken::GetText);

    bbl::Class<PXR_NS::TfType>("Type");

    bbl::Class<PXR_NS::TfTokenVector>("TokenVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::TfToken);
}

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