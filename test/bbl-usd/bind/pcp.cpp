#include <pxr/usd/pcp/node.h>
#if defined(__clang__)

#include "babble"
#include "babble-std"

#include <pxr/usd/pcp/primIndex.h>
#include <pxr/usd/pcp/mapFunction.h>
#include <pxr/usd/pcp/types.h>

BBL_MODULE(pcp) {
    bbl::Enum<PXR_NS::PcpArcType>("ArcType");

    bbl::Class<PXR_NS::PcpMapFunction>("MapFunction")
        .ctor(bbl::Ctor<PXR_NS::PcpMapFunction>(), "default")
        .m(&PXR_NS::PcpMapFunction::IsNull)
        .m(&PXR_NS::PcpMapFunction::IsIdentity)
        .m(&PXR_NS::PcpMapFunction::IsIdentityPathMapping)
        .m(&PXR_NS::PcpMapFunction::HasRootIdentity)
        .m(&PXR_NS::PcpMapFunction::MapSourceToTarget)
        .m(&PXR_NS::PcpMapFunction::MapTargetToSource)
        .m(&PXR_NS::PcpMapFunction::Compose)
        .m(&PXR_NS::PcpMapFunction::ComposeOffset)
        .m(&PXR_NS::PcpMapFunction::GetInverse)
        .m(&PXR_NS::PcpMapFunction::GetSourceToTargetMap)
        .m(&PXR_NS::PcpMapFunction::GetTimeOffset)
        .m(&PXR_NS::PcpMapFunction::GetString)
        .m(&PXR_NS::PcpMapFunction::Hash)
        .m(&PXR_NS::PcpMapFunction::Create)
        .m(&PXR_NS::PcpMapFunction::Identity)
        .m(&PXR_NS::PcpMapFunction::IdentityPathMap)
        ;

    bbl::Class<PXR_NS::PcpNodeRef>("NodeRef");

    bbl::Class<PXR_NS::PcpMapFunction::PathMap>("MapFunctionPathMap")
    ;

    bbl::Class<PXR_NS::PcpPrimIndex>("PrimIndex");

    bbl::Class<PXR_NS::PcpVariantFallbackMap>("VariantFallbackMap")
        ;
}


#endif

