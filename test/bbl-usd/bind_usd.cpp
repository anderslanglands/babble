#include <pxr/usd/ar/resolverContext.h>
#include <pxr/usd/usd/interpolation.h>
#include <pxr/usd/usd/schemaRegistry.h>
#include <pxr/usd/usd/stagePopulationMask.h>
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
#include <pxr/usd/usd/common.h>
#include <pxr/usd/usd/stage.h>

#include <pxr/base/tf/token.h>


// We define modules that just act as a way to group binding declarations
// together. These would then likely be turned into real modules in languages
// that support them like Rust or Python
BBL_MODULE(usd) {

    // clang-format off

    using Token = PXR_NS::TfToken;
    using Type = PXR_NS::TfType;
    using Property = PXR_NS::UsdProperty;
    using Attribute = PXR_NS::UsdAttribute;
    using Relationship = PXR_NS::UsdRelationship;
    using Object = PXR_NS::UsdObject;
    using Prim = PXR_NS::UsdPrim;
    using PrimRange = PXR_NS::UsdPrimRange;
    using PrimRangeIterator = PXR_NS::UsdPrimRange::iterator;
    using SchemaVersion = PXR_NS::UsdSchemaVersion;
    using SchemaRegistry = PXR_NS::UsdSchemaRegistry;
    using Stage = PXR_NS::UsdStage;
    using StageRefPtr = PXR_NS::UsdStageRefPtr;
    using EditTarget = PXR_NS::UsdEditTarget;
    using LayerHandle = PXR_NS::SdfLayerHandle;
    using Value = PXR_NS::VtValue;
    using StageInitialLoadSet = PXR_NS::UsdStage::InitialLoadSet;
    using ResolverContext = PXR_NS::ArResolverContext;
    using StagePopulationMask = PXR_NS::UsdStagePopulationMask;

    bbl::Class<PXR_NS::UsdAttribute>("Attribute").opaque_ptr();

    bbl::Class<std::vector<PXR_NS::UsdAttribute>>("AttributeVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::UsdAttribute);

    bbl::Class<PXR_NS::UsdEditTarget>("EditTarget");

    bbl::Class<PXR_NS::UsdInherits>("Inherits");

    bbl::Enum<PXR_NS::UsdInterpolationType>("InterpolationType")
        ;

    bbl::Enum<PXR_NS::UsdLoadPolicy>("LoadPolicy");

    bbl::Class<PXR_NS::UsdObject>("Object");

    bbl::Class<PXR_NS::UsdPayloads>("Payloads");

    // Binding a class looks like this
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
        .m((std::vector<Property>(Prim::*)(std::string const&) const)
            &Prim::GetPropertiesInNamespace
        )
        .m((std::vector<Property>(Prim::*)(std::string const&) const)
            &Prim::GetAuthoredPropertiesInNamespace
        )
        .m(&Prim::GetPropertyOrder)
        .m(&Prim::SetPropertyOrder)
        .m(&Prim::ClearPropertyOrder)
        .m(&Prim::RemoveProperty)
        .m(&Prim::GetProperty)
        .m(&Prim::HasProperty)
        .m(&Prim::AddAppliedSchema)
        .m(&Prim::RemoveAppliedSchema)

        // IsA
        .m((bool(Prim::*)(Token const&) const)
            &Prim::IsA
        )
        // methods can be renamed to give useful names to overloads
        .m((bool(Prim::*)(Token const&, SchemaVersion) const)
            &Prim::IsA, "IsA_with_version"
        )

        // IsInFamily
        .m((bool(Prim::*)(Token const&) const)
            &Prim::IsInFamily
        )
        .m((bool(Prim::*)(Token const&, SchemaRegistry::VersionPolicy) const)
            &Prim::IsInFamily, "IsInFamily_with_policy"
        )
        .m((bool(Prim::*)(Token const&, SchemaVersion, SchemaRegistry::VersionPolicy) const)
            &Prim::IsInFamily, "IsInFamily_with_policy_and_version"
        )
        .m((bool(Prim::*)(Type const&, SchemaRegistry::VersionPolicy) const)
            &Prim::IsInFamily, "IsInFamily_with_type"
        )
        .m(&Prim::GetVersionIfIsInFamily)

        // HasAPI
        .m((bool(Prim::*)(Type const&) const)
            &Prim::HasAPI
        )
        .m((bool(Prim::*)(Type const&, Token const&) const)
            &Prim::HasAPI, "HasAPI_with_instance_name"
        )
        .m((bool(Prim::*)(Token const&) const)
            &Prim::HasAPI, "HasAPI_with_schema_identifier"
        )
        .m((bool(Prim::*)(Token const&, Token const&) const)
            &Prim::HasAPI, "HasAPI_with_instance_name_and_schema_identifier"
        )

        // HasAPIInFamily
        .m((bool(Prim::*)(Token const&) const)
            &Prim::HasAPIInFamily
        )
        // .m((bool(Prim::*)(Token const&,
        //                              Token const&) const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(
        //        Token const&, SchemaVersion,
        //        SchemaRegistry::VersionPolicy) const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(Token const&,
        //                              SchemaVersion,
        //                              SchemaRegistry::VersionPolicy,
        //                              Token const&) const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(Type const&,
        //                              SchemaRegistry::VersionPolicy)
        //         const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(Type const&,
        //                              SchemaRegistry::VersionPolicy,
        //                              Token const&) const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(Token const&,
        //                              SchemaRegistry::VersionPolicy)
        //         const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(Token const&,
        //                              SchemaRegistry::VersionPolicy,
        //                              Token const&) const) &
        //    Prim::HasAPIInFamily)
        // .m((bool(Prim::*)(Token const&,
        //                              SchemaVersion*) const) &
        //    Prim::GetVersionIfHasAPIInFamily)
        .m((bool(Prim::*)(Token const&, Token const&, SchemaVersion*) const)
            &Prim::GetVersionIfHasAPIInFamily
        )

        // CanApplyAPI
        .m((bool(Prim::*)(Type const&, std::string*) const)
            &Prim::CanApplyAPI
        )
        // .m((bool(Prim::*)(Type const&,
        //                              Token const&, std::string*)
        //         const) &
        //    Prim::CanApplyAPI)
        // .m((bool(Prim::*)(Token const&, std::string*)
        //         const) &
        //    Prim::CanApplyAPI)
        // .m((bool(Prim::*)(Token const&,
        //                              Token const&, std::string*)
        //         const) &
        //    Prim::CanApplyAPI)
        // .m((bool(Prim::*)(
        //        Token const&, SchemaVersion,
        //        Token const&, std::string*) const) &
        //    Prim::CanApplyAPI)

        // ApplyAPI
        .m((bool(Prim::*)(Type const&) const)
            &Prim::ApplyAPI
        )
        // .m((bool(Prim::*)(Type const&,
        //                              Token const&) const) &
        //    Prim::ApplyAPI)
        // .m((bool(Prim::*)(Token const&) const) &
        //    Prim::ApplyAPI)
        // .m((bool(Prim::*)(Token const&,
        //                              Token const&) const) &
        //    Prim::ApplyAPI)
        // .m((bool(Prim::*)(Token const&,
        //                              SchemaVersion) const) &
        //    Prim::ApplyAPI)
        // .m((bool(Prim::*)(Token const&,
        //                              SchemaVersion,
        //                              Token const&) const) &
        //    Prim::ApplyAPI)

        // RemoveAPI
        .m((bool(Prim::*)(Type const&) const)
            &Prim::RemoveAPI
        )
        // .m((bool(Prim::*)(Type const&,
        //                              Token const&) const) &
        //    Prim::RemoveAPI)
        // .m((bool(Prim::*)(Token const&) const) &
        //    Prim::RemoveAPI)
        // .m((bool(Prim::*)(Token const&,
        //                              Token const&) const) &
        //    Prim::RemoveAPI)
        // .m((bool(Prim::*)(Token const&,
        //                              SchemaVersion) const) &
        //    Prim::RemoveAPI)
        // .m((bool(Prim::*)(Token const&,
        //                              SchemaVersion,
        //                              Token const&) const) &
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
        .m((Attribute(Prim::*)(Token const&, PXR_NS::SdfValueTypeName const&, bool, PXR_NS::SdfVariability) const)
            &Prim::CreateAttribute
        )
        .m(&Prim::GetAttributes)
        .m(&Prim::GetAuthoredAttributes)
        .m(&Prim::GetAttribute)
        .m(&Prim::HasAttribute)
        // .m(&Prim::FindAllAttributeConnectionPaths)

        // Relationships
        .m((Relationship(Prim::*)(Token const&, bool) const)
            &Prim::CreateRelationship
        )
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

    // end Prim

    bbl::Class<PXR_NS::UsdPrimDefinition>("PrimDefinition");

    bbl::Class<PXR_NS::Usd_PrimFlagsPredicate>("PrimFlagsPredicate");

    bbl::Class<PXR_NS::UsdPrimRange>("PrimRange")
        .opaque_ptr()
        .ctor(bbl::Ctor<PrimRange, Prim>("start"), "from_prim")
        .m(&PrimRange::begin)
        .m(&PrimRange::end);

    bbl::Class<PXR_NS::UsdPrimRange::iterator>("PrimRangeIterator")
        .opaque_ptr()
        .ctor(bbl::Ctor<PrimRangeIterator>(), "new")
        .m(&PrimRangeIterator::operator++, "op_inc")
        .m((PrimRangeIterator&(PrimRangeIterator::*)(PrimRangeIterator const&))&PrimRangeIterator::operator=, "op_assign")
        .m((bool(PrimRangeIterator::*)(PrimRangeIterator const&) const) &
               PrimRangeIterator::operator==,
           "op_eq")
        .m(&PrimRangeIterator::operator*, "deref");

    bbl::Class<PXR_NS::UsdPrimSiblingRange>("PrimSiblingRange")
        .m(&PXR_NS::UsdPrimSiblingRange::begin)
        .m(&PXR_NS::UsdPrimSiblingRange::end);

    bbl::Class<PXR_NS::UsdPrimSiblingIterator>("PrimSiblingIterator")
        .m(&PXR_NS::UsdPrimSiblingIterator::operator*, "deref")
        .m(&PXR_NS::UsdPrimSiblingIterator::operator++, "op_inc");

    bbl::Class<PXR_NS::UsdPrimSubtreeRange>("PrimSubtreeRange");

    bbl::Class<PXR_NS::UsdPrimTypeInfo>("PrimTypeInfo");

    bbl::Class<std::vector<PXR_NS::UsdPrim>>("PrimVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::UsdPrim)
        ;

    bbl::Class<PXR_NS::UsdProperty>("Property");

    bbl::Class<PXR_NS::UsdPrim::PropertyPredicateFunc>("PropertyPredicateFunc");

    bbl::Class<std::vector<PXR_NS::UsdProperty>>("PropertyVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::UsdProperty);

    bbl::Class<PXR_NS::UsdReferences>("References");

    bbl::Class<PXR_NS::UsdRelationship>("Relationship").opaque_ptr();

    bbl::Class<std::vector<PXR_NS::UsdRelationship>>("RelationshipVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::UsdRelationship);

    bbl::Class<PXR_NS::UsdResolveTarget>("ResolveTarget");

    bbl::Enum<PXR_NS::UsdSchemaRegistry::VersionPolicy>(
        "SchemaRegistryVersionPolicy");

    bbl::Class<PXR_NS::UsdSpecializes>("Specializes");

    bbl::Class<PXR_NS::UsdVariantSet>("VariantSet");

    bbl::Class<PXR_NS::UsdVariantSets>("VariantSets");


    // bbl::Class<std::function<bool(PXR_NS::UsdAttribute const&)>>(
    //     "FindAttributePredicate");
    // bbl::Class<std::function<bool(PXR_NS::UsdRelationship const&)>>(
    //     "FindRelationshipPredicate");

    bbl::Class<PXR_NS::UsdStage>("Stage")
        // Layer Serialization
        .m(&Stage::Save)
        .m(&Stage::SaveSessionLayers)

        // Working Set Management
        .m(&Stage::Load)
        .m(&Stage::Unload)
        .m(&Stage::LoadAndUnload)
        .m(&Stage::GetLoadSet)
        .m(&Stage::FindLoadable)
        .m(&Stage::GetLoadRules)
        .m(&Stage::SetLoadRules)
        .m(&Stage::GetPopulationMask)
        .m(&Stage::SetPopulationMask)
        // .m(&Stage::ExpandPopulationMask) //< need std::function

        // Layers and Edit Targets
        .m(&Stage::GetSessionLayer)
        .m(&Stage::GetRootLayer)
        .m(&Stage::GetPathResolverContext)
        .m(&Stage::ResolveIdentifierToEditTarget)
        .m(&Stage::GetLayerStack)
        .m(&Stage::GetUsedLayers)
        .m(&Stage::HasLocalLayer)
        .m(&Stage::GetEditTarget)
        .m((EditTarget(Stage::*)(LayerHandle const&))
            &Stage::GetEditTargetForLocalLayer
        )
        .m((EditTarget(Stage::*)(size_t))
            &Stage::GetEditTargetForLocalLayer, "GetEditTargetForLocalLayer_with_index"
        )
        .m(&Stage::SetEditTarget)
        .m(&Stage::MuteLayer)
        .m(&Stage::UnmuteLayer)
        .m(&Stage::MuteAndUnmuteLayers)
        .m(&Stage::GetMutedLayers)
        .m(&Stage::IsLayerMuted)

        // Flatten & Export Utilities
        .m(&Stage::Export)
        .m(&Stage::ExportToString)
        .m(&Stage::Flatten)

        // Stage Metadata
        .m((bool (Stage::*)(Token const&, Value*) const)
            &Stage::GetMetadata
        )
        .m(&Stage::HasMetadata)
        .m(&Stage::HasAuthoredMetadata)
        .m((bool (Stage::*)(Token const&, Value const&) const)
            &Stage::SetMetadata
        )
        .m(&Stage::ClearMetadata)
        .m((bool (Stage::*)(Token const&, Token const&, Value*) const)
            &Stage::GetMetadataByDictKey
        )
        .m(&Stage::HasMetadataDictKey)
        .m(&Stage::HasAuthoredMetadataDictKey)
        .m((bool (Stage::*)(Token const&, Token const&, Value const&) const)
            &Stage::SetMetadataByDictKey
        )
        .m(&Stage::ClearMetadataByDictKey)
        .m(&Stage::WriteFallbackPrimTypes)

        // TimeCode API
        .m(&Stage::GetStartTimeCode)
        .m(&Stage::SetStartTimeCode)
        .m(&Stage::GetEndTimeCode)
        .m(&Stage::SetEndTimeCode)
        .m(&Stage::HasAuthoredTimeCodeRange)
        .m(&Stage::GetTimeCodesPerSecond)
        .m(&Stage::SetTimeCodesPerSecond)
        .m(&Stage::GetFramesPerSecond)
        .m(&Stage::SetFramesPerSecond)

        // Attribute Value Interpolation
        .m(&Stage::SetInterpolationType)
        .m(&Stage::GetInterpolationType)

        // Instancing
        .m(&Stage::GetPrototypes)

        // Variant Management
        .m(&Stage::GetGlobalVariantFallbacks)
        .m(&Stage::SetGlobalVariantFallbacks)

        // Lifetime Management
        .m((StageRefPtr(*)(std::string const&, StageInitialLoadSet))
            &Stage::CreateNew
        )
        .m((StageRefPtr(*)(std::string const&, LayerHandle const&, StageInitialLoadSet))
            &Stage::CreateNew, "CreateNew_with_session_layer"
        )
        .m((StageRefPtr(*)(std::string const&, LayerHandle const&, ResolverContext const&, StageInitialLoadSet))
            &Stage::CreateNew, "CreateNew_with_session_layer_and_resolver_context"
        )
        .m((StageRefPtr(*)(std::string const&, ResolverContext const&, StageInitialLoadSet))
            &Stage::CreateNew, "CreateNew_with_resolver_context"
        )
        .m((StageRefPtr(*)(StageInitialLoadSet))
            &Stage::CreateInMemory
        )
        .m((StageRefPtr(*)(std::string const&, StageInitialLoadSet))
            &Stage::CreateInMemory, "CreateInMemory_with_identifier"
        )
        .m((StageRefPtr(*)(std::string const&, ResolverContext const&, StageInitialLoadSet))
            &Stage::CreateInMemory, "CreateInMemory_with_resolver_context"
        )
        .m((StageRefPtr(*)(std::string const&, LayerHandle const&, StageInitialLoadSet))
            &Stage::CreateInMemory, "CreateInMemory_with_session_layer"
        )
        .m((StageRefPtr(*)(std::string const&, LayerHandle const&, ResolverContext const&, StageInitialLoadSet))
            &Stage::CreateInMemory, "CreateInMemory_with_session_layer_and_resolver_context"
        )
        .m((StageRefPtr(*)(std::string const&, Stage::InitialLoadSet))
            &Stage::Open
        )
        .m((StageRefPtr(*)(std::string const&, ResolverContext const&, Stage::InitialLoadSet))
            &Stage::Open, "Open_with_resolver_context"
        )
        .m((StageRefPtr(*)(LayerHandle const&, Stage::InitialLoadSet))
            &Stage::Open, "Open_at_root"
        )
        .m((StageRefPtr(*)(LayerHandle const&, ResolverContext const&, Stage::InitialLoadSet))
            &Stage::Open, "Open_at_root_with_resolver_context"
        )
        .m((StageRefPtr(*)(LayerHandle const&, LayerHandle const&, ResolverContext const&, Stage::InitialLoadSet))
            &Stage::Open, "Open_at_root_with_session_layer_and_resolver_context"
        )
        .m((StageRefPtr(*)(LayerHandle const&, LayerHandle const&, Stage::InitialLoadSet))
            &Stage::Open, "Open_at_root_with_session_layer"
        )
        .m((StageRefPtr(*)(std::string const&, StagePopulationMask const&, Stage::InitialLoadSet))
            &Stage::OpenMasked
        )
        .m((StageRefPtr(*)(std::string const&, ResolverContext const&, StagePopulationMask const&, Stage::InitialLoadSet))
            &Stage::OpenMasked, "OpenMasked_with_resolver_context"
        )
        .m((StageRefPtr(*)(LayerHandle const&, StagePopulationMask const&, Stage::InitialLoadSet))
            &Stage::OpenMasked, "OpenMasked_at_root"
        )
        .m((StageRefPtr(*)(LayerHandle const&, ResolverContext const&, StagePopulationMask const&, Stage::InitialLoadSet))
            &Stage::OpenMasked, "OpenMasked_at_root_with_resolver_context"
        )
        .m((StageRefPtr(*)(LayerHandle const&, LayerHandle const&, ResolverContext const&, StagePopulationMask const&, Stage::InitialLoadSet))
            &Stage::OpenMasked, "OpenMasked_at_root_with_session_layer_and_resolver_context"
        )
        .m((StageRefPtr(*)(LayerHandle const&, LayerHandle const&, StagePopulationMask const&, Stage::InitialLoadSet))
            &Stage::OpenMasked, "OpenMasked_at_root_with_session_layer"
        )
        .m(&Stage::IsSupportedFile)
        .m(&Stage::Reload)

        // Color Configuration API
        .m(&Stage::GetColorConfigFallbacks)
        .m(&Stage::SetColorConfigFallbacks)
        .m(&Stage::SetColorConfiguration)
        .m(&Stage::GetColorConfiguration)
        .m(&Stage::SetColorManagementSystem)
        .m(&Stage::GetColorManagementSystem)

        // Prim Access, Creation and Mutation
        .m(&Stage::GetPseudoRoot)
        .m(&Stage::GetDefaultPrim)
        .m(&Stage::SetDefaultPrim)
        .m(&Stage::ClearDefaultPrim)
        .m(&Stage::HasDefaultPrim)
        .m(&Stage::GetPrimAtPath)
        .m(&Stage::GetObjectAtPath)
        .m(&Stage::GetPropertyAtPath)
        .m(&Stage::GetAttributeAtPath)
        .m(&Stage::GetRelationshipAtPath)
        // Cannot bind Traverse because it returns a PrimRange by value and PrimRange has no default constructor
        // Could possibly work around this by defining one ourselves that just zeroed the memory...
        .m(&Stage::OverridePrim)
        .m(&Stage::DefinePrim)
        .m(&Stage::CreateClassPrim)
        .m(&Stage::RemovePrim)
        ;

    bbl::Enum<PXR_NS::UsdStage::InitialLoadSet>("StageInitialLoadSet");

    bbl::Class<PXR_NS::UsdStageLoadRules>("StageLoadRules");

    bbl::Class<PXR_NS::UsdStagePopulationMask>("StagePopulationMask");

    bbl::Class<PXR_NS::UsdStageRefPtr>("StageRefPtr")
        .ctor(bbl::Ctor<StageRefPtr>(), "ctor")
        .m(&StageRefPtr::operator->, "get");


    // clang-format off
}

#endif