#include "babble"
#include "babble-std"

#include <pxr/usd/usd/attribute.h>
#include <pxr/usd/usd/editTarget.h>
#include <pxr/usd/usd/inherits.h>
#include <pxr/usd/usd/payloads.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/property.h>
#include <pxr/usd/usd/references.h>
#include <pxr/usd/usd/relationship.h>
#include <pxr/usd/usd/resolveTarget.h>
#include <pxr/usd/usd/specializes.h>
#include <pxr/usd/usd/variantSets.h>

#include <pxr/base/tf/token.h>

// We define modules that just act as a way to group binding declarations
// together. These would then likely be turned into real modules in languages
// that support them like Rust or Python
BBL_MODULE(usd) {

    bbl::rename_namespace("PXR_NS::", "pxr");
    bbl::prepend_module_name(true);

    // Binding a class looks like this
    bbl::Class<PXR_NS::UsdPrim>("Prim")
        .ctor(bbl::Ctor<PXR_NS::UsdPrim>(), "new")
        .opaque_bytes()
        // Binding a (default) constructor looks like this
        // Method bindings just take the member function pointer and all
        // parameters with known types are extracted automatically
        .m(&PXR_NS::UsdPrim::GetPrimTypeInfo)
        .m(&PXR_NS::UsdPrim::GetPrimDefinition)
        .m(&PXR_NS::UsdPrim::GetSpecifier)
        .m(&PXR_NS::UsdPrim::SetSpecifier)
        .m(&PXR_NS::UsdPrim::GetPrimStack)
        .m(&PXR_NS::UsdPrim::GetPrimStackWithLayerOffsets)
        .m(&PXR_NS::UsdPrim::GetTypeName)
        .m(&PXR_NS::UsdPrim::SetTypeName)
        .m(&PXR_NS::UsdPrim::ClearTypeName)
        .m(&PXR_NS::UsdPrim::HasAuthoredTypeName)
        .m(&PXR_NS::UsdPrim::IsActive)
        .m(&PXR_NS::UsdPrim::SetActive)
        .m(&PXR_NS::UsdPrim::ClearActive)
        .m(&PXR_NS::UsdPrim::HasAuthoredActive)
        .m(&PXR_NS::UsdPrim::IsLoaded)
        .m(&PXR_NS::UsdPrim::IsModel)
        .m(&PXR_NS::UsdPrim::IsGroup)
        .m(&PXR_NS::UsdPrim::IsAbstract)
        .m(&PXR_NS::UsdPrim::IsDefined)
        .m(&PXR_NS::UsdPrim::GetName)
        .m(&PXR_NS::UsdPrim::HasDefiningSpecifier)
        .m(&PXR_NS::UsdPrim::GetAppliedSchemas)
        .m(&PXR_NS::UsdPrim::GetPropertyNames)
        .m(&PXR_NS::UsdPrim::GetAuthoredPropertyNames)
        .m(&PXR_NS::UsdPrim::GetProperties)
        .m(&PXR_NS::UsdPrim::GetAuthoredProperties)
        // Overloads require casting the member function pointer to select a particular one
        .m((std::vector<PXR_NS::UsdProperty>(PXR_NS::UsdPrim::*)(
               std::string const&) const) &
           PXR_NS::UsdPrim::GetPropertiesInNamespace)
        .m((std::vector<PXR_NS::UsdProperty>(PXR_NS::UsdPrim::*)(
               std::string const&) const) &
           PXR_NS::UsdPrim::GetAuthoredPropertiesInNamespace)
        .m(&PXR_NS::UsdPrim::GetPropertyOrder)
        .m(&PXR_NS::UsdPrim::SetPropertyOrder)
        .m(&PXR_NS::UsdPrim::ClearPropertyOrder)
        .m(&PXR_NS::UsdPrim::RemoveProperty)
        .m(&PXR_NS::UsdPrim::GetProperty)
        .m(&PXR_NS::UsdPrim::HasProperty)
        .m(&PXR_NS::UsdPrim::AddAppliedSchema)
        .m(&PXR_NS::UsdPrim::RemoveAppliedSchema)

        // IsA
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::IsA)
        // methods can be renamed to give useful names to overloads 
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::UsdSchemaVersion) const) &
           PXR_NS::UsdPrim::IsA, "IsA_with_version")

        // IsInFamily
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::IsInFamily)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&, PXR_NS::UsdSchemaRegistry::VersionPolicy) const) &
           PXR_NS::UsdPrim::IsInFamily, "IsInFamily_with_policy")
        .m((bool(PXR_NS::UsdPrim::*)( PXR_NS::TfToken const&, PXR_NS::UsdSchemaVersion, PXR_NS::UsdSchemaRegistry::VersionPolicy) const) &
           PXR_NS::UsdPrim::IsInFamily)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfType const&, PXR_NS::UsdSchemaRegistry::VersionPolicy) const) &
           PXR_NS::UsdPrim::IsInFamily)
        .m(&PXR_NS::UsdPrim::GetVersionIfIsInFamily)

        // HasAPI
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfType const&) const) &
           PXR_NS::UsdPrim::HasAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfType const&,
                                     PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::HasAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::HasAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::HasAPI)

        // HasAPIInFamily
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::HasAPIInFamily)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::HasAPIInFamily)
        .m((bool(PXR_NS::UsdPrim::*)(
               PXR_NS::TfToken const&, PXR_NS::UsdSchemaVersion,
               PXR_NS::UsdSchemaRegistry::VersionPolicy) const) &
           PXR_NS::UsdPrim::HasAPIInFamily)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::UsdSchemaVersion,
                                     PXR_NS::UsdSchemaRegistry::VersionPolicy,
                                     PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::HasAPIInFamily)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfType const&,
                                     PXR_NS::UsdSchemaRegistry::VersionPolicy)
                const) &
           PXR_NS::UsdPrim::HasAPIInFamily)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfType const&,
                                     PXR_NS::UsdSchemaRegistry::VersionPolicy,
                                     PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::HasAPIInFamily)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::UsdSchemaRegistry::VersionPolicy)
                const) &
           PXR_NS::UsdPrim::HasAPIInFamily)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::UsdSchemaRegistry::VersionPolicy,
                                     PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::HasAPIInFamily)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::UsdSchemaVersion*) const) &
           PXR_NS::UsdPrim::GetVersionIfHasAPIInFamily)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::TfToken const&,
                                     PXR_NS::UsdSchemaVersion*) const) &
           PXR_NS::UsdPrim::GetVersionIfHasAPIInFamily)

        // CanApplyAPI
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfType const&, std::string*)
                const) &
           PXR_NS::UsdPrim::CanApplyAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfType const&,
                                     PXR_NS::TfToken const&, std::string*)
                const) &
           PXR_NS::UsdPrim::CanApplyAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&, std::string*)
                const) &
           PXR_NS::UsdPrim::CanApplyAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::TfToken const&, std::string*)
                const) &
           PXR_NS::UsdPrim::CanApplyAPI)
        .m((bool(PXR_NS::UsdPrim::*)(
               PXR_NS::TfToken const&, PXR_NS::UsdSchemaVersion,
               PXR_NS::TfToken const&, std::string*) const) &
           PXR_NS::UsdPrim::CanApplyAPI)

        // ApplyAPI
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfType const&) const) &
           PXR_NS::UsdPrim::ApplyAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfType const&,
                                     PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::ApplyAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::ApplyAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::ApplyAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::UsdSchemaVersion) const) &
           PXR_NS::UsdPrim::ApplyAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::UsdSchemaVersion,
                                     PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::ApplyAPI)

        // RemoveAPI
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfType const&) const) &
           PXR_NS::UsdPrim::RemoveAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfType const&,
                                     PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::RemoveAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::RemoveAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::RemoveAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::UsdSchemaVersion) const) &
           PXR_NS::UsdPrim::RemoveAPI)
        .m((bool(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                     PXR_NS::UsdSchemaVersion,
                                     PXR_NS::TfToken const&) const) &
           PXR_NS::UsdPrim::RemoveAPI)

        // Prim Children
        .m(&PXR_NS::UsdPrim::GetChild)
        .m(&PXR_NS::UsdPrim::GetChildren)
        .m(&PXR_NS::UsdPrim::GetAllChildren)
        .m(&PXR_NS::UsdPrim::GetFilteredChildren)
        .m(&PXR_NS::UsdPrim::GetChildrenNames)
        .m(&PXR_NS::UsdPrim::GetFilteredChildrenNames)
        .m(&PXR_NS::UsdPrim::GetDescendants)
        .m(&PXR_NS::UsdPrim::GetAllDescendants)
        .m(&PXR_NS::UsdPrim::GetFilteredDescendants)
        .m(&PXR_NS::UsdPrim::GetChildrenReorder)

        // Parent & Stage
        .m(&PXR_NS::UsdPrim::GetParent)
        .m(&PXR_NS::UsdPrim::GetNextSibling)
        .m(&PXR_NS::UsdPrim::GetFilteredNextSibling)
        .m(&PXR_NS::UsdPrim::IsPseudoRoot)
        .m(&PXR_NS::UsdPrim::GetPrimAtPath)
        .m(&PXR_NS::UsdPrim::GetObjectAtPath)
        .m(&PXR_NS::UsdPrim::GetPropertyAtPath)
        .m(&PXR_NS::UsdPrim::GetAttributeAtPath)
        .m(&PXR_NS::UsdPrim::GetRelationshipAtPath)

        // Variants
        .m(&PXR_NS::UsdPrim::GetVariantSets)
        .m(&PXR_NS::UsdPrim::GetVariantSet)
        .m(&PXR_NS::UsdPrim::HasVariantSets)

        // Attributes
        // We'll only do one of these create methods as the rest are for
        // convenience, and we should do convenience on the FFI side
        .m((PXR_NS::UsdAttribute(PXR_NS::UsdPrim::*)(
               PXR_NS::TfToken const&, PXR_NS::SdfValueTypeName const&, bool,
               PXR_NS::SdfVariability) const) &
           PXR_NS::UsdPrim::CreateAttribute)
        .m(&PXR_NS::UsdPrim::GetAttributes)
        .m(&PXR_NS::UsdPrim::GetAuthoredAttributes)
        .m(&PXR_NS::UsdPrim::GetAttribute)
        .m(&PXR_NS::UsdPrim::HasAttribute)
        .m(&PXR_NS::UsdPrim::FindAllAttributeConnectionPaths)

        // Relationships
        .m((PXR_NS::UsdRelationship(PXR_NS::UsdPrim::*)(PXR_NS::TfToken const&,
                                                        bool) const) &
           PXR_NS::UsdPrim::CreateRelationship)
        .m(&PXR_NS::UsdPrim::GetRelationships)
        .m(&PXR_NS::UsdPrim::GetAuthoredRelationships)
        .m(&PXR_NS::UsdPrim::GetRelationship)
        .m(&PXR_NS::UsdPrim::HasRelationship)
        .m(&PXR_NS::UsdPrim::FindAllRelationshipTargetPaths)

        // Payloads, load and unload
        .m(&PXR_NS::UsdPrim::GetPayloads)
        .m(&PXR_NS::UsdPrim::HasAuthoredPayloads)
        .m(&PXR_NS::UsdPrim::Load)
        .m(&PXR_NS::UsdPrim::Unload)

        // References
        .m(&PXR_NS::UsdPrim::GetReferences)
        .m(&PXR_NS::UsdPrim::HasAuthoredReferences)

        // Inherits
        .m(&PXR_NS::UsdPrim::GetInherits)
        .m(&PXR_NS::UsdPrim::HasAuthoredInherits)

        // Specializes
        .m(&PXR_NS::UsdPrim::GetSpecializes)
        .m(&PXR_NS::UsdPrim::HasAuthoredSpecializes)

        // Composition
        .m(&PXR_NS::UsdPrim::GetPrimIndex)
        .m(&PXR_NS::UsdPrim::ComputeExpandedPrimIndex)
        .m(&PXR_NS::UsdPrim::MakeResolveTargetUpToEditTarget)
        .m(&PXR_NS::UsdPrim::MakeResolveTargetStrongerThanEditTarget);

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

    bbl::Class<PXR_NS::UsdAttribute>("Attribute").opaque_bytes();

    bbl::Class<std::vector<PXR_NS::UsdAttribute>>("AttributeVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::UsdAttribute);

    bbl::Class<PXR_NS::UsdRelationship>("Relationship").opaque_bytes();

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
}

BBL_MODULE(pcp) { 
    bbl::rename_namespace("PXR_NS::", "pxr");
    bbl::prepend_module_name(true);

    bbl::Class<PXR_NS::PcpPrimIndex>("PrimIndex"); 
}

BBL_MODULE(std) {
    bbl::prepend_module_name(false);
    bbl::Class<std::string>("String");

    // We provide macros for automating the binding of common types' methods
    bbl::Class<std::vector<std::string>>("StringVector")
        BBL_STD_VECTOR_METHODS(std::string);
}

BBL_MODULE(tf) {
    bbl::rename_namespace("PXR_NS::", "pxr");
    bbl::prepend_module_name(true);

    bbl::Class<PXR_NS::TfToken>("Token");
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
    BBL_STD_PAIR_FUNCTIONS(PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset,
                           "PrimSpecHandleOffsetPair");

    bbl::Class<std::vector<
        std::pair<PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset>>>(
        "PrimSpecHandleOffsetPairVector")
        BBL_STD_VECTOR_METHODS(
            (std::pair<PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset>));

    bbl::Class<PXR_NS::SdfLayerOffset>("LayerOffset");

    bbl::Enum<PXR_NS::SdfVariability>("Variability");

    bbl::Class<PXR_NS::SdfPath>("Path").opaque_bytes();

    bbl::Class<PXR_NS::SdfPathVector>("PathVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::SdfPath);

    bbl::Class<PXR_NS::SdfValueTypeName>("ValueTypeName").opaque_bytes();
}
