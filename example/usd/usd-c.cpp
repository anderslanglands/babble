#include <pxr/usd/ar/resolverContext.h>
#include <pxr/base/gf/interval.h>
#include <pxr/base/gf/vec2f.h>
#include <pxr/base/gf/vec3f.h>
#include <pxr/base/gf/vec4f.h>
#include <pxr/base/gf/vec2h.h>
#include <pxr/base/gf/vec3h.h>
#include <pxr/base/gf/vec4h.h>
#include <pxr/base/gf/vec2d.h>
#include <pxr/base/gf/vec3d.h>
#include <pxr/base/gf/vec4d.h>
#include <pxr/base/gf/vec2i.h>
#include <pxr/base/gf/vec3i.h>
#include <pxr/base/gf/vec4i.h>
#include <pxr/base/gf/matrix2d.h>
#include <pxr/base/gf/matrix3d.h>
#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/gf/matrix2f.h>
#include <pxr/base/gf/matrix3f.h>
#include <pxr/base/gf/matrix4f.h>
#include <pxr/base/gf/quatd.h>
#include <pxr/base/gf/quatf.h>
#include <pxr/base/gf/quath.h>
#include <pxr/base/gf/bbox3d.h>
#include <pxr/base/gf/range1d.h>
#include <pxr/base/gf/range1f.h>
#include <pxr/base/gf/range2d.h>
#include <pxr/base/gf/range2f.h>
#include <pxr/base/gf/range3d.h>
#include <pxr/base/gf/range3f.h>
#include <pxr/base/gf/camera.h>
#include <pxr/base/gf/declare.h>
#include <pxr/base/gf/frustum.h>
#include <pxr/base/gf/rotation.h>
#include <pxr/base/vt/types.h>
#include <pxr/base/vt/array.h>
#include <stddef.h>
#include <pxr/base/js/value.h>
#include <pxr/base/js/types.h>
#include <pxr/usd/pcp/node.h>
#include <pxr/usd/pcp/primIndex.h>
#include <pxr/usd/pcp/mapFunction.h>
#include <pxr/usd/pcp/types.h>
#include <pxr/base/tf/token.h>
#include <pxr/base/vt/dictionary.h>
#include <pxr/base/vt/value.h>
#include <pxr/usd/sdf/allowed.h>
#include <pxr/usd/sdf/assetPath.h>
#include <pxr/usd/sdf/attributeSpec.h>
#include <pxr/usd/sdf/changeBlock.h>
#include <pxr/usd/sdf/changeList.h>
#include <pxr/usd/sdf/declareHandles.h>
#include <pxr/usd/sdf/fileFormat.h>
#include <pxr/usd/sdf/layer.h>
#include <pxr/usd/sdf/layerOffset.h>
#include <pxr/usd/sdf/layerTree.h>
#include <pxr/usd/sdf/listEditorProxy.h>
#include <pxr/usd/sdf/namespaceEdit.h>
#include <pxr/usd/sdf/notice.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/sdf/pathExpression.h>
#include <pxr/usd/sdf/payload.h>
#include <pxr/usd/sdf/primSpec.h>
#include <pxr/usd/sdf/propertySpec.h>
#include <pxr/usd/sdf/proxyTypes.h>
#include <pxr/usd/sdf/reference.h>
#include <pxr/usd/sdf/relationshipSpec.h>
#include <pxr/usd/sdf/schema.h>
#include <pxr/usd/sdf/spec.h>
#include <pxr/usd/sdf/textFileFormat.h>
#include <pxr/usd/sdf/timeCode.h>
#include <pxr/usd/sdf/types.h>
#include <pxr/usd/sdf/valueTypeName.h>
#include <pxr/usd/sdf/variableExpression.h>
#include <pxr/usd/sdf/variantSetSpec.h>
#include <pxr/usd/sdf/variantSpec.h>
#include <vector>
#include <string>
#include <set>
#include <pxr/base/tf/enum.h>
#include <pxr/base/tf/type.h>
#include <pxr/base/tf/hashmap.h>
#include <pxr/usd/usd/attribute.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/timeCode.h>
#include <pxr/usd/usdGeom/basisCurves.h>
#include <pxr/usd/usdGeom/bboxCache.h>
#include <pxr/usd/usdGeom/boundable.h>
#include <pxr/usd/usdGeom/camera.h>
#include <pxr/usd/usdGeom/capsule.h>
#include <pxr/usd/usdGeom/cone.h>
#include <pxr/usd/usdGeom/constraintTarget.h>
#include <pxr/usd/usdGeom/cube.h>
#include <pxr/usd/usdGeom/curves.h>
#include <pxr/usd/usdGeom/cylinder.h>
#include <pxr/usd/usdGeom/gprim.h>
#include <pxr/usd/usdGeom/hermiteCurves.h>
#include <pxr/usd/usdGeom/imageable.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdGeom/modelAPI.h>
#include <pxr/usd/usdGeom/motionAPI.h>
#include <pxr/usd/usdGeom/nurbsCurves.h>
#include <pxr/usd/usdGeom/nurbsPatch.h>
#include <pxr/usd/usdGeom/plane.h>
#include <pxr/usd/usdGeom/pointBased.h>
#include <pxr/usd/usdGeom/pointInstancer.h>
#include <pxr/usd/usdGeom/points.h>
#include <pxr/usd/usdGeom/primvar.h>
#include <pxr/usd/usdGeom/primvarsAPI.h>
#include <pxr/usd/usdGeom/scope.h>
#include <pxr/usd/usdGeom/sphere.h>
#include <pxr/usd/usdGeom/subset.h>
#include <pxr/usd/usdGeom/visibilityAPI.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/xformCache.h>
#include <pxr/usd/usdGeom/xformCommonAPI.h>
#include <pxr/usd/usdGeom/xformOp.h>
#include <pxr/usd/usdGeom/xformable.h>
#include <pxr/usd/usd/property.h>
#include <pxr/usd/usd/primTypeInfo.h>
#include <pxr/usd/usd/primCompositionQuery.h>
#include <pxr/usd/usd/primDefinition.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usd/schemaRegistry.h>
#include <pxr/usd/usd/clipsAPI.h>
#include <pxr/usd/usd/collectionAPI.h>
#include <pxr/usd/usd/collectionMembershipQuery.h>
#include <pxr/usd/usd/modelAPI.h>
#include <pxr/usd/usd/attributeQuery.h>
#include <pxr/usd/usd/common.h>
#include <pxr/usd/usd/editTarget.h>
#include <pxr/usd/usd/inherits.h>
#include <pxr/usd/usd/interpolation.h>
#include <pxr/usd/usd/object.h>
#include <pxr/usd/usd/payloads.h>
#include <pxr/usd/usd/references.h>
#include <pxr/usd/usd/relationship.h>
#include <pxr/usd/usd/resolveInfo.h>
#include <pxr/usd/usd/resolveTarget.h>
#include <pxr/usd/usd/schemaBase.h>
#include <pxr/usd/usd/specializes.h>
#include <pxr/usd/usd/stageCache.h>
#include <pxr/usd/usd/stagePopulationMask.h>
#include <pxr/usd/usd/variantSets.h>
#include <pxr/usd/usd/stageLoadRules.h>

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>

char const* AssetPath_GetAssetPath(PXR_NS::SdfAssetPath const& path) {
    return path.GetAssetPath().c_str();
}
char const* AssetPath_GetResolvedPath(PXR_NS::SdfAssetPath const& path) {
    return path.GetResolvedPath().c_str();
}
PXR_NS::SdfPath const&
NamespaceEdit_get_current_path(PXR_NS::SdfNamespaceEdit const& edit) {
    return edit.currentPath;
}
PXR_NS::SdfPath const&
NamespaceEdit_get_new_path(PXR_NS::SdfNamespaceEdit const& edit) {
    return edit.newPath;
}
PXR_NS::SdfNamespaceEdit::Index const&
NamespaceEdit_get_index(PXR_NS::SdfNamespaceEdit const& edit) {
    return edit.index;
}
PXR_NS::SdfNamespaceEdit const&
NamespaceEditDetail_get_edit(PXR_NS::SdfNamespaceEditDetail const& detail) {
    return detail.edit;
}
PXR_NS::SdfNamespaceEditDetail::Result
NamespaceEditDetail_get_result(PXR_NS::SdfNamespaceEditDetail const& detail) {
    return detail.result;
}
char const*
NamespaceEditDetail_get_reason(PXR_NS::SdfNamespaceEditDetail const& detail) {
    return detail.reason.c_str();
}
PXR_NS::SdfPathExpression*
PathExpression_from_string(char const* expr, char const* parseContext) {
    return new PXR_NS::SdfPathExpression(expr, parseContext);
}
PXR_NS::SdfPayload* Payload_from_asset_path(char const* assetPath) {
    return new PXR_NS::SdfPayload(assetPath);
}
PXR_NS::SdfPayload*
Payload_from_asset_path_and_prim_path(char const* assetPath,
                                      PXR_NS::SdfPath const& primPath) {
    return new PXR_NS::SdfPayload(assetPath, primPath);
}
PXR_NS::SdfPayload* Payload_from_asset_path_and_prim_path_and_layer_offset(
    char const* assetPath, PXR_NS::SdfPath const& primPath,
    PXR_NS::SdfLayerOffset const& layerOffset) {
    return new PXR_NS::SdfPayload(assetPath, primPath, layerOffset);
}
PXR_NS::SdfReference* Reference_from_asset_path(char const* assetPath) {
    return new PXR_NS::SdfReference(assetPath);
}
PXR_NS::SdfReference*
Reference_from_asset_path_and_prim_path(char const* assetPath,
                                        PXR_NS::SdfPath const& primPath) {
    return new PXR_NS::SdfReference(assetPath, primPath);
}
PXR_NS::SdfReference* Reference_from_asset_path_and_prim_path_and_layer_offset(
    char const* assetPath, PXR_NS::SdfPath const& primPath,
    PXR_NS::SdfLayerOffset const& layerOffset) {
    return new PXR_NS::SdfReference(assetPath, primPath, layerOffset);
}
PXR_NS::SdfReference*
Reference_from_asset_path_and_prim_path_and_layer_offset_and_custom_data(
    char const* assetPath, PXR_NS::SdfPath const& primPath,
    PXR_NS::SdfLayerOffset const& layerOffset,
    PXR_NS::VtDictionary const& customData) {
    return new PXR_NS::SdfReference(assetPath, primPath, layerOffset,
                                    customData);
}
char const* ValueTypeName_GetCPPTypeName(PXR_NS::SdfValueTypeName const& vtn) {
    return vtn.GetCPPTypeName().c_str();
}
auto UsdPrim_GetProperties(PXR_NS::UsdPrim prim) -> std::vector<PXR_NS::UsdProperty> {
    return prim.GetProperties();
}

extern "C" {
using gf_CameraProjection = int;
using gf_CameraFOVDirection = int;
using gf_FrustumProjectionType = int;
using pcp_ArcType = int;
using sdf_ChangleListSubLayerChangeType = int;
using sdf_NamespaceEditDetailResult = int;
using sdf_Permission = int;
using sdf_Specifier = int;
using sdf_SpecType = int;
using sdf_Variability = int;
using usdGeom_ProtoXformInclusion = int;
using usdGeom_MaskApplication = int;
using usdGeom_XformCommonAPIRotationOrder = int;
using usdGeom_XformCommonAPIOpFlags = int;
using usdGeom_XformOpType = int;
using usdGeom_XformOpPrecision = int;
using usd_PrimCompositionQueryArcTypeFilter = int;
using usd_PrimCompositionQueryArcIntroducedFilter = int;
using usd_PrimCompositionQueryDependencyTypeFilter = int;
using usd_PrimCompositionQueryHasSpecsFilter = int;
using usd_ModelAPIKindValidation = int;
using usd_InterpolationType = int;
using usd_ListPosition = int;
using usd_LoadPolicy = int;
using usd_ResolveInfoSource = int;
using usd_SchemaRegistryVersionPolicy = int;
using usd_SchemaKind = int;
using usd_StageInitialLoadSet = int;
using usd_StageLoadRulesRule = int;

using ar_ResolverContext_t = PXR_NS::ArResolverContext;
using gf_Vec2f_t = PXR_NS::GfVec2f;
struct BBL_ALIGN(4) gf_Vec2f_t_bbl_size_check {
    float x;
    float y;
};
static_assert(sizeof(gf_Vec2f_t_bbl_size_check) == sizeof(PXR_NS::GfVec2f), "size of value type does not match");
static_assert(alignof(gf_Vec2f_t_bbl_size_check) == alignof(PXR_NS::GfVec2f), "align of value type does not match");

using gf_Vec2fArray_t = PXR_NS::VtArray<PXR_NS::GfVec2f>;
using gf_Vec3f_t = PXR_NS::GfVec3f;
struct BBL_ALIGN(4) gf_Vec3f_t_bbl_size_check {
    float x;
    float y;
    float z;
};
static_assert(sizeof(gf_Vec3f_t_bbl_size_check) == sizeof(PXR_NS::GfVec3f), "size of value type does not match");
static_assert(alignof(gf_Vec3f_t_bbl_size_check) == alignof(PXR_NS::GfVec3f), "align of value type does not match");

using gf_Vec3fArray_t = PXR_NS::VtArray<PXR_NS::GfVec3f>;
using gf_Vec3fArrayVector_t = std::vector<PXR_NS::VtVec3fArray>;
using gf_Vec4f_t = PXR_NS::GfVec4f;
struct BBL_ALIGN(4) gf_Vec4f_t_bbl_size_check {
    float x;
    float y;
    float z;
    float w;
};
static_assert(sizeof(gf_Vec4f_t_bbl_size_check) == sizeof(PXR_NS::GfVec4f), "size of value type does not match");
static_assert(alignof(gf_Vec4f_t_bbl_size_check) == alignof(PXR_NS::GfVec4f), "align of value type does not match");

using gf_Vec4fArray_t = PXR_NS::VtArray<PXR_NS::GfVec4f>;
using gf_Vec4fVector_t = std::vector<PXR_NS::GfVec4f>;
using gf_Vec2h_t = PXR_NS::GfVec2h;
struct BBL_ALIGN(2) gf_Vec2h_t_bbl_size_check {
    short x;
    short y;
};
static_assert(sizeof(gf_Vec2h_t_bbl_size_check) == sizeof(PXR_NS::GfVec2h), "size of value type does not match");
static_assert(alignof(gf_Vec2h_t_bbl_size_check) == alignof(PXR_NS::GfVec2h), "align of value type does not match");

using gf_Vec2hArray_t = PXR_NS::VtArray<PXR_NS::GfVec2h>;
using gf_Vec3h_t = PXR_NS::GfVec3h;
struct BBL_ALIGN(2) gf_Vec3h_t_bbl_size_check {
    short x;
    short y;
    short z;
};
static_assert(sizeof(gf_Vec3h_t_bbl_size_check) == sizeof(PXR_NS::GfVec3h), "size of value type does not match");
static_assert(alignof(gf_Vec3h_t_bbl_size_check) == alignof(PXR_NS::GfVec3h), "align of value type does not match");

using gf_Vec3hArray_t = PXR_NS::VtArray<PXR_NS::GfVec3h>;
using gf_Vec4h_t = PXR_NS::GfVec4h;
struct BBL_ALIGN(2) gf_Vec4h_t_bbl_size_check {
    short x;
    short y;
    short z;
    short w;
};
static_assert(sizeof(gf_Vec4h_t_bbl_size_check) == sizeof(PXR_NS::GfVec4h), "size of value type does not match");
static_assert(alignof(gf_Vec4h_t_bbl_size_check) == alignof(PXR_NS::GfVec4h), "align of value type does not match");

using gf_Vec4hArray_t = PXR_NS::VtArray<PXR_NS::GfVec4h>;
using gf_Vec2d_t = PXR_NS::GfVec2d;
struct BBL_ALIGN(8) gf_Vec2d_t_bbl_size_check {
    double x;
    double y;
};
static_assert(sizeof(gf_Vec2d_t_bbl_size_check) == sizeof(PXR_NS::GfVec2d), "size of value type does not match");
static_assert(alignof(gf_Vec2d_t_bbl_size_check) == alignof(PXR_NS::GfVec2d), "align of value type does not match");

using gf_Vec2dArray_t = PXR_NS::VtArray<PXR_NS::GfVec2d>;
using gf_Vec3d_t = PXR_NS::GfVec3d;
struct BBL_ALIGN(8) gf_Vec3d_t_bbl_size_check {
    double x;
    double y;
    double z;
};
static_assert(sizeof(gf_Vec3d_t_bbl_size_check) == sizeof(PXR_NS::GfVec3d), "size of value type does not match");
static_assert(alignof(gf_Vec3d_t_bbl_size_check) == alignof(PXR_NS::GfVec3d), "align of value type does not match");

using gf_Vec3dVector_t = std::vector<PXR_NS::GfVec3d>;
using gf_Vec3dArray_t = PXR_NS::VtArray<PXR_NS::GfVec3d>;
using gf_Vec4d_t = PXR_NS::GfVec4d;
struct BBL_ALIGN(8) gf_Vec4d_t_bbl_size_check {
    double x;
    double y;
    double z;
    double w;
};
static_assert(sizeof(gf_Vec4d_t_bbl_size_check) == sizeof(PXR_NS::GfVec4d), "size of value type does not match");
static_assert(alignof(gf_Vec4d_t_bbl_size_check) == alignof(PXR_NS::GfVec4d), "align of value type does not match");

using gf_Vec4dArray_t = PXR_NS::VtArray<PXR_NS::GfVec4d>;
using gf_Vec2i_t = PXR_NS::GfVec2i;
struct BBL_ALIGN(4) gf_Vec2i_t_bbl_size_check {
    int x;
    int y;
};
static_assert(sizeof(gf_Vec2i_t_bbl_size_check) == sizeof(PXR_NS::GfVec2i), "size of value type does not match");
static_assert(alignof(gf_Vec2i_t_bbl_size_check) == alignof(PXR_NS::GfVec2i), "align of value type does not match");

using gf_Vec2iArray_t = PXR_NS::VtArray<PXR_NS::GfVec2i>;
using gf_Vec3i_t = PXR_NS::GfVec3i;
struct BBL_ALIGN(4) gf_Vec3i_t_bbl_size_check {
    int x;
    int y;
    int z;
};
static_assert(sizeof(gf_Vec3i_t_bbl_size_check) == sizeof(PXR_NS::GfVec3i), "size of value type does not match");
static_assert(alignof(gf_Vec3i_t_bbl_size_check) == alignof(PXR_NS::GfVec3i), "align of value type does not match");

using gf_Vec3iArray_t = PXR_NS::VtArray<PXR_NS::GfVec3i>;
using gf_Vec4i_t = PXR_NS::GfVec4i;
struct BBL_ALIGN(4) gf_Vec4i_t_bbl_size_check {
    int x;
    int y;
    int z;
    int w;
};
static_assert(sizeof(gf_Vec4i_t_bbl_size_check) == sizeof(PXR_NS::GfVec4i), "size of value type does not match");
static_assert(alignof(gf_Vec4i_t_bbl_size_check) == alignof(PXR_NS::GfVec4i), "align of value type does not match");

using gf_Vec4iArray_t = PXR_NS::VtArray<PXR_NS::GfVec4i>;
using gf_Matrix2d_t = PXR_NS::GfMatrix2d;
struct BBL_ALIGN(8) gf_Matrix2d_t_bbl_size_check {
    double m[4];
};
static_assert(sizeof(gf_Matrix2d_t_bbl_size_check) == sizeof(PXR_NS::GfMatrix2d), "size of value type does not match");
static_assert(alignof(gf_Matrix2d_t_bbl_size_check) == alignof(PXR_NS::GfMatrix2d), "align of value type does not match");

using gf_Matrix2dArray_t = PXR_NS::VtArray<PXR_NS::GfMatrix2d>;
using gf_Matrix3d_t = PXR_NS::GfMatrix3d;
struct BBL_ALIGN(8) gf_Matrix3d_t_bbl_size_check {
    double m[9];
};
static_assert(sizeof(gf_Matrix3d_t_bbl_size_check) == sizeof(PXR_NS::GfMatrix3d), "size of value type does not match");
static_assert(alignof(gf_Matrix3d_t_bbl_size_check) == alignof(PXR_NS::GfMatrix3d), "align of value type does not match");

using gf_Matrix3dArray_t = PXR_NS::VtArray<PXR_NS::GfMatrix3d>;
using gf_Matrix4d_t = PXR_NS::GfMatrix4d;
struct BBL_ALIGN(8) gf_Matrix4d_t_bbl_size_check {
    double m[16];
};
static_assert(sizeof(gf_Matrix4d_t_bbl_size_check) == sizeof(PXR_NS::GfMatrix4d), "size of value type does not match");
static_assert(alignof(gf_Matrix4d_t_bbl_size_check) == alignof(PXR_NS::GfMatrix4d), "align of value type does not match");

using gf_Matrix4dArray_t = PXR_NS::VtArray<PXR_NS::GfMatrix4d>;
using gf_Matrix4dArrayVector_t = std::vector<PXR_NS::VtMatrix4dArray>;
using gf_Matrix2f_t = PXR_NS::GfMatrix2f;
struct BBL_ALIGN(4) gf_Matrix2f_t_bbl_size_check {
    float m[4];
};
static_assert(sizeof(gf_Matrix2f_t_bbl_size_check) == sizeof(PXR_NS::GfMatrix2f), "size of value type does not match");
static_assert(alignof(gf_Matrix2f_t_bbl_size_check) == alignof(PXR_NS::GfMatrix2f), "align of value type does not match");

using gf_Matrix2fArray_t = PXR_NS::VtArray<PXR_NS::GfMatrix2f>;
using gf_Matrix3f_t = PXR_NS::GfMatrix3f;
struct BBL_ALIGN(4) gf_Matrix3f_t_bbl_size_check {
    float m[9];
};
static_assert(sizeof(gf_Matrix3f_t_bbl_size_check) == sizeof(PXR_NS::GfMatrix3f), "size of value type does not match");
static_assert(alignof(gf_Matrix3f_t_bbl_size_check) == alignof(PXR_NS::GfMatrix3f), "align of value type does not match");

using gf_Matrix3fArray_t = PXR_NS::VtArray<PXR_NS::GfMatrix3f>;
using gf_Matrix4f_t = PXR_NS::GfMatrix4f;
struct BBL_ALIGN(4) gf_Matrix4f_t_bbl_size_check {
    float m[16];
};
static_assert(sizeof(gf_Matrix4f_t_bbl_size_check) == sizeof(PXR_NS::GfMatrix4f), "size of value type does not match");
static_assert(alignof(gf_Matrix4f_t_bbl_size_check) == alignof(PXR_NS::GfMatrix4f), "align of value type does not match");

using gf_Matrix4fArray_t = PXR_NS::VtArray<PXR_NS::GfMatrix4f>;
using gf_Quatd_t = PXR_NS::GfQuatd;
struct BBL_ALIGN(8) gf_Quatd_t_bbl_size_check {
    double x;
    double y;
    double z;
    double w;
};
static_assert(sizeof(gf_Quatd_t_bbl_size_check) == sizeof(PXR_NS::GfQuatd), "size of value type does not match");
static_assert(alignof(gf_Quatd_t_bbl_size_check) == alignof(PXR_NS::GfQuatd), "align of value type does not match");

using gf_QuatdArray_t = PXR_NS::VtArray<PXR_NS::GfQuatd>;
using gf_Quatf_t = PXR_NS::GfQuatf;
struct BBL_ALIGN(4) gf_Quatf_t_bbl_size_check {
    float x;
    float y;
    float z;
    float w;
};
static_assert(sizeof(gf_Quatf_t_bbl_size_check) == sizeof(PXR_NS::GfQuatf), "size of value type does not match");
static_assert(alignof(gf_Quatf_t_bbl_size_check) == alignof(PXR_NS::GfQuatf), "align of value type does not match");

using gf_QuatfArray_t = PXR_NS::VtArray<PXR_NS::GfQuatf>;
using gf_Quath_t = PXR_NS::GfQuath;
struct BBL_ALIGN(2) gf_Quath_t_bbl_size_check {
    short x;
    short y;
    short z;
    short w;
};
static_assert(sizeof(gf_Quath_t_bbl_size_check) == sizeof(PXR_NS::GfQuath), "size of value type does not match");
static_assert(alignof(gf_Quath_t_bbl_size_check) == alignof(PXR_NS::GfQuath), "align of value type does not match");

using gf_QuathArray_t = PXR_NS::VtArray<PXR_NS::GfQuath>;
using gf_Interval_t = PXR_NS::GfInterval;
static_assert(sizeof(PXR_NS::GfInterval) == 32, "size of PXR_NS::GfInterval and gf_Interval_t do not match");
static_assert(alignof(PXR_NS::GfInterval) == 8, "align of PXR_NS::GfInterval and gf_Interval_t do not match");

using gf_BBox3d_t = PXR_NS::GfBBox3d;
using gf_Range1d_t = PXR_NS::GfRange1d;
struct BBL_ALIGN(8) gf_Range1d_t_bbl_size_check {
    double mn;
    double mx;
};
static_assert(sizeof(gf_Range1d_t_bbl_size_check) == sizeof(PXR_NS::GfRange1d), "size of value type does not match");
static_assert(alignof(gf_Range1d_t_bbl_size_check) == alignof(PXR_NS::GfRange1d), "align of value type does not match");

using gf_Range1f_t = PXR_NS::GfRange1f;
struct BBL_ALIGN(4) gf_Range1f_t_bbl_size_check {
    float mn;
    float mx;
};
static_assert(sizeof(gf_Range1f_t_bbl_size_check) == sizeof(PXR_NS::GfRange1f), "size of value type does not match");
static_assert(alignof(gf_Range1f_t_bbl_size_check) == alignof(PXR_NS::GfRange1f), "align of value type does not match");

using gf_Range2d_t = PXR_NS::GfRange2d;
struct BBL_ALIGN(8) gf_Range2d_t_bbl_size_check {
    gf_Vec2d_t mn;
    gf_Vec2d_t mx;
};
static_assert(sizeof(gf_Range2d_t_bbl_size_check) == sizeof(PXR_NS::GfRange2d), "size of value type does not match");
static_assert(alignof(gf_Range2d_t_bbl_size_check) == alignof(PXR_NS::GfRange2d), "align of value type does not match");

using gf_Range2f_t = PXR_NS::GfRange2f;
struct BBL_ALIGN(4) gf_Range2f_t_bbl_size_check {
    gf_Vec2f_t mn;
    gf_Vec2f_t mx;
};
static_assert(sizeof(gf_Range2f_t_bbl_size_check) == sizeof(PXR_NS::GfRange2f), "size of value type does not match");
static_assert(alignof(gf_Range2f_t_bbl_size_check) == alignof(PXR_NS::GfRange2f), "align of value type does not match");

using gf_Range3d_t = PXR_NS::GfRange3d;
struct BBL_ALIGN(8) gf_Range3d_t_bbl_size_check {
    gf_Vec3d_t mn;
    gf_Vec3d_t mx;
};
static_assert(sizeof(gf_Range3d_t_bbl_size_check) == sizeof(PXR_NS::GfRange3d), "size of value type does not match");
static_assert(alignof(gf_Range3d_t_bbl_size_check) == alignof(PXR_NS::GfRange3d), "align of value type does not match");

using gf_Range3f_t = PXR_NS::GfRange3f;
struct BBL_ALIGN(4) gf_Range3f_t_bbl_size_check {
    gf_Vec3f_t mn;
    gf_Vec3f_t mx;
};
static_assert(sizeof(gf_Range3f_t_bbl_size_check) == sizeof(PXR_NS::GfRange3f), "size of value type does not match");
static_assert(alignof(gf_Range3f_t_bbl_size_check) == alignof(PXR_NS::GfRange3f), "align of value type does not match");

using gf_Camera_t = PXR_NS::GfCamera;
using gf_Frustum_t = PXR_NS::GfFrustum;
using gf_Ray_t = PXR_NS::GfRay;
struct BBL_ALIGN(8) gf_Ray_t_bbl_size_check {
    gf_Vec3d_t startPoint;
    gf_Vec3d_t direction;
};
static_assert(sizeof(gf_Ray_t_bbl_size_check) == sizeof(PXR_NS::GfRay), "size of value type does not match");
static_assert(alignof(gf_Ray_t_bbl_size_check) == alignof(PXR_NS::GfRay), "align of value type does not match");

using gf_Rotation_t = PXR_NS::GfRotation;
struct BBL_ALIGN(8) gf_Rotation_t_bbl_size_check {
    gf_Vec3d_t axis;
    double angle;
};
static_assert(sizeof(gf_Rotation_t_bbl_size_check) == sizeof(PXR_NS::GfRotation), "size of value type does not match");
static_assert(alignof(gf_Rotation_t_bbl_size_check) == alignof(PXR_NS::GfRotation), "align of value type does not match");

using js_Array_t = PXR_NS::JsArray;
using js_Object_t = PXR_NS::JsObject;
using js_Value_t = PXR_NS::JsValue;
using pcp_MapFunction_t = PXR_NS::PcpMapFunction;
using pcp_NodeRef_t = PXR_NS::PcpNodeRef;
using pcp_MapFunctionPathMap_t = PXR_NS::PcpMapFunction::PathMap;
using pcp_PrimIndex_t = PXR_NS::PcpPrimIndex;
using pcp_VariantFallbackMap_t = PXR_NS::PcpVariantFallbackMap;
using sdf_Allowed_t = PXR_NS::SdfAllowed;
using sdf_AssetPath_t = PXR_NS::SdfAssetPath;
using sdf_AssetPathArray_t = PXR_NS::VtArray<PXR_NS::SdfAssetPath>;
using sdf_AttributeSpec_t = PXR_NS::SdfAttributeSpec;
using sdf_AttributeSpecHandle_t = PXR_NS::SdfAttributeSpecHandle;
using sdf_AttributeSpecHandleVector_t = PXR_NS::SdfAttributeSpecHandleVector;
using sdf_AttributeSpecView_t = PXR_NS::SdfAttributeSpecView;
using sdf_BatchNamespaceEdit_t = PXR_NS::SdfBatchNamespaceEdit;
using sdf_ChangeBlock_t = PXR_NS::SdfChangeBlock;
using sdf_ChangeList_t = PXR_NS::SdfChangeList;
using sdf_ChangeListEntry_t = PXR_NS::SdfChangeList::Entry;
using sdf_PathChangeListEntryPair_t = std::pair<PXR_NS::SdfPath, PXR_NS::SdfChangeList::Entry>;
using sdf_ChangeListEntryList_t = PXR_NS::SdfChangeList::EntryList;
using sdf_DictionaryProxy_t = PXR_NS::SdfDictionaryProxy;
using sdf_FileFormat_t = PXR_NS::SdfFileFormat;
using sdf_InheritsProxy_t = PXR_NS::SdfInheritsProxy;
using sdf_LayerFileFormatArguments_t = PXR_NS::SdfLayer::FileFormatArguments;
using sdf_Layer_t = PXR_NS::SdfLayer;
using sdf_LayerHandle_t = PXR_NS::SdfLayerHandle;
using sdf_LayerHandleVector_t = PXR_NS::SdfLayerHandleVector;
using sdf_LayerOffset_t = PXR_NS::SdfLayerOffset;
using sdf_LayerOffsetVector_t = PXR_NS::SdfLayerOffsetVector;
using sdf_LayerRefPtr_t = PXR_NS::SdfLayerRefPtr;
using sdf_LayerTree_t = PXR_NS::SdfLayerTree;
using sdf_LayerTreeHandle_t = PXR_NS::SdfLayerTreeHandle;
using sdf_LayerTreeHandleVector_t = PXR_NS::SdfLayerTreeHandleVector;
using sdf_StringListOp_t = PXR_NS::SdfListOp<std::string>;
using sdf_NameChildrenOrderProxy_t = PXR_NS::SdfNameChildrenOrderProxy;
using sdf_NameEditorProxy_t = PXR_NS::SdfNameEditorProxy;
using sdf_NamespaceEdit_t = PXR_NS::SdfNamespaceEdit;
using sdf_NamespaceEditVector_t = std::vector<PXR_NS::SdfNamespaceEdit>;
using sdf_NamespaceEditDetail_t = PXR_NS::SdfNamespaceEditDetail;
using sdf_NamespaceEditDetailVector_t = PXR_NS::SdfNamespaceEditDetailVector;
using sdf_NoticeBase_t = PXR_NS::SdfNotice::Base;
using sdf_NoticeBaseLayersDidChange_t = PXR_NS::SdfNotice::BaseLayersDidChange;
using sdf_NoticeLayerDidReloadContent_t = PXR_NS::SdfNotice::LayerDidReloadContent;
using sdf_NoticeLayerDidReplaceContent_t = PXR_NS::SdfNotice::LayerDidReplaceContent;
using sdf_NoticeLayerDidSaveLayerToFile_t = PXR_NS::SdfNotice::LayerDidSaveLayerToFile;
using sdf_NoticeLayerIdentifierDidChange_t = PXR_NS::SdfNotice::LayerIdentifierDidChange;
using sdf_NoticeLayerInfoDidChange_t = PXR_NS::SdfNotice::LayerInfoDidChange;
using sdf_NoticeLayerMutenessChanged_t = PXR_NS::SdfNotice::LayerMutenessChanged;
using sdf_NoticeLayersDidChange_t = PXR_NS::SdfNotice::LayersDidChange;
using sdf_NoticeLayersDidChangeSentPerLayer_t = PXR_NS::SdfNotice::LayersDidChangeSentPerLayer;
using sdf_Path_t = PXR_NS::SdfPath;
using sdf_PathExpression_t = PXR_NS::SdfPathExpression;
using sdf_PathSet_t = PXR_NS::SdfPathSet;
using sdf_PathVector_t = PXR_NS::SdfPathVector;
using sdf_Payload_t = PXR_NS::SdfPayload;
using sdf_PayloadEditorProxy_t = PXR_NS::SdfPayloadEditorProxy;
using sdf_PayloadVector_t = PXR_NS::SdfPayloadVector;
using sdf_PrimSpec_t = PXR_NS::SdfPrimSpec;
using sdf_PrimSpecHandle_t = PXR_NS::SdfPrimSpecHandle;
using sdf_PrimSpecHandleVector_t = PXR_NS::SdfPrimSpecHandleVector;
using sdf_PrimSpecHandleOffsetPair_t = std::pair<PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset>;
using sdf_PrimSpecHandleOffsetPairVector_t = std::vector<
        std::pair<PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset>>;
using sdf_PrimSpecView_t = PXR_NS::SdfPrimSpecView;
using sdf_PropertySpec_t = PXR_NS::SdfPropertySpec;
using sdf_PropertySpecHandle_t = PXR_NS::SdfPropertySpecHandle;
using sdf_PropertySpecHandleVector_t = PXR_NS::SdfPropertySpecHandleVector;
using sdf_PropertySpecHandleOffsetPair_t = std::pair<PXR_NS::SdfPropertySpecHandle, PXR_NS::SdfLayerOffset>;
using sdf_PropertySpecHandleOffsetPairVector_t = std::vector<std::pair<PXR_NS::SdfPropertySpecHandle, PXR_NS::SdfLayerOffset>>;
using sdf_PropertySpecView_t = PXR_NS::SdfPropertySpecView;
using sdf_Reference_t = PXR_NS::SdfReference;
using sdf_ReferenceEditorProxy_t = PXR_NS::SdfReferenceEditorProxy;
using sdf_ReferenceVector_t = PXR_NS::SdfReferenceVector;
using sdf_RelationshipSpec_t = PXR_NS::SdfRelationshipSpec;
using sdf_RelationshipSpecHandle_t = PXR_NS::SdfRelationshipSpecHandle;
using sdf_RelationshipSpecHandleVector_t = PXR_NS::SdfRelationshipSpecHandleVector;
using sdf_RelationshipSpecView_t = PXR_NS::SdfRelationshipSpecView;
using sdf_RelationshipSpecHandleOffsetPair_t = 
        std::pair<PXR_NS::SdfRelationshipSpecHandle, PXR_NS::SdfLayerOffset>;
using sdf_RelationshipSpecHandleOffsetPairVector_t = std::vector<
        std::pair<PXR_NS::SdfRelationshipSpecHandle, PXR_NS::SdfLayerOffset>>;
using sdf_RelocatesMap_t = PXR_NS::SdfRelocatesMap;
using sdf_RelocatesMapProxy_t = PXR_NS::SdfRelocatesMapProxy;
using sdf_SchemaBase_t = PXR_NS::SdfSchemaBase;
using sdf_SchemaBaseFieldDefinition_t = PXR_NS::SdfSchemaBase::FieldDefinition;
using sdf_SchemaBaseFieldDefinitionInfoVec_t = PXR_NS::SdfSchemaBase::FieldDefinition::InfoVec;
using sdf_TokenValuePair_t = std::pair<PXR_NS::TfToken, PXR_NS::JsValue>;
using sdf_SchemaBaseSpecDefinition_t = PXR_NS::SdfSchemaBase::SpecDefinition;
using sdf_Spec_t = PXR_NS::SdfSpec;
using sdf_SpecHandle_t = PXR_NS::SdfSpecHandle;
using sdf_SubLayerProxy_t = PXR_NS::SdfSubLayerProxy;
using sdf_TextFileFormat_t = PXR_NS::SdfTextFileFormat;
using sdf_TimeCode_t = PXR_NS::SdfTimeCode;
using sdf_TimeSampleMap_t = PXR_NS::SdfTimeSampleMap;
using sdf_TupleDimensions_t = PXR_NS::SdfTupleDimensions;
struct BBL_ALIGN(8) sdf_TupleDimensions_t_bbl_size_check {
    size_t d[2];
    size_t size;
};
static_assert(sizeof(sdf_TupleDimensions_t_bbl_size_check) == sizeof(PXR_NS::SdfTupleDimensions), "size of value type does not match");
static_assert(alignof(sdf_TupleDimensions_t_bbl_size_check) == alignof(PXR_NS::SdfTupleDimensions), "align of value type does not match");

using sdf_ValueBlock_t = PXR_NS::SdfValueBlock;
using sdf_ValueTypeName_t = PXR_NS::SdfValueTypeName;
using sdf_ValueTypeNameVector_t = std::vector<PXR_NS::SdfValueTypeName>;
using sdf_VariableExpression_t = PXR_NS::SdfVariableExpression;
using sdf_VariableExpressionResult_t = PXR_NS::SdfVariableExpression::Result;
using sdf_VariantSelectionProxy_t = PXR_NS::SdfVariantSelectionProxy;
using sdf_VariantSetSpec_t = PXR_NS::SdfVariantSetSpec;
using sdf_VariantSetsProxy_t = PXR_NS::SdfVariantSetsProxy;
using sdf_VariantSpec_t = PXR_NS::SdfVariantSpec;
using sdf_VariantSetSpecHandle_t = PXR_NS::SdfVariantSetSpecHandle;
using sdf_VariantSpecHandle_t = PXR_NS::SdfVariantSpecHandle;
using sdf_VariantSpecHandleVector_t = PXR_NS::SdfVariantSpecHandleVector;
using sdf_VariantView_t = PXR_NS::SdfVariantView;
using std_String_t = std::string;
using std_StringVector_t = std::vector<std::string>;
using std_StringSet_t = std::set<std::string>;
using std_DoubleVector_t = std::vector<double>;
using std_DoubleSet_t = std::set<double>;
using tf_Enum_t = PXR_NS::TfEnum;
using tf_Token_t = PXR_NS::TfToken;
using tf_Type_t = PXR_NS::TfType;
using tf_TokenVector_t = PXR_NS::TfTokenVector;
using usdGeom_BasisCurves_t = PXR_NS::UsdGeomBasisCurves;
using usdGeom_BBoxCache_t = PXR_NS::UsdGeomBBoxCache;
using usdGeom_PathMatrixHashMap_t = PXR_NS::TfHashMap<PXR_NS::SdfPath, PXR_NS::GfMatrix4d>;
using usdGeom_Boundable_t = PXR_NS::UsdGeomBoundable;
using usdGeom_Camera_t = PXR_NS::UsdGeomCamera;
using usdGeom_Capsule_t = PXR_NS::UsdGeomCapsule;
using usdGeom_Cone_t = PXR_NS::UsdGeomCone;
using usdGeom_ConstraintTarget_t = PXR_NS::UsdGeomConstraintTarget;
using usdGeom_ConstraintTargetVector_t = std::vector<PXR_NS::UsdGeomConstraintTarget>;
using usdGeom_Cube_t = PXR_NS::UsdGeomCube;
using usdGeom_Curves_t = PXR_NS::UsdGeomCurves;
using usdGeom_Cylinder_t = PXR_NS::UsdGeomCylinder;
using usdGeom_Gprim_t = PXR_NS::UsdGeomGprim;
using usdGeom_HermiteCurves_t = PXR_NS::UsdGeomHermiteCurves;
using usdGeom_Imageable_t = PXR_NS::UsdGeomImageable;
using usdGeom_ImageablePurposeInfo_t = PXR_NS::UsdGeomImageable::PurposeInfo;
using usdGeom_Mesh_t = PXR_NS::UsdGeomMesh;
using usdGeom_ModelAPI_t = PXR_NS::UsdGeomModelAPI;
using usdGeom_MotionAPI_t = PXR_NS::UsdGeomMotionAPI;
using usdGeom_NurbsCurves_t = PXR_NS::UsdGeomNurbsCurves;
using usdGeom_NurbsPatch_t = PXR_NS::UsdGeomNurbsPatch;
using usdGeom_Plane_t = PXR_NS::UsdGeomPlane;
using usdGeom_PointBased_t = PXR_NS::UsdGeomPointBased;
using usdGeom_PointInstancer_t = PXR_NS::UsdGeomPointInstancer;
using usdGeom_Points_t = PXR_NS::UsdGeomPoints;
using usdGeom_Primvar_t = PXR_NS::UsdGeomPrimvar;
using usdGeom_PrimvarVector_t = std::vector<PXR_NS::UsdGeomPrimvar>;
using usdGeom_PrimvarsAPI_t = PXR_NS::UsdGeomPrimvarsAPI;
using usdGeom_Scope_t = PXR_NS::UsdGeomScope;
using usdGeom_Sphere_t = PXR_NS::UsdGeomSphere;
using usdGeom_Subset_t = PXR_NS::UsdGeomSubset;
using usdGeom_SubsetVector_t = std::vector<PXR_NS::UsdGeomSubset>;
using usdGeom_VisibilityAPI_t = PXR_NS::UsdGeomVisibilityAPI;
using usdGeom_Xform_t = PXR_NS::UsdGeomXform;
using usdGeom_XformCache_t = PXR_NS::UsdGeomXformCache;
using usdGeom_XformCommonAPI_t = PXR_NS::UsdGeomXformCommonAPI;
using usdGeom_XformCommonAPIOps_t = PXR_NS::UsdGeomXformCommonAPI::Ops;
using usdGeom_Xformable_t = PXR_NS::UsdGeomXformable;
using usdGeom_XformOp_t = PXR_NS::UsdGeomXformOp;
using usdGeom_XformOpVector_t = std::vector<PXR_NS::UsdGeomXformOp>;
using usd_Prim_t = PXR_NS::UsdPrim;
using usd_PrimCompositionQuery_t = PXR_NS::UsdPrimCompositionQuery;
using usd_PrimCompositionQueryArc_t = PXR_NS::UsdPrimCompositionQueryArc;
using usd_PrimCompositionQueryArcVector_t = std::vector<PXR_NS::UsdPrimCompositionQueryArc>;
using usd_PrimCompositionQueryFilter_t = PXR_NS::UsdPrimCompositionQuery::Filter;
struct BBL_ALIGN(4) usd_PrimCompositionQueryFilter_t_bbl_size_check {
    int arcTypeFilter;
    int dependencyTypeFilter;
    int arcIntroducedFilter;
    int hasSpecsFilter;
};
static_assert(sizeof(usd_PrimCompositionQueryFilter_t_bbl_size_check) == sizeof(PXR_NS::UsdPrimCompositionQuery::Filter), "size of value type does not match");
static_assert(alignof(usd_PrimCompositionQueryFilter_t_bbl_size_check) == alignof(PXR_NS::UsdPrimCompositionQuery::Filter), "align of value type does not match");

using usd_PrimDefinition_t = PXR_NS::UsdPrimDefinition;
using usd_PrimFlagsPredicate_t = PXR_NS::Usd_PrimFlagsPredicate;
using usd_PrimRange_t = PXR_NS::UsdPrimRange;
using usd_PrimRangeIterator_t = PXR_NS::UsdPrimRange::iterator;
using usd_PrimSiblingRange_t = PXR_NS::UsdPrimSiblingRange;
using usd_PrimSiblingIterator_t = PXR_NS::UsdPrimSiblingIterator;
using usd_PrimSubtreeRange_t = PXR_NS::UsdPrimSubtreeRange;
using usd_PrimTypeInfo_t = PXR_NS::UsdPrimTypeInfo;
using usd_PrimVector_t = std::vector<PXR_NS::UsdPrim>;
using usd_ClipsAPI_t = PXR_NS::UsdClipsAPI;
using usd_CollectionAPI_t = PXR_NS::UsdCollectionAPI;
using usd_CollectionAPIVector_t = std::vector<PXR_NS::UsdCollectionAPI>;
using usd_CollectionMembershipQuery_t = PXR_NS::UsdCollectionMembershipQuery;
using usd_CollectionMembershipQueryPathExpansionRuleMap_t = PXR_NS::UsdCollectionMembershipQuery::PathExpansionRuleMap;
using usd_ModelAPI_t = PXR_NS::UsdModelAPI;
using usd_Attribute_t = PXR_NS::UsdAttribute;
using usd_AttributeQuery_t = PXR_NS::UsdAttributeQuery;
using usd_AttributeQueryVector_t = std::vector<PXR_NS::UsdAttributeQuery>;
using usd_AttributeVector_t = std::vector<PXR_NS::UsdAttribute>;
using usd_EditTarget_t = PXR_NS::UsdEditTarget;
using usd_Inherits_t = PXR_NS::UsdInherits;
using usd_MetadataValueMap_t = PXR_NS::UsdMetadataValueMap;
using usd_Object_t = PXR_NS::UsdObject;
using usd_ObjectSet_t = std::set<PXR_NS::UsdObject>;
using usd_Payloads_t = PXR_NS::UsdPayloads;
using usd_Property_t = PXR_NS::UsdProperty;
using usd_PropertyVector_t = std::vector<PXR_NS::UsdProperty>;
using usd_References_t = PXR_NS::UsdReferences;
using usd_Relationship_t = PXR_NS::UsdRelationship;
using usd_RelationshipVector_t = std::vector<PXR_NS::UsdRelationship>;
using usd_ResolveInfo_t = PXR_NS::UsdResolveInfo;
using usd_ResolveTarget_t = PXR_NS::UsdResolveTarget;
using usd_Specializes_t = PXR_NS::UsdSpecializes;
using usd_VariantSet_t = PXR_NS::UsdVariantSet;
using usd_VariantSets_t = PXR_NS::UsdVariantSets;
using usd_Stage_t = PXR_NS::UsdStage;
using usd_StageCache_t = PXR_NS::UsdStageCache;
using usd_StageCacheId_t = PXR_NS::UsdStageCache::Id;
static_assert(sizeof(PXR_NS::UsdStageCache::Id) == 4, "size of PXR_NS::UsdStageCache::Id and usd_StageCacheId_t do not match");
static_assert(alignof(PXR_NS::UsdStageCache::Id) == 4, "align of PXR_NS::UsdStageCache::Id and usd_StageCacheId_t do not match");

using usd_StageLoadRules_t = PXR_NS::UsdStageLoadRules;
using usd_PathStageLoadRulesRulePair_t = std::pair<PXR_NS::SdfPath, PXR_NS::UsdStageLoadRules::Rule>;
using usd_PathStageLoadRulesRulePairVector_t = std::vector<std::pair<PXR_NS::SdfPath, PXR_NS::UsdStageLoadRules::Rule>>;
using usd_StagePopulationMask_t = PXR_NS::UsdStagePopulationMask;
using usd_StageRefPtr_t = PXR_NS::UsdStageRefPtr;
using usd_StageRefPtrVector_t = PXR_NS::UsdStageRefPtrVector;
using usd_StageWeakPtr_t = PXR_NS::UsdStageWeakPtr;
using usd_TimeCode_t = PXR_NS::UsdTimeCode;
static_assert(sizeof(PXR_NS::UsdTimeCode) == 8, "size of PXR_NS::UsdTimeCode and usd_TimeCode_t do not match");
static_assert(alignof(PXR_NS::UsdTimeCode) == 8, "align of PXR_NS::UsdTimeCode and usd_TimeCode_t do not match");

using usd_TimeCodeVector_t = std::vector<PXR_NS::UsdTimeCode>;
using vt_Value_t = PXR_NS::VtValue;
using vt_TokenArray_t = PXR_NS::VtTokenArray;
using vt_BoolArray_t = PXR_NS::VtBoolArray;
using vt_IntArray_t = PXR_NS::VtIntArray;
using vt_FloatArray_t = PXR_NS::VtFloatArray;
using vt_DoubleArray_t = PXR_NS::VtDoubleArray;
using vt_Dictionary_t = PXR_NS::VtDictionary;

int ar_ResolverContext_dtor(ar_ResolverContext_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec2fArray_data(gf_Vec2fArray_t* _this, gf_Vec2f_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec2fArray_data_const(gf_Vec2fArray_t const* _this, gf_Vec2f_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec2fArray_size(gf_Vec2fArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec2fArray_op_index(gf_Vec2fArray_t* _this, size_t index, gf_Vec2f_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec2fArray_op_index_const(gf_Vec2fArray_t const* _this, size_t index, gf_Vec2f_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec2fArray_new(gf_Vec2fArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfVec2f>();
    return 0;
}

int gf_Vec2fArray_dtor(gf_Vec2fArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec3fArray_data(gf_Vec3fArray_t* _this, gf_Vec3f_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec3fArray_data_const(gf_Vec3fArray_t const* _this, gf_Vec3f_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec3fArray_size(gf_Vec3fArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec3fArray_op_index(gf_Vec3fArray_t* _this, size_t index, gf_Vec3f_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec3fArray_op_index_const(gf_Vec3fArray_t const* _this, size_t index, gf_Vec3f_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec3fArray_new(gf_Vec3fArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfVec3f>();
    return 0;
}

int gf_Vec3fArray_dtor(gf_Vec3fArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec3fArrayVector_data(gf_Vec3fArrayVector_t* _this, gf_Vec3fArray_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec3fArrayVector_data_const(gf_Vec3fArrayVector_t const* _this, gf_Vec3fArray_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec3fArrayVector_empty(gf_Vec3fArrayVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int gf_Vec3fArrayVector_size(gf_Vec3fArrayVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec3fArrayVector_max_size(gf_Vec3fArrayVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int gf_Vec3fArrayVector_reserve(gf_Vec3fArrayVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int gf_Vec3fArrayVector_capacity(gf_Vec3fArrayVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int gf_Vec3fArrayVector_clear(gf_Vec3fArrayVector_t* _this) {
    _this->clear();
    return 0;
}

int gf_Vec3fArrayVector_push_back(gf_Vec3fArrayVector_t* _this, gf_Vec3fArray_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int gf_Vec3fArrayVector_pop_back(gf_Vec3fArrayVector_t* _this) {
    _this->pop_back();
    return 0;
}

int gf_Vec3fArrayVector_resize(gf_Vec3fArrayVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int gf_Vec3fArrayVector_resize_with(gf_Vec3fArrayVector_t* _this, size_t const _Newsize, gf_Vec3fArray_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int gf_Vec3fArrayVector_op_index(gf_Vec3fArrayVector_t const* _this, size_t const _Pos, gf_Vec3fArray_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int gf_Vec3fArrayVector_default(gf_Vec3fArrayVector_t** _result) {
    *_result = new std::vector<PXR_NS::VtVec3fArray>();
    return 0;
}

int gf_Vec3fArrayVector_dtor(gf_Vec3fArrayVector_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec4fArray_data(gf_Vec4fArray_t* _this, gf_Vec4f_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec4fArray_data_const(gf_Vec4fArray_t const* _this, gf_Vec4f_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec4fArray_size(gf_Vec4fArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec4fArray_op_index(gf_Vec4fArray_t* _this, size_t index, gf_Vec4f_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec4fArray_op_index_const(gf_Vec4fArray_t const* _this, size_t index, gf_Vec4f_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec4fArray_new(gf_Vec4fArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfVec4f>();
    return 0;
}

int gf_Vec4fArray_dtor(gf_Vec4fArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec4fVector_data(gf_Vec4fVector_t* _this, gf_Vec4f_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec4fVector_data_const(gf_Vec4fVector_t const* _this, gf_Vec4f_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec4fVector_empty(gf_Vec4fVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int gf_Vec4fVector_size(gf_Vec4fVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec4fVector_max_size(gf_Vec4fVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int gf_Vec4fVector_reserve(gf_Vec4fVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int gf_Vec4fVector_capacity(gf_Vec4fVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int gf_Vec4fVector_clear(gf_Vec4fVector_t* _this) {
    _this->clear();
    return 0;
}

int gf_Vec4fVector_push_back(gf_Vec4fVector_t* _this, gf_Vec4f_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int gf_Vec4fVector_pop_back(gf_Vec4fVector_t* _this) {
    _this->pop_back();
    return 0;
}

int gf_Vec4fVector_resize(gf_Vec4fVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int gf_Vec4fVector_resize_with(gf_Vec4fVector_t* _this, size_t const _Newsize, gf_Vec4f_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int gf_Vec4fVector_op_index(gf_Vec4fVector_t const* _this, size_t const _Pos, gf_Vec4f_t* _result) {
    *_result = _this->operator[](_Pos);
    return 0;
}

int gf_Vec4fVector_default(gf_Vec4fVector_t** _result) {
    *_result = new std::vector<PXR_NS::GfVec4f>();
    return 0;
}

int gf_Vec4fVector_dtor(gf_Vec4fVector_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec2hArray_data(gf_Vec2hArray_t* _this, gf_Vec2h_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec2hArray_data_const(gf_Vec2hArray_t const* _this, gf_Vec2h_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec2hArray_size(gf_Vec2hArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec2hArray_op_index(gf_Vec2hArray_t* _this, size_t index, gf_Vec2h_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec2hArray_op_index_const(gf_Vec2hArray_t const* _this, size_t index, gf_Vec2h_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec2hArray_new(gf_Vec2hArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfVec2h>();
    return 0;
}

int gf_Vec2hArray_dtor(gf_Vec2hArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec3hArray_data(gf_Vec3hArray_t* _this, gf_Vec3h_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec3hArray_data_const(gf_Vec3hArray_t const* _this, gf_Vec3h_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec3hArray_size(gf_Vec3hArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec3hArray_op_index(gf_Vec3hArray_t* _this, size_t index, gf_Vec3h_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec3hArray_op_index_const(gf_Vec3hArray_t const* _this, size_t index, gf_Vec3h_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec3hArray_new(gf_Vec3hArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfVec3h>();
    return 0;
}

int gf_Vec3hArray_dtor(gf_Vec3hArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec4hArray_data(gf_Vec4hArray_t* _this, gf_Vec4h_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec4hArray_data_const(gf_Vec4hArray_t const* _this, gf_Vec4h_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec4hArray_size(gf_Vec4hArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec4hArray_op_index(gf_Vec4hArray_t* _this, size_t index, gf_Vec4h_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec4hArray_op_index_const(gf_Vec4hArray_t const* _this, size_t index, gf_Vec4h_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec4hArray_new(gf_Vec4hArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfVec4h>();
    return 0;
}

int gf_Vec4hArray_dtor(gf_Vec4hArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec2dArray_data(gf_Vec2dArray_t* _this, gf_Vec2d_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec2dArray_data_const(gf_Vec2dArray_t const* _this, gf_Vec2d_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec2dArray_size(gf_Vec2dArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec2dArray_op_index(gf_Vec2dArray_t* _this, size_t index, gf_Vec2d_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec2dArray_op_index_const(gf_Vec2dArray_t const* _this, size_t index, gf_Vec2d_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec2dArray_new(gf_Vec2dArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfVec2d>();
    return 0;
}

int gf_Vec2dArray_dtor(gf_Vec2dArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec3dVector_data(gf_Vec3dVector_t* _this, gf_Vec3d_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec3dVector_data_const(gf_Vec3dVector_t const* _this, gf_Vec3d_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec3dVector_empty(gf_Vec3dVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int gf_Vec3dVector_size(gf_Vec3dVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec3dVector_max_size(gf_Vec3dVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int gf_Vec3dVector_reserve(gf_Vec3dVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int gf_Vec3dVector_capacity(gf_Vec3dVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int gf_Vec3dVector_clear(gf_Vec3dVector_t* _this) {
    _this->clear();
    return 0;
}

int gf_Vec3dVector_push_back(gf_Vec3dVector_t* _this, gf_Vec3d_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int gf_Vec3dVector_pop_back(gf_Vec3dVector_t* _this) {
    _this->pop_back();
    return 0;
}

int gf_Vec3dVector_resize(gf_Vec3dVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int gf_Vec3dVector_resize_with(gf_Vec3dVector_t* _this, size_t const _Newsize, gf_Vec3d_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int gf_Vec3dVector_op_index(gf_Vec3dVector_t const* _this, size_t const _Pos, gf_Vec3d_t* _result) {
    *_result = _this->operator[](_Pos);
    return 0;
}

int gf_Vec3dVector_default(gf_Vec3dVector_t** _result) {
    *_result = new std::vector<PXR_NS::GfVec3d>();
    return 0;
}

int gf_Vec3dVector_dtor(gf_Vec3dVector_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec3dArray_data(gf_Vec3dArray_t* _this, gf_Vec3d_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec3dArray_data_const(gf_Vec3dArray_t const* _this, gf_Vec3d_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec3dArray_size(gf_Vec3dArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec3dArray_op_index(gf_Vec3dArray_t* _this, size_t index, gf_Vec3d_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec3dArray_op_index_const(gf_Vec3dArray_t const* _this, size_t index, gf_Vec3d_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec3dArray_new(gf_Vec3dArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfVec3d>();
    return 0;
}

int gf_Vec3dArray_dtor(gf_Vec3dArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec4dArray_data(gf_Vec4dArray_t* _this, gf_Vec4d_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec4dArray_data_const(gf_Vec4dArray_t const* _this, gf_Vec4d_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec4dArray_size(gf_Vec4dArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec4dArray_op_index(gf_Vec4dArray_t* _this, size_t index, gf_Vec4d_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec4dArray_op_index_const(gf_Vec4dArray_t const* _this, size_t index, gf_Vec4d_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec4dArray_new(gf_Vec4dArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfVec4d>();
    return 0;
}

int gf_Vec4dArray_dtor(gf_Vec4dArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec2iArray_data(gf_Vec2iArray_t* _this, gf_Vec2i_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec2iArray_data_const(gf_Vec2iArray_t const* _this, gf_Vec2i_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec2iArray_size(gf_Vec2iArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec2iArray_op_index(gf_Vec2iArray_t* _this, size_t index, gf_Vec2i_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec2iArray_op_index_const(gf_Vec2iArray_t const* _this, size_t index, gf_Vec2i_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec2iArray_new(gf_Vec2iArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfVec2i>();
    return 0;
}

int gf_Vec2iArray_dtor(gf_Vec2iArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec3iArray_data(gf_Vec3iArray_t* _this, gf_Vec3i_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec3iArray_data_const(gf_Vec3iArray_t const* _this, gf_Vec3i_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec3iArray_size(gf_Vec3iArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec3iArray_op_index(gf_Vec3iArray_t* _this, size_t index, gf_Vec3i_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec3iArray_op_index_const(gf_Vec3iArray_t const* _this, size_t index, gf_Vec3i_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec3iArray_new(gf_Vec3iArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfVec3i>();
    return 0;
}

int gf_Vec3iArray_dtor(gf_Vec3iArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Vec4iArray_data(gf_Vec4iArray_t* _this, gf_Vec4i_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec4iArray_data_const(gf_Vec4iArray_t const* _this, gf_Vec4i_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Vec4iArray_size(gf_Vec4iArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Vec4iArray_op_index(gf_Vec4iArray_t* _this, size_t index, gf_Vec4i_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec4iArray_op_index_const(gf_Vec4iArray_t const* _this, size_t index, gf_Vec4i_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Vec4iArray_new(gf_Vec4iArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfVec4i>();
    return 0;
}

int gf_Vec4iArray_dtor(gf_Vec4iArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Matrix2dArray_data(gf_Matrix2dArray_t* _this, gf_Matrix2d_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix2dArray_data_const(gf_Matrix2dArray_t const* _this, gf_Matrix2d_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix2dArray_size(gf_Matrix2dArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Matrix2dArray_op_index(gf_Matrix2dArray_t* _this, size_t index, gf_Matrix2d_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Matrix2dArray_op_index_const(gf_Matrix2dArray_t const* _this, size_t index, gf_Matrix2d_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Matrix2dArray_new(gf_Matrix2dArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfMatrix2d>();
    return 0;
}

int gf_Matrix2dArray_dtor(gf_Matrix2dArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Matrix3dArray_data(gf_Matrix3dArray_t* _this, gf_Matrix3d_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix3dArray_data_const(gf_Matrix3dArray_t const* _this, gf_Matrix3d_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix3dArray_size(gf_Matrix3dArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Matrix3dArray_op_index(gf_Matrix3dArray_t* _this, size_t index, gf_Matrix3d_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Matrix3dArray_op_index_const(gf_Matrix3dArray_t const* _this, size_t index, gf_Matrix3d_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Matrix3dArray_new(gf_Matrix3dArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfMatrix3d>();
    return 0;
}

int gf_Matrix3dArray_dtor(gf_Matrix3dArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Matrix4dArray_data(gf_Matrix4dArray_t* _this, gf_Matrix4d_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix4dArray_data_const(gf_Matrix4dArray_t const* _this, gf_Matrix4d_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix4dArray_size(gf_Matrix4dArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Matrix4dArray_op_index(gf_Matrix4dArray_t* _this, size_t index, gf_Matrix4d_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Matrix4dArray_op_index_const(gf_Matrix4dArray_t const* _this, size_t index, gf_Matrix4d_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Matrix4dArray_new(gf_Matrix4dArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfMatrix4d>();
    return 0;
}

int gf_Matrix4dArray_dtor(gf_Matrix4dArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Matrix4dArrayVector_data(gf_Matrix4dArrayVector_t* _this, gf_Matrix4dArray_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix4dArrayVector_data_const(gf_Matrix4dArrayVector_t const* _this, gf_Matrix4dArray_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix4dArrayVector_empty(gf_Matrix4dArrayVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int gf_Matrix4dArrayVector_size(gf_Matrix4dArrayVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Matrix4dArrayVector_max_size(gf_Matrix4dArrayVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int gf_Matrix4dArrayVector_reserve(gf_Matrix4dArrayVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int gf_Matrix4dArrayVector_capacity(gf_Matrix4dArrayVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int gf_Matrix4dArrayVector_clear(gf_Matrix4dArrayVector_t* _this) {
    _this->clear();
    return 0;
}

int gf_Matrix4dArrayVector_push_back(gf_Matrix4dArrayVector_t* _this, gf_Matrix4dArray_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int gf_Matrix4dArrayVector_pop_back(gf_Matrix4dArrayVector_t* _this) {
    _this->pop_back();
    return 0;
}

int gf_Matrix4dArrayVector_resize(gf_Matrix4dArrayVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int gf_Matrix4dArrayVector_resize_with(gf_Matrix4dArrayVector_t* _this, size_t const _Newsize, gf_Matrix4dArray_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int gf_Matrix4dArrayVector_op_index(gf_Matrix4dArrayVector_t const* _this, size_t const _Pos, gf_Matrix4dArray_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int gf_Matrix4dArrayVector_default(gf_Matrix4dArrayVector_t** _result) {
    *_result = new std::vector<PXR_NS::VtMatrix4dArray>();
    return 0;
}

int gf_Matrix4dArrayVector_dtor(gf_Matrix4dArrayVector_t* _this) {
    delete _this;
    return 0;
}

int gf_Matrix2fArray_data(gf_Matrix2fArray_t* _this, gf_Matrix2f_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix2fArray_data_const(gf_Matrix2fArray_t const* _this, gf_Matrix2f_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix2fArray_size(gf_Matrix2fArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Matrix2fArray_op_index(gf_Matrix2fArray_t* _this, size_t index, gf_Matrix2f_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Matrix2fArray_op_index_const(gf_Matrix2fArray_t const* _this, size_t index, gf_Matrix2f_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Matrix2fArray_new(gf_Matrix2fArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfMatrix2f>();
    return 0;
}

int gf_Matrix2fArray_dtor(gf_Matrix2fArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Matrix3fArray_data(gf_Matrix3fArray_t* _this, gf_Matrix3f_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix3fArray_data_const(gf_Matrix3fArray_t const* _this, gf_Matrix3f_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix3fArray_size(gf_Matrix3fArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Matrix3fArray_op_index(gf_Matrix3fArray_t* _this, size_t index, gf_Matrix3f_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Matrix3fArray_op_index_const(gf_Matrix3fArray_t const* _this, size_t index, gf_Matrix3f_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Matrix3fArray_new(gf_Matrix3fArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfMatrix3f>();
    return 0;
}

int gf_Matrix3fArray_dtor(gf_Matrix3fArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Matrix4fArray_data(gf_Matrix4fArray_t* _this, gf_Matrix4f_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix4fArray_data_const(gf_Matrix4fArray_t const* _this, gf_Matrix4f_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_Matrix4fArray_size(gf_Matrix4fArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_Matrix4fArray_op_index(gf_Matrix4fArray_t* _this, size_t index, gf_Matrix4f_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Matrix4fArray_op_index_const(gf_Matrix4fArray_t const* _this, size_t index, gf_Matrix4f_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_Matrix4fArray_new(gf_Matrix4fArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfMatrix4f>();
    return 0;
}

int gf_Matrix4fArray_dtor(gf_Matrix4fArray_t* _this) {
    delete _this;
    return 0;
}

int gf_QuatdArray_data(gf_QuatdArray_t* _this, gf_Quatd_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_QuatdArray_data_const(gf_QuatdArray_t const* _this, gf_Quatd_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_QuatdArray_size(gf_QuatdArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_QuatdArray_op_index(gf_QuatdArray_t* _this, size_t index, gf_Quatd_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_QuatdArray_op_index_const(gf_QuatdArray_t const* _this, size_t index, gf_Quatd_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_QuatdArray_new(gf_QuatdArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfQuatd>();
    return 0;
}

int gf_QuatdArray_dtor(gf_QuatdArray_t* _this) {
    delete _this;
    return 0;
}

int gf_QuatfArray_data(gf_QuatfArray_t* _this, gf_Quatf_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_QuatfArray_data_const(gf_QuatfArray_t const* _this, gf_Quatf_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_QuatfArray_size(gf_QuatfArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_QuatfArray_op_index(gf_QuatfArray_t* _this, size_t index, gf_Quatf_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_QuatfArray_op_index_const(gf_QuatfArray_t const* _this, size_t index, gf_Quatf_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_QuatfArray_new(gf_QuatfArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfQuatf>();
    return 0;
}

int gf_QuatfArray_dtor(gf_QuatfArray_t* _this) {
    delete _this;
    return 0;
}

int gf_QuathArray_data(gf_QuathArray_t* _this, gf_Quath_t** _result) {
    *_result = _this->data();
    return 0;
}

int gf_QuathArray_data_const(gf_QuathArray_t const* _this, gf_Quath_t const** _result) {
    *_result = _this->data();
    return 0;
}

int gf_QuathArray_size(gf_QuathArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int gf_QuathArray_op_index(gf_QuathArray_t* _this, size_t index, gf_Quath_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_QuathArray_op_index_const(gf_QuathArray_t const* _this, size_t index, gf_Quath_t* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int gf_QuathArray_new(gf_QuathArray_t** _result) {
    *_result = new PXR_NS::VtArray<PXR_NS::GfQuath>();
    return 0;
}

int gf_QuathArray_dtor(gf_QuathArray_t* _this) {
    delete _this;
    return 0;
}

int gf_Interval_GetMin(gf_Interval_t const* _this, double* _result) {
    *_result = _this->GetMin();
    return 0;
}

int gf_Interval_GetMax(gf_Interval_t const* _this, double* _result) {
    *_result = _this->GetMax();
    return 0;
}

int gf_Interval_GetSize(gf_Interval_t const* _this, double* _result) {
    *_result = _this->GetSize();
    return 0;
}

int gf_Interval_default(gf_Interval_t* _result) {
    new (_result) PXR_NS::GfInterval();
    return 0;
}

int gf_BBox3d_Set(gf_BBox3d_t* _this, gf_Range3d_t const* box, gf_Matrix4d_t const* matrix) {
    _this->Set(*box, *matrix);
    return 0;
}

int gf_BBox3d_SetMatrix(gf_BBox3d_t* _this, gf_Matrix4d_t const* matrix) {
    _this->SetMatrix(*matrix);
    return 0;
}

int gf_BBox3d_SetRange(gf_BBox3d_t* _this, gf_Range3d_t const* box) {
    _this->SetRange(*box);
    return 0;
}

int gf_BBox3d_from_range_and_transform(gf_BBox3d_t** _result) {
    *_result = new PXR_NS::GfBBox3d();
    return 0;
}

int gf_BBox3d_from_range(gf_BBox3d_t** _result) {
    *_result = new PXR_NS::GfBBox3d();
    return 0;
}

int gf_BBox3d_new(gf_BBox3d_t** _result) {
    *_result = new PXR_NS::GfBBox3d();
    return 0;
}

int gf_BBox3d_dtor(gf_BBox3d_t* _this) {
    delete _this;
    return 0;
}

int gf_Camera_SetTransform(gf_Camera_t* _this, gf_Matrix4d_t const* val) {
    _this->SetTransform(*val);
    return 0;
}

int gf_Camera_SetClippingRange(gf_Camera_t* _this, gf_Range1f_t const* val) {
    _this->SetClippingRange(*val);
    return 0;
}

int gf_Camera_SetClippingPlanes(gf_Camera_t* _this, gf_Vec4fVector_t const* val) {
    _this->SetClippingPlanes(*val);
    return 0;
}

int gf_Camera_SetFStop(gf_Camera_t* _this, float const val) {
    _this->SetFStop(val);
    return 0;
}

int gf_Camera_SetFocusDistance(gf_Camera_t* _this, float const val) {
    _this->SetFocusDistance(val);
    return 0;
}

int gf_Camera_SetFocalLength(gf_Camera_t* _this, float const val) {
    _this->SetFocalLength(val);
    return 0;
}

int gf_Camera_SetHorizontalAperture(gf_Camera_t* _this, float const val) {
    _this->SetHorizontalAperture(val);
    return 0;
}

int gf_Camera_SetHorizontalApertureOffset(gf_Camera_t* _this, float const val) {
    _this->SetHorizontalApertureOffset(val);
    return 0;
}

int gf_Camera_SetVerticalAperture(gf_Camera_t* _this, float const val) {
    _this->SetVerticalAperture(val);
    return 0;
}

int gf_Camera_SetVerticalApertureOffset(gf_Camera_t* _this, float const val) {
    _this->SetVerticalApertureOffset(val);
    return 0;
}

int gf_Camera_GetTransform(gf_Camera_t const* _this, gf_Matrix4d_t* _result) {
    *_result = _this->GetTransform();
    return 0;
}

int gf_Camera_GetClippingRange(gf_Camera_t const* _this, gf_Range1f_t* _result) {
    *_result = _this->GetClippingRange();
    return 0;
}

int gf_Camera_GetClippingPlanes(gf_Camera_t const* _this, gf_Vec4fVector_t const** _result) {
    *_result = &_this->GetClippingPlanes();
    return 0;
}

int gf_Camera_GetFStop(gf_Camera_t const* _this, float* _result) {
    *_result = _this->GetFStop();
    return 0;
}

int gf_Camera_GetFocusDistance(gf_Camera_t const* _this, float* _result) {
    *_result = _this->GetFocusDistance();
    return 0;
}

int gf_Camera_GetFocalLength(gf_Camera_t const* _this, float* _result) {
    *_result = _this->GetFocalLength();
    return 0;
}

int gf_Camera_GetHorizontalAperture(gf_Camera_t const* _this, float* _result) {
    *_result = _this->GetHorizontalAperture();
    return 0;
}

int gf_Camera_GetHorizontalApertureOffset(gf_Camera_t const* _this, float* _result) {
    *_result = _this->GetHorizontalApertureOffset();
    return 0;
}

int gf_Camera_GetVerticalAperture(gf_Camera_t const* _this, float* _result) {
    *_result = _this->GetVerticalAperture();
    return 0;
}

int gf_Camera_GetVerticalApertureOffset(gf_Camera_t const* _this, float* _result) {
    *_result = _this->GetVerticalApertureOffset();
    return 0;
}

int gf_Camera_GetFieldOfView(gf_Camera_t const* _this, int direction, float* _result) {
    *_result = _this->GetFieldOfView(static_cast<PXR_NS::GfCamera::FOVDirection>(direction));
    return 0;
}

int gf_Camera_GetFrustum(gf_Camera_t const* _this, gf_Frustum_t* _result) {
    *_result = _this->GetFrustum();
    return 0;
}

int gf_Camera_SetPerspectiveFromAspectRatioAndFieldOfView(gf_Camera_t* _this, float aspectRatio, float fieldOfView, int direction, float horizontalAperture) {
    _this->SetPerspectiveFromAspectRatioAndFieldOfView(aspectRatio, fieldOfView, static_cast<PXR_NS::GfCamera::FOVDirection>(direction), horizontalAperture);
    return 0;
}

int gf_Camera_SetOrthographicFromAspectRatioAndSize(gf_Camera_t* _this, float aspectRatio, float orthographicSize, int direction) {
    _this->SetOrthographicFromAspectRatioAndSize(aspectRatio, orthographicSize, static_cast<PXR_NS::GfCamera::FOVDirection>(direction));
    return 0;
}

int gf_Camera_SetFromViewAndProjectionMatrix(gf_Camera_t* _this, gf_Matrix4d_t const* viewMatrix, gf_Matrix4d_t const* projMatix, float const focalLength) {
    _this->SetFromViewAndProjectionMatrix(*viewMatrix, *projMatix, focalLength);
    return 0;
}

int gf_Camera_new(gf_Matrix4d_t const* transform, int projection, float horizontalAperture, float verticalAperture, float horizontalApertureOffset, float verticalApertureOffset, float focalLength, gf_Range1f_t clippingRange, gf_Vec4fVector_t const* clippingPlanes, float fStop, float focusDistance, gf_Camera_t** _result) {
    *_result = new PXR_NS::GfCamera(*transform, static_cast<PXR_NS::GfCamera::Projection>(projection), horizontalAperture, verticalAperture, horizontalApertureOffset, verticalApertureOffset, focalLength, clippingRange, *clippingPlanes, fStop, focusDistance);
    return 0;
}

int gf_Camera_dtor(gf_Camera_t* _this) {
    delete _this;
    return 0;
}

int gf_Frustum_SetPerspective(gf_Frustum_t* _this, double fieldOfView, bool isFovVertical, double aspectRatio, double nearDistance, double farDistance) {
    _this->SetPerspective(fieldOfView, isFovVertical, aspectRatio, nearDistance, farDistance);
    return 0;
}

int gf_Frustum_GetPerspective(gf_Frustum_t const* _this, bool isFovVertical, double* fieldOfView, double* aspectRatio, double* nearDistance, double* farDistance, bool* _result) {
    *_result = _this->GetPerspective(isFovVertical, fieldOfView, aspectRatio, nearDistance, farDistance);
    return 0;
}

int gf_Frustum_GetFOV(gf_Frustum_t* _this, bool isFovVertical, double* _result) {
    *_result = _this->GetFOV(isFovVertical);
    return 0;
}

int gf_Frustum_GetOrthographic(gf_Frustum_t const* _this, double* left, double* right, double* bottom, double* top, double* nearPlane, double* farPlane, bool* _result) {
    *_result = _this->GetOrthographic(left, right, bottom, top, nearPlane, farPlane);
    return 0;
}

int gf_Frustum_SetOrthographic(gf_Frustum_t* _this, double left, double right, double bottom, double top, double nearPlane, double farPlane) {
    _this->SetOrthographic(left, right, bottom, top, nearPlane, farPlane);
    return 0;
}

int gf_Frustum_FitToSphere(gf_Frustum_t* _this, gf_Vec3d_t const* center, double radius, double slack) {
    _this->FitToSphere(*center, radius, slack);
    return 0;
}

int gf_Frustum_Transform(gf_Frustum_t* _this, gf_Matrix4d_t const* matrix, gf_Frustum_t** _result) {
    *_result = &_this->Transform(*matrix);
    return 0;
}

int gf_Frustum_ComputeViewDirection(gf_Frustum_t const* _this, gf_Vec3d_t* _result) {
    *_result = _this->ComputeViewDirection();
    return 0;
}

int gf_Frustum_ComputeViewFrame(gf_Frustum_t const* _this, gf_Vec3d_t* side, gf_Vec3d_t* up, gf_Vec3d_t* view) {
    _this->ComputeViewFrame(side, up, view);
    return 0;
}

int gf_Frustum_ComputeLookAtPoint(gf_Frustum_t const* _this, gf_Vec3d_t* _result) {
    *_result = _this->ComputeLookAtPoint();
    return 0;
}

int gf_Frustum_ComputeViewMatrix(gf_Frustum_t const* _this, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeViewMatrix();
    return 0;
}

int gf_Frustum_ComputeViewInverse(gf_Frustum_t const* _this, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeViewInverse();
    return 0;
}

int gf_Frustum_ComputeProjectionMatrix(gf_Frustum_t const* _this, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeProjectionMatrix();
    return 0;
}

int gf_Frustum_ComputeAspectRatio(gf_Frustum_t const* _this, double* _result) {
    *_result = _this->ComputeAspectRatio();
    return 0;
}

int gf_Frustum_ComputeCorners(gf_Frustum_t const* _this, gf_Vec3dVector_t* _result) {
    *_result = _this->ComputeCorners();
    return 0;
}

int gf_Frustum_ComputeCornersAtDistance(gf_Frustum_t const* _this, double d, gf_Vec3dVector_t* _result) {
    *_result = _this->ComputeCornersAtDistance(d);
    return 0;
}

int gf_Frustum_ComputeNarrowedFrustum(gf_Frustum_t const* _this, gf_Vec2d_t const* windowPos, gf_Vec2d_t const* size, gf_Frustum_t* _result) {
    *_result = _this->ComputeNarrowedFrustum(*windowPos, *size);
    return 0;
}

int gf_Frustum_ComputeRay_to_point(gf_Frustum_t const* _this, gf_Vec3d_t const* worldSpacePos, gf_Ray_t* _result) {
    *_result = _this->ComputeRay(*worldSpacePos);
    return 0;
}

int gf_Frustum_ComputeRay_from_screen(gf_Frustum_t const* _this, gf_Vec2d_t const* windowPos, gf_Ray_t* _result) {
    *_result = _this->ComputeRay(*windowPos);
    return 0;
}

int gf_Frustum_ComputePickRay_to_point(gf_Frustum_t const* _this, gf_Vec3d_t const* worldSpacePos, gf_Ray_t* _result) {
    *_result = _this->ComputePickRay(*worldSpacePos);
    return 0;
}

int gf_Frustum_ComputePickRay_from_screen(gf_Frustum_t const* _this, gf_Vec2d_t const* windowPos, gf_Ray_t* _result) {
    *_result = _this->ComputePickRay(*windowPos);
    return 0;
}

int gf_Frustum_SetPosition(gf_Frustum_t* _this, gf_Vec3d_t const* position) {
    _this->SetPosition(*position);
    return 0;
}

int gf_Frustum_GetPosition(gf_Frustum_t const* _this, gf_Vec3d_t* _result) {
    *_result = _this->GetPosition();
    return 0;
}

int gf_Frustum_GetRotation(gf_Frustum_t const* _this, gf_Rotation_t* _result) {
    *_result = _this->GetRotation();
    return 0;
}

int gf_Frustum_SetRotation(gf_Frustum_t* _this, gf_Rotation_t const* rotation) {
    _this->SetRotation(*rotation);
    return 0;
}

int gf_Frustum_SetPositionAndRotationFromMatrix(gf_Frustum_t* _this, gf_Matrix4d_t const* camToWorldXf) {
    _this->SetPositionAndRotationFromMatrix(*camToWorldXf);
    return 0;
}

int gf_Frustum_SetWindow(gf_Frustum_t* _this, gf_Range2d_t const* window) {
    _this->SetWindow(*window);
    return 0;
}

int gf_Frustum_GetWindow(gf_Frustum_t const* _this, gf_Range2d_t* _result) {
    *_result = _this->GetWindow();
    return 0;
}

int gf_Frustum_SetNearFar(gf_Frustum_t* _this, gf_Range1d_t const* nearFar) {
    _this->SetNearFar(*nearFar);
    return 0;
}

int gf_Frustum_GetNearFar(gf_Frustum_t const* _this, gf_Range1d_t* _result) {
    *_result = _this->GetNearFar();
    return 0;
}

int gf_Frustum_SetViewDistance(gf_Frustum_t* _this, double viewDistance) {
    _this->SetViewDistance(viewDistance);
    return 0;
}

int gf_Frustum_GetViewDistance(gf_Frustum_t const* _this, double* _result) {
    *_result = _this->GetViewDistance();
    return 0;
}

int gf_Frustum_SetProjectionType(gf_Frustum_t* _this, int projectionType) {
    _this->SetProjectionType(static_cast<PXR_NS::GfFrustum::ProjectionType>(projectionType));
    return 0;
}

int gf_Frustum_GetProjectionType(gf_Frustum_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetProjectionType());
    return 0;
}

int gf_Frustum_GetReferencePlaneDepth(double* _result) {
    *_result = PXR_NS::GfFrustum::GetReferencePlaneDepth();
    return 0;
}

int gf_Frustum_Intersects_box(gf_Frustum_t const* _this, gf_BBox3d_t const* bbox, bool* _result) {
    *_result = _this->Intersects(*bbox);
    return 0;
}

int gf_Frustum_Intersects_point(gf_Frustum_t const* _this, gf_Vec3d_t const* point, bool* _result) {
    *_result = _this->Intersects(*point);
    return 0;
}

int gf_Frustum_Intersects_line(gf_Frustum_t const* _this, gf_Vec3d_t const* p0, gf_Vec3d_t const* p1, bool* _result) {
    *_result = _this->Intersects(*p0, *p1);
    return 0;
}

int gf_Frustum_Intersects_triangle(gf_Frustum_t const* _this, gf_Vec3d_t const* p0, gf_Vec3d_t const* p1, gf_Vec3d_t const* p2, bool* _result) {
    *_result = _this->Intersects(*p0, *p1, *p2);
    return 0;
}

int gf_Frustum_IntersectsViewVolume(gf_BBox3d_t const* bbox, gf_Matrix4d_t const* vpMat, bool* _result) {
    *_result = PXR_NS::GfFrustum::IntersectsViewVolume(*bbox, *vpMat);
    return 0;
}

int gf_Frustum_from_transform(gf_Frustum_t** _result) {
    *_result = new PXR_NS::GfFrustum();
    return 0;
}

int gf_Frustum_from_position_and_rotation(gf_Frustum_t** _result) {
    *_result = new PXR_NS::GfFrustum();
    return 0;
}

int gf_Frustum_new(gf_Frustum_t** _result) {
    *_result = new PXR_NS::GfFrustum();
    return 0;
}

int gf_Frustum_dtor(gf_Frustum_t* _this) {
    delete _this;
    return 0;
}

int gf_Ray_GetStartPoint(gf_Ray_t const* _this, gf_Vec3d_t* _result) {
    *_result = _this->GetStartPoint();
    return 0;
}

int gf_Ray_GetDirection(gf_Ray_t const* _this, gf_Vec3d_t* _result) {
    *_result = _this->GetDirection();
    return 0;
}

int gf_Ray_GetPoint(gf_Ray_t const* _this, double distance, gf_Vec3d_t* _result) {
    *_result = _this->GetPoint(distance);
    return 0;
}

int js_Array_data(js_Array_t* _this, js_Value_t** _result) {
    *_result = _this->data();
    return 0;
}

int js_Array_data_const(js_Array_t const* _this, js_Value_t const** _result) {
    *_result = _this->data();
    return 0;
}

int js_Array_empty(js_Array_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int js_Array_size(js_Array_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int js_Array_max_size(js_Array_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int js_Array_reserve(js_Array_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int js_Array_capacity(js_Array_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int js_Array_clear(js_Array_t* _this) {
    _this->clear();
    return 0;
}

int js_Array_push_back(js_Array_t* _this, js_Value_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int js_Array_pop_back(js_Array_t* _this) {
    _this->pop_back();
    return 0;
}

int js_Array_resize(js_Array_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int js_Array_resize_with(js_Array_t* _this, size_t const _Newsize, js_Value_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int js_Array_op_index(js_Array_t const* _this, size_t const _Pos, js_Value_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int js_Array_default(js_Array_t** _result) {
    *_result = new PXR_NS::JsArray();
    return 0;
}

int js_Array_dtor(js_Array_t* _this) {
    delete _this;
    return 0;
}

int js_Object_dtor(js_Object_t* _this) {
    delete _this;
    return 0;
}

int js_Value_GetJsObject(js_Value_t const* _this, js_Object_t const** _result) {
    *_result = &_this->GetJsObject();
    return 0;
}

int js_Value_GetJsArray(js_Value_t const* _this, js_Array_t const** _result) {
    *_result = &_this->GetJsArray();
    return 0;
}

int js_Value_GetBool(js_Value_t const* _this, bool* _result) {
    *_result = _this->GetBool();
    return 0;
}

int js_Value_GetInt(js_Value_t const* _this, int* _result) {
    *_result = _this->GetInt();
    return 0;
}

int js_Value_GetInt64(js_Value_t const* _this, int64_t* _result) {
    *_result = _this->GetInt64();
    return 0;
}

int js_Value_GetUInt64(js_Value_t const* _this, uint64_t* _result) {
    *_result = _this->GetUInt64();
    return 0;
}

int js_Value_GetReal(js_Value_t const* _this, double* _result) {
    *_result = _this->GetReal();
    return 0;
}

int js_Value_IsObject(js_Value_t const* _this, bool* _result) {
    *_result = _this->IsObject();
    return 0;
}

int js_Value_IsArray(js_Value_t const* _this, bool* _result) {
    *_result = _this->IsArray();
    return 0;
}

int js_Value_IsBool(js_Value_t const* _this, bool* _result) {
    *_result = _this->IsBool();
    return 0;
}

int js_Value_IsInt(js_Value_t const* _this, bool* _result) {
    *_result = _this->IsInt();
    return 0;
}

int js_Value_IsUInt64(js_Value_t const* _this, bool* _result) {
    *_result = _this->IsUInt64();
    return 0;
}

int js_Value_IsReal(js_Value_t const* _this, bool* _result) {
    *_result = _this->IsReal();
    return 0;
}

int js_Value_from_double(js_Value_t** _result) {
    *_result = new PXR_NS::JsValue();
    return 0;
}

int js_Value_from_uint64_t(js_Value_t** _result) {
    *_result = new PXR_NS::JsValue();
    return 0;
}

int js_Value_from_int64_t(js_Value_t** _result) {
    *_result = new PXR_NS::JsValue();
    return 0;
}

int js_Value_from_int(js_Value_t** _result) {
    *_result = new PXR_NS::JsValue();
    return 0;
}

int js_Value_from_bool(js_Value_t** _result) {
    *_result = new PXR_NS::JsValue();
    return 0;
}

int js_Value_from_string(js_Value_t** _result) {
    *_result = new PXR_NS::JsValue();
    return 0;
}

int js_Value_from_array(js_Value_t** _result) {
    *_result = new PXR_NS::JsValue();
    return 0;
}

int js_Value_from_object(js_Value_t** _result) {
    *_result = new PXR_NS::JsValue();
    return 0;
}

int js_Value_new(js_Value_t** _result) {
    *_result = new PXR_NS::JsValue();
    return 0;
}

int js_Value_dtor(js_Value_t* _this) {
    delete _this;
    return 0;
}

int pcp_MapFunction_IsNull(pcp_MapFunction_t const* _this, bool* _result) {
    *_result = _this->IsNull();
    return 0;
}

int pcp_MapFunction_IsIdentity(pcp_MapFunction_t const* _this, bool* _result) {
    *_result = _this->IsIdentity();
    return 0;
}

int pcp_MapFunction_IsIdentityPathMapping(pcp_MapFunction_t const* _this, bool* _result) {
    *_result = _this->IsIdentityPathMapping();
    return 0;
}

int pcp_MapFunction_HasRootIdentity(pcp_MapFunction_t const* _this, bool* _result) {
    *_result = _this->HasRootIdentity();
    return 0;
}

int pcp_MapFunction_MapSourceToTarget(pcp_MapFunction_t const* _this, sdf_Path_t const* path, sdf_Path_t* _result) {
    *_result = _this->MapSourceToTarget(*path);
    return 0;
}

int pcp_MapFunction_MapTargetToSource(pcp_MapFunction_t const* _this, sdf_Path_t const* path, sdf_Path_t* _result) {
    *_result = _this->MapTargetToSource(*path);
    return 0;
}

int pcp_MapFunction_Compose(pcp_MapFunction_t const* _this, pcp_MapFunction_t const* f, pcp_MapFunction_t* _result) {
    *_result = _this->Compose(*f);
    return 0;
}

int pcp_MapFunction_ComposeOffset(pcp_MapFunction_t const* _this, sdf_LayerOffset_t const* newOffset, pcp_MapFunction_t* _result) {
    *_result = _this->ComposeOffset(*newOffset);
    return 0;
}

int pcp_MapFunction_GetInverse(pcp_MapFunction_t const* _this, pcp_MapFunction_t* _result) {
    *_result = _this->GetInverse();
    return 0;
}

int pcp_MapFunction_GetSourceToTargetMap(pcp_MapFunction_t const* _this, pcp_MapFunctionPathMap_t* _result) {
    *_result = _this->GetSourceToTargetMap();
    return 0;
}

int pcp_MapFunction_GetTimeOffset(pcp_MapFunction_t const* _this, sdf_LayerOffset_t const** _result) {
    *_result = &_this->GetTimeOffset();
    return 0;
}

int pcp_MapFunction_GetString(pcp_MapFunction_t const* _this, std_String_t* _result) {
    *_result = _this->GetString();
    return 0;
}

int pcp_MapFunction_Hash(pcp_MapFunction_t const* _this, size_t* _result) {
    *_result = _this->Hash();
    return 0;
}

int pcp_MapFunction_Create(pcp_MapFunctionPathMap_t const* sourceToTargetMap, sdf_LayerOffset_t const* offset, pcp_MapFunction_t* _result) {
    *_result = PXR_NS::PcpMapFunction::Create(*sourceToTargetMap, *offset);
    return 0;
}

int pcp_MapFunction_Identity(pcp_MapFunction_t const** _result) {
    *_result = &PXR_NS::PcpMapFunction::Identity();
    return 0;
}

int pcp_MapFunction_IdentityPathMap(pcp_MapFunctionPathMap_t const** _result) {
    *_result = &PXR_NS::PcpMapFunction::IdentityPathMap();
    return 0;
}

int pcp_MapFunction_default(pcp_MapFunction_t** _result) {
    *_result = new PXR_NS::PcpMapFunction();
    return 0;
}

int pcp_MapFunction_dtor(pcp_MapFunction_t* _this) {
    delete _this;
    return 0;
}

int pcp_NodeRef_dtor(pcp_NodeRef_t* _this) {
    delete _this;
    return 0;
}

int pcp_MapFunctionPathMap_dtor(pcp_MapFunctionPathMap_t* _this) {
    delete _this;
    return 0;
}

int pcp_PrimIndex_dtor(pcp_PrimIndex_t* _this) {
    delete _this;
    return 0;
}

int pcp_VariantFallbackMap_dtor(pcp_VariantFallbackMap_t* _this) {
    delete _this;
    return 0;
}

int sdf_Allowed_IsAllowed(sdf_Allowed_t const* _this, std_String_t* whyNot, bool* _result) {
    *_result = _this->IsAllowed(whyNot);
    return 0;
}

int sdf_Allowed_from_bool_and_string(sdf_Allowed_t** _result) {
    *_result = new PXR_NS::SdfAllowed();
    return 0;
}

int sdf_Allowed_from_string(sdf_Allowed_t** _result) {
    *_result = new PXR_NS::SdfAllowed();
    return 0;
}

int sdf_Allowed_from_bool(sdf_Allowed_t** _result) {
    *_result = new PXR_NS::SdfAllowed();
    return 0;
}

int sdf_Allowed_new(sdf_Allowed_t** _result) {
    *_result = new PXR_NS::SdfAllowed();
    return 0;
}

int sdf_Allowed_dtor(sdf_Allowed_t* _this) {
    delete _this;
    return 0;
}

int sdf_AssetPath_new(sdf_AssetPath_t** _result) {
    *_result = new PXR_NS::SdfAssetPath();
    return 0;
}

int sdf_AssetPath_dtor(sdf_AssetPath_t* _this) {
    delete _this;
    return 0;
}

int sdf_AssetPathArray_dtor(sdf_AssetPathArray_t* _this) {
    delete _this;
    return 0;
}

int sdf_AttributeSpec_New(sdf_PrimSpecHandle_t const* owner, std_String_t const* name, sdf_ValueTypeName_t const* typeName, int variability, bool custom, sdf_AttributeSpecHandle_t* _result) {
    *_result = PXR_NS::SdfAttributeSpec::New(*owner, *name, *typeName, static_cast<PXR_NS::SdfVariability>(variability), custom);
    return 0;
}

int sdf_AttributeSpec_HasConnectionPaths(sdf_AttributeSpec_t const* _this, bool* _result) {
    *_result = _this->HasConnectionPaths();
    return 0;
}

int sdf_AttributeSpec_ClearConnectionPaths(sdf_AttributeSpec_t* _this) {
    _this->ClearConnectionPaths();
    return 0;
}

int sdf_AttributeSpec_GetAllowedTokens(sdf_AttributeSpec_t const* _this, vt_TokenArray_t* _result) {
    *_result = _this->GetAllowedTokens();
    return 0;
}

int sdf_AttributeSpec_HasAllowedTokens(sdf_AttributeSpec_t const* _this, bool* _result) {
    *_result = _this->HasAllowedTokens();
    return 0;
}

int sdf_AttributeSpec_SetAllowedTokens(sdf_AttributeSpec_t* _this, vt_TokenArray_t const* allowedTokens) {
    _this->SetAllowedTokens(*allowedTokens);
    return 0;
}

int sdf_AttributeSpec_GetConnectionPathList(sdf_AttributeSpec_t const* _this, sdf_InheritsProxy_t* _result) {
    *_result = _this->GetConnectionPathList();
    return 0;
}

int sdf_AttributeSpec_ClearAllowedTokens(sdf_AttributeSpec_t* _this) {
    _this->ClearAllowedTokens();
    return 0;
}

int sdf_AttributeSpec_GetDisplayUnit(sdf_AttributeSpec_t const* _this, tf_Enum_t* _result) {
    *_result = _this->GetDisplayUnit();
    return 0;
}

int sdf_AttributeSpec_SetDisplayUnit(sdf_AttributeSpec_t* _this, tf_Enum_t const* displayUnit) {
    _this->SetDisplayUnit(*displayUnit);
    return 0;
}

int sdf_AttributeSpec_HasDisplayUnit(sdf_AttributeSpec_t const* _this, bool* _result) {
    *_result = _this->HasDisplayUnit();
    return 0;
}

int sdf_AttributeSpec_ClearDisplayUnit(sdf_AttributeSpec_t* _this) {
    _this->ClearDisplayUnit();
    return 0;
}

int sdf_AttributeSpec_GetColorSpace(sdf_AttributeSpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetColorSpace();
    return 0;
}

int sdf_AttributeSpec_SetColorSpace(sdf_AttributeSpec_t* _this, tf_Token_t const* colorSpace) {
    _this->SetColorSpace(*colorSpace);
    return 0;
}

int sdf_AttributeSpec_HasColorSpace(sdf_AttributeSpec_t const* _this, bool* _result) {
    *_result = _this->HasColorSpace();
    return 0;
}

int sdf_AttributeSpec_ClearColorSpace(sdf_AttributeSpec_t* _this) {
    _this->ClearColorSpace();
    return 0;
}

int sdf_AttributeSpec_GetRoleName(sdf_AttributeSpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetRoleName();
    return 0;
}

int sdf_AttributeSpec_GetOwner(sdf_AttributeSpec_t const* _this, sdf_SpecHandle_t* _result) {
    *_result = _this->GetOwner();
    return 0;
}

int sdf_AttributeSpec_GetCustomData(sdf_AttributeSpec_t const* _this, sdf_DictionaryProxy_t* _result) {
    *_result = _this->GetCustomData();
    return 0;
}

int sdf_AttributeSpec_GetAssetInfo(sdf_AttributeSpec_t const* _this, sdf_DictionaryProxy_t* _result) {
    *_result = _this->GetAssetInfo();
    return 0;
}

int sdf_AttributeSpec_SetCustomData(sdf_AttributeSpec_t* _this, std_String_t const* name, vt_Value_t const* value) {
    _this->SetCustomData(*name, *value);
    return 0;
}

int sdf_AttributeSpec_SetAssetInfo(sdf_AttributeSpec_t* _this, std_String_t const* name, vt_Value_t const* value) {
    _this->SetAssetInfo(*name, *value);
    return 0;
}

int sdf_AttributeSpec_GetDisplayGroup(sdf_AttributeSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayGroup();
    return 0;
}

int sdf_AttributeSpec_SetDisplayGroup(sdf_AttributeSpec_t* _this, std_String_t const* value) {
    _this->SetDisplayGroup(*value);
    return 0;
}

int sdf_AttributeSpec_GetDisplayName(sdf_AttributeSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayName();
    return 0;
}

int sdf_AttributeSpec_SetDisplayName(sdf_AttributeSpec_t* _this, std_String_t const* value) {
    _this->SetDisplayName(*value);
    return 0;
}

int sdf_AttributeSpec_GetDocumentation(sdf_AttributeSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetDocumentation();
    return 0;
}

int sdf_AttributeSpec_SetDocumentation(sdf_AttributeSpec_t* _this, std_String_t const* value) {
    _this->SetDocumentation(*value);
    return 0;
}

int sdf_AttributeSpec_GetHidden(sdf_AttributeSpec_t const* _this, bool* _result) {
    *_result = _this->GetHidden();
    return 0;
}

int sdf_AttributeSpec_SetHidden(sdf_AttributeSpec_t* _this, bool value) {
    _this->SetHidden(value);
    return 0;
}

int sdf_AttributeSpec_GetPermission(sdf_AttributeSpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetPermission());
    return 0;
}

int sdf_AttributeSpec_SetPermission(sdf_AttributeSpec_t* _this, int value) {
    _this->SetPermission(static_cast<PXR_NS::SdfPermission>(value));
    return 0;
}

int sdf_AttributeSpec_GetPrefix(sdf_AttributeSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetPrefix();
    return 0;
}

int sdf_AttributeSpec_SetPrefix(sdf_AttributeSpec_t* _this, std_String_t const* value) {
    _this->SetPrefix(*value);
    return 0;
}

int sdf_AttributeSpec_GetSuffix(sdf_AttributeSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetSuffix();
    return 0;
}

int sdf_AttributeSpec_SetSuffix(sdf_AttributeSpec_t* _this, std_String_t const* value) {
    _this->SetSuffix(*value);
    return 0;
}

int sdf_AttributeSpec_GetSymmetricPeer(sdf_AttributeSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetSymmetricPeer();
    return 0;
}

int sdf_AttributeSpec_SetSymmetricPeer(sdf_AttributeSpec_t* _this, std_String_t const* peerName) {
    _this->SetSymmetricPeer(*peerName);
    return 0;
}

int sdf_AttributeSpec_GetSymmetryArguments(sdf_AttributeSpec_t const* _this, sdf_DictionaryProxy_t* _result) {
    *_result = _this->GetSymmetryArguments();
    return 0;
}

int sdf_AttributeSpec_SetSymmetryArgument(sdf_AttributeSpec_t* _this, std_String_t const* name, vt_Value_t const* value) {
    _this->SetSymmetryArgument(*name, *value);
    return 0;
}

int sdf_AttributeSpec_GetSymmetryFunction(sdf_AttributeSpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetSymmetryFunction();
    return 0;
}

int sdf_AttributeSpec_SetSymmetryFunction(sdf_AttributeSpec_t* _this, tf_Token_t const* functionName) {
    _this->SetSymmetryFunction(*functionName);
    return 0;
}

int sdf_AttributeSpec_GetTimeSampleMap(sdf_AttributeSpec_t const* _this, sdf_TimeSampleMap_t* _result) {
    *_result = _this->GetTimeSampleMap();
    return 0;
}

int sdf_AttributeSpec_GetValueType(sdf_AttributeSpec_t const* _this, tf_Type_t* _result) {
    *_result = _this->GetValueType();
    return 0;
}

int sdf_AttributeSpec_GetTypeName(sdf_AttributeSpec_t const* _this, sdf_ValueTypeName_t* _result) {
    *_result = _this->GetTypeName();
    return 0;
}

int sdf_AttributeSpec_GetDefaultValue(sdf_AttributeSpec_t const* _this, vt_Value_t* _result) {
    *_result = _this->GetDefaultValue();
    return 0;
}

int sdf_AttributeSpec_SetDefaultValue(sdf_AttributeSpec_t* _this, vt_Value_t const* defaultValue, bool* _result) {
    *_result = _this->SetDefaultValue(*defaultValue);
    return 0;
}

int sdf_AttributeSpec_HasDefaultValue(sdf_AttributeSpec_t const* _this, bool* _result) {
    *_result = _this->HasDefaultValue();
    return 0;
}

int sdf_AttributeSpec_ClearDefaultValue(sdf_AttributeSpec_t* _this) {
    _this->ClearDefaultValue();
    return 0;
}

int sdf_AttributeSpec_GetComment(sdf_AttributeSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetComment();
    return 0;
}

int sdf_AttributeSpec_SetComment(sdf_AttributeSpec_t* _this, std_String_t const* value) {
    _this->SetComment(*value);
    return 0;
}

int sdf_AttributeSpec_IsCustom(sdf_AttributeSpec_t const* _this, bool* _result) {
    *_result = _this->IsCustom();
    return 0;
}

int sdf_AttributeSpec_SetCustom(sdf_AttributeSpec_t* _this, bool custom) {
    _this->SetCustom(custom);
    return 0;
}

int sdf_AttributeSpec_GetVariability(sdf_AttributeSpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetVariability());
    return 0;
}

int sdf_AttributeSpec_HasOnlyRequiredFields(sdf_AttributeSpec_t const* _this, bool* _result) {
    *_result = _this->HasOnlyRequiredFields();
    return 0;
}

int sdf_AttributeSpec_GetName(sdf_AttributeSpec_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int sdf_AttributeSpec_GetNameToken(sdf_AttributeSpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNameToken();
    return 0;
}

int sdf_AttributeSpec_CanSetName(sdf_AttributeSpec_t const* _this, std_String_t const* newName, std_String_t* whyNot, bool* _result) {
    *_result = _this->CanSetName(*newName, whyNot);
    return 0;
}

int sdf_AttributeSpec_SetName(sdf_AttributeSpec_t* _this, std_String_t const* newName, bool validate, bool* _result) {
    *_result = _this->SetName(*newName, validate);
    return 0;
}

int sdf_AttributeSpec_IsValidName(std_String_t const* name, bool* _result) {
    *_result = PXR_NS::SdfAttributeSpec::IsValidName(*name);
    return 0;
}

int sdf_AttributeSpec_GetSchema(sdf_AttributeSpec_t const* _this, sdf_SchemaBase_t const** _result) {
    *_result = &_this->GetSchema();
    return 0;
}

int sdf_AttributeSpec_GetSpecType(sdf_AttributeSpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSpecType());
    return 0;
}

int sdf_AttributeSpec_IsDormant(sdf_AttributeSpec_t const* _this, bool* _result) {
    *_result = _this->IsDormant();
    return 0;
}

int sdf_AttributeSpec_GetLayer(sdf_AttributeSpec_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetLayer();
    return 0;
}

int sdf_AttributeSpec_GetPath(sdf_AttributeSpec_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int sdf_AttributeSpec_PermissionToEdit(sdf_AttributeSpec_t const* _this, bool* _result) {
    *_result = _this->PermissionToEdit();
    return 0;
}

int sdf_AttributeSpec_ListInfoKeys(sdf_AttributeSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListInfoKeys();
    return 0;
}

int sdf_AttributeSpec_GetMetaDataInfoKeys(sdf_AttributeSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->GetMetaDataInfoKeys();
    return 0;
}

int sdf_AttributeSpec_GetMetaDataDisplayGroup(sdf_AttributeSpec_t const* _this, tf_Token_t const* key, tf_Token_t* _result) {
    *_result = _this->GetMetaDataDisplayGroup(*key);
    return 0;
}

int sdf_AttributeSpec_GetInfo(sdf_AttributeSpec_t const* _this, tf_Token_t const* key, vt_Value_t* _result) {
    *_result = _this->GetInfo(*key);
    return 0;
}

int sdf_AttributeSpec_SetInfo(sdf_AttributeSpec_t* _this, tf_Token_t const* key, vt_Value_t const* value) {
    _this->SetInfo(*key, *value);
    return 0;
}

int sdf_AttributeSpec_SetInfoDictionaryValue(sdf_AttributeSpec_t* _this, tf_Token_t const* dictionaryKey, tf_Token_t const* entryKey, vt_Value_t const* value) {
    _this->SetInfoDictionaryValue(*dictionaryKey, *entryKey, *value);
    return 0;
}

int sdf_AttributeSpec_HasInfo(sdf_AttributeSpec_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasInfo(*key);
    return 0;
}

int sdf_AttributeSpec_ClearInfo(sdf_AttributeSpec_t* _this, tf_Token_t const* key) {
    _this->ClearInfo(*key);
    return 0;
}

int sdf_AttributeSpec_GetTypeForInfo(sdf_AttributeSpec_t const* _this, tf_Token_t const* key, tf_Type_t* _result) {
    *_result = _this->GetTypeForInfo(*key);
    return 0;
}

int sdf_AttributeSpec_GetFallbackForInfo(sdf_AttributeSpec_t const* _this, tf_Token_t const* key, vt_Value_t const** _result) {
    *_result = &_this->GetFallbackForInfo(*key);
    return 0;
}

int sdf_AttributeSpec_IsInert(sdf_AttributeSpec_t const* _this, bool ignoreChildren, bool* _result) {
    *_result = _this->IsInert(ignoreChildren);
    return 0;
}

int sdf_AttributeSpec_ListFields(sdf_AttributeSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListFields();
    return 0;
}

int sdf_AttributeSpec_HasField(sdf_AttributeSpec_t const* _this, tf_Token_t const* name, bool* _result) {
    *_result = _this->HasField(*name);
    return 0;
}

int sdf_AttributeSpec_GetField(sdf_AttributeSpec_t const* _this, tf_Token_t const* name, vt_Value_t* _result) {
    *_result = _this->GetField(*name);
    return 0;
}

int sdf_AttributeSpec_SetField(sdf_AttributeSpec_t* _this, tf_Token_t const* name, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetField(*name, *value);
    return 0;
}

int sdf_AttributeSpec_op_eq(sdf_AttributeSpec_t const* _this, sdf_Spec_t const* rhs, bool* _result) {
    *_result = _this->operator==(*rhs);
    return 0;
}

int sdf_AttributeSpec_dtor(sdf_AttributeSpec_t* _this) {
    delete _this;
    return 0;
}

int sdf_AttributeSpecHandle_dtor(sdf_AttributeSpecHandle_t* _this) {
    delete _this;
    return 0;
}

int sdf_AttributeSpecHandleVector_data(sdf_AttributeSpecHandleVector_t* _this, sdf_AttributeSpecHandle_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_AttributeSpecHandleVector_data_const(sdf_AttributeSpecHandleVector_t const* _this, sdf_AttributeSpecHandle_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_AttributeSpecHandleVector_empty(sdf_AttributeSpecHandleVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_AttributeSpecHandleVector_size(sdf_AttributeSpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_AttributeSpecHandleVector_max_size(sdf_AttributeSpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_AttributeSpecHandleVector_reserve(sdf_AttributeSpecHandleVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_AttributeSpecHandleVector_capacity(sdf_AttributeSpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_AttributeSpecHandleVector_clear(sdf_AttributeSpecHandleVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_AttributeSpecHandleVector_push_back(sdf_AttributeSpecHandleVector_t* _this, sdf_AttributeSpecHandle_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_AttributeSpecHandleVector_pop_back(sdf_AttributeSpecHandleVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_AttributeSpecHandleVector_resize(sdf_AttributeSpecHandleVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_AttributeSpecHandleVector_resize_with(sdf_AttributeSpecHandleVector_t* _this, size_t const _Newsize, sdf_AttributeSpecHandle_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_AttributeSpecHandleVector_op_index(sdf_AttributeSpecHandleVector_t const* _this, size_t const _Pos, sdf_AttributeSpecHandle_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_AttributeSpecHandleVector_default(sdf_AttributeSpecHandleVector_t** _result) {
    *_result = new PXR_NS::SdfAttributeSpecHandleVector();
    return 0;
}

int sdf_AttributeSpecHandleVector_dtor(sdf_AttributeSpecHandleVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_AttributeSpecView_dtor(sdf_AttributeSpecView_t* _this) {
    delete _this;
    return 0;
}

int sdf_BatchNamespaceEdit_Add(sdf_BatchNamespaceEdit_t* _this, sdf_NamespaceEdit_t const* edit) {
    _this->Add(*edit);
    return 0;
}

int sdf_BatchNamespaceEdit_Add_from_paths(sdf_BatchNamespaceEdit_t* _this, sdf_Path_t const* currentPath, sdf_Path_t const* newPath, int index) {
    _this->Add(*currentPath, *newPath, index);
    return 0;
}

int sdf_BatchNamespaceEdit_GetEdits(sdf_BatchNamespaceEdit_t const* _this, sdf_NamespaceEditVector_t const** _result) {
    *_result = &_this->GetEdits();
    return 0;
}

int sdf_BatchNamespaceEdit_new(sdf_BatchNamespaceEdit_t** _result) {
    *_result = new PXR_NS::SdfBatchNamespaceEdit();
    return 0;
}

int sdf_BatchNamespaceEdit_dtor(sdf_BatchNamespaceEdit_t* _this) {
    delete _this;
    return 0;
}

int sdf_ChangeBlock_new(sdf_ChangeBlock_t** _result) {
    *_result = new PXR_NS::SdfChangeBlock();
    return 0;
}

int sdf_ChangeBlock_dtor(sdf_ChangeBlock_t* _this) {
    delete _this;
    return 0;
}

int sdf_ChangeList_GetEntryList(sdf_ChangeList_t const* _this, sdf_ChangeListEntryList_t const** _result) {
    *_result = &_this->GetEntryList();
    return 0;
}

int sdf_ChangeList_GetEntry(sdf_ChangeList_t const* _this, sdf_Path_t const* param00, sdf_ChangeListEntry_t const** _result) {
    *_result = &_this->GetEntry(*param00);
    return 0;
}

int sdf_ChangeList_FindEntry(sdf_ChangeList_t const* _this, sdf_Path_t const* param00, sdf_PathChangeListEntryPair_t const** _result) {
    *_result = _this->FindEntry(*param00);
    return 0;
}

int sdf_ChangeList_new(sdf_ChangeList_t** _result) {
    *_result = new PXR_NS::SdfChangeList();
    return 0;
}

int sdf_ChangeList_dtor(sdf_ChangeList_t* _this) {
    delete _this;
    return 0;
}

int sdf_ChangeListEntry_new(sdf_ChangeListEntry_t** _result) {
    *_result = new PXR_NS::SdfChangeList::Entry();
    return 0;
}

int sdf_ChangeListEntry_dtor(sdf_ChangeListEntry_t* _this) {
    delete _this;
    return 0;
}

int sdf_PathChangeListEntryPair_dtor(sdf_PathChangeListEntryPair_t* _this) {
    delete _this;
    return 0;
}

int sdf_ChangeListEntryList_dtor(sdf_ChangeListEntryList_t* _this) {
    delete _this;
    return 0;
}

int sdf_DictionaryProxy_dtor(sdf_DictionaryProxy_t* _this) {
    delete _this;
    return 0;
}

int sdf_FileFormat_GetSchema(sdf_FileFormat_t const* _this, sdf_SchemaBase_t const** _result) {
    *_result = &_this->GetSchema();
    return 0;
}

int sdf_FileFormat_GetFormatId(sdf_FileFormat_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetFormatId();
    return 0;
}

int sdf_FileFormat_GetTarget(sdf_FileFormat_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetTarget();
    return 0;
}

int sdf_FileFormat_GetFileCookie(sdf_FileFormat_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetFileCookie();
    return 0;
}

int sdf_FileFormat_GetVersionString(sdf_FileFormat_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetVersionString();
    return 0;
}

int sdf_FileFormat_IsPrimaryFormatForExtensions(sdf_FileFormat_t const* _this, bool* _result) {
    *_result = _this->IsPrimaryFormatForExtensions();
    return 0;
}

int sdf_FileFormat_GetFileExtensions(sdf_FileFormat_t const* _this, std_StringVector_t const** _result) {
    *_result = &_this->GetFileExtensions();
    return 0;
}

int sdf_FileFormat_GetPrimaryFileExtension(sdf_FileFormat_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetPrimaryFileExtension();
    return 0;
}

int sdf_FileFormat_IsSupportedExtension(sdf_FileFormat_t const* _this, std_String_t const* extension, bool* _result) {
    *_result = _this->IsSupportedExtension(*extension);
    return 0;
}

int sdf_FileFormat_IsPackage(sdf_FileFormat_t const* _this, bool* _result) {
    *_result = _this->IsPackage();
    return 0;
}

int sdf_FileFormat_GetPackageRootLayerPath(sdf_FileFormat_t const* _this, std_String_t const* resolvedPath, std_String_t* _result) {
    *_result = _this->GetPackageRootLayerPath(*resolvedPath);
    return 0;
}

int sdf_FileFormat_ShouldSkipAnonymousReload(sdf_FileFormat_t const* _this, bool* _result) {
    *_result = _this->ShouldSkipAnonymousReload();
    return 0;
}

int sdf_FileFormat_ShouldReadAnonymousLayers(sdf_FileFormat_t const* _this, bool* _result) {
    *_result = _this->ShouldReadAnonymousLayers();
    return 0;
}

int sdf_FileFormat_ReadDetached(sdf_FileFormat_t const* _this, sdf_Layer_t* layer, std_String_t const* resolvedPath, bool metadataOnly, bool* _result) {
    *_result = _this->ReadDetached(layer, *resolvedPath, metadataOnly);
    return 0;
}

int sdf_FileFormat_GetExternalAssetDependencies(sdf_FileFormat_t const* _this, sdf_Layer_t const* layer, std_StringSet_t* _result) {
    *_result = _this->GetExternalAssetDependencies(*layer);
    return 0;
}

int sdf_FileFormat_SupportsReading(sdf_FileFormat_t const* _this, bool* _result) {
    *_result = _this->SupportsReading();
    return 0;
}

int sdf_FileFormat_SupportsWriting(sdf_FileFormat_t const* _this, bool* _result) {
    *_result = _this->SupportsWriting();
    return 0;
}

int sdf_FileFormat_SupportsEditing(sdf_FileFormat_t const* _this, bool* _result) {
    *_result = _this->SupportsEditing();
    return 0;
}

int sdf_FileFormat_GetFileExtension(std_String_t const* s, std_String_t* _result) {
    *_result = PXR_NS::SdfFileFormat::GetFileExtension(*s);
    return 0;
}

int sdf_FileFormat_FindAllFileFormatExtensions(std_StringSet_t* _result) {
    *_result = PXR_NS::SdfFileFormat::FindAllFileFormatExtensions();
    return 0;
}

int sdf_FileFormat_FindAllDerivedFileFormatExtensions(tf_Type_t const* baseType, std_StringSet_t* _result) {
    *_result = PXR_NS::SdfFileFormat::FindAllDerivedFileFormatExtensions(*baseType);
    return 0;
}

int sdf_FileFormat_FormatSupportsReading(std_String_t const* extension, std_String_t const* target, bool* _result) {
    *_result = PXR_NS::SdfFileFormat::FormatSupportsReading(*extension, *target);
    return 0;
}

int sdf_FileFormat_FormatSupportsWriting(std_String_t const* extension, std_String_t const* target, bool* _result) {
    *_result = PXR_NS::SdfFileFormat::FormatSupportsWriting(*extension, *target);
    return 0;
}

int sdf_FileFormat_FormatSupportsEditing(std_String_t const* extension, std_String_t const* target, bool* _result) {
    *_result = PXR_NS::SdfFileFormat::FormatSupportsEditing(*extension, *target);
    return 0;
}

int sdf_FileFormat_CanRead(sdf_FileFormat_t const* _this, std_String_t const* file, bool* _result) {
    *_result = _this->CanRead(*file);
    return 0;
}

int sdf_FileFormat_Read(sdf_FileFormat_t const* _this, sdf_Layer_t* layer, std_String_t const* resolvedPath, bool metadataOnly, bool* _result) {
    *_result = _this->Read(layer, *resolvedPath, metadataOnly);
    return 0;
}

int sdf_FileFormat_WriteToFile(sdf_FileFormat_t const* _this, sdf_Layer_t const* layer, std_String_t const* filePath, std_String_t const* comment, sdf_LayerFileFormatArguments_t const* args, bool* _result) {
    *_result = _this->WriteToFile(*layer, *filePath, *comment, *args);
    return 0;
}

int sdf_FileFormat_ReadFromString(sdf_FileFormat_t const* _this, sdf_Layer_t* layer, std_String_t const* str, bool* _result) {
    *_result = _this->ReadFromString(layer, *str);
    return 0;
}

int sdf_FileFormat_WriteToString(sdf_FileFormat_t const* _this, sdf_Layer_t const* layer, std_String_t* str, std_String_t const* comment, bool* _result) {
    *_result = _this->WriteToString(*layer, str, *comment);
    return 0;
}

int sdf_InheritsProxy_dtor(sdf_InheritsProxy_t* _this) {
    delete _this;
    return 0;
}

int sdf_LayerFileFormatArguments_dtor(sdf_LayerFileFormatArguments_t* _this) {
    delete _this;
    return 0;
}

int sdf_Layer_GetExternalReferences(sdf_Layer_t const* _this, std_StringSet_t* _result) {
    *_result = _this->GetExternalReferences();
    return 0;
}

int sdf_Layer_UpdateExternalReference(sdf_Layer_t* _this, std_String_t const* oldAssetPath, std_String_t const* newAssetPath, bool* _result) {
    *_result = _this->UpdateExternalReference(*oldAssetPath, *newAssetPath);
    return 0;
}

int sdf_Layer_GetCompositionAssetDependencies(sdf_Layer_t const* _this, std_StringSet_t* _result) {
    *_result = _this->GetCompositionAssetDependencies();
    return 0;
}

int sdf_Layer_UpdateCompositionAssetDependency(sdf_Layer_t* _this, std_String_t const* oldAssetPath, std_String_t const* newAssetPath, bool* _result) {
    *_result = _this->UpdateCompositionAssetDependency(*oldAssetPath, *newAssetPath);
    return 0;
}

int sdf_Layer_GetExternalAssetDependencies(sdf_Layer_t const* _this, std_StringSet_t* _result) {
    *_result = _this->GetExternalAssetDependencies();
    return 0;
}

int sdf_Layer_GetSpecType(sdf_Layer_t const* _this, sdf_Path_t const* path, int* _result) {
    *_result = static_cast<int>(_this->GetSpecType(*path));
    return 0;
}

int sdf_Layer_HasSpec(sdf_Layer_t const* _this, sdf_Path_t const* path, bool* _result) {
    *_result = _this->HasSpec(*path);
    return 0;
}

int sdf_Layer_ListFields(sdf_Layer_t const* _this, sdf_Path_t const* path, tf_TokenVector_t* _result) {
    *_result = _this->ListFields(*path);
    return 0;
}

int sdf_Layer_HasField(sdf_Layer_t const* _this, sdf_Path_t const* path, tf_Token_t const* fieldName, vt_Value_t* value, bool* _result) {
    *_result = _this->HasField(*path, *fieldName, value);
    return 0;
}

int sdf_Layer_HasFieldDictKey(sdf_Layer_t const* _this, sdf_Path_t const* path, tf_Token_t const* fieldName, tf_Token_t const* keyPath, vt_Value_t* value, bool* _result) {
    *_result = _this->HasFieldDictKey(*path, *fieldName, *keyPath, value);
    return 0;
}

int sdf_Layer_GetField(sdf_Layer_t const* _this, sdf_Path_t const* path, tf_Token_t const* fieldName, vt_Value_t* _result) {
    *_result = _this->GetField(*path, *fieldName);
    return 0;
}

int sdf_Layer_GetFieldDictValueByKey(sdf_Layer_t const* _this, sdf_Path_t const* path, tf_Token_t const* fieldName, tf_Token_t const* keyPath, vt_Value_t* _result) {
    *_result = _this->GetFieldDictValueByKey(*path, *fieldName, *keyPath);
    return 0;
}

int sdf_Layer_SetField(sdf_Layer_t* _this, sdf_Path_t const* path, tf_Token_t const* fieldName, vt_Value_t const* value) {
    _this->SetField(*path, *fieldName, *value);
    return 0;
}

int sdf_Layer_SetFieldDictValueByKey(sdf_Layer_t* _this, sdf_Path_t const* path, tf_Token_t const* fieldName, tf_Token_t const* keyPath, vt_Value_t const* value) {
    _this->SetFieldDictValueByKey(*path, *fieldName, *keyPath, *value);
    return 0;
}

int sdf_Layer_EraseField(sdf_Layer_t* _this, sdf_Path_t const* path, tf_Token_t const* fieldName) {
    _this->EraseField(*path, *fieldName);
    return 0;
}

int sdf_Layer_EraseFieldDictValueByKey(sdf_Layer_t* _this, sdf_Path_t const* path, tf_Token_t const* fieldName, tf_Token_t const* keyPath) {
    _this->EraseFieldDictValueByKey(*path, *fieldName, *keyPath);
    return 0;
}

int sdf_Layer_GetColorConfiguration(sdf_Layer_t const* _this, sdf_AssetPath_t* _result) {
    *_result = _this->GetColorConfiguration();
    return 0;
}

int sdf_Layer_SetColorConfiguration(sdf_Layer_t* _this, sdf_AssetPath_t const* colorConfiguration) {
    _this->SetColorConfiguration(*colorConfiguration);
    return 0;
}

int sdf_Layer_HasColorConfiguration(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->HasColorConfiguration();
    return 0;
}

int sdf_Layer_ClearColorConfiguration(sdf_Layer_t* _this) {
    _this->ClearColorConfiguration();
    return 0;
}

int sdf_Layer_GetColorManagementSystem(sdf_Layer_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetColorManagementSystem();
    return 0;
}

int sdf_Layer_SetColorManagementSystem(sdf_Layer_t* _this, tf_Token_t const* cms) {
    _this->SetColorManagementSystem(*cms);
    return 0;
}

int sdf_Layer_HasColorManagementSystem(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->HasColorManagementSystem();
    return 0;
}

int sdf_Layer_ClearColorManagementSystem(sdf_Layer_t* _this) {
    _this->ClearColorManagementSystem();
    return 0;
}

int sdf_Layer_GetComment(sdf_Layer_t const* _this, std_String_t* _result) {
    *_result = _this->GetComment();
    return 0;
}

int sdf_Layer_SetComment(sdf_Layer_t* _this, std_String_t const* comment) {
    _this->SetComment(*comment);
    return 0;
}

int sdf_Layer_GetDefaultPrim(sdf_Layer_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetDefaultPrim();
    return 0;
}

int sdf_Layer_SetDefaultPrim(sdf_Layer_t* _this, tf_Token_t const* name) {
    _this->SetDefaultPrim(*name);
    return 0;
}

int sdf_Layer_HasDefaultPrim(sdf_Layer_t* _this, bool* _result) {
    *_result = _this->HasDefaultPrim();
    return 0;
}

int sdf_Layer_ClearDefaultPrim(sdf_Layer_t* _this) {
    _this->ClearDefaultPrim();
    return 0;
}

int sdf_Layer_GetDocumentation(sdf_Layer_t const* _this, std_String_t* _result) {
    *_result = _this->GetDocumentation();
    return 0;
}

int sdf_Layer_SetDocumentation(sdf_Layer_t* _this, std_String_t const* documentation) {
    _this->SetDocumentation(*documentation);
    return 0;
}

int sdf_Layer_GetStartTimeCode(sdf_Layer_t const* _this, double* _result) {
    *_result = _this->GetStartTimeCode();
    return 0;
}

int sdf_Layer_SetStartTimeCode(sdf_Layer_t* _this, double startTimecode) {
    _this->SetStartTimeCode(startTimecode);
    return 0;
}

int sdf_Layer_HasStartTimeCode(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->HasStartTimeCode();
    return 0;
}

int sdf_Layer_ClearStartTimeCode(sdf_Layer_t* _this) {
    _this->ClearStartTimeCode();
    return 0;
}

int sdf_Layer_GetEndTimeCode(sdf_Layer_t const* _this, double* _result) {
    *_result = _this->GetEndTimeCode();
    return 0;
}

int sdf_Layer_SetEndTimeCode(sdf_Layer_t* _this, double endTimeCode) {
    _this->SetEndTimeCode(endTimeCode);
    return 0;
}

int sdf_Layer_HasEndTimeCode(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->HasEndTimeCode();
    return 0;
}

int sdf_Layer_ClearEndTimeCode(sdf_Layer_t* _this) {
    _this->ClearEndTimeCode();
    return 0;
}

int sdf_Layer_GetTimeCodesPerSecond(sdf_Layer_t const* _this, double* _result) {
    *_result = _this->GetTimeCodesPerSecond();
    return 0;
}

int sdf_Layer_SetTimeCodesPerSecond(sdf_Layer_t* _this, double timeCodesPerSecond) {
    _this->SetTimeCodesPerSecond(timeCodesPerSecond);
    return 0;
}

int sdf_Layer_HasTimeCodesPerSecond(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->HasTimeCodesPerSecond();
    return 0;
}

int sdf_Layer_ClearTimeCodesPerSecond(sdf_Layer_t* _this) {
    _this->ClearTimeCodesPerSecond();
    return 0;
}

int sdf_Layer_GetFramesPerSecond(sdf_Layer_t const* _this, double* _result) {
    *_result = _this->GetFramesPerSecond();
    return 0;
}

int sdf_Layer_SetFramesPerSecond(sdf_Layer_t* _this, double framesPerSecond) {
    _this->SetFramesPerSecond(framesPerSecond);
    return 0;
}

int sdf_Layer_HasFramesPerSecond(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->HasFramesPerSecond();
    return 0;
}

int sdf_Layer_ClearFramesPerSecond(sdf_Layer_t* _this) {
    _this->ClearFramesPerSecond();
    return 0;
}

int sdf_Layer_GetFramePrecision(sdf_Layer_t const* _this, int* _result) {
    *_result = _this->GetFramePrecision();
    return 0;
}

int sdf_Layer_SetFramePrecision(sdf_Layer_t* _this, int framePrecision) {
    _this->SetFramePrecision(framePrecision);
    return 0;
}

int sdf_Layer_HasFramePrecision(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->HasFramePrecision();
    return 0;
}

int sdf_Layer_ClearFramePrecision(sdf_Layer_t* _this) {
    _this->ClearFramePrecision();
    return 0;
}

int sdf_Layer_GetOwner(sdf_Layer_t const* _this, std_String_t* _result) {
    *_result = _this->GetOwner();
    return 0;
}

int sdf_Layer_SetOwner(sdf_Layer_t* _this, std_String_t const* owner) {
    _this->SetOwner(*owner);
    return 0;
}

int sdf_Layer_HasOwner(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->HasOwner();
    return 0;
}

int sdf_Layer_ClearOwner(sdf_Layer_t* _this) {
    _this->ClearOwner();
    return 0;
}

int sdf_Layer_GetSessionOwner(sdf_Layer_t const* _this, std_String_t* _result) {
    *_result = _this->GetSessionOwner();
    return 0;
}

int sdf_Layer_SetSessionOwner(sdf_Layer_t* _this, std_String_t const* owner) {
    _this->SetSessionOwner(*owner);
    return 0;
}

int sdf_Layer_HasSessionOwner(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->HasSessionOwner();
    return 0;
}

int sdf_Layer_ClearSessionOwner(sdf_Layer_t* _this) {
    _this->ClearSessionOwner();
    return 0;
}

int sdf_Layer_GetHasOwnedSubLayers(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->GetHasOwnedSubLayers();
    return 0;
}

int sdf_Layer_SetHasOwnedSubLayers(sdf_Layer_t* _this, bool param00) {
    _this->SetHasOwnedSubLayers(param00);
    return 0;
}

int sdf_Layer_GetCustomLayerData(sdf_Layer_t const* _this, vt_Dictionary_t* _result) {
    *_result = _this->GetCustomLayerData();
    return 0;
}

int sdf_Layer_SetCustomLayerData(sdf_Layer_t* _this, vt_Dictionary_t const* value) {
    _this->SetCustomLayerData(*value);
    return 0;
}

int sdf_Layer_HasCustomLayerData(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->HasCustomLayerData();
    return 0;
}

int sdf_Layer_ClearCustomLayerData(sdf_Layer_t* _this) {
    _this->ClearCustomLayerData();
    return 0;
}

int sdf_Layer_GetExpressionVariables(sdf_Layer_t const* _this, vt_Dictionary_t* _result) {
    *_result = _this->GetExpressionVariables();
    return 0;
}

int sdf_Layer_SetExpressionVariables(sdf_Layer_t* _this, vt_Dictionary_t const* expressionVars) {
    _this->SetExpressionVariables(*expressionVars);
    return 0;
}

int sdf_Layer_HasExpressionVariables(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->HasExpressionVariables();
    return 0;
}

int sdf_Layer_ClearExpressionVariables(sdf_Layer_t* _this) {
    _this->ClearExpressionVariables();
    return 0;
}

int sdf_Layer_GetSubLayerPaths(sdf_Layer_t const* _this, sdf_SubLayerProxy_t* _result) {
    *_result = _this->GetSubLayerPaths();
    return 0;
}

int sdf_Layer_SetSubLayerPaths(sdf_Layer_t* _this, std_StringVector_t const* newPaths) {
    _this->SetSubLayerPaths(*newPaths);
    return 0;
}

int sdf_Layer_GetNumSubLayerPaths(sdf_Layer_t const* _this, size_t* _result) {
    *_result = _this->GetNumSubLayerPaths();
    return 0;
}

int sdf_Layer_InsertSubLayerPath(sdf_Layer_t* _this, std_String_t const* path, int index) {
    _this->InsertSubLayerPath(*path, index);
    return 0;
}

int sdf_Layer_RemoveSubLayerPath(sdf_Layer_t* _this, int index) {
    _this->RemoveSubLayerPath(index);
    return 0;
}

int sdf_Layer_GetSubLayerOffsets(sdf_Layer_t const* _this, sdf_LayerOffsetVector_t* _result) {
    *_result = _this->GetSubLayerOffsets();
    return 0;
}

int sdf_Layer_GetSubLayerOffset(sdf_Layer_t const* _this, int index, sdf_LayerOffset_t* _result) {
    *_result = _this->GetSubLayerOffset(index);
    return 0;
}

int sdf_Layer_SetSubLayerOffset(sdf_Layer_t* _this, sdf_LayerOffset_t const* offset, int index) {
    _this->SetSubLayerOffset(*offset, index);
    return 0;
}

int sdf_Layer_GetPseudoRoot(sdf_Layer_t const* _this, sdf_PrimSpecHandle_t* _result) {
    *_result = _this->GetPseudoRoot();
    return 0;
}

int sdf_Layer_GetObjectAtPath(sdf_Layer_t* _this, sdf_Path_t const* path, sdf_SpecHandle_t* _result) {
    *_result = _this->GetObjectAtPath(*path);
    return 0;
}

int sdf_Layer_GetPrimAtPath(sdf_Layer_t* _this, sdf_Path_t const* path, sdf_PrimSpecHandle_t* _result) {
    *_result = _this->GetPrimAtPath(*path);
    return 0;
}

int sdf_Layer_GetPropertyAtPath(sdf_Layer_t* _this, sdf_Path_t const* path, sdf_PropertySpecHandle_t* _result) {
    *_result = _this->GetPropertyAtPath(*path);
    return 0;
}

int sdf_Layer_GetAttributeAtPath(sdf_Layer_t* _this, sdf_Path_t const* path, sdf_AttributeSpecHandle_t* _result) {
    *_result = _this->GetAttributeAtPath(*path);
    return 0;
}

int sdf_Layer_GetRelationshipAtPath(sdf_Layer_t* _this, sdf_Path_t const* path, sdf_RelationshipSpecHandle_t* _result) {
    *_result = _this->GetRelationshipAtPath(*path);
    return 0;
}

int sdf_Layer_PermissionToEdit(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->PermissionToEdit();
    return 0;
}

int sdf_Layer_PermissionToSave(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->PermissionToSave();
    return 0;
}

int sdf_Layer_SetPermissionToEdit(sdf_Layer_t* _this, bool allow) {
    _this->SetPermissionToEdit(allow);
    return 0;
}

int sdf_Layer_SetPermissionToSave(sdf_Layer_t* _this, bool allow) {
    _this->SetPermissionToSave(allow);
    return 0;
}

int sdf_Layer_CanApply(sdf_Layer_t const* _this, sdf_BatchNamespaceEdit_t const* param00, sdf_NamespaceEditDetailVector_t* details, int* _result) {
    *_result = static_cast<int>(_this->CanApply(*param00, details));
    return 0;
}

int sdf_Layer_Apply(sdf_Layer_t* _this, sdf_BatchNamespaceEdit_t const* param00, bool* _result) {
    *_result = _this->Apply(*param00);
    return 0;
}

int sdf_Layer_IsDirty(sdf_Layer_t const* _this, bool* _result) {
    *_result = _this->IsDirty();
    return 0;
}

int sdf_Layer_ListAllTimeSamples(sdf_Layer_t const* _this, std_DoubleSet_t* _result) {
    *_result = _this->ListAllTimeSamples();
    return 0;
}

int sdf_Layer_ListTimeSamplesForPath(sdf_Layer_t const* _this, sdf_Path_t const* path, std_DoubleSet_t* _result) {
    *_result = _this->ListTimeSamplesForPath(*path);
    return 0;
}

int sdf_Layer_GetBracketingTimeSamplesForPath(sdf_Layer_t* _this, sdf_Path_t const* path, double time, double* tLower, double* tUpper, bool* _result) {
    *_result = _this->GetBracketingTimeSamplesForPath(*path, time, tLower, tUpper);
    return 0;
}

int sdf_Layer_GetBracketingTimeSamples(sdf_Layer_t* _this, double time, double* tLower, double* tUpper, bool* _result) {
    *_result = _this->GetBracketingTimeSamples(time, tLower, tUpper);
    return 0;
}

int sdf_Layer_GetNumTimeSamplesForPath(sdf_Layer_t const* _this, sdf_Path_t const* path, size_t* _result) {
    *_result = _this->GetNumTimeSamplesForPath(*path);
    return 0;
}

int sdf_Layer_QueryTimeSample(sdf_Layer_t const* _this, sdf_Path_t const* path, double time, vt_Value_t* value, bool* _result) {
    *_result = _this->QueryTimeSample(*path, time, value);
    return 0;
}

int sdf_Layer_SetTimeSample(sdf_Layer_t* _this, sdf_Path_t const* path, double time, vt_Value_t const* value) {
    _this->SetTimeSample(*path, time, *value);
    return 0;
}

int sdf_Layer_EraseTimeSample(sdf_Layer_t* _this, sdf_Path_t const* path, double time) {
    _this->EraseTimeSample(*path, time);
    return 0;
}

int sdf_Layer_DumpLayerInfo() {
    PXR_NS::SdfLayer::DumpLayerInfo();
    return 0;
}

int sdf_Layer_dtor(sdf_Layer_t* _this) {
    delete _this;
    return 0;
}

int sdf_LayerHandle_dtor(sdf_LayerHandle_t* _this) {
    delete _this;
    return 0;
}

int sdf_LayerHandleVector_data(sdf_LayerHandleVector_t* _this, sdf_LayerHandle_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_LayerHandleVector_data_const(sdf_LayerHandleVector_t const* _this, sdf_LayerHandle_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_LayerHandleVector_empty(sdf_LayerHandleVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_LayerHandleVector_size(sdf_LayerHandleVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_LayerHandleVector_max_size(sdf_LayerHandleVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_LayerHandleVector_reserve(sdf_LayerHandleVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_LayerHandleVector_capacity(sdf_LayerHandleVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_LayerHandleVector_clear(sdf_LayerHandleVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_LayerHandleVector_push_back(sdf_LayerHandleVector_t* _this, sdf_LayerHandle_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_LayerHandleVector_pop_back(sdf_LayerHandleVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_LayerHandleVector_resize(sdf_LayerHandleVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_LayerHandleVector_resize_with(sdf_LayerHandleVector_t* _this, size_t const _Newsize, sdf_LayerHandle_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_LayerHandleVector_op_index(sdf_LayerHandleVector_t const* _this, size_t const _Pos, sdf_LayerHandle_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_LayerHandleVector_default(sdf_LayerHandleVector_t** _result) {
    *_result = new PXR_NS::SdfLayerHandleVector();
    return 0;
}

int sdf_LayerHandleVector_dtor(sdf_LayerHandleVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_LayerOffset_GetOffset(sdf_LayerOffset_t const* _this, double* _result) {
    *_result = _this->GetOffset();
    return 0;
}

int sdf_LayerOffset_GetScale(sdf_LayerOffset_t const* _this, double* _result) {
    *_result = _this->GetScale();
    return 0;
}

int sdf_LayerOffset_SetOffset(sdf_LayerOffset_t* _this, double newOffset) {
    _this->SetOffset(newOffset);
    return 0;
}

int sdf_LayerOffset_SetScale(sdf_LayerOffset_t* _this, double newScale) {
    _this->SetScale(newScale);
    return 0;
}

int sdf_LayerOffset_IsIdentity(sdf_LayerOffset_t const* _this, bool* _result) {
    *_result = _this->IsIdentity();
    return 0;
}

int sdf_LayerOffset_IsValid(sdf_LayerOffset_t const* _this, bool* _result) {
    *_result = _this->IsValid();
    return 0;
}

int sdf_LayerOffset_GetInverse(sdf_LayerOffset_t const* _this, sdf_LayerOffset_t* _result) {
    *_result = _this->GetInverse();
    return 0;
}

int sdf_LayerOffset_op_eq(sdf_LayerOffset_t const* _this, sdf_LayerOffset_t const* rhs, bool* _result) {
    *_result = _this->operator==(*rhs);
    return 0;
}

int sdf_LayerOffset_op_lt(sdf_LayerOffset_t const* _this, sdf_LayerOffset_t const* rhs, bool* _result) {
    *_result = _this->operator<(*rhs);
    return 0;
}

int sdf_LayerOffset_apply(sdf_LayerOffset_t const* _this, sdf_LayerOffset_t const* rhs, sdf_LayerOffset_t* _result) {
    *_result = _this->operator*(*rhs);
    return 0;
}

int sdf_LayerOffset_apply_to_double(sdf_LayerOffset_t const* _this, double rhs, double* _result) {
    *_result = _this->operator*(rhs);
    return 0;
}

int sdf_LayerOffset_apply_to_timecode(sdf_LayerOffset_t const* _this, sdf_TimeCode_t const* rhs, sdf_TimeCode_t* _result) {
    *_result = _this->operator*(*rhs);
    return 0;
}

int sdf_LayerOffset_new(double offset, double scale, sdf_LayerOffset_t** _result) {
    *_result = new PXR_NS::SdfLayerOffset(offset, scale);
    return 0;
}

int sdf_LayerOffset_dtor(sdf_LayerOffset_t* _this) {
    delete _this;
    return 0;
}

int sdf_LayerOffsetVector_data(sdf_LayerOffsetVector_t* _this, sdf_LayerOffset_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_LayerOffsetVector_data_const(sdf_LayerOffsetVector_t const* _this, sdf_LayerOffset_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_LayerOffsetVector_empty(sdf_LayerOffsetVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_LayerOffsetVector_size(sdf_LayerOffsetVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_LayerOffsetVector_max_size(sdf_LayerOffsetVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_LayerOffsetVector_reserve(sdf_LayerOffsetVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_LayerOffsetVector_capacity(sdf_LayerOffsetVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_LayerOffsetVector_clear(sdf_LayerOffsetVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_LayerOffsetVector_push_back(sdf_LayerOffsetVector_t* _this, sdf_LayerOffset_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_LayerOffsetVector_pop_back(sdf_LayerOffsetVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_LayerOffsetVector_resize(sdf_LayerOffsetVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_LayerOffsetVector_resize_with(sdf_LayerOffsetVector_t* _this, size_t const _Newsize, sdf_LayerOffset_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_LayerOffsetVector_op_index(sdf_LayerOffsetVector_t const* _this, size_t const _Pos, sdf_LayerOffset_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_LayerOffsetVector_default(sdf_LayerOffsetVector_t** _result) {
    *_result = new PXR_NS::SdfLayerOffsetVector();
    return 0;
}

int sdf_LayerOffsetVector_dtor(sdf_LayerOffsetVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_LayerRefPtr_dtor(sdf_LayerRefPtr_t* _this) {
    delete _this;
    return 0;
}

int sdf_LayerTree_GetLayer(sdf_LayerTree_t const* _this, sdf_LayerHandle_t const** _result) {
    *_result = &_this->GetLayer();
    return 0;
}

int sdf_LayerTree_GetOffset(sdf_LayerTree_t const* _this, sdf_LayerOffset_t const** _result) {
    *_result = &_this->GetOffset();
    return 0;
}

int sdf_LayerTree_GetChildTrees(sdf_LayerTree_t const* _this, sdf_LayerTreeHandleVector_t const** _result) {
    *_result = &_this->GetChildTrees();
    return 0;
}

int sdf_LayerTree_dtor(sdf_LayerTree_t* _this) {
    delete _this;
    return 0;
}

int sdf_LayerTreeHandle_dtor(sdf_LayerTreeHandle_t* _this) {
    delete _this;
    return 0;
}

int sdf_LayerTreeHandleVector_data(sdf_LayerTreeHandleVector_t* _this, sdf_LayerTreeHandle_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_LayerTreeHandleVector_data_const(sdf_LayerTreeHandleVector_t const* _this, sdf_LayerTreeHandle_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_LayerTreeHandleVector_empty(sdf_LayerTreeHandleVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_LayerTreeHandleVector_size(sdf_LayerTreeHandleVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_LayerTreeHandleVector_max_size(sdf_LayerTreeHandleVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_LayerTreeHandleVector_reserve(sdf_LayerTreeHandleVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_LayerTreeHandleVector_capacity(sdf_LayerTreeHandleVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_LayerTreeHandleVector_clear(sdf_LayerTreeHandleVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_LayerTreeHandleVector_push_back(sdf_LayerTreeHandleVector_t* _this, sdf_LayerTreeHandle_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_LayerTreeHandleVector_pop_back(sdf_LayerTreeHandleVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_LayerTreeHandleVector_resize(sdf_LayerTreeHandleVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_LayerTreeHandleVector_resize_with(sdf_LayerTreeHandleVector_t* _this, size_t const _Newsize, sdf_LayerTreeHandle_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_LayerTreeHandleVector_op_index(sdf_LayerTreeHandleVector_t const* _this, size_t const _Pos, sdf_LayerTreeHandle_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_LayerTreeHandleVector_default(sdf_LayerTreeHandleVector_t** _result) {
    *_result = new PXR_NS::SdfLayerTreeHandleVector();
    return 0;
}

int sdf_LayerTreeHandleVector_dtor(sdf_LayerTreeHandleVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_StringListOp_HasKeys(sdf_StringListOp_t const* _this, bool* _result) {
    *_result = _this->HasKeys();
    return 0;
}

int sdf_StringListOp_IsExplicit(sdf_StringListOp_t const* _this, bool* _result) {
    *_result = _this->IsExplicit();
    return 0;
}

int sdf_StringListOp_GetExplicitItems(sdf_StringListOp_t const* _this, std_StringVector_t const** _result) {
    *_result = &_this->GetExplicitItems();
    return 0;
}

int sdf_StringListOp_GetAddedItems(sdf_StringListOp_t const* _this, std_StringVector_t const** _result) {
    *_result = &_this->GetAddedItems();
    return 0;
}

int sdf_StringListOp_GetPrependedItems(sdf_StringListOp_t const* _this, std_StringVector_t const** _result) {
    *_result = &_this->GetPrependedItems();
    return 0;
}

int sdf_StringListOp_GetAppendedItems(sdf_StringListOp_t const* _this, std_StringVector_t const** _result) {
    *_result = &_this->GetAppendedItems();
    return 0;
}

int sdf_StringListOp_GetDeletedItems(sdf_StringListOp_t const* _this, std_StringVector_t const** _result) {
    *_result = &_this->GetDeletedItems();
    return 0;
}

int sdf_StringListOp_GetOrderedItems(sdf_StringListOp_t const* _this, std_StringVector_t const** _result) {
    *_result = &_this->GetOrderedItems();
    return 0;
}

int sdf_StringListOp_GetAppliedItems(sdf_StringListOp_t const* _this, std_StringVector_t* _result) {
    *_result = _this->GetAppliedItems();
    return 0;
}

int sdf_StringListOp_new(sdf_StringListOp_t** _result) {
    *_result = new PXR_NS::SdfListOp<std::string>();
    return 0;
}

int sdf_StringListOp_dtor(sdf_StringListOp_t* _this) {
    delete _this;
    return 0;
}

int sdf_NameChildrenOrderProxy_dtor(sdf_NameChildrenOrderProxy_t* _this) {
    delete _this;
    return 0;
}

int sdf_NameEditorProxy_dtor(sdf_NameEditorProxy_t* _this) {
    delete _this;
    return 0;
}

int sdf_NamespaceEdit_Remove(sdf_Path_t const* currentPath, sdf_NamespaceEdit_t* _result) {
    *_result = PXR_NS::SdfNamespaceEdit::Remove(*currentPath);
    return 0;
}

int sdf_NamespaceEdit_Rename(sdf_Path_t const* currentPath, tf_Token_t const* name, sdf_NamespaceEdit_t* _result) {
    *_result = PXR_NS::SdfNamespaceEdit::Rename(*currentPath, *name);
    return 0;
}

int sdf_NamespaceEdit_Reorder(sdf_Path_t const* currentPath, int index, sdf_NamespaceEdit_t* _result) {
    *_result = PXR_NS::SdfNamespaceEdit::Reorder(*currentPath, index);
    return 0;
}

int sdf_NamespaceEdit_Reparent(sdf_Path_t const* currentPath, sdf_Path_t const* newParentPath, int index, sdf_NamespaceEdit_t* _result) {
    *_result = PXR_NS::SdfNamespaceEdit::Reparent(*currentPath, *newParentPath, index);
    return 0;
}

int sdf_NamespaceEdit_ReparentAndRename(sdf_Path_t const* currentPath, sdf_Path_t const* newParentPath, tf_Token_t const* name, int index, sdf_NamespaceEdit_t* _result) {
    *_result = PXR_NS::SdfNamespaceEdit::ReparentAndRename(*currentPath, *newParentPath, *name, index);
    return 0;
}

int sdf_NamespaceEdit_from_paths(sdf_NamespaceEdit_t** _result) {
    *_result = new PXR_NS::SdfNamespaceEdit();
    return 0;
}

int sdf_NamespaceEdit_new(sdf_NamespaceEdit_t** _result) {
    *_result = new PXR_NS::SdfNamespaceEdit();
    return 0;
}

int sdf_NamespaceEdit_dtor(sdf_NamespaceEdit_t* _this) {
    delete _this;
    return 0;
}

int sdf_NamespaceEditVector_data(sdf_NamespaceEditVector_t* _this, sdf_NamespaceEdit_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_NamespaceEditVector_data_const(sdf_NamespaceEditVector_t const* _this, sdf_NamespaceEdit_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_NamespaceEditVector_empty(sdf_NamespaceEditVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_NamespaceEditVector_size(sdf_NamespaceEditVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_NamespaceEditVector_max_size(sdf_NamespaceEditVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_NamespaceEditVector_reserve(sdf_NamespaceEditVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_NamespaceEditVector_capacity(sdf_NamespaceEditVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_NamespaceEditVector_clear(sdf_NamespaceEditVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_NamespaceEditVector_push_back(sdf_NamespaceEditVector_t* _this, sdf_NamespaceEdit_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_NamespaceEditVector_pop_back(sdf_NamespaceEditVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_NamespaceEditVector_resize(sdf_NamespaceEditVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_NamespaceEditVector_resize_with(sdf_NamespaceEditVector_t* _this, size_t const _Newsize, sdf_NamespaceEdit_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_NamespaceEditVector_op_index(sdf_NamespaceEditVector_t const* _this, size_t const _Pos, sdf_NamespaceEdit_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_NamespaceEditVector_default(sdf_NamespaceEditVector_t** _result) {
    *_result = new std::vector<PXR_NS::SdfNamespaceEdit>();
    return 0;
}

int sdf_NamespaceEditVector_dtor(sdf_NamespaceEditVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_NamespaceEditDetail_dtor(sdf_NamespaceEditDetail_t* _this) {
    delete _this;
    return 0;
}

int sdf_NamespaceEditDetailVector_data(sdf_NamespaceEditDetailVector_t* _this, sdf_NamespaceEditDetail_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_NamespaceEditDetailVector_data_const(sdf_NamespaceEditDetailVector_t const* _this, sdf_NamespaceEditDetail_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_NamespaceEditDetailVector_empty(sdf_NamespaceEditDetailVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_NamespaceEditDetailVector_size(sdf_NamespaceEditDetailVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_NamespaceEditDetailVector_max_size(sdf_NamespaceEditDetailVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_NamespaceEditDetailVector_reserve(sdf_NamespaceEditDetailVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_NamespaceEditDetailVector_capacity(sdf_NamespaceEditDetailVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_NamespaceEditDetailVector_clear(sdf_NamespaceEditDetailVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_NamespaceEditDetailVector_push_back(sdf_NamespaceEditDetailVector_t* _this, sdf_NamespaceEditDetail_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_NamespaceEditDetailVector_pop_back(sdf_NamespaceEditDetailVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_NamespaceEditDetailVector_resize(sdf_NamespaceEditDetailVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_NamespaceEditDetailVector_resize_with(sdf_NamespaceEditDetailVector_t* _this, size_t const _Newsize, sdf_NamespaceEditDetail_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_NamespaceEditDetailVector_op_index(sdf_NamespaceEditDetailVector_t const* _this, size_t const _Pos, sdf_NamespaceEditDetail_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_NamespaceEditDetailVector_default(sdf_NamespaceEditDetailVector_t** _result) {
    *_result = new PXR_NS::SdfNamespaceEditDetailVector();
    return 0;
}

int sdf_NamespaceEditDetailVector_dtor(sdf_NamespaceEditDetailVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_NoticeBase_dtor(sdf_NoticeBase_t* _this) {
    delete _this;
    return 0;
}

int sdf_NoticeBaseLayersDidChange_dtor(sdf_NoticeBaseLayersDidChange_t* _this) {
    delete _this;
    return 0;
}

int sdf_NoticeLayerDidReloadContent_dtor(sdf_NoticeLayerDidReloadContent_t* _this) {
    delete _this;
    return 0;
}

int sdf_NoticeLayerDidReplaceContent_dtor(sdf_NoticeLayerDidReplaceContent_t* _this) {
    delete _this;
    return 0;
}

int sdf_NoticeLayerDidSaveLayerToFile_dtor(sdf_NoticeLayerDidSaveLayerToFile_t* _this) {
    delete _this;
    return 0;
}

int sdf_NoticeLayerIdentifierDidChange_dtor(sdf_NoticeLayerIdentifierDidChange_t* _this) {
    delete _this;
    return 0;
}

int sdf_NoticeLayerInfoDidChange_dtor(sdf_NoticeLayerInfoDidChange_t* _this) {
    delete _this;
    return 0;
}

int sdf_NoticeLayerMutenessChanged_dtor(sdf_NoticeLayerMutenessChanged_t* _this) {
    delete _this;
    return 0;
}

int sdf_NoticeLayersDidChange_dtor(sdf_NoticeLayersDidChange_t* _this) {
    delete _this;
    return 0;
}

int sdf_NoticeLayersDidChangeSentPerLayer_dtor(sdf_NoticeLayersDidChangeSentPerLayer_t* _this) {
    delete _this;
    return 0;
}

int sdf_Path_GetPathElementCount(sdf_Path_t const* _this, size_t* _result) {
    *_result = _this->GetPathElementCount();
    return 0;
}

int sdf_Path_IsAbsolutePath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsAbsolutePath();
    return 0;
}

int sdf_Path_IsAbsoluteRootPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsAbsoluteRootPath();
    return 0;
}

int sdf_Path_IsPrimPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsPrimPath();
    return 0;
}

int sdf_Path_IsAbsoluteRootOrPrimPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsAbsoluteRootOrPrimPath();
    return 0;
}

int sdf_Path_IsRootPrimPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsRootPrimPath();
    return 0;
}

int sdf_Path_IsPropertyPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsPropertyPath();
    return 0;
}

int sdf_Path_IsPrimPropertyPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsPrimPropertyPath();
    return 0;
}

int sdf_Path_IsNamespacedPropertyPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsNamespacedPropertyPath();
    return 0;
}

int sdf_Path_IsPrimVariantSelectionPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsPrimVariantSelectionPath();
    return 0;
}

int sdf_Path_IsPrimOrPrimVariantSelectionPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsPrimOrPrimVariantSelectionPath();
    return 0;
}

int sdf_Path_ContainsPrimVariantSelection(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->ContainsPrimVariantSelection();
    return 0;
}

int sdf_Path_ContainsPropertyElements(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->ContainsPropertyElements();
    return 0;
}

int sdf_Path_ContainsTargetPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->ContainsTargetPath();
    return 0;
}

int sdf_Path_IsRelationalAttributePath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsRelationalAttributePath();
    return 0;
}

int sdf_Path_IsTargetPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsTargetPath();
    return 0;
}

int sdf_Path_IsMapperPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsMapperPath();
    return 0;
}

int sdf_Path_IsMapperArgPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsMapperArgPath();
    return 0;
}

int sdf_Path_IsExpressionPath(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsExpressionPath();
    return 0;
}

int sdf_Path_IsEmpty(sdf_Path_t const* _this, bool* _result) {
    *_result = _this->IsEmpty();
    return 0;
}

int sdf_Path_GetAsToken(sdf_Path_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetAsToken();
    return 0;
}

int sdf_Path_GetAsString(sdf_Path_t const* _this, std_String_t* _result) {
    *_result = _this->GetAsString();
    return 0;
}

int sdf_Path_GetString(sdf_Path_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetString();
    return 0;
}

int sdf_Path_GetText(sdf_Path_t const* _this, char const** _result) {
    *_result = _this->GetText();
    return 0;
}

int sdf_Path_GetPrefixes(sdf_Path_t const* _this, sdf_PathVector_t* _result) {
    *_result = _this->GetPrefixes();
    return 0;
}

int sdf_Path_GetPrefixes_up_to(sdf_Path_t const* _this, size_t numPrefixes, sdf_PathVector_t* _result) {
    *_result = _this->GetPrefixes(numPrefixes);
    return 0;
}

int sdf_Path_GetPrefixes_in(sdf_Path_t const* _this, sdf_PathVector_t* prefixes) {
    _this->GetPrefixes(prefixes);
    return 0;
}

int sdf_Path_GetPrefixes_in_up_to(sdf_Path_t const* _this, sdf_PathVector_t* prefixes, size_t numPrefixes) {
    _this->GetPrefixes(prefixes, numPrefixes);
    return 0;
}

int sdf_Path_GetName(sdf_Path_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int sdf_Path_GetNameToken(sdf_Path_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetNameToken();
    return 0;
}

int sdf_Path_GetElementString(sdf_Path_t const* _this, std_String_t* _result) {
    *_result = _this->GetElementString();
    return 0;
}

int sdf_Path_GetElementToken(sdf_Path_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetElementToken();
    return 0;
}

int sdf_Path_ReplaceName(sdf_Path_t const* _this, tf_Token_t const* newName, sdf_Path_t* _result) {
    *_result = _this->ReplaceName(*newName);
    return 0;
}

int sdf_Path_GetTargetPath(sdf_Path_t const* _this, sdf_Path_t const** _result) {
    *_result = &_this->GetTargetPath();
    return 0;
}

int sdf_Path_GetAllTargetPathsRecursively(sdf_Path_t const* _this, sdf_PathVector_t* result) {
    _this->GetAllTargetPathsRecursively(result);
    return 0;
}

int sdf_Path_HasPrefix(sdf_Path_t const* _this, sdf_Path_t const* prefix, bool* _result) {
    *_result = _this->HasPrefix(*prefix);
    return 0;
}

int sdf_Path_GetParentPath(sdf_Path_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetParentPath();
    return 0;
}

int sdf_Path_GetPrimPath(sdf_Path_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPrimPath();
    return 0;
}

int sdf_Path_GetPrimOrPrimVariantSelectionPath(sdf_Path_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPrimOrPrimVariantSelectionPath();
    return 0;
}

int sdf_Path_GetAbsoluteRootOrPrimPath(sdf_Path_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetAbsoluteRootOrPrimPath();
    return 0;
}

int sdf_Path_StripAllVariantSelections(sdf_Path_t const* _this, sdf_Path_t* _result) {
    *_result = _this->StripAllVariantSelections();
    return 0;
}

int sdf_Path_AppendPath(sdf_Path_t const* _this, sdf_Path_t const* newSuffix, sdf_Path_t* _result) {
    *_result = _this->AppendPath(*newSuffix);
    return 0;
}

int sdf_Path_AppendChild(sdf_Path_t const* _this, tf_Token_t const* childName, sdf_Path_t* _result) {
    *_result = _this->AppendChild(*childName);
    return 0;
}

int sdf_Path_AppendProperty(sdf_Path_t const* _this, tf_Token_t const* propName, sdf_Path_t* _result) {
    *_result = _this->AppendProperty(*propName);
    return 0;
}

int sdf_Path_AppendVariantSelection(sdf_Path_t const* _this, std_String_t const* variantSet, std_String_t const* variant, sdf_Path_t* _result) {
    *_result = _this->AppendVariantSelection(*variantSet, *variant);
    return 0;
}

int sdf_Path_AppendTarget(sdf_Path_t const* _this, sdf_Path_t const* targetPath, sdf_Path_t* _result) {
    *_result = _this->AppendTarget(*targetPath);
    return 0;
}

int sdf_Path_AppendRelationalAttribute(sdf_Path_t const* _this, tf_Token_t const* attrName, sdf_Path_t* _result) {
    *_result = _this->AppendRelationalAttribute(*attrName);
    return 0;
}

int sdf_Path_ReplaceTargetPath(sdf_Path_t const* _this, sdf_Path_t const* newTargetPath, sdf_Path_t* _result) {
    *_result = _this->ReplaceTargetPath(*newTargetPath);
    return 0;
}

int sdf_Path_AppendMapper(sdf_Path_t const* _this, sdf_Path_t const* targetPath, sdf_Path_t* _result) {
    *_result = _this->AppendMapper(*targetPath);
    return 0;
}

int sdf_Path_AppendMapperArg(sdf_Path_t const* _this, tf_Token_t const* argName, sdf_Path_t* _result) {
    *_result = _this->AppendMapperArg(*argName);
    return 0;
}

int sdf_Path_AppendExpression(sdf_Path_t const* _this, sdf_Path_t* _result) {
    *_result = _this->AppendExpression();
    return 0;
}

int sdf_Path_AppendElementString(sdf_Path_t const* _this, std_String_t const* element, sdf_Path_t* _result) {
    *_result = _this->AppendElementString(*element);
    return 0;
}

int sdf_Path_AppendElementToken(sdf_Path_t const* _this, tf_Token_t const* elementTok, sdf_Path_t* _result) {
    *_result = _this->AppendElementToken(*elementTok);
    return 0;
}

int sdf_Path_ReplacePrefix(sdf_Path_t const* _this, sdf_Path_t const* oldPrefix, sdf_Path_t const* newPrefix, bool fixTargetPaths, sdf_Path_t* _result) {
    *_result = _this->ReplacePrefix(*oldPrefix, *newPrefix, fixTargetPaths);
    return 0;
}

int sdf_Path_GetCommonPrefix(sdf_Path_t const* _this, sdf_Path_t const* path, sdf_Path_t* _result) {
    *_result = _this->GetCommonPrefix(*path);
    return 0;
}

int sdf_Path_MakeAbsolutePath(sdf_Path_t const* _this, sdf_Path_t const* anchor, sdf_Path_t* _result) {
    *_result = _this->MakeAbsolutePath(*anchor);
    return 0;
}

int sdf_Path_MakeRelativePath(sdf_Path_t const* _this, sdf_Path_t const* anchor, sdf_Path_t* _result) {
    *_result = _this->MakeRelativePath(*anchor);
    return 0;
}

int sdf_Path_EmptyPath(sdf_Path_t const** _result) {
    *_result = &PXR_NS::SdfPath::EmptyPath();
    return 0;
}

int sdf_Path_AbsoluteRootPath(sdf_Path_t const** _result) {
    *_result = &PXR_NS::SdfPath::AbsoluteRootPath();
    return 0;
}

int sdf_Path_ReflexiveRelativePath(sdf_Path_t const** _result) {
    *_result = &PXR_NS::SdfPath::ReflexiveRelativePath();
    return 0;
}

int sdf_Path_IsValidIdentifier(std_String_t const* name, bool* _result) {
    *_result = PXR_NS::SdfPath::IsValidIdentifier(*name);
    return 0;
}

int sdf_Path_IsValidNamespacedIdentifier(std_String_t const* name, bool* _result) {
    *_result = PXR_NS::SdfPath::IsValidNamespacedIdentifier(*name);
    return 0;
}

int sdf_Path_TokenizeIdentifier(std_String_t const* name, std_StringVector_t* _result) {
    *_result = PXR_NS::SdfPath::TokenizeIdentifier(*name);
    return 0;
}

int sdf_Path_TokenizeIdentifierAsTokens(std_String_t const* name, tf_TokenVector_t* _result) {
    *_result = PXR_NS::SdfPath::TokenizeIdentifierAsTokens(*name);
    return 0;
}

int sdf_Path_IsValidPathString(std_String_t const* pathString, std_String_t* errMsg, bool* _result) {
    *_result = PXR_NS::SdfPath::IsValidPathString(*pathString, errMsg);
    return 0;
}

int sdf_Path_GetConciseRelativePaths(sdf_PathVector_t const* paths, sdf_PathVector_t* _result) {
    *_result = PXR_NS::SdfPath::GetConciseRelativePaths(*paths);
    return 0;
}

int sdf_Path_RemoveDescendentPaths(sdf_PathVector_t* paths) {
    PXR_NS::SdfPath::RemoveDescendentPaths(paths);
    return 0;
}

int sdf_Path_RemoveAncestorPaths(sdf_PathVector_t* paths) {
    PXR_NS::SdfPath::RemoveAncestorPaths(paths);
    return 0;
}

int sdf_Path_new(sdf_Path_t** _result) {
    *_result = new PXR_NS::SdfPath();
    return 0;
}

int sdf_Path_dtor(sdf_Path_t* _this) {
    delete _this;
    return 0;
}

int sdf_PathExpression_ReplacePrefix(sdf_PathExpression_t const* _this, sdf_Path_t const* oldPrefix, sdf_Path_t const* newPrefix, sdf_PathExpression_t* _result) {
    *_result = _this->ReplacePrefix(*oldPrefix, *newPrefix);
    return 0;
}

int sdf_PathExpression_IsAbsolute(sdf_PathExpression_t const* _this, bool* _result) {
    *_result = _this->IsAbsolute();
    return 0;
}

int sdf_PathExpression_MakeAbsolute(sdf_PathExpression_t const* _this, sdf_Path_t const* anchor, sdf_PathExpression_t* _result) {
    *_result = _this->MakeAbsolute(*anchor);
    return 0;
}

int sdf_PathExpression_ContainsExpressionReferences(sdf_PathExpression_t const* _this, bool* _result) {
    *_result = _this->ContainsExpressionReferences();
    return 0;
}

int sdf_PathExpression_ComposeOver(sdf_PathExpression_t const* _this, sdf_PathExpression_t const* weaker, sdf_PathExpression_t* _result) {
    *_result = _this->ComposeOver(*weaker);
    return 0;
}

int sdf_PathExpression_IsComplete(sdf_PathExpression_t const* _this, bool* _result) {
    *_result = _this->IsComplete();
    return 0;
}

int sdf_PathExpression_GetDebugString(sdf_PathExpression_t const* _this, std_String_t* _result) {
    *_result = _this->GetDebugString();
    return 0;
}

int sdf_PathExpression_IsEmpty(sdf_PathExpression_t const* _this, bool* _result) {
    *_result = _this->IsEmpty();
    return 0;
}

int sdf_PathExpression_GetParseError(sdf_PathExpression_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetParseError();
    return 0;
}

int sdf_PathExpression_MakeComplement(sdf_PathExpression_t const* right, sdf_PathExpression_t* _result) {
    *_result = PXR_NS::SdfPathExpression::MakeComplement(*right);
    return 0;
}

int sdf_PathExpression_new(sdf_PathExpression_t** _result) {
    *_result = new PXR_NS::SdfPathExpression();
    return 0;
}

int sdf_PathExpression_dtor(sdf_PathExpression_t* _this) {
    delete _this;
    return 0;
}

int sdf_PathSet_dtor(sdf_PathSet_t* _this) {
    delete _this;
    return 0;
}

int sdf_PathVector_data(sdf_PathVector_t* _this, sdf_Path_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_PathVector_data_const(sdf_PathVector_t const* _this, sdf_Path_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_PathVector_empty(sdf_PathVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_PathVector_size(sdf_PathVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_PathVector_max_size(sdf_PathVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_PathVector_reserve(sdf_PathVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_PathVector_capacity(sdf_PathVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_PathVector_clear(sdf_PathVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_PathVector_push_back(sdf_PathVector_t* _this, sdf_Path_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_PathVector_pop_back(sdf_PathVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_PathVector_resize(sdf_PathVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_PathVector_resize_with(sdf_PathVector_t* _this, size_t const _Newsize, sdf_Path_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_PathVector_op_index(sdf_PathVector_t const* _this, size_t const _Pos, sdf_Path_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_PathVector_default(sdf_PathVector_t** _result) {
    *_result = new PXR_NS::SdfPathVector();
    return 0;
}

int sdf_PathVector_dtor(sdf_PathVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_Payload_GetAssetPath(sdf_Payload_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetAssetPath();
    return 0;
}

int sdf_Payload_SetAssetPath(sdf_Payload_t* _this, std_String_t const* assetPath) {
    _this->SetAssetPath(*assetPath);
    return 0;
}

int sdf_Payload_GetPrimPath(sdf_Payload_t const* _this, sdf_Path_t const** _result) {
    *_result = &_this->GetPrimPath();
    return 0;
}

int sdf_Payload_SetPrimPath(sdf_Payload_t* _this, sdf_Path_t const* primPath) {
    _this->SetPrimPath(*primPath);
    return 0;
}

int sdf_Payload_GetLayerOffset(sdf_Payload_t const* _this, sdf_LayerOffset_t const** _result) {
    *_result = &_this->GetLayerOffset();
    return 0;
}

int sdf_Payload_SetLayerOffset(sdf_Payload_t* _this, sdf_LayerOffset_t const* layerOffset) {
    _this->SetLayerOffset(*layerOffset);
    return 0;
}

int sdf_Payload_op_eq(sdf_Payload_t const* _this, sdf_Payload_t const* rhs, bool* _result) {
    *_result = _this->operator==(*rhs);
    return 0;
}

int sdf_Payload_op_lt(sdf_Payload_t const* _this, sdf_Payload_t const* rhs, bool* _result) {
    *_result = _this->operator<(*rhs);
    return 0;
}

int sdf_Payload_new(sdf_Payload_t** _result) {
    *_result = new PXR_NS::SdfPayload();
    return 0;
}

int sdf_Payload_dtor(sdf_Payload_t* _this) {
    delete _this;
    return 0;
}

int sdf_PayloadEditorProxy_dtor(sdf_PayloadEditorProxy_t* _this) {
    delete _this;
    return 0;
}

int sdf_PayloadVector_dtor(sdf_PayloadVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_PrimSpec_GetNameRoot(sdf_PrimSpec_t const* _this, sdf_PrimSpecHandle_t* _result) {
    *_result = _this->GetNameRoot();
    return 0;
}

int sdf_PrimSpec_GetNameParent(sdf_PrimSpec_t const* _this, sdf_PrimSpecHandle_t* _result) {
    *_result = _this->GetNameParent();
    return 0;
}

int sdf_PrimSpec_GetRealNameParent(sdf_PrimSpec_t const* _this, sdf_PrimSpecHandle_t* _result) {
    *_result = _this->GetRealNameParent();
    return 0;
}

int sdf_PrimSpec_GetNameChildren(sdf_PrimSpec_t const* _this, sdf_PrimSpecView_t* _result) {
    *_result = _this->GetNameChildren();
    return 0;
}

int sdf_PrimSpec_SetNameChildren(sdf_PrimSpec_t* _this, sdf_PrimSpecHandleVector_t const* param00) {
    _this->SetNameChildren(*param00);
    return 0;
}

int sdf_PrimSpec_InsertNameChild(sdf_PrimSpec_t* _this, sdf_PrimSpecHandle_t const* child, int index, bool* _result) {
    *_result = _this->InsertNameChild(*child, index);
    return 0;
}

int sdf_PrimSpec_RemoveNameChild(sdf_PrimSpec_t* _this, sdf_PrimSpecHandle_t const* child, bool* _result) {
    *_result = _this->RemoveNameChild(*child);
    return 0;
}

int sdf_PrimSpec_GetNameChildrenOrder(sdf_PrimSpec_t const* _this, sdf_NameChildrenOrderProxy_t* _result) {
    *_result = _this->GetNameChildrenOrder();
    return 0;
}

int sdf_PrimSpec_HasNameChildrenOrder(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->HasNameChildrenOrder();
    return 0;
}

int sdf_PrimSpec_SetNameChildrenOrder(sdf_PrimSpec_t* _this, tf_TokenVector_t const* names) {
    _this->SetNameChildrenOrder(*names);
    return 0;
}

int sdf_PrimSpec_InsertInNameChildrenOrder(sdf_PrimSpec_t* _this, tf_Token_t const* name, int index) {
    _this->InsertInNameChildrenOrder(*name, index);
    return 0;
}

int sdf_PrimSpec_RemoveFromNameChildrenOrder(sdf_PrimSpec_t* _this, tf_Token_t const* name) {
    _this->RemoveFromNameChildrenOrder(*name);
    return 0;
}

int sdf_PrimSpec_RemoveFromNameChildrenOrderByIndex(sdf_PrimSpec_t* _this, int index) {
    _this->RemoveFromNameChildrenOrderByIndex(index);
    return 0;
}

int sdf_PrimSpec_ApplyNameChildrenOrder(sdf_PrimSpec_t const* _this, tf_TokenVector_t* vec) {
    _this->ApplyNameChildrenOrder(vec);
    return 0;
}

int sdf_PrimSpec_GetProperties(sdf_PrimSpec_t const* _this, sdf_PropertySpecView_t* _result) {
    *_result = _this->GetProperties();
    return 0;
}

int sdf_PrimSpec_SetProperties(sdf_PrimSpec_t* _this, sdf_PropertySpecHandleVector_t const* param00) {
    _this->SetProperties(*param00);
    return 0;
}

int sdf_PrimSpec_InsertProperty(sdf_PrimSpec_t* _this, sdf_PropertySpecHandle_t const* property, int index, bool* _result) {
    *_result = _this->InsertProperty(*property, index);
    return 0;
}

int sdf_PrimSpec_RemoveProperty(sdf_PrimSpec_t* _this, sdf_PropertySpecHandle_t const* property) {
    _this->RemoveProperty(*property);
    return 0;
}

int sdf_PrimSpec_GetAttributes(sdf_PrimSpec_t const* _this, sdf_AttributeSpecView_t* _result) {
    *_result = _this->GetAttributes();
    return 0;
}

int sdf_PrimSpec_GetRelationships(sdf_PrimSpec_t const* _this, sdf_RelationshipSpecView_t* _result) {
    *_result = _this->GetRelationships();
    return 0;
}

int sdf_PrimSpec_GetPropertyOrder(sdf_PrimSpec_t const* _this, sdf_NameChildrenOrderProxy_t* _result) {
    *_result = _this->GetPropertyOrder();
    return 0;
}

int sdf_PrimSpec_HasPropertyOrder(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->HasPropertyOrder();
    return 0;
}

int sdf_PrimSpec_SetPropertyOrder(sdf_PrimSpec_t* _this, tf_TokenVector_t const* names) {
    _this->SetPropertyOrder(*names);
    return 0;
}

int sdf_PrimSpec_InsertInPropertyOrder(sdf_PrimSpec_t* _this, tf_Token_t const* name, int index) {
    _this->InsertInPropertyOrder(*name, index);
    return 0;
}

int sdf_PrimSpec_RemoveFromPropertyOrder(sdf_PrimSpec_t* _this, tf_Token_t const* name) {
    _this->RemoveFromPropertyOrder(*name);
    return 0;
}

int sdf_PrimSpec_RemoveFromPropertyOrderByIndex(sdf_PrimSpec_t* _this, int index) {
    _this->RemoveFromPropertyOrderByIndex(index);
    return 0;
}

int sdf_PrimSpec_ApplyPropertyOrder(sdf_PrimSpec_t const* _this, tf_TokenVector_t* vec) {
    _this->ApplyPropertyOrder(vec);
    return 0;
}

int sdf_PrimSpec_GetObjectAtPath(sdf_PrimSpec_t const* _this, sdf_Path_t const* path, sdf_SpecHandle_t* _result) {
    *_result = _this->GetObjectAtPath(*path);
    return 0;
}

int sdf_PrimSpec_GetPrimAtPath(sdf_PrimSpec_t const* _this, sdf_Path_t const* path, sdf_PrimSpecHandle_t* _result) {
    *_result = _this->GetPrimAtPath(*path);
    return 0;
}

int sdf_PrimSpec_GetPropertyAtPath(sdf_PrimSpec_t const* _this, sdf_Path_t const* path, sdf_PropertySpecHandle_t* _result) {
    *_result = _this->GetPropertyAtPath(*path);
    return 0;
}

int sdf_PrimSpec_GetAttributeAtPath(sdf_PrimSpec_t const* _this, sdf_Path_t const* path, sdf_AttributeSpecHandle_t* _result) {
    *_result = _this->GetAttributeAtPath(*path);
    return 0;
}

int sdf_PrimSpec_GetRelationshipAtPath(sdf_PrimSpec_t const* _this, sdf_Path_t const* path, sdf_RelationshipSpecHandle_t* _result) {
    *_result = _this->GetRelationshipAtPath(*path);
    return 0;
}

int sdf_PrimSpec_GetTypeName(sdf_PrimSpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetTypeName();
    return 0;
}

int sdf_PrimSpec_SetTypeName(sdf_PrimSpec_t* _this, std_String_t const* value) {
    _this->SetTypeName(*value);
    return 0;
}

int sdf_PrimSpec_GetComment(sdf_PrimSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetComment();
    return 0;
}

int sdf_PrimSpec_SetComment(sdf_PrimSpec_t* _this, std_String_t const* value) {
    _this->SetComment(*value);
    return 0;
}

int sdf_PrimSpec_GetDocumentation(sdf_PrimSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetDocumentation();
    return 0;
}

int sdf_PrimSpec_SetDocumentation(sdf_PrimSpec_t* _this, std_String_t const* value) {
    _this->SetDocumentation(*value);
    return 0;
}

int sdf_PrimSpec_GetActive(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->GetActive();
    return 0;
}

int sdf_PrimSpec_SetActive(sdf_PrimSpec_t* _this, bool value) {
    _this->SetActive(value);
    return 0;
}

int sdf_PrimSpec_HasActive(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->HasActive();
    return 0;
}

int sdf_PrimSpec_ClearActive(sdf_PrimSpec_t* _this) {
    _this->ClearActive();
    return 0;
}

int sdf_PrimSpec_GetHidden(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->GetHidden();
    return 0;
}

int sdf_PrimSpec_SetHidden(sdf_PrimSpec_t* _this, bool value) {
    _this->SetHidden(value);
    return 0;
}

int sdf_PrimSpec_GetKind(sdf_PrimSpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetKind();
    return 0;
}

int sdf_PrimSpec_SetKind(sdf_PrimSpec_t* _this, tf_Token_t const* value) {
    _this->SetKind(*value);
    return 0;
}

int sdf_PrimSpec_HasKind(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->HasKind();
    return 0;
}

int sdf_PrimSpec_ClearKind(sdf_PrimSpec_t* _this) {
    _this->ClearKind();
    return 0;
}

int sdf_PrimSpec_GetSymmetryFunction(sdf_PrimSpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetSymmetryFunction();
    return 0;
}

int sdf_PrimSpec_SetSymmetryFunction(sdf_PrimSpec_t* _this, tf_Token_t const* functionName) {
    _this->SetSymmetryFunction(*functionName);
    return 0;
}

int sdf_PrimSpec_GetSymmetricPeer(sdf_PrimSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetSymmetricPeer();
    return 0;
}

int sdf_PrimSpec_SetSymmetricPeer(sdf_PrimSpec_t* _this, std_String_t const* peerName) {
    _this->SetSymmetricPeer(*peerName);
    return 0;
}

int sdf_PrimSpec_GetPrefix(sdf_PrimSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetPrefix();
    return 0;
}

int sdf_PrimSpec_SetPrefix(sdf_PrimSpec_t* _this, std_String_t const* value) {
    _this->SetPrefix(*value);
    return 0;
}

int sdf_PrimSpec_GetSuffix(sdf_PrimSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetSuffix();
    return 0;
}

int sdf_PrimSpec_SetSuffix(sdf_PrimSpec_t* _this, std_String_t const* value) {
    _this->SetSuffix(*value);
    return 0;
}

int sdf_PrimSpec_GetCustomData(sdf_PrimSpec_t const* _this, sdf_DictionaryProxy_t* _result) {
    *_result = _this->GetCustomData();
    return 0;
}

int sdf_PrimSpec_SetCustomData(sdf_PrimSpec_t* _this, std_String_t const* name, vt_Value_t const* value) {
    _this->SetCustomData(*name, *value);
    return 0;
}

int sdf_PrimSpec_GetAssetInfo(sdf_PrimSpec_t const* _this, sdf_DictionaryProxy_t* _result) {
    *_result = _this->GetAssetInfo();
    return 0;
}

int sdf_PrimSpec_SetAssetInfo(sdf_PrimSpec_t* _this, std_String_t const* name, vt_Value_t const* value) {
    _this->SetAssetInfo(*name, *value);
    return 0;
}

int sdf_PrimSpec_GetSpecifier(sdf_PrimSpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSpecifier());
    return 0;
}

int sdf_PrimSpec_SetSpecifier(sdf_PrimSpec_t* _this, int value) {
    _this->SetSpecifier(static_cast<PXR_NS::SdfSpecifier>(value));
    return 0;
}

int sdf_PrimSpec_GetPermission(sdf_PrimSpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetPermission());
    return 0;
}

int sdf_PrimSpec_SetPermission(sdf_PrimSpec_t* _this, int value) {
    _this->SetPermission(static_cast<PXR_NS::SdfPermission>(value));
    return 0;
}

int sdf_PrimSpec_GetPrefixSubstitutions(sdf_PrimSpec_t const* _this, vt_Dictionary_t* _result) {
    *_result = _this->GetPrefixSubstitutions();
    return 0;
}

int sdf_PrimSpec_SetPrefixSubstitutions(sdf_PrimSpec_t* _this, vt_Dictionary_t const* prefixSubstitutions) {
    _this->SetPrefixSubstitutions(*prefixSubstitutions);
    return 0;
}

int sdf_PrimSpec_GetSuffixSubstitutions(sdf_PrimSpec_t const* _this, vt_Dictionary_t* _result) {
    *_result = _this->GetSuffixSubstitutions();
    return 0;
}

int sdf_PrimSpec_SetSuffixSubstitutions(sdf_PrimSpec_t* _this, vt_Dictionary_t const* suffixSubstitutions) {
    _this->SetSuffixSubstitutions(*suffixSubstitutions);
    return 0;
}

int sdf_PrimSpec_GetInstanceable(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->GetInstanceable();
    return 0;
}

int sdf_PrimSpec_SetInstanceable(sdf_PrimSpec_t* _this, bool instanceable) {
    _this->SetInstanceable(instanceable);
    return 0;
}

int sdf_PrimSpec_HasInstanceable(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->HasInstanceable();
    return 0;
}

int sdf_PrimSpec_ClearInstanceable(sdf_PrimSpec_t* _this) {
    _this->ClearInstanceable();
    return 0;
}

int sdf_PrimSpec_GetPayloadList(sdf_PrimSpec_t const* _this, sdf_PayloadEditorProxy_t* _result) {
    *_result = _this->GetPayloadList();
    return 0;
}

int sdf_PrimSpec_HasPayloads(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->HasPayloads();
    return 0;
}

int sdf_PrimSpec_ClearPayloadList(sdf_PrimSpec_t* _this) {
    _this->ClearPayloadList();
    return 0;
}

int sdf_PrimSpec_GetInheritPathList(sdf_PrimSpec_t const* _this, sdf_InheritsProxy_t* _result) {
    *_result = _this->GetInheritPathList();
    return 0;
}

int sdf_PrimSpec_HasInheritPaths(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->HasInheritPaths();
    return 0;
}

int sdf_PrimSpec_ClearInheritPathList(sdf_PrimSpec_t* _this) {
    _this->ClearInheritPathList();
    return 0;
}

int sdf_PrimSpec_GetSpecializesList(sdf_PrimSpec_t const* _this, sdf_InheritsProxy_t* _result) {
    *_result = _this->GetSpecializesList();
    return 0;
}

int sdf_PrimSpec_HasSpecializes(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->HasSpecializes();
    return 0;
}

int sdf_PrimSpec_ClearSpecializesList(sdf_PrimSpec_t* _this) {
    _this->ClearSpecializesList();
    return 0;
}

int sdf_PrimSpec_GetReferenceList(sdf_PrimSpec_t const* _this, sdf_ReferenceEditorProxy_t* _result) {
    *_result = _this->GetReferenceList();
    return 0;
}

int sdf_PrimSpec_HasReferences(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->HasReferences();
    return 0;
}

int sdf_PrimSpec_ClearReferenceList(sdf_PrimSpec_t* _this) {
    _this->ClearReferenceList();
    return 0;
}

int sdf_PrimSpec_GetVariantSetNameList(sdf_PrimSpec_t const* _this, sdf_NameEditorProxy_t* _result) {
    *_result = _this->GetVariantSetNameList();
    return 0;
}

int sdf_PrimSpec_HasVariantSetNames(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->HasVariantSetNames();
    return 0;
}

int sdf_PrimSpec_GetVariantNames(sdf_PrimSpec_t const* _this, std_String_t const* name, std_StringVector_t* _result) {
    *_result = _this->GetVariantNames(*name);
    return 0;
}

int sdf_PrimSpec_RemoveVariantSet(sdf_PrimSpec_t* _this, std_String_t const* name) {
    _this->RemoveVariantSet(*name);
    return 0;
}

int sdf_PrimSpec_GetVariantSelections(sdf_PrimSpec_t const* _this, sdf_VariantSelectionProxy_t* _result) {
    *_result = _this->GetVariantSelections();
    return 0;
}

int sdf_PrimSpec_BlockVariantSelection(sdf_PrimSpec_t* _this, std_String_t const* variantSetName) {
    _this->BlockVariantSelection(*variantSetName);
    return 0;
}

int sdf_PrimSpec_GetRelocates(sdf_PrimSpec_t const* _this, sdf_RelocatesMapProxy_t* _result) {
    *_result = _this->GetRelocates();
    return 0;
}

int sdf_PrimSpec_SetRelocates(sdf_PrimSpec_t* _this, sdf_RelocatesMap_t const* newMap) {
    _this->SetRelocates(*newMap);
    return 0;
}

int sdf_PrimSpec_HasRelocates(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->HasRelocates();
    return 0;
}

int sdf_PrimSpec_ClearRelocates(sdf_PrimSpec_t* _this) {
    _this->ClearRelocates();
    return 0;
}

int sdf_PrimSpec_New(sdf_LayerHandle_t const* parentLayer, std_String_t const* name, int spec, std_String_t const* typeName, sdf_PrimSpecHandle_t* _result) {
    *_result = PXR_NS::SdfPrimSpec::New(*parentLayer, *name, static_cast<PXR_NS::SdfSpecifier>(spec), *typeName);
    return 0;
}

int sdf_PrimSpec_New_under_parent(sdf_PrimSpecHandle_t const* parentPrim, std_String_t const* name, int spec, std_String_t const* typeName, sdf_PrimSpecHandle_t* _result) {
    *_result = PXR_NS::SdfPrimSpec::New(*parentPrim, *name, static_cast<PXR_NS::SdfSpecifier>(spec), *typeName);
    return 0;
}

int sdf_PrimSpec_IsValidName(std_String_t const* name, bool* _result) {
    *_result = PXR_NS::SdfPrimSpec::IsValidName(*name);
    return 0;
}

int sdf_PrimSpec_GetName(sdf_PrimSpec_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int sdf_PrimSpec_GetNameToken(sdf_PrimSpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNameToken();
    return 0;
}

int sdf_PrimSpec_CanSetName(sdf_PrimSpec_t const* _this, std_String_t const* newName, std_String_t* whyNot, bool* _result) {
    *_result = _this->CanSetName(*newName, whyNot);
    return 0;
}

int sdf_PrimSpec_SetName(sdf_PrimSpec_t* _this, std_String_t const* newName, bool validate, bool* _result) {
    *_result = _this->SetName(*newName, validate);
    return 0;
}

int sdf_PrimSpec_GetSchema(sdf_PrimSpec_t const* _this, sdf_SchemaBase_t const** _result) {
    *_result = &_this->GetSchema();
    return 0;
}

int sdf_PrimSpec_GetSpecType(sdf_PrimSpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSpecType());
    return 0;
}

int sdf_PrimSpec_IsDormant(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->IsDormant();
    return 0;
}

int sdf_PrimSpec_GetLayer(sdf_PrimSpec_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetLayer();
    return 0;
}

int sdf_PrimSpec_GetPath(sdf_PrimSpec_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int sdf_PrimSpec_PermissionToEdit(sdf_PrimSpec_t const* _this, bool* _result) {
    *_result = _this->PermissionToEdit();
    return 0;
}

int sdf_PrimSpec_ListInfoKeys(sdf_PrimSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListInfoKeys();
    return 0;
}

int sdf_PrimSpec_GetMetaDataInfoKeys(sdf_PrimSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->GetMetaDataInfoKeys();
    return 0;
}

int sdf_PrimSpec_GetMetaDataDisplayGroup(sdf_PrimSpec_t const* _this, tf_Token_t const* key, tf_Token_t* _result) {
    *_result = _this->GetMetaDataDisplayGroup(*key);
    return 0;
}

int sdf_PrimSpec_GetInfo(sdf_PrimSpec_t const* _this, tf_Token_t const* key, vt_Value_t* _result) {
    *_result = _this->GetInfo(*key);
    return 0;
}

int sdf_PrimSpec_SetInfo(sdf_PrimSpec_t* _this, tf_Token_t const* key, vt_Value_t const* value) {
    _this->SetInfo(*key, *value);
    return 0;
}

int sdf_PrimSpec_SetInfoDictionaryValue(sdf_PrimSpec_t* _this, tf_Token_t const* dictionaryKey, tf_Token_t const* entryKey, vt_Value_t const* value) {
    _this->SetInfoDictionaryValue(*dictionaryKey, *entryKey, *value);
    return 0;
}

int sdf_PrimSpec_HasInfo(sdf_PrimSpec_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasInfo(*key);
    return 0;
}

int sdf_PrimSpec_ClearInfo(sdf_PrimSpec_t* _this, tf_Token_t const* key) {
    _this->ClearInfo(*key);
    return 0;
}

int sdf_PrimSpec_GetTypeForInfo(sdf_PrimSpec_t const* _this, tf_Token_t const* key, tf_Type_t* _result) {
    *_result = _this->GetTypeForInfo(*key);
    return 0;
}

int sdf_PrimSpec_GetFallbackForInfo(sdf_PrimSpec_t const* _this, tf_Token_t const* key, vt_Value_t const** _result) {
    *_result = &_this->GetFallbackForInfo(*key);
    return 0;
}

int sdf_PrimSpec_IsInert(sdf_PrimSpec_t const* _this, bool ignoreChildren, bool* _result) {
    *_result = _this->IsInert(ignoreChildren);
    return 0;
}

int sdf_PrimSpec_ListFields(sdf_PrimSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListFields();
    return 0;
}

int sdf_PrimSpec_HasField(sdf_PrimSpec_t const* _this, tf_Token_t const* name, bool* _result) {
    *_result = _this->HasField(*name);
    return 0;
}

int sdf_PrimSpec_GetField(sdf_PrimSpec_t const* _this, tf_Token_t const* name, vt_Value_t* _result) {
    *_result = _this->GetField(*name);
    return 0;
}

int sdf_PrimSpec_SetField(sdf_PrimSpec_t* _this, tf_Token_t const* name, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetField(*name, *value);
    return 0;
}

int sdf_PrimSpec_op_eq(sdf_PrimSpec_t const* _this, sdf_Spec_t const* rhs, bool* _result) {
    *_result = _this->operator==(*rhs);
    return 0;
}

int sdf_PrimSpec_dtor(sdf_PrimSpec_t* _this) {
    delete _this;
    return 0;
}

int sdf_PrimSpecHandle_dtor(sdf_PrimSpecHandle_t* _this) {
    delete _this;
    return 0;
}

int sdf_PrimSpecHandleVector_data(sdf_PrimSpecHandleVector_t* _this, sdf_PrimSpecHandle_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_PrimSpecHandleVector_data_const(sdf_PrimSpecHandleVector_t const* _this, sdf_PrimSpecHandle_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_PrimSpecHandleVector_empty(sdf_PrimSpecHandleVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_PrimSpecHandleVector_size(sdf_PrimSpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_PrimSpecHandleVector_max_size(sdf_PrimSpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_PrimSpecHandleVector_reserve(sdf_PrimSpecHandleVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_PrimSpecHandleVector_capacity(sdf_PrimSpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_PrimSpecHandleVector_clear(sdf_PrimSpecHandleVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_PrimSpecHandleVector_push_back(sdf_PrimSpecHandleVector_t* _this, sdf_PrimSpecHandle_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_PrimSpecHandleVector_pop_back(sdf_PrimSpecHandleVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_PrimSpecHandleVector_resize(sdf_PrimSpecHandleVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_PrimSpecHandleVector_resize_with(sdf_PrimSpecHandleVector_t* _this, size_t const _Newsize, sdf_PrimSpecHandle_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_PrimSpecHandleVector_op_index(sdf_PrimSpecHandleVector_t const* _this, size_t const _Pos, sdf_PrimSpecHandle_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_PrimSpecHandleVector_default(sdf_PrimSpecHandleVector_t** _result) {
    *_result = new PXR_NS::SdfPrimSpecHandleVector();
    return 0;
}

int sdf_PrimSpecHandleVector_dtor(sdf_PrimSpecHandleVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_PrimSpecHandleOffsetPair_dtor(sdf_PrimSpecHandleOffsetPair_t* _this) {
    delete _this;
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_data(sdf_PrimSpecHandleOffsetPairVector_t* _this, sdf_PrimSpecHandleOffsetPair_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_data_const(sdf_PrimSpecHandleOffsetPairVector_t const* _this, sdf_PrimSpecHandleOffsetPair_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_empty(sdf_PrimSpecHandleOffsetPairVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_size(sdf_PrimSpecHandleOffsetPairVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_max_size(sdf_PrimSpecHandleOffsetPairVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_reserve(sdf_PrimSpecHandleOffsetPairVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_capacity(sdf_PrimSpecHandleOffsetPairVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_clear(sdf_PrimSpecHandleOffsetPairVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_push_back(sdf_PrimSpecHandleOffsetPairVector_t* _this, sdf_PrimSpecHandleOffsetPair_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_pop_back(sdf_PrimSpecHandleOffsetPairVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_resize(sdf_PrimSpecHandleOffsetPairVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_resize_with(sdf_PrimSpecHandleOffsetPairVector_t* _this, size_t const _Newsize, sdf_PrimSpecHandleOffsetPair_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_op_index(sdf_PrimSpecHandleOffsetPairVector_t const* _this, size_t const _Pos, sdf_PrimSpecHandleOffsetPair_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_default(sdf_PrimSpecHandleOffsetPairVector_t** _result) {
    *_result = new std::vector<
        std::pair<PXR_NS::SdfPrimSpecHandle, PXR_NS::SdfLayerOffset>>();
    return 0;
}

int sdf_PrimSpecHandleOffsetPairVector_dtor(sdf_PrimSpecHandleOffsetPairVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_PrimSpecView_dtor(sdf_PrimSpecView_t* _this) {
    delete _this;
    return 0;
}

int sdf_PropertySpec_GetOwner(sdf_PropertySpec_t const* _this, sdf_SpecHandle_t* _result) {
    *_result = _this->GetOwner();
    return 0;
}

int sdf_PropertySpec_GetCustomData(sdf_PropertySpec_t const* _this, sdf_DictionaryProxy_t* _result) {
    *_result = _this->GetCustomData();
    return 0;
}

int sdf_PropertySpec_GetAssetInfo(sdf_PropertySpec_t const* _this, sdf_DictionaryProxy_t* _result) {
    *_result = _this->GetAssetInfo();
    return 0;
}

int sdf_PropertySpec_SetCustomData(sdf_PropertySpec_t* _this, std_String_t const* name, vt_Value_t const* value) {
    _this->SetCustomData(*name, *value);
    return 0;
}

int sdf_PropertySpec_SetAssetInfo(sdf_PropertySpec_t* _this, std_String_t const* name, vt_Value_t const* value) {
    _this->SetAssetInfo(*name, *value);
    return 0;
}

int sdf_PropertySpec_GetDisplayGroup(sdf_PropertySpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayGroup();
    return 0;
}

int sdf_PropertySpec_SetDisplayGroup(sdf_PropertySpec_t* _this, std_String_t const* value) {
    _this->SetDisplayGroup(*value);
    return 0;
}

int sdf_PropertySpec_GetDisplayName(sdf_PropertySpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayName();
    return 0;
}

int sdf_PropertySpec_SetDisplayName(sdf_PropertySpec_t* _this, std_String_t const* value) {
    _this->SetDisplayName(*value);
    return 0;
}

int sdf_PropertySpec_GetDocumentation(sdf_PropertySpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetDocumentation();
    return 0;
}

int sdf_PropertySpec_SetDocumentation(sdf_PropertySpec_t* _this, std_String_t const* value) {
    _this->SetDocumentation(*value);
    return 0;
}

int sdf_PropertySpec_GetHidden(sdf_PropertySpec_t const* _this, bool* _result) {
    *_result = _this->GetHidden();
    return 0;
}

int sdf_PropertySpec_SetHidden(sdf_PropertySpec_t* _this, bool value) {
    _this->SetHidden(value);
    return 0;
}

int sdf_PropertySpec_GetPermission(sdf_PropertySpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetPermission());
    return 0;
}

int sdf_PropertySpec_SetPermission(sdf_PropertySpec_t* _this, int value) {
    _this->SetPermission(static_cast<PXR_NS::SdfPermission>(value));
    return 0;
}

int sdf_PropertySpec_GetPrefix(sdf_PropertySpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetPrefix();
    return 0;
}

int sdf_PropertySpec_SetPrefix(sdf_PropertySpec_t* _this, std_String_t const* value) {
    _this->SetPrefix(*value);
    return 0;
}

int sdf_PropertySpec_GetSuffix(sdf_PropertySpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetSuffix();
    return 0;
}

int sdf_PropertySpec_SetSuffix(sdf_PropertySpec_t* _this, std_String_t const* value) {
    _this->SetSuffix(*value);
    return 0;
}

int sdf_PropertySpec_GetSymmetricPeer(sdf_PropertySpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetSymmetricPeer();
    return 0;
}

int sdf_PropertySpec_SetSymmetricPeer(sdf_PropertySpec_t* _this, std_String_t const* peerName) {
    _this->SetSymmetricPeer(*peerName);
    return 0;
}

int sdf_PropertySpec_GetSymmetryArguments(sdf_PropertySpec_t const* _this, sdf_DictionaryProxy_t* _result) {
    *_result = _this->GetSymmetryArguments();
    return 0;
}

int sdf_PropertySpec_SetSymmetryArgument(sdf_PropertySpec_t* _this, std_String_t const* name, vt_Value_t const* value) {
    _this->SetSymmetryArgument(*name, *value);
    return 0;
}

int sdf_PropertySpec_GetSymmetryFunction(sdf_PropertySpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetSymmetryFunction();
    return 0;
}

int sdf_PropertySpec_SetSymmetryFunction(sdf_PropertySpec_t* _this, tf_Token_t const* functionName) {
    _this->SetSymmetryFunction(*functionName);
    return 0;
}

int sdf_PropertySpec_GetTimeSampleMap(sdf_PropertySpec_t const* _this, sdf_TimeSampleMap_t* _result) {
    *_result = _this->GetTimeSampleMap();
    return 0;
}

int sdf_PropertySpec_GetValueType(sdf_PropertySpec_t const* _this, tf_Type_t* _result) {
    *_result = _this->GetValueType();
    return 0;
}

int sdf_PropertySpec_GetTypeName(sdf_PropertySpec_t const* _this, sdf_ValueTypeName_t* _result) {
    *_result = _this->GetTypeName();
    return 0;
}

int sdf_PropertySpec_GetDefaultValue(sdf_PropertySpec_t const* _this, vt_Value_t* _result) {
    *_result = _this->GetDefaultValue();
    return 0;
}

int sdf_PropertySpec_SetDefaultValue(sdf_PropertySpec_t* _this, vt_Value_t const* defaultValue, bool* _result) {
    *_result = _this->SetDefaultValue(*defaultValue);
    return 0;
}

int sdf_PropertySpec_HasDefaultValue(sdf_PropertySpec_t const* _this, bool* _result) {
    *_result = _this->HasDefaultValue();
    return 0;
}

int sdf_PropertySpec_ClearDefaultValue(sdf_PropertySpec_t* _this) {
    _this->ClearDefaultValue();
    return 0;
}

int sdf_PropertySpec_GetComment(sdf_PropertySpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetComment();
    return 0;
}

int sdf_PropertySpec_SetComment(sdf_PropertySpec_t* _this, std_String_t const* value) {
    _this->SetComment(*value);
    return 0;
}

int sdf_PropertySpec_IsCustom(sdf_PropertySpec_t const* _this, bool* _result) {
    *_result = _this->IsCustom();
    return 0;
}

int sdf_PropertySpec_SetCustom(sdf_PropertySpec_t* _this, bool custom) {
    _this->SetCustom(custom);
    return 0;
}

int sdf_PropertySpec_GetVariability(sdf_PropertySpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetVariability());
    return 0;
}

int sdf_PropertySpec_HasOnlyRequiredFields(sdf_PropertySpec_t const* _this, bool* _result) {
    *_result = _this->HasOnlyRequiredFields();
    return 0;
}

int sdf_PropertySpec_GetName(sdf_PropertySpec_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int sdf_PropertySpec_GetNameToken(sdf_PropertySpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNameToken();
    return 0;
}

int sdf_PropertySpec_CanSetName(sdf_PropertySpec_t const* _this, std_String_t const* newName, std_String_t* whyNot, bool* _result) {
    *_result = _this->CanSetName(*newName, whyNot);
    return 0;
}

int sdf_PropertySpec_SetName(sdf_PropertySpec_t* _this, std_String_t const* newName, bool validate, bool* _result) {
    *_result = _this->SetName(*newName, validate);
    return 0;
}

int sdf_PropertySpec_IsValidName(std_String_t const* name, bool* _result) {
    *_result = PXR_NS::SdfPropertySpec::IsValidName(*name);
    return 0;
}

int sdf_PropertySpec_GetSchema(sdf_PropertySpec_t const* _this, sdf_SchemaBase_t const** _result) {
    *_result = &_this->GetSchema();
    return 0;
}

int sdf_PropertySpec_GetSpecType(sdf_PropertySpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSpecType());
    return 0;
}

int sdf_PropertySpec_IsDormant(sdf_PropertySpec_t const* _this, bool* _result) {
    *_result = _this->IsDormant();
    return 0;
}

int sdf_PropertySpec_GetLayer(sdf_PropertySpec_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetLayer();
    return 0;
}

int sdf_PropertySpec_GetPath(sdf_PropertySpec_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int sdf_PropertySpec_PermissionToEdit(sdf_PropertySpec_t const* _this, bool* _result) {
    *_result = _this->PermissionToEdit();
    return 0;
}

int sdf_PropertySpec_ListInfoKeys(sdf_PropertySpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListInfoKeys();
    return 0;
}

int sdf_PropertySpec_GetMetaDataInfoKeys(sdf_PropertySpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->GetMetaDataInfoKeys();
    return 0;
}

int sdf_PropertySpec_GetMetaDataDisplayGroup(sdf_PropertySpec_t const* _this, tf_Token_t const* key, tf_Token_t* _result) {
    *_result = _this->GetMetaDataDisplayGroup(*key);
    return 0;
}

int sdf_PropertySpec_GetInfo(sdf_PropertySpec_t const* _this, tf_Token_t const* key, vt_Value_t* _result) {
    *_result = _this->GetInfo(*key);
    return 0;
}

int sdf_PropertySpec_SetInfo(sdf_PropertySpec_t* _this, tf_Token_t const* key, vt_Value_t const* value) {
    _this->SetInfo(*key, *value);
    return 0;
}

int sdf_PropertySpec_SetInfoDictionaryValue(sdf_PropertySpec_t* _this, tf_Token_t const* dictionaryKey, tf_Token_t const* entryKey, vt_Value_t const* value) {
    _this->SetInfoDictionaryValue(*dictionaryKey, *entryKey, *value);
    return 0;
}

int sdf_PropertySpec_HasInfo(sdf_PropertySpec_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasInfo(*key);
    return 0;
}

int sdf_PropertySpec_ClearInfo(sdf_PropertySpec_t* _this, tf_Token_t const* key) {
    _this->ClearInfo(*key);
    return 0;
}

int sdf_PropertySpec_GetTypeForInfo(sdf_PropertySpec_t const* _this, tf_Token_t const* key, tf_Type_t* _result) {
    *_result = _this->GetTypeForInfo(*key);
    return 0;
}

int sdf_PropertySpec_GetFallbackForInfo(sdf_PropertySpec_t const* _this, tf_Token_t const* key, vt_Value_t const** _result) {
    *_result = &_this->GetFallbackForInfo(*key);
    return 0;
}

int sdf_PropertySpec_IsInert(sdf_PropertySpec_t const* _this, bool ignoreChildren, bool* _result) {
    *_result = _this->IsInert(ignoreChildren);
    return 0;
}

int sdf_PropertySpec_ListFields(sdf_PropertySpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListFields();
    return 0;
}

int sdf_PropertySpec_HasField(sdf_PropertySpec_t const* _this, tf_Token_t const* name, bool* _result) {
    *_result = _this->HasField(*name);
    return 0;
}

int sdf_PropertySpec_GetField(sdf_PropertySpec_t const* _this, tf_Token_t const* name, vt_Value_t* _result) {
    *_result = _this->GetField(*name);
    return 0;
}

int sdf_PropertySpec_SetField(sdf_PropertySpec_t* _this, tf_Token_t const* name, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetField(*name, *value);
    return 0;
}

int sdf_PropertySpec_op_eq(sdf_PropertySpec_t const* _this, sdf_Spec_t const* rhs, bool* _result) {
    *_result = _this->operator==(*rhs);
    return 0;
}

int sdf_PropertySpec_dtor(sdf_PropertySpec_t* _this) {
    delete _this;
    return 0;
}

int sdf_PropertySpecHandle_dtor(sdf_PropertySpecHandle_t* _this) {
    delete _this;
    return 0;
}

int sdf_PropertySpecHandleVector_data(sdf_PropertySpecHandleVector_t* _this, sdf_PropertySpecHandle_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_PropertySpecHandleVector_data_const(sdf_PropertySpecHandleVector_t const* _this, sdf_PropertySpecHandle_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_PropertySpecHandleVector_empty(sdf_PropertySpecHandleVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_PropertySpecHandleVector_size(sdf_PropertySpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_PropertySpecHandleVector_max_size(sdf_PropertySpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_PropertySpecHandleVector_reserve(sdf_PropertySpecHandleVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_PropertySpecHandleVector_capacity(sdf_PropertySpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_PropertySpecHandleVector_clear(sdf_PropertySpecHandleVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_PropertySpecHandleVector_push_back(sdf_PropertySpecHandleVector_t* _this, sdf_PropertySpecHandle_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_PropertySpecHandleVector_pop_back(sdf_PropertySpecHandleVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_PropertySpecHandleVector_resize(sdf_PropertySpecHandleVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_PropertySpecHandleVector_resize_with(sdf_PropertySpecHandleVector_t* _this, size_t const _Newsize, sdf_PropertySpecHandle_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_PropertySpecHandleVector_op_index(sdf_PropertySpecHandleVector_t const* _this, size_t const _Pos, sdf_PropertySpecHandle_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_PropertySpecHandleVector_default(sdf_PropertySpecHandleVector_t** _result) {
    *_result = new PXR_NS::SdfPropertySpecHandleVector();
    return 0;
}

int sdf_PropertySpecHandleVector_dtor(sdf_PropertySpecHandleVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_PropertySpecHandleOffsetPair_dtor(sdf_PropertySpecHandleOffsetPair_t* _this) {
    delete _this;
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_data(sdf_PropertySpecHandleOffsetPairVector_t* _this, sdf_PropertySpecHandleOffsetPair_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_data_const(sdf_PropertySpecHandleOffsetPairVector_t const* _this, sdf_PropertySpecHandleOffsetPair_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_empty(sdf_PropertySpecHandleOffsetPairVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_size(sdf_PropertySpecHandleOffsetPairVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_max_size(sdf_PropertySpecHandleOffsetPairVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_reserve(sdf_PropertySpecHandleOffsetPairVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_capacity(sdf_PropertySpecHandleOffsetPairVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_clear(sdf_PropertySpecHandleOffsetPairVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_push_back(sdf_PropertySpecHandleOffsetPairVector_t* _this, sdf_PropertySpecHandleOffsetPair_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_pop_back(sdf_PropertySpecHandleOffsetPairVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_resize(sdf_PropertySpecHandleOffsetPairVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_resize_with(sdf_PropertySpecHandleOffsetPairVector_t* _this, size_t const _Newsize, sdf_PropertySpecHandleOffsetPair_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_op_index(sdf_PropertySpecHandleOffsetPairVector_t const* _this, size_t const _Pos, sdf_PropertySpecHandleOffsetPair_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_default(sdf_PropertySpecHandleOffsetPairVector_t** _result) {
    *_result = new std::vector<std::pair<PXR_NS::SdfPropertySpecHandle, PXR_NS::SdfLayerOffset>>();
    return 0;
}

int sdf_PropertySpecHandleOffsetPairVector_dtor(sdf_PropertySpecHandleOffsetPairVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_PropertySpecView_dtor(sdf_PropertySpecView_t* _this) {
    delete _this;
    return 0;
}

int sdf_Reference_GetAssetPath(sdf_Reference_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetAssetPath();
    return 0;
}

int sdf_Reference_SetAssetPath(sdf_Reference_t* _this, std_String_t const* assetPath) {
    _this->SetAssetPath(*assetPath);
    return 0;
}

int sdf_Reference_GetPrimPath(sdf_Reference_t const* _this, sdf_Path_t const** _result) {
    *_result = &_this->GetPrimPath();
    return 0;
}

int sdf_Reference_SetPrimPath(sdf_Reference_t* _this, sdf_Path_t const* primPath) {
    _this->SetPrimPath(*primPath);
    return 0;
}

int sdf_Reference_GetLayerOffset(sdf_Reference_t const* _this, sdf_LayerOffset_t const** _result) {
    *_result = &_this->GetLayerOffset();
    return 0;
}

int sdf_Reference_SetLayerOffset(sdf_Reference_t* _this, sdf_LayerOffset_t const* layerOffset) {
    _this->SetLayerOffset(*layerOffset);
    return 0;
}

int sdf_Reference_GetCustomData(sdf_Reference_t const* _this, vt_Dictionary_t const** _result) {
    *_result = &_this->GetCustomData();
    return 0;
}

int sdf_Reference_SetCustomData(sdf_Reference_t* _this, vt_Dictionary_t const* customData) {
    _this->SetCustomData(*customData);
    return 0;
}

int sdf_Reference_SetCustomData_value(sdf_Reference_t* _this, std_String_t const* name, vt_Value_t const* value) {
    _this->SetCustomData(*name, *value);
    return 0;
}

int sdf_Reference_SwapCustomData(sdf_Reference_t* _this, vt_Dictionary_t* customData) {
    _this->SwapCustomData(*customData);
    return 0;
}

int sdf_Reference_IsInternal(sdf_Reference_t const* _this, bool* _result) {
    *_result = _this->IsInternal();
    return 0;
}

int sdf_Reference_op_eq(sdf_Reference_t const* _this, sdf_Reference_t const* rhs, bool* _result) {
    *_result = _this->operator==(*rhs);
    return 0;
}

int sdf_Reference_op_lt(sdf_Reference_t const* _this, sdf_Reference_t const* rhs, bool* _result) {
    *_result = _this->operator<(*rhs);
    return 0;
}

int sdf_Reference_dtor(sdf_Reference_t* _this) {
    delete _this;
    return 0;
}

int sdf_ReferenceEditorProxy_dtor(sdf_ReferenceEditorProxy_t* _this) {
    delete _this;
    return 0;
}

int sdf_ReferenceVector_data(sdf_ReferenceVector_t* _this, sdf_Reference_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_ReferenceVector_data_const(sdf_ReferenceVector_t const* _this, sdf_Reference_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_ReferenceVector_empty(sdf_ReferenceVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_ReferenceVector_size(sdf_ReferenceVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_ReferenceVector_max_size(sdf_ReferenceVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_ReferenceVector_reserve(sdf_ReferenceVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_ReferenceVector_capacity(sdf_ReferenceVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_ReferenceVector_clear(sdf_ReferenceVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_ReferenceVector_push_back(sdf_ReferenceVector_t* _this, sdf_Reference_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_ReferenceVector_pop_back(sdf_ReferenceVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_ReferenceVector_resize(sdf_ReferenceVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_ReferenceVector_resize_with(sdf_ReferenceVector_t* _this, size_t const _Newsize, sdf_Reference_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_ReferenceVector_op_index(sdf_ReferenceVector_t const* _this, size_t const _Pos, sdf_Reference_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_ReferenceVector_default(sdf_ReferenceVector_t** _result) {
    *_result = new PXR_NS::SdfReferenceVector();
    return 0;
}

int sdf_ReferenceVector_dtor(sdf_ReferenceVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_RelationshipSpec_New(sdf_PrimSpecHandle_t const* owner, std_String_t const* name, bool custom, int variability, sdf_RelationshipSpecHandle_t* _result) {
    *_result = PXR_NS::SdfRelationshipSpec::New(*owner, *name, custom, static_cast<PXR_NS::SdfVariability>(variability));
    return 0;
}

int sdf_RelationshipSpec_GetNoLoadHint(sdf_RelationshipSpec_t const* _this, bool* _result) {
    *_result = _this->GetNoLoadHint();
    return 0;
}

int sdf_RelationshipSpec_SetNoLoadHint(sdf_RelationshipSpec_t* _this, bool noload) {
    _this->SetNoLoadHint(noload);
    return 0;
}

int sdf_RelationshipSpec_GetTargetPathList(sdf_RelationshipSpec_t const* _this, sdf_InheritsProxy_t* _result) {
    *_result = _this->GetTargetPathList();
    return 0;
}

int sdf_RelationshipSpec_HasTargetPathList(sdf_RelationshipSpec_t const* _this, bool* _result) {
    *_result = _this->HasTargetPathList();
    return 0;
}

int sdf_RelationshipSpec_ClearTargetPathList(sdf_RelationshipSpec_t const* _this) {
    _this->ClearTargetPathList();
    return 0;
}

int sdf_RelationshipSpec_ReplaceTargetPath(sdf_RelationshipSpec_t* _this, sdf_Path_t const* oldPath, sdf_Path_t const* newPath) {
    _this->ReplaceTargetPath(*oldPath, *newPath);
    return 0;
}

int sdf_RelationshipSpec_RemoveTargetPath(sdf_RelationshipSpec_t* _this, sdf_Path_t const* path, bool preserveTargetOrder) {
    _this->RemoveTargetPath(*path, preserveTargetOrder);
    return 0;
}

int sdf_RelationshipSpec_GetOwner(sdf_RelationshipSpec_t const* _this, sdf_SpecHandle_t* _result) {
    *_result = _this->GetOwner();
    return 0;
}

int sdf_RelationshipSpec_GetCustomData(sdf_RelationshipSpec_t const* _this, sdf_DictionaryProxy_t* _result) {
    *_result = _this->GetCustomData();
    return 0;
}

int sdf_RelationshipSpec_GetAssetInfo(sdf_RelationshipSpec_t const* _this, sdf_DictionaryProxy_t* _result) {
    *_result = _this->GetAssetInfo();
    return 0;
}

int sdf_RelationshipSpec_SetCustomData(sdf_RelationshipSpec_t* _this, std_String_t const* name, vt_Value_t const* value) {
    _this->SetCustomData(*name, *value);
    return 0;
}

int sdf_RelationshipSpec_SetAssetInfo(sdf_RelationshipSpec_t* _this, std_String_t const* name, vt_Value_t const* value) {
    _this->SetAssetInfo(*name, *value);
    return 0;
}

int sdf_RelationshipSpec_GetDisplayGroup(sdf_RelationshipSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayGroup();
    return 0;
}

int sdf_RelationshipSpec_SetDisplayGroup(sdf_RelationshipSpec_t* _this, std_String_t const* value) {
    _this->SetDisplayGroup(*value);
    return 0;
}

int sdf_RelationshipSpec_GetDisplayName(sdf_RelationshipSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayName();
    return 0;
}

int sdf_RelationshipSpec_SetDisplayName(sdf_RelationshipSpec_t* _this, std_String_t const* value) {
    _this->SetDisplayName(*value);
    return 0;
}

int sdf_RelationshipSpec_GetDocumentation(sdf_RelationshipSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetDocumentation();
    return 0;
}

int sdf_RelationshipSpec_SetDocumentation(sdf_RelationshipSpec_t* _this, std_String_t const* value) {
    _this->SetDocumentation(*value);
    return 0;
}

int sdf_RelationshipSpec_GetHidden(sdf_RelationshipSpec_t const* _this, bool* _result) {
    *_result = _this->GetHidden();
    return 0;
}

int sdf_RelationshipSpec_SetHidden(sdf_RelationshipSpec_t* _this, bool value) {
    _this->SetHidden(value);
    return 0;
}

int sdf_RelationshipSpec_GetPermission(sdf_RelationshipSpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetPermission());
    return 0;
}

int sdf_RelationshipSpec_SetPermission(sdf_RelationshipSpec_t* _this, int value) {
    _this->SetPermission(static_cast<PXR_NS::SdfPermission>(value));
    return 0;
}

int sdf_RelationshipSpec_GetPrefix(sdf_RelationshipSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetPrefix();
    return 0;
}

int sdf_RelationshipSpec_SetPrefix(sdf_RelationshipSpec_t* _this, std_String_t const* value) {
    _this->SetPrefix(*value);
    return 0;
}

int sdf_RelationshipSpec_GetSuffix(sdf_RelationshipSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetSuffix();
    return 0;
}

int sdf_RelationshipSpec_SetSuffix(sdf_RelationshipSpec_t* _this, std_String_t const* value) {
    _this->SetSuffix(*value);
    return 0;
}

int sdf_RelationshipSpec_GetSymmetricPeer(sdf_RelationshipSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetSymmetricPeer();
    return 0;
}

int sdf_RelationshipSpec_SetSymmetricPeer(sdf_RelationshipSpec_t* _this, std_String_t const* peerName) {
    _this->SetSymmetricPeer(*peerName);
    return 0;
}

int sdf_RelationshipSpec_GetSymmetryArguments(sdf_RelationshipSpec_t const* _this, sdf_DictionaryProxy_t* _result) {
    *_result = _this->GetSymmetryArguments();
    return 0;
}

int sdf_RelationshipSpec_SetSymmetryArgument(sdf_RelationshipSpec_t* _this, std_String_t const* name, vt_Value_t const* value) {
    _this->SetSymmetryArgument(*name, *value);
    return 0;
}

int sdf_RelationshipSpec_GetSymmetryFunction(sdf_RelationshipSpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetSymmetryFunction();
    return 0;
}

int sdf_RelationshipSpec_SetSymmetryFunction(sdf_RelationshipSpec_t* _this, tf_Token_t const* functionName) {
    _this->SetSymmetryFunction(*functionName);
    return 0;
}

int sdf_RelationshipSpec_GetTimeSampleMap(sdf_RelationshipSpec_t const* _this, sdf_TimeSampleMap_t* _result) {
    *_result = _this->GetTimeSampleMap();
    return 0;
}

int sdf_RelationshipSpec_GetValueType(sdf_RelationshipSpec_t const* _this, tf_Type_t* _result) {
    *_result = _this->GetValueType();
    return 0;
}

int sdf_RelationshipSpec_GetTypeName(sdf_RelationshipSpec_t const* _this, sdf_ValueTypeName_t* _result) {
    *_result = _this->GetTypeName();
    return 0;
}

int sdf_RelationshipSpec_GetDefaultValue(sdf_RelationshipSpec_t const* _this, vt_Value_t* _result) {
    *_result = _this->GetDefaultValue();
    return 0;
}

int sdf_RelationshipSpec_SetDefaultValue(sdf_RelationshipSpec_t* _this, vt_Value_t const* defaultValue, bool* _result) {
    *_result = _this->SetDefaultValue(*defaultValue);
    return 0;
}

int sdf_RelationshipSpec_HasDefaultValue(sdf_RelationshipSpec_t const* _this, bool* _result) {
    *_result = _this->HasDefaultValue();
    return 0;
}

int sdf_RelationshipSpec_ClearDefaultValue(sdf_RelationshipSpec_t* _this) {
    _this->ClearDefaultValue();
    return 0;
}

int sdf_RelationshipSpec_GetComment(sdf_RelationshipSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetComment();
    return 0;
}

int sdf_RelationshipSpec_SetComment(sdf_RelationshipSpec_t* _this, std_String_t const* value) {
    _this->SetComment(*value);
    return 0;
}

int sdf_RelationshipSpec_IsCustom(sdf_RelationshipSpec_t const* _this, bool* _result) {
    *_result = _this->IsCustom();
    return 0;
}

int sdf_RelationshipSpec_SetCustom(sdf_RelationshipSpec_t* _this, bool custom) {
    _this->SetCustom(custom);
    return 0;
}

int sdf_RelationshipSpec_GetVariability(sdf_RelationshipSpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetVariability());
    return 0;
}

int sdf_RelationshipSpec_HasOnlyRequiredFields(sdf_RelationshipSpec_t const* _this, bool* _result) {
    *_result = _this->HasOnlyRequiredFields();
    return 0;
}

int sdf_RelationshipSpec_GetName(sdf_RelationshipSpec_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int sdf_RelationshipSpec_GetNameToken(sdf_RelationshipSpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNameToken();
    return 0;
}

int sdf_RelationshipSpec_CanSetName(sdf_RelationshipSpec_t const* _this, std_String_t const* newName, std_String_t* whyNot, bool* _result) {
    *_result = _this->CanSetName(*newName, whyNot);
    return 0;
}

int sdf_RelationshipSpec_SetName(sdf_RelationshipSpec_t* _this, std_String_t const* newName, bool validate, bool* _result) {
    *_result = _this->SetName(*newName, validate);
    return 0;
}

int sdf_RelationshipSpec_IsValidName(std_String_t const* name, bool* _result) {
    *_result = PXR_NS::SdfRelationshipSpec::IsValidName(*name);
    return 0;
}

int sdf_RelationshipSpec_GetSchema(sdf_RelationshipSpec_t const* _this, sdf_SchemaBase_t const** _result) {
    *_result = &_this->GetSchema();
    return 0;
}

int sdf_RelationshipSpec_GetSpecType(sdf_RelationshipSpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSpecType());
    return 0;
}

int sdf_RelationshipSpec_IsDormant(sdf_RelationshipSpec_t const* _this, bool* _result) {
    *_result = _this->IsDormant();
    return 0;
}

int sdf_RelationshipSpec_GetLayer(sdf_RelationshipSpec_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetLayer();
    return 0;
}

int sdf_RelationshipSpec_GetPath(sdf_RelationshipSpec_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int sdf_RelationshipSpec_PermissionToEdit(sdf_RelationshipSpec_t const* _this, bool* _result) {
    *_result = _this->PermissionToEdit();
    return 0;
}

int sdf_RelationshipSpec_ListInfoKeys(sdf_RelationshipSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListInfoKeys();
    return 0;
}

int sdf_RelationshipSpec_GetMetaDataInfoKeys(sdf_RelationshipSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->GetMetaDataInfoKeys();
    return 0;
}

int sdf_RelationshipSpec_GetMetaDataDisplayGroup(sdf_RelationshipSpec_t const* _this, tf_Token_t const* key, tf_Token_t* _result) {
    *_result = _this->GetMetaDataDisplayGroup(*key);
    return 0;
}

int sdf_RelationshipSpec_GetInfo(sdf_RelationshipSpec_t const* _this, tf_Token_t const* key, vt_Value_t* _result) {
    *_result = _this->GetInfo(*key);
    return 0;
}

int sdf_RelationshipSpec_SetInfo(sdf_RelationshipSpec_t* _this, tf_Token_t const* key, vt_Value_t const* value) {
    _this->SetInfo(*key, *value);
    return 0;
}

int sdf_RelationshipSpec_SetInfoDictionaryValue(sdf_RelationshipSpec_t* _this, tf_Token_t const* dictionaryKey, tf_Token_t const* entryKey, vt_Value_t const* value) {
    _this->SetInfoDictionaryValue(*dictionaryKey, *entryKey, *value);
    return 0;
}

int sdf_RelationshipSpec_HasInfo(sdf_RelationshipSpec_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasInfo(*key);
    return 0;
}

int sdf_RelationshipSpec_ClearInfo(sdf_RelationshipSpec_t* _this, tf_Token_t const* key) {
    _this->ClearInfo(*key);
    return 0;
}

int sdf_RelationshipSpec_GetTypeForInfo(sdf_RelationshipSpec_t const* _this, tf_Token_t const* key, tf_Type_t* _result) {
    *_result = _this->GetTypeForInfo(*key);
    return 0;
}

int sdf_RelationshipSpec_GetFallbackForInfo(sdf_RelationshipSpec_t const* _this, tf_Token_t const* key, vt_Value_t const** _result) {
    *_result = &_this->GetFallbackForInfo(*key);
    return 0;
}

int sdf_RelationshipSpec_IsInert(sdf_RelationshipSpec_t const* _this, bool ignoreChildren, bool* _result) {
    *_result = _this->IsInert(ignoreChildren);
    return 0;
}

int sdf_RelationshipSpec_ListFields(sdf_RelationshipSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListFields();
    return 0;
}

int sdf_RelationshipSpec_HasField(sdf_RelationshipSpec_t const* _this, tf_Token_t const* name, bool* _result) {
    *_result = _this->HasField(*name);
    return 0;
}

int sdf_RelationshipSpec_GetField(sdf_RelationshipSpec_t const* _this, tf_Token_t const* name, vt_Value_t* _result) {
    *_result = _this->GetField(*name);
    return 0;
}

int sdf_RelationshipSpec_SetField(sdf_RelationshipSpec_t* _this, tf_Token_t const* name, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetField(*name, *value);
    return 0;
}

int sdf_RelationshipSpec_op_eq(sdf_RelationshipSpec_t const* _this, sdf_Spec_t const* rhs, bool* _result) {
    *_result = _this->operator==(*rhs);
    return 0;
}

int sdf_RelationshipSpec_dtor(sdf_RelationshipSpec_t* _this) {
    delete _this;
    return 0;
}

int sdf_RelationshipSpecHandle_dtor(sdf_RelationshipSpecHandle_t* _this) {
    delete _this;
    return 0;
}

int sdf_RelationshipSpecHandleVector_data(sdf_RelationshipSpecHandleVector_t* _this, sdf_RelationshipSpecHandle_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_RelationshipSpecHandleVector_data_const(sdf_RelationshipSpecHandleVector_t const* _this, sdf_RelationshipSpecHandle_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_RelationshipSpecHandleVector_empty(sdf_RelationshipSpecHandleVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_RelationshipSpecHandleVector_size(sdf_RelationshipSpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_RelationshipSpecHandleVector_max_size(sdf_RelationshipSpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_RelationshipSpecHandleVector_reserve(sdf_RelationshipSpecHandleVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_RelationshipSpecHandleVector_capacity(sdf_RelationshipSpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_RelationshipSpecHandleVector_clear(sdf_RelationshipSpecHandleVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_RelationshipSpecHandleVector_push_back(sdf_RelationshipSpecHandleVector_t* _this, sdf_RelationshipSpecHandle_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_RelationshipSpecHandleVector_pop_back(sdf_RelationshipSpecHandleVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_RelationshipSpecHandleVector_resize(sdf_RelationshipSpecHandleVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_RelationshipSpecHandleVector_resize_with(sdf_RelationshipSpecHandleVector_t* _this, size_t const _Newsize, sdf_RelationshipSpecHandle_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_RelationshipSpecHandleVector_op_index(sdf_RelationshipSpecHandleVector_t const* _this, size_t const _Pos, sdf_RelationshipSpecHandle_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_RelationshipSpecHandleVector_default(sdf_RelationshipSpecHandleVector_t** _result) {
    *_result = new PXR_NS::SdfRelationshipSpecHandleVector();
    return 0;
}

int sdf_RelationshipSpecHandleVector_dtor(sdf_RelationshipSpecHandleVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_RelationshipSpecView_dtor(sdf_RelationshipSpecView_t* _this) {
    delete _this;
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPair_dtor(sdf_RelationshipSpecHandleOffsetPair_t* _this) {
    delete _this;
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_data(sdf_RelationshipSpecHandleOffsetPairVector_t* _this, sdf_RelationshipSpecHandleOffsetPair_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_data_const(sdf_RelationshipSpecHandleOffsetPairVector_t const* _this, sdf_RelationshipSpecHandleOffsetPair_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_empty(sdf_RelationshipSpecHandleOffsetPairVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_size(sdf_RelationshipSpecHandleOffsetPairVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_max_size(sdf_RelationshipSpecHandleOffsetPairVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_reserve(sdf_RelationshipSpecHandleOffsetPairVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_capacity(sdf_RelationshipSpecHandleOffsetPairVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_clear(sdf_RelationshipSpecHandleOffsetPairVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_push_back(sdf_RelationshipSpecHandleOffsetPairVector_t* _this, sdf_RelationshipSpecHandleOffsetPair_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_pop_back(sdf_RelationshipSpecHandleOffsetPairVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_resize(sdf_RelationshipSpecHandleOffsetPairVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_resize_with(sdf_RelationshipSpecHandleOffsetPairVector_t* _this, size_t const _Newsize, sdf_RelationshipSpecHandleOffsetPair_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_op_index(sdf_RelationshipSpecHandleOffsetPairVector_t const* _this, size_t const _Pos, sdf_RelationshipSpecHandleOffsetPair_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_default(sdf_RelationshipSpecHandleOffsetPairVector_t** _result) {
    *_result = new std::vector<
        std::pair<PXR_NS::SdfRelationshipSpecHandle, PXR_NS::SdfLayerOffset>>();
    return 0;
}

int sdf_RelationshipSpecHandleOffsetPairVector_dtor(sdf_RelationshipSpecHandleOffsetPairVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_RelocatesMap_dtor(sdf_RelocatesMap_t* _this) {
    delete _this;
    return 0;
}

int sdf_RelocatesMapProxy_dtor(sdf_RelocatesMapProxy_t* _this) {
    delete _this;
    return 0;
}

int sdf_SchemaBase_GetFieldDefinition(sdf_SchemaBase_t const* _this, tf_Token_t const* fieldKey, sdf_SchemaBaseFieldDefinition_t const** _result) {
    *_result = _this->GetFieldDefinition(*fieldKey);
    return 0;
}

int sdf_SchemaBase_GetSpecDefinition(sdf_SchemaBase_t const* _this, int specType, sdf_SchemaBaseSpecDefinition_t const** _result) {
    *_result = _this->GetSpecDefinition(static_cast<PXR_NS::SdfSpecType>(specType));
    return 0;
}

int sdf_SchemaBase_IsRegistered(sdf_SchemaBase_t const* _this, tf_Token_t const* fieldKey, vt_Value_t* fallback, bool* _result) {
    *_result = _this->IsRegistered(*fieldKey, fallback);
    return 0;
}

int sdf_SchemaBase_HoldsChildren(sdf_SchemaBase_t const* _this, tf_Token_t const* fieldKey, bool* _result) {
    *_result = _this->HoldsChildren(*fieldKey);
    return 0;
}

int sdf_SchemaBase_GetFallback(sdf_SchemaBase_t const* _this, tf_Token_t const* fieldKey, vt_Value_t const** _result) {
    *_result = &_this->GetFallback(*fieldKey);
    return 0;
}

int sdf_SchemaBase_CastToTypeOf(sdf_SchemaBase_t const* _this, tf_Token_t const* fieldKey, vt_Value_t const* value, vt_Value_t* _result) {
    *_result = _this->CastToTypeOf(*fieldKey, *value);
    return 0;
}

int sdf_SchemaBase_IsValidFieldForSpec(sdf_SchemaBase_t const* _this, tf_Token_t const* fieldKey, int specType, bool* _result) {
    *_result = _this->IsValidFieldForSpec(*fieldKey, static_cast<PXR_NS::SdfSpecType>(specType));
    return 0;
}

int sdf_SchemaBase_GetFields(sdf_SchemaBase_t const* _this, int specType, tf_TokenVector_t* _result) {
    *_result = _this->GetFields(static_cast<PXR_NS::SdfSpecType>(specType));
    return 0;
}

int sdf_SchemaBase_GetMetadataFields(sdf_SchemaBase_t const* _this, int specType, tf_TokenVector_t* _result) {
    *_result = _this->GetMetadataFields(static_cast<PXR_NS::SdfSpecType>(specType));
    return 0;
}

int sdf_SchemaBase_GetMetadataFieldDisplayGroup(sdf_SchemaBase_t const* _this, int specType, tf_Token_t const* metadataField, tf_Token_t* _result) {
    *_result = _this->GetMetadataFieldDisplayGroup(static_cast<PXR_NS::SdfSpecType>(specType), *metadataField);
    return 0;
}

int sdf_SchemaBase_GetRequiredFields(sdf_SchemaBase_t const* _this, int specType, tf_TokenVector_t const** _result) {
    *_result = &_this->GetRequiredFields(static_cast<PXR_NS::SdfSpecType>(specType));
    return 0;
}

int sdf_SchemaBase_IsRequiredFieldName(sdf_SchemaBase_t const* _this, tf_Token_t const* fieldName, bool* _result) {
    *_result = _this->IsRequiredFieldName(*fieldName);
    return 0;
}

int sdf_SchemaBase_IsValidValue(sdf_SchemaBase_t const* _this, vt_Value_t const* value, sdf_Allowed_t* _result) {
    *_result = _this->IsValidValue(*value);
    return 0;
}

int sdf_SchemaBase_GetAllTypes(sdf_SchemaBase_t const* _this, sdf_ValueTypeNameVector_t* _result) {
    *_result = _this->GetAllTypes();
    return 0;
}

int sdf_SchemaBase_FindType(sdf_SchemaBase_t const* _this, tf_Token_t const* typeName, sdf_ValueTypeName_t* _result) {
    *_result = _this->FindType(*typeName);
    return 0;
}

int sdf_SchemaBase_FindType_with_role(sdf_SchemaBase_t const* _this, tf_Type_t const* type, tf_Token_t const* role, sdf_ValueTypeName_t* _result) {
    *_result = _this->FindType(*type, *role);
    return 0;
}

int sdf_SchemaBase_FindOrCreateType(sdf_SchemaBase_t const* _this, tf_Token_t const* typeName, sdf_ValueTypeName_t* _result) {
    *_result = _this->FindOrCreateType(*typeName);
    return 0;
}

int sdf_SchemaBase_IsValidAttributeConnectionPath(sdf_Path_t const* path, sdf_Allowed_t* _result) {
    *_result = PXR_NS::SdfSchemaBase::IsValidAttributeConnectionPath(*path);
    return 0;
}

int sdf_SchemaBase_IsValidIdentifier(std_String_t const* name, sdf_Allowed_t* _result) {
    *_result = PXR_NS::SdfSchemaBase::IsValidIdentifier(*name);
    return 0;
}

int sdf_SchemaBase_IsValidNamespacedIdentifier(std_String_t const* name, sdf_Allowed_t* _result) {
    *_result = PXR_NS::SdfSchemaBase::IsValidNamespacedIdentifier(*name);
    return 0;
}

int sdf_SchemaBase_IsValidInheritPath(sdf_Path_t const* path, sdf_Allowed_t* _result) {
    *_result = PXR_NS::SdfSchemaBase::IsValidInheritPath(*path);
    return 0;
}

int sdf_SchemaBase_IsValidPayload(sdf_Payload_t const* payload, sdf_Allowed_t* _result) {
    *_result = PXR_NS::SdfSchemaBase::IsValidPayload(*payload);
    return 0;
}

int sdf_SchemaBase_IsValidReference(sdf_Reference_t const* ref, sdf_Allowed_t* _result) {
    *_result = PXR_NS::SdfSchemaBase::IsValidReference(*ref);
    return 0;
}

int sdf_SchemaBase_IsValidRelationshipTargetPath(sdf_Path_t const* path, sdf_Allowed_t* _result) {
    *_result = PXR_NS::SdfSchemaBase::IsValidRelationshipTargetPath(*path);
    return 0;
}

int sdf_SchemaBase_IsValidRelocatesPath(sdf_Path_t const* path, sdf_Allowed_t* _result) {
    *_result = PXR_NS::SdfSchemaBase::IsValidRelocatesPath(*path);
    return 0;
}

int sdf_SchemaBase_IsValidSpecializesPath(sdf_Path_t const* path, sdf_Allowed_t* _result) {
    *_result = PXR_NS::SdfSchemaBase::IsValidSpecializesPath(*path);
    return 0;
}

int sdf_SchemaBase_IsValidSubLayer(std_String_t const* sublayer, sdf_Allowed_t* _result) {
    *_result = PXR_NS::SdfSchemaBase::IsValidSubLayer(*sublayer);
    return 0;
}

int sdf_SchemaBase_IsValidVariantIdentifier(std_String_t const* name, sdf_Allowed_t* _result) {
    *_result = PXR_NS::SdfSchemaBase::IsValidVariantIdentifier(*name);
    return 0;
}

int sdf_SchemaBase_IsValidVariantSelection(std_String_t const* sel, sdf_Allowed_t* _result) {
    *_result = PXR_NS::SdfSchemaBase::IsValidVariantSelection(*sel);
    return 0;
}

int sdf_SchemaBaseFieldDefinition_GetName(sdf_SchemaBaseFieldDefinition_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int sdf_SchemaBaseFieldDefinition_GetFallbackValue(sdf_SchemaBaseFieldDefinition_t const* _this, vt_Value_t const** _result) {
    *_result = &_this->GetFallbackValue();
    return 0;
}

int sdf_SchemaBaseFieldDefinition_GetInfo(sdf_SchemaBaseFieldDefinition_t const* _this, sdf_SchemaBaseFieldDefinitionInfoVec_t const** _result) {
    *_result = &_this->GetInfo();
    return 0;
}

int sdf_SchemaBaseFieldDefinition_IsPlugin(sdf_SchemaBaseFieldDefinition_t const* _this, bool* _result) {
    *_result = _this->IsPlugin();
    return 0;
}

int sdf_SchemaBaseFieldDefinition_IsReadOnly(sdf_SchemaBaseFieldDefinition_t const* _this, bool* _result) {
    *_result = _this->IsReadOnly();
    return 0;
}

int sdf_SchemaBaseFieldDefinition_HoldsChildren(sdf_SchemaBaseFieldDefinition_t const* _this, bool* _result) {
    *_result = _this->HoldsChildren();
    return 0;
}

int sdf_SchemaBaseFieldDefinition_dtor(sdf_SchemaBaseFieldDefinition_t* _this) {
    delete _this;
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_data(sdf_SchemaBaseFieldDefinitionInfoVec_t* _this, sdf_TokenValuePair_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_data_const(sdf_SchemaBaseFieldDefinitionInfoVec_t const* _this, sdf_TokenValuePair_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_empty(sdf_SchemaBaseFieldDefinitionInfoVec_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_size(sdf_SchemaBaseFieldDefinitionInfoVec_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_max_size(sdf_SchemaBaseFieldDefinitionInfoVec_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_reserve(sdf_SchemaBaseFieldDefinitionInfoVec_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_capacity(sdf_SchemaBaseFieldDefinitionInfoVec_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_clear(sdf_SchemaBaseFieldDefinitionInfoVec_t* _this) {
    _this->clear();
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_push_back(sdf_SchemaBaseFieldDefinitionInfoVec_t* _this, sdf_TokenValuePair_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_pop_back(sdf_SchemaBaseFieldDefinitionInfoVec_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_resize(sdf_SchemaBaseFieldDefinitionInfoVec_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_resize_with(sdf_SchemaBaseFieldDefinitionInfoVec_t* _this, size_t const _Newsize, sdf_TokenValuePair_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_op_index(sdf_SchemaBaseFieldDefinitionInfoVec_t const* _this, size_t const _Pos, sdf_TokenValuePair_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_default(sdf_SchemaBaseFieldDefinitionInfoVec_t** _result) {
    *_result = new PXR_NS::SdfSchemaBase::FieldDefinition::InfoVec();
    return 0;
}

int sdf_SchemaBaseFieldDefinitionInfoVec_dtor(sdf_SchemaBaseFieldDefinitionInfoVec_t* _this) {
    delete _this;
    return 0;
}

int sdf_TokenValuePair_dtor(sdf_TokenValuePair_t* _this) {
    delete _this;
    return 0;
}

int sdf_SchemaBaseSpecDefinition_dtor(sdf_SchemaBaseSpecDefinition_t* _this) {
    delete _this;
    return 0;
}

int sdf_Spec_GetSchema(sdf_Spec_t const* _this, sdf_SchemaBase_t const** _result) {
    *_result = &_this->GetSchema();
    return 0;
}

int sdf_Spec_GetSpecType(sdf_Spec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSpecType());
    return 0;
}

int sdf_Spec_IsDormant(sdf_Spec_t const* _this, bool* _result) {
    *_result = _this->IsDormant();
    return 0;
}

int sdf_Spec_GetLayer(sdf_Spec_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetLayer();
    return 0;
}

int sdf_Spec_GetPath(sdf_Spec_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int sdf_Spec_PermissionToEdit(sdf_Spec_t const* _this, bool* _result) {
    *_result = _this->PermissionToEdit();
    return 0;
}

int sdf_Spec_ListInfoKeys(sdf_Spec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListInfoKeys();
    return 0;
}

int sdf_Spec_GetMetaDataInfoKeys(sdf_Spec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->GetMetaDataInfoKeys();
    return 0;
}

int sdf_Spec_GetMetaDataDisplayGroup(sdf_Spec_t const* _this, tf_Token_t const* key, tf_Token_t* _result) {
    *_result = _this->GetMetaDataDisplayGroup(*key);
    return 0;
}

int sdf_Spec_GetInfo(sdf_Spec_t const* _this, tf_Token_t const* key, vt_Value_t* _result) {
    *_result = _this->GetInfo(*key);
    return 0;
}

int sdf_Spec_SetInfo(sdf_Spec_t* _this, tf_Token_t const* key, vt_Value_t const* value) {
    _this->SetInfo(*key, *value);
    return 0;
}

int sdf_Spec_SetInfoDictionaryValue(sdf_Spec_t* _this, tf_Token_t const* dictionaryKey, tf_Token_t const* entryKey, vt_Value_t const* value) {
    _this->SetInfoDictionaryValue(*dictionaryKey, *entryKey, *value);
    return 0;
}

int sdf_Spec_HasInfo(sdf_Spec_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasInfo(*key);
    return 0;
}

int sdf_Spec_ClearInfo(sdf_Spec_t* _this, tf_Token_t const* key) {
    _this->ClearInfo(*key);
    return 0;
}

int sdf_Spec_GetTypeForInfo(sdf_Spec_t const* _this, tf_Token_t const* key, tf_Type_t* _result) {
    *_result = _this->GetTypeForInfo(*key);
    return 0;
}

int sdf_Spec_GetFallbackForInfo(sdf_Spec_t const* _this, tf_Token_t const* key, vt_Value_t const** _result) {
    *_result = &_this->GetFallbackForInfo(*key);
    return 0;
}

int sdf_Spec_IsInert(sdf_Spec_t const* _this, bool ignoreChildren, bool* _result) {
    *_result = _this->IsInert(ignoreChildren);
    return 0;
}

int sdf_Spec_ListFields(sdf_Spec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListFields();
    return 0;
}

int sdf_Spec_HasField(sdf_Spec_t const* _this, tf_Token_t const* name, bool* _result) {
    *_result = _this->HasField(*name);
    return 0;
}

int sdf_Spec_GetField(sdf_Spec_t const* _this, tf_Token_t const* name, vt_Value_t* _result) {
    *_result = _this->GetField(*name);
    return 0;
}

int sdf_Spec_SetField(sdf_Spec_t* _this, tf_Token_t const* name, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetField(*name, *value);
    return 0;
}

int sdf_Spec_op_eq(sdf_Spec_t const* _this, sdf_Spec_t const* rhs, bool* _result) {
    *_result = _this->operator==(*rhs);
    return 0;
}

int sdf_Spec_dtor(sdf_Spec_t* _this) {
    delete _this;
    return 0;
}

int sdf_SpecHandle_dtor(sdf_SpecHandle_t* _this) {
    delete _this;
    return 0;
}

int sdf_SubLayerProxy_dtor(sdf_SubLayerProxy_t* _this) {
    delete _this;
    return 0;
}

int sdf_TextFileFormat_GetSchema(sdf_TextFileFormat_t const* _this, sdf_SchemaBase_t const** _result) {
    *_result = &_this->GetSchema();
    return 0;
}

int sdf_TextFileFormat_GetFormatId(sdf_TextFileFormat_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetFormatId();
    return 0;
}

int sdf_TextFileFormat_GetTarget(sdf_TextFileFormat_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetTarget();
    return 0;
}

int sdf_TextFileFormat_GetFileCookie(sdf_TextFileFormat_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetFileCookie();
    return 0;
}

int sdf_TextFileFormat_GetVersionString(sdf_TextFileFormat_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetVersionString();
    return 0;
}

int sdf_TextFileFormat_IsPrimaryFormatForExtensions(sdf_TextFileFormat_t const* _this, bool* _result) {
    *_result = _this->IsPrimaryFormatForExtensions();
    return 0;
}

int sdf_TextFileFormat_GetFileExtensions(sdf_TextFileFormat_t const* _this, std_StringVector_t const** _result) {
    *_result = &_this->GetFileExtensions();
    return 0;
}

int sdf_TextFileFormat_GetPrimaryFileExtension(sdf_TextFileFormat_t const* _this, std_String_t const** _result) {
    *_result = &_this->GetPrimaryFileExtension();
    return 0;
}

int sdf_TextFileFormat_IsSupportedExtension(sdf_TextFileFormat_t const* _this, std_String_t const* extension, bool* _result) {
    *_result = _this->IsSupportedExtension(*extension);
    return 0;
}

int sdf_TextFileFormat_IsPackage(sdf_TextFileFormat_t const* _this, bool* _result) {
    *_result = _this->IsPackage();
    return 0;
}

int sdf_TextFileFormat_GetPackageRootLayerPath(sdf_TextFileFormat_t const* _this, std_String_t const* resolvedPath, std_String_t* _result) {
    *_result = _this->GetPackageRootLayerPath(*resolvedPath);
    return 0;
}

int sdf_TextFileFormat_ShouldSkipAnonymousReload(sdf_TextFileFormat_t const* _this, bool* _result) {
    *_result = _this->ShouldSkipAnonymousReload();
    return 0;
}

int sdf_TextFileFormat_ShouldReadAnonymousLayers(sdf_TextFileFormat_t const* _this, bool* _result) {
    *_result = _this->ShouldReadAnonymousLayers();
    return 0;
}

int sdf_TextFileFormat_ReadDetached(sdf_TextFileFormat_t const* _this, sdf_Layer_t* layer, std_String_t const* resolvedPath, bool metadataOnly, bool* _result) {
    *_result = _this->ReadDetached(layer, *resolvedPath, metadataOnly);
    return 0;
}

int sdf_TextFileFormat_GetExternalAssetDependencies(sdf_TextFileFormat_t const* _this, sdf_Layer_t const* layer, std_StringSet_t* _result) {
    *_result = _this->GetExternalAssetDependencies(*layer);
    return 0;
}

int sdf_TextFileFormat_SupportsReading(sdf_TextFileFormat_t const* _this, bool* _result) {
    *_result = _this->SupportsReading();
    return 0;
}

int sdf_TextFileFormat_SupportsWriting(sdf_TextFileFormat_t const* _this, bool* _result) {
    *_result = _this->SupportsWriting();
    return 0;
}

int sdf_TextFileFormat_SupportsEditing(sdf_TextFileFormat_t const* _this, bool* _result) {
    *_result = _this->SupportsEditing();
    return 0;
}

int sdf_TextFileFormat_GetFileExtension(std_String_t const* s, std_String_t* _result) {
    *_result = PXR_NS::SdfTextFileFormat::GetFileExtension(*s);
    return 0;
}

int sdf_TextFileFormat_FindAllFileFormatExtensions(std_StringSet_t* _result) {
    *_result = PXR_NS::SdfTextFileFormat::FindAllFileFormatExtensions();
    return 0;
}

int sdf_TextFileFormat_FindAllDerivedFileFormatExtensions(tf_Type_t const* baseType, std_StringSet_t* _result) {
    *_result = PXR_NS::SdfTextFileFormat::FindAllDerivedFileFormatExtensions(*baseType);
    return 0;
}

int sdf_TextFileFormat_FormatSupportsReading(std_String_t const* extension, std_String_t const* target, bool* _result) {
    *_result = PXR_NS::SdfTextFileFormat::FormatSupportsReading(*extension, *target);
    return 0;
}

int sdf_TextFileFormat_FormatSupportsWriting(std_String_t const* extension, std_String_t const* target, bool* _result) {
    *_result = PXR_NS::SdfTextFileFormat::FormatSupportsWriting(*extension, *target);
    return 0;
}

int sdf_TextFileFormat_FormatSupportsEditing(std_String_t const* extension, std_String_t const* target, bool* _result) {
    *_result = PXR_NS::SdfTextFileFormat::FormatSupportsEditing(*extension, *target);
    return 0;
}

int sdf_TextFileFormat_CanRead(sdf_TextFileFormat_t const* _this, std_String_t const* file, bool* _result) {
    *_result = _this->CanRead(*file);
    return 0;
}

int sdf_TextFileFormat_Read(sdf_TextFileFormat_t const* _this, sdf_Layer_t* layer, std_String_t const* resolvedPath, bool metadataOnly, bool* _result) {
    *_result = _this->Read(layer, *resolvedPath, metadataOnly);
    return 0;
}

int sdf_TextFileFormat_WriteToFile(sdf_TextFileFormat_t const* _this, sdf_Layer_t const* layer, std_String_t const* filePath, std_String_t const* comment, sdf_LayerFileFormatArguments_t const* args, bool* _result) {
    *_result = _this->WriteToFile(*layer, *filePath, *comment, *args);
    return 0;
}

int sdf_TextFileFormat_ReadFromString(sdf_TextFileFormat_t const* _this, sdf_Layer_t* layer, std_String_t const* str, bool* _result) {
    *_result = _this->ReadFromString(layer, *str);
    return 0;
}

int sdf_TextFileFormat_WriteToString(sdf_TextFileFormat_t const* _this, sdf_Layer_t const* layer, std_String_t* str, std_String_t const* comment, bool* _result) {
    *_result = _this->WriteToString(*layer, str, *comment);
    return 0;
}

int sdf_TimeCode_GetValue(sdf_TimeCode_t const* _this, double* _result) {
    *_result = _this->GetValue();
    return 0;
}

int sdf_TimeCode_from_time(double param00, sdf_TimeCode_t** _result) {
    *_result = new PXR_NS::SdfTimeCode(param00);
    return 0;
}

int sdf_TimeCode_dtor(sdf_TimeCode_t* _this) {
    delete _this;
    return 0;
}

int sdf_TimeSampleMap_dtor(sdf_TimeSampleMap_t* _this) {
    delete _this;
    return 0;
}

int sdf_ValueBlock_new(sdf_ValueBlock_t** _result) {
    *_result = new PXR_NS::SdfValueBlock();
    return 0;
}

int sdf_ValueBlock_dtor(sdf_ValueBlock_t* _this) {
    delete _this;
    return 0;
}

int sdf_ValueTypeName_GetAsToken(sdf_ValueTypeName_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetAsToken();
    return 0;
}

int sdf_ValueTypeName_GetType(sdf_ValueTypeName_t const* _this, tf_Type_t const** _result) {
    *_result = &_this->GetType();
    return 0;
}

int sdf_ValueTypeName_GetRole(sdf_ValueTypeName_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetRole();
    return 0;
}

int sdf_ValueTypeName_GetDefaultValue(sdf_ValueTypeName_t const* _this, vt_Value_t const** _result) {
    *_result = &_this->GetDefaultValue();
    return 0;
}

int sdf_ValueTypeName_GetDefaultUnit(sdf_ValueTypeName_t const* _this, tf_Enum_t const** _result) {
    *_result = &_this->GetDefaultUnit();
    return 0;
}

int sdf_ValueTypeName_GetScalarType(sdf_ValueTypeName_t const* _this, sdf_ValueTypeName_t* _result) {
    *_result = _this->GetScalarType();
    return 0;
}

int sdf_ValueTypeName_GetArrayType(sdf_ValueTypeName_t const* _this, sdf_ValueTypeName_t* _result) {
    *_result = _this->GetArrayType();
    return 0;
}

int sdf_ValueTypeName_IsScalar(sdf_ValueTypeName_t const* _this, bool* _result) {
    *_result = _this->IsScalar();
    return 0;
}

int sdf_ValueTypeName_IsArray(sdf_ValueTypeName_t const* _this, bool* _result) {
    *_result = _this->IsArray();
    return 0;
}

int sdf_ValueTypeName_GetDimensions(sdf_ValueTypeName_t const* _this, sdf_TupleDimensions_t* _result) {
    *_result = _this->GetDimensions();
    return 0;
}

int sdf_ValueTypeName_GetAliasesAsTokens(sdf_ValueTypeName_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->GetAliasesAsTokens();
    return 0;
}

int sdf_ValueTypeName_new(sdf_ValueTypeName_t** _result) {
    *_result = new PXR_NS::SdfValueTypeName();
    return 0;
}

int sdf_ValueTypeName_dtor(sdf_ValueTypeName_t* _this) {
    delete _this;
    return 0;
}

int sdf_ValueTypeNameVector_data(sdf_ValueTypeNameVector_t* _this, sdf_ValueTypeName_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_ValueTypeNameVector_data_const(sdf_ValueTypeNameVector_t const* _this, sdf_ValueTypeName_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_ValueTypeNameVector_empty(sdf_ValueTypeNameVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_ValueTypeNameVector_size(sdf_ValueTypeNameVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_ValueTypeNameVector_max_size(sdf_ValueTypeNameVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_ValueTypeNameVector_reserve(sdf_ValueTypeNameVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_ValueTypeNameVector_capacity(sdf_ValueTypeNameVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_ValueTypeNameVector_clear(sdf_ValueTypeNameVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_ValueTypeNameVector_push_back(sdf_ValueTypeNameVector_t* _this, sdf_ValueTypeName_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_ValueTypeNameVector_pop_back(sdf_ValueTypeNameVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_ValueTypeNameVector_resize(sdf_ValueTypeNameVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_ValueTypeNameVector_resize_with(sdf_ValueTypeNameVector_t* _this, size_t const _Newsize, sdf_ValueTypeName_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_ValueTypeNameVector_op_index(sdf_ValueTypeNameVector_t const* _this, size_t const _Pos, sdf_ValueTypeName_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_ValueTypeNameVector_default(sdf_ValueTypeNameVector_t** _result) {
    *_result = new std::vector<PXR_NS::SdfValueTypeName>();
    return 0;
}

int sdf_ValueTypeNameVector_dtor(sdf_ValueTypeNameVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_VariableExpression_GetErrors(sdf_VariableExpression_t const* _this, std_StringVector_t const** _result) {
    *_result = &_this->GetErrors();
    return 0;
}

int sdf_VariableExpression_Evaluate(sdf_VariableExpression_t const* _this, vt_Dictionary_t const* variables, sdf_VariableExpressionResult_t* _result) {
    *_result = _this->Evaluate(*variables);
    return 0;
}

int sdf_VariableExpression_new(sdf_VariableExpression_t** _result) {
    *_result = new PXR_NS::SdfVariableExpression();
    return 0;
}

int sdf_VariableExpression_dtor(sdf_VariableExpression_t* _this) {
    delete _this;
    return 0;
}

int sdf_VariableExpressionResult_dtor(sdf_VariableExpressionResult_t* _this) {
    delete _this;
    return 0;
}

int sdf_VariantSelectionProxy_dtor(sdf_VariantSelectionProxy_t* _this) {
    delete _this;
    return 0;
}

int sdf_VariantSetSpec_GetName(sdf_VariantSetSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetName();
    return 0;
}

int sdf_VariantSetSpec_GetNameToken(sdf_VariantSetSpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNameToken();
    return 0;
}

int sdf_VariantSetSpec_GetOwner(sdf_VariantSetSpec_t const* _this, sdf_SpecHandle_t* _result) {
    *_result = _this->GetOwner();
    return 0;
}

int sdf_VariantSetSpec_GetVariants(sdf_VariantSetSpec_t const* _this, sdf_VariantView_t* _result) {
    *_result = _this->GetVariants();
    return 0;
}

int sdf_VariantSetSpec_GetVariantList(sdf_VariantSetSpec_t const* _this, sdf_VariantSpecHandleVector_t* _result) {
    *_result = _this->GetVariantList();
    return 0;
}

int sdf_VariantSetSpec_RemoveVariant(sdf_VariantSetSpec_t* _this, sdf_VariantSpecHandle_t const* variant) {
    _this->RemoveVariant(*variant);
    return 0;
}

int sdf_VariantSetSpec_GetSchema(sdf_VariantSetSpec_t const* _this, sdf_SchemaBase_t const** _result) {
    *_result = &_this->GetSchema();
    return 0;
}

int sdf_VariantSetSpec_GetSpecType(sdf_VariantSetSpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSpecType());
    return 0;
}

int sdf_VariantSetSpec_IsDormant(sdf_VariantSetSpec_t const* _this, bool* _result) {
    *_result = _this->IsDormant();
    return 0;
}

int sdf_VariantSetSpec_GetLayer(sdf_VariantSetSpec_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetLayer();
    return 0;
}

int sdf_VariantSetSpec_GetPath(sdf_VariantSetSpec_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int sdf_VariantSetSpec_PermissionToEdit(sdf_VariantSetSpec_t const* _this, bool* _result) {
    *_result = _this->PermissionToEdit();
    return 0;
}

int sdf_VariantSetSpec_ListInfoKeys(sdf_VariantSetSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListInfoKeys();
    return 0;
}

int sdf_VariantSetSpec_GetMetaDataInfoKeys(sdf_VariantSetSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->GetMetaDataInfoKeys();
    return 0;
}

int sdf_VariantSetSpec_GetMetaDataDisplayGroup(sdf_VariantSetSpec_t const* _this, tf_Token_t const* key, tf_Token_t* _result) {
    *_result = _this->GetMetaDataDisplayGroup(*key);
    return 0;
}

int sdf_VariantSetSpec_GetInfo(sdf_VariantSetSpec_t const* _this, tf_Token_t const* key, vt_Value_t* _result) {
    *_result = _this->GetInfo(*key);
    return 0;
}

int sdf_VariantSetSpec_SetInfo(sdf_VariantSetSpec_t* _this, tf_Token_t const* key, vt_Value_t const* value) {
    _this->SetInfo(*key, *value);
    return 0;
}

int sdf_VariantSetSpec_SetInfoDictionaryValue(sdf_VariantSetSpec_t* _this, tf_Token_t const* dictionaryKey, tf_Token_t const* entryKey, vt_Value_t const* value) {
    _this->SetInfoDictionaryValue(*dictionaryKey, *entryKey, *value);
    return 0;
}

int sdf_VariantSetSpec_HasInfo(sdf_VariantSetSpec_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasInfo(*key);
    return 0;
}

int sdf_VariantSetSpec_ClearInfo(sdf_VariantSetSpec_t* _this, tf_Token_t const* key) {
    _this->ClearInfo(*key);
    return 0;
}

int sdf_VariantSetSpec_GetTypeForInfo(sdf_VariantSetSpec_t const* _this, tf_Token_t const* key, tf_Type_t* _result) {
    *_result = _this->GetTypeForInfo(*key);
    return 0;
}

int sdf_VariantSetSpec_GetFallbackForInfo(sdf_VariantSetSpec_t const* _this, tf_Token_t const* key, vt_Value_t const** _result) {
    *_result = &_this->GetFallbackForInfo(*key);
    return 0;
}

int sdf_VariantSetSpec_IsInert(sdf_VariantSetSpec_t const* _this, bool ignoreChildren, bool* _result) {
    *_result = _this->IsInert(ignoreChildren);
    return 0;
}

int sdf_VariantSetSpec_ListFields(sdf_VariantSetSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListFields();
    return 0;
}

int sdf_VariantSetSpec_HasField(sdf_VariantSetSpec_t const* _this, tf_Token_t const* name, bool* _result) {
    *_result = _this->HasField(*name);
    return 0;
}

int sdf_VariantSetSpec_GetField(sdf_VariantSetSpec_t const* _this, tf_Token_t const* name, vt_Value_t* _result) {
    *_result = _this->GetField(*name);
    return 0;
}

int sdf_VariantSetSpec_SetField(sdf_VariantSetSpec_t* _this, tf_Token_t const* name, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetField(*name, *value);
    return 0;
}

int sdf_VariantSetSpec_op_eq(sdf_VariantSetSpec_t const* _this, sdf_Spec_t const* rhs, bool* _result) {
    *_result = _this->operator==(*rhs);
    return 0;
}

int sdf_VariantSetSpec_dtor(sdf_VariantSetSpec_t* _this) {
    delete _this;
    return 0;
}

int sdf_VariantSetsProxy_dtor(sdf_VariantSetsProxy_t* _this) {
    delete _this;
    return 0;
}

int sdf_VariantSpec_GetName(sdf_VariantSpec_t const* _this, std_String_t* _result) {
    *_result = _this->GetName();
    return 0;
}

int sdf_VariantSpec_GetNameToken(sdf_VariantSpec_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNameToken();
    return 0;
}

int sdf_VariantSpec_GetOwner(sdf_VariantSpec_t const* _this, sdf_VariantSetSpecHandle_t* _result) {
    *_result = _this->GetOwner();
    return 0;
}

int sdf_VariantSpec_GetPrimSpec(sdf_VariantSpec_t const* _this, sdf_PrimSpecHandle_t* _result) {
    *_result = _this->GetPrimSpec();
    return 0;
}

int sdf_VariantSpec_GetVariantSets(sdf_VariantSpec_t const* _this, sdf_VariantSetsProxy_t* _result) {
    *_result = _this->GetVariantSets();
    return 0;
}

int sdf_VariantSpec_GetVariantNames(sdf_VariantSpec_t const* _this, std_String_t const* name, std_StringVector_t* _result) {
    *_result = _this->GetVariantNames(*name);
    return 0;
}

int sdf_VariantSpec_GetSchema(sdf_VariantSpec_t const* _this, sdf_SchemaBase_t const** _result) {
    *_result = &_this->GetSchema();
    return 0;
}

int sdf_VariantSpec_GetSpecType(sdf_VariantSpec_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSpecType());
    return 0;
}

int sdf_VariantSpec_IsDormant(sdf_VariantSpec_t const* _this, bool* _result) {
    *_result = _this->IsDormant();
    return 0;
}

int sdf_VariantSpec_GetLayer(sdf_VariantSpec_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetLayer();
    return 0;
}

int sdf_VariantSpec_GetPath(sdf_VariantSpec_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int sdf_VariantSpec_PermissionToEdit(sdf_VariantSpec_t const* _this, bool* _result) {
    *_result = _this->PermissionToEdit();
    return 0;
}

int sdf_VariantSpec_ListInfoKeys(sdf_VariantSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListInfoKeys();
    return 0;
}

int sdf_VariantSpec_GetMetaDataInfoKeys(sdf_VariantSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->GetMetaDataInfoKeys();
    return 0;
}

int sdf_VariantSpec_GetMetaDataDisplayGroup(sdf_VariantSpec_t const* _this, tf_Token_t const* key, tf_Token_t* _result) {
    *_result = _this->GetMetaDataDisplayGroup(*key);
    return 0;
}

int sdf_VariantSpec_GetInfo(sdf_VariantSpec_t const* _this, tf_Token_t const* key, vt_Value_t* _result) {
    *_result = _this->GetInfo(*key);
    return 0;
}

int sdf_VariantSpec_SetInfo(sdf_VariantSpec_t* _this, tf_Token_t const* key, vt_Value_t const* value) {
    _this->SetInfo(*key, *value);
    return 0;
}

int sdf_VariantSpec_SetInfoDictionaryValue(sdf_VariantSpec_t* _this, tf_Token_t const* dictionaryKey, tf_Token_t const* entryKey, vt_Value_t const* value) {
    _this->SetInfoDictionaryValue(*dictionaryKey, *entryKey, *value);
    return 0;
}

int sdf_VariantSpec_HasInfo(sdf_VariantSpec_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasInfo(*key);
    return 0;
}

int sdf_VariantSpec_ClearInfo(sdf_VariantSpec_t* _this, tf_Token_t const* key) {
    _this->ClearInfo(*key);
    return 0;
}

int sdf_VariantSpec_GetTypeForInfo(sdf_VariantSpec_t const* _this, tf_Token_t const* key, tf_Type_t* _result) {
    *_result = _this->GetTypeForInfo(*key);
    return 0;
}

int sdf_VariantSpec_GetFallbackForInfo(sdf_VariantSpec_t const* _this, tf_Token_t const* key, vt_Value_t const** _result) {
    *_result = &_this->GetFallbackForInfo(*key);
    return 0;
}

int sdf_VariantSpec_IsInert(sdf_VariantSpec_t const* _this, bool ignoreChildren, bool* _result) {
    *_result = _this->IsInert(ignoreChildren);
    return 0;
}

int sdf_VariantSpec_ListFields(sdf_VariantSpec_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListFields();
    return 0;
}

int sdf_VariantSpec_HasField(sdf_VariantSpec_t const* _this, tf_Token_t const* name, bool* _result) {
    *_result = _this->HasField(*name);
    return 0;
}

int sdf_VariantSpec_GetField(sdf_VariantSpec_t const* _this, tf_Token_t const* name, vt_Value_t* _result) {
    *_result = _this->GetField(*name);
    return 0;
}

int sdf_VariantSpec_SetField(sdf_VariantSpec_t* _this, tf_Token_t const* name, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetField(*name, *value);
    return 0;
}

int sdf_VariantSpec_op_eq(sdf_VariantSpec_t const* _this, sdf_Spec_t const* rhs, bool* _result) {
    *_result = _this->operator==(*rhs);
    return 0;
}

int sdf_VariantSpec_dtor(sdf_VariantSpec_t* _this) {
    delete _this;
    return 0;
}

int sdf_VariantSetSpecHandle_dtor(sdf_VariantSetSpecHandle_t* _this) {
    delete _this;
    return 0;
}

int sdf_VariantSpecHandle_dtor(sdf_VariantSpecHandle_t* _this) {
    delete _this;
    return 0;
}

int sdf_VariantSpecHandleVector_data(sdf_VariantSpecHandleVector_t* _this, sdf_VariantSpecHandle_t** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_VariantSpecHandleVector_data_const(sdf_VariantSpecHandleVector_t const* _this, sdf_VariantSpecHandle_t const** _result) {
    *_result = _this->data();
    return 0;
}

int sdf_VariantSpecHandleVector_empty(sdf_VariantSpecHandleVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int sdf_VariantSpecHandleVector_size(sdf_VariantSpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int sdf_VariantSpecHandleVector_max_size(sdf_VariantSpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int sdf_VariantSpecHandleVector_reserve(sdf_VariantSpecHandleVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int sdf_VariantSpecHandleVector_capacity(sdf_VariantSpecHandleVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int sdf_VariantSpecHandleVector_clear(sdf_VariantSpecHandleVector_t* _this) {
    _this->clear();
    return 0;
}

int sdf_VariantSpecHandleVector_push_back(sdf_VariantSpecHandleVector_t* _this, sdf_VariantSpecHandle_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int sdf_VariantSpecHandleVector_pop_back(sdf_VariantSpecHandleVector_t* _this) {
    _this->pop_back();
    return 0;
}

int sdf_VariantSpecHandleVector_resize(sdf_VariantSpecHandleVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int sdf_VariantSpecHandleVector_resize_with(sdf_VariantSpecHandleVector_t* _this, size_t const _Newsize, sdf_VariantSpecHandle_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int sdf_VariantSpecHandleVector_op_index(sdf_VariantSpecHandleVector_t const* _this, size_t const _Pos, sdf_VariantSpecHandle_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int sdf_VariantSpecHandleVector_default(sdf_VariantSpecHandleVector_t** _result) {
    *_result = new PXR_NS::SdfVariantSpecHandleVector();
    return 0;
}

int sdf_VariantSpecHandleVector_dtor(sdf_VariantSpecHandleVector_t* _this) {
    delete _this;
    return 0;
}

int sdf_VariantView_dtor(sdf_VariantView_t* _this) {
    delete _this;
    return 0;
}

int sdf_AssetPath_GetAssetPath(sdf_AssetPath_t const* path, char const** _result) {
    *_result = AssetPath_GetAssetPath(*path);
    return 0;
}

int sdf_AssetPath_GetResolvedPath(sdf_AssetPath_t const* path, char const** _result) {
    *_result = AssetPath_GetResolvedPath(*path);
    return 0;
}

int sdf_NamespaceEdit_get_current_path(sdf_NamespaceEdit_t const* edit, sdf_Path_t const** _result) {
    *_result = &NamespaceEdit_get_current_path(*edit);
    return 0;
}

int sdf_NamespaceEdit_get_new_path(sdf_NamespaceEdit_t const* edit, sdf_Path_t const** _result) {
    *_result = &NamespaceEdit_get_new_path(*edit);
    return 0;
}

int sdf_NamespaceEdit_get_index(sdf_NamespaceEdit_t const* edit, int* _result) {
    *_result = NamespaceEdit_get_index(*edit);
    return 0;
}

int sdf_NamespaceEditDetail_get_edit(sdf_NamespaceEditDetail_t const* detail, sdf_NamespaceEdit_t const** _result) {
    *_result = &NamespaceEditDetail_get_edit(*detail);
    return 0;
}

int sdf_NamespaceEditDetail_get_result(sdf_NamespaceEditDetail_t const* detail, int* _result) {
    *_result = static_cast<int>(NamespaceEditDetail_get_result(*detail));
    return 0;
}

int sdf_NamespaceEditDetail_get_reason(sdf_NamespaceEditDetail_t const* detail, char const** _result) {
    *_result = NamespaceEditDetail_get_reason(*detail);
    return 0;
}

int sdf_PathExpression_from_string(char const* expr, char const* parseContext, sdf_PathExpression_t** _result) {
    *_result = PathExpression_from_string(expr, parseContext);
    return 0;
}

int sdf_Payload_from_asset_path(char const* assetPath, sdf_Payload_t** _result) {
    *_result = Payload_from_asset_path(assetPath);
    return 0;
}

int sdf_Payload_from_asset_path_and_prim_path(char const* assetPath, sdf_Path_t const* primPath, sdf_Payload_t** _result) {
    *_result = Payload_from_asset_path_and_prim_path(assetPath, *primPath);
    return 0;
}

int sdf_Payload_from_asset_path_and_prim_path_and_layer_offset(char const* assetPath, sdf_Path_t const* primPath, sdf_LayerOffset_t const* layerOffset, sdf_Payload_t** _result) {
    *_result = Payload_from_asset_path_and_prim_path_and_layer_offset(assetPath, *primPath, *layerOffset);
    return 0;
}

int sdf_Reference_from_asset_path(char const* assetPath, sdf_Reference_t** _result) {
    *_result = Reference_from_asset_path(assetPath);
    return 0;
}

int sdf_Reference_from_asset_path_and_prim_path(char const* assetPath, sdf_Path_t const* primPath, sdf_Reference_t** _result) {
    *_result = Reference_from_asset_path_and_prim_path(assetPath, *primPath);
    return 0;
}

int sdf_Reference_from_asset_path_and_prim_path_and_layer_offset(char const* assetPath, sdf_Path_t const* primPath, sdf_LayerOffset_t const* layerOffset, sdf_Reference_t** _result) {
    *_result = Reference_from_asset_path_and_prim_path_and_layer_offset(assetPath, *primPath, *layerOffset);
    return 0;
}

int sdf_Reference_from_asset_path_and_prim_path_and_layer_offset_and_custom_data(char const* assetPath, sdf_Path_t const* primPath, sdf_LayerOffset_t const* layerOffset, vt_Dictionary_t const* customData, sdf_Reference_t** _result) {
    *_result = Reference_from_asset_path_and_prim_path_and_layer_offset_and_custom_data(assetPath, *primPath, *layerOffset, *customData);
    return 0;
}

int sdf_ValueTypeName_GetCPPTypeName(sdf_ValueTypeName_t const* vtn, char const** _result) {
    *_result = ValueTypeName_GetCPPTypeName(*vtn);
    return 0;
}

int std_String_c_str(std_String_t const* _this, char const** _result) {
    *_result = _this->c_str();
    return 0;
}

int std_String_from_char_ptr(char const* param00, std_String_t** _result) {
    *_result = new std::string(param00);
    return 0;
}

int std_String_dtor(std_String_t* _this) {
    delete _this;
    return 0;
}

int std_StringVector_data(std_StringVector_t* _this, std_String_t** _result) {
    *_result = _this->data();
    return 0;
}

int std_StringVector_data_const(std_StringVector_t const* _this, std_String_t const** _result) {
    *_result = _this->data();
    return 0;
}

int std_StringVector_empty(std_StringVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int std_StringVector_size(std_StringVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int std_StringVector_max_size(std_StringVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int std_StringVector_reserve(std_StringVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int std_StringVector_capacity(std_StringVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int std_StringVector_clear(std_StringVector_t* _this) {
    _this->clear();
    return 0;
}

int std_StringVector_push_back(std_StringVector_t* _this, std_String_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int std_StringVector_pop_back(std_StringVector_t* _this) {
    _this->pop_back();
    return 0;
}

int std_StringVector_resize(std_StringVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int std_StringVector_resize_with(std_StringVector_t* _this, size_t const _Newsize, std_String_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int std_StringVector_op_index(std_StringVector_t const* _this, size_t const _Pos, std_String_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int std_StringVector_default(std_StringVector_t** _result) {
    *_result = new std::vector<std::string>();
    return 0;
}

int std_StringVector_dtor(std_StringVector_t* _this) {
    delete _this;
    return 0;
}

int std_StringSet_dtor(std_StringSet_t* _this) {
    delete _this;
    return 0;
}

int std_DoubleVector_data(std_DoubleVector_t* _this, double** _result) {
    *_result = _this->data();
    return 0;
}

int std_DoubleVector_data_const(std_DoubleVector_t const* _this, double const** _result) {
    *_result = _this->data();
    return 0;
}

int std_DoubleVector_empty(std_DoubleVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int std_DoubleVector_size(std_DoubleVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int std_DoubleVector_max_size(std_DoubleVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int std_DoubleVector_reserve(std_DoubleVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int std_DoubleVector_capacity(std_DoubleVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int std_DoubleVector_clear(std_DoubleVector_t* _this) {
    _this->clear();
    return 0;
}

int std_DoubleVector_push_back(std_DoubleVector_t* _this, double const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int std_DoubleVector_pop_back(std_DoubleVector_t* _this) {
    _this->pop_back();
    return 0;
}

int std_DoubleVector_resize(std_DoubleVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int std_DoubleVector_resize_with(std_DoubleVector_t* _this, size_t const _Newsize, double const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int std_DoubleVector_op_index(std_DoubleVector_t const* _this, size_t const _Pos, double* _result) {
    *_result = _this->operator[](_Pos);
    return 0;
}

int std_DoubleVector_default(std_DoubleVector_t** _result) {
    *_result = new std::vector<double>();
    return 0;
}

int std_DoubleVector_dtor(std_DoubleVector_t* _this) {
    delete _this;
    return 0;
}

int std_DoubleSet_dtor(std_DoubleSet_t* _this) {
    delete _this;
    return 0;
}

int tf_Enum_GetValueAsInt(tf_Enum_t const* _this, int* _result) {
    *_result = _this->GetValueAsInt();
    return 0;
}

int tf_Enum_GetName(tf_Enum_t* val, std_String_t* _result) {
    *_result = PXR_NS::TfEnum::GetName(*val);
    return 0;
}

int tf_Enum_GetDisplayName(tf_Enum_t* val, std_String_t* _result) {
    *_result = PXR_NS::TfEnum::GetDisplayName(*val);
    return 0;
}

int tf_Enum_GetFullName(tf_Enum_t* val, std_String_t* _result) {
    *_result = PXR_NS::TfEnum::GetFullName(*val);
    return 0;
}

int tf_Enum_new(tf_Enum_t** _result) {
    *_result = new PXR_NS::TfEnum();
    return 0;
}

int tf_Enum_dtor(tf_Enum_t* _this) {
    delete _this;
    return 0;
}

int tf_Token_op_assign(tf_Token_t* _this, tf_Token_t const* rhs, tf_Token_t** _result) {
    *_result = &_this->operator=(*rhs);
    return 0;
}

int tf_Token_GetText(tf_Token_t const* _this, char const** _result) {
    *_result = _this->GetText();
    return 0;
}

int tf_Token_new(tf_Token_t** _result) {
    *_result = new PXR_NS::TfToken();
    return 0;
}

int tf_Token_dtor(tf_Token_t* _this) {
    delete _this;
    return 0;
}

int tf_Type_dtor(tf_Type_t* _this) {
    delete _this;
    return 0;
}

int tf_TokenVector_data(tf_TokenVector_t* _this, tf_Token_t** _result) {
    *_result = _this->data();
    return 0;
}

int tf_TokenVector_data_const(tf_TokenVector_t const* _this, tf_Token_t const** _result) {
    *_result = _this->data();
    return 0;
}

int tf_TokenVector_empty(tf_TokenVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int tf_TokenVector_size(tf_TokenVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int tf_TokenVector_max_size(tf_TokenVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int tf_TokenVector_reserve(tf_TokenVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int tf_TokenVector_capacity(tf_TokenVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int tf_TokenVector_clear(tf_TokenVector_t* _this) {
    _this->clear();
    return 0;
}

int tf_TokenVector_push_back(tf_TokenVector_t* _this, tf_Token_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int tf_TokenVector_pop_back(tf_TokenVector_t* _this) {
    _this->pop_back();
    return 0;
}

int tf_TokenVector_resize(tf_TokenVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int tf_TokenVector_resize_with(tf_TokenVector_t* _this, size_t const _Newsize, tf_Token_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int tf_TokenVector_op_index(tf_TokenVector_t const* _this, size_t const _Pos, tf_Token_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int tf_TokenVector_default(tf_TokenVector_t** _result) {
    *_result = new PXR_NS::TfTokenVector();
    return 0;
}

int tf_TokenVector_dtor(tf_TokenVector_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_BasisCurves_GetTypeAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetTypeAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateTypeAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateTypeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetBasisAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetBasisAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateBasisAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateBasisAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetWrapAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetWrapAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateWrapAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateWrapAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetCurveVertexCountsAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetCurveVertexCountsAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateCurveVertexCountsAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateCurveVertexCountsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetWidthsAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetWidthsAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateWidthsAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateWidthsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetWidthsInterpolation(usdGeom_BasisCurves_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetWidthsInterpolation();
    return 0;
}

int usdGeom_BasisCurves_SetWidthsInterpolation(usdGeom_BasisCurves_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetWidthsInterpolation(*interpolation);
    return 0;
}

int usdGeom_BasisCurves_GetCurveCount(usdGeom_BasisCurves_t const* _this, usd_TimeCode_t timeCode, size_t* _result) {
    *_result = _this->GetCurveCount(timeCode);
    return 0;
}

int usdGeom_BasisCurves_GetPointsAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPointsAttr();
    return 0;
}

int usdGeom_BasisCurves_CreatePointsAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePointsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetVelocitiesAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVelocitiesAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateVelocitiesAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVelocitiesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetAccelerationsAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAccelerationsAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateAccelerationsAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAccelerationsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetNormalsAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetNormalsAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateNormalsAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateNormalsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetNormalsInterpolation(usdGeom_BasisCurves_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNormalsInterpolation();
    return 0;
}

int usdGeom_BasisCurves_SetNormalsInterpolation(usdGeom_BasisCurves_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetNormalsInterpolation(*interpolation);
    return 0;
}

int usdGeom_BasisCurves_ComputePointsAtTime(usdGeom_BasisCurves_t const* _this, gf_Vec3fArray_t* points, usd_TimeCode_t const time, usd_TimeCode_t const baseTime, bool* _result) {
    *_result = _this->ComputePointsAtTime(points, time, baseTime);
    return 0;
}

int usdGeom_BasisCurves_GetDisplayColorAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateDisplayColorAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetDisplayOpacityAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateDisplayOpacityAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetDoubleSidedAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateDoubleSidedAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetOrientationAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateOrientationAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetDisplayColorPrimvar(usdGeom_BasisCurves_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_BasisCurves_CreateDisplayColorPrimvar(usdGeom_BasisCurves_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_BasisCurves_GetExtentAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateExtentAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetXformOpOrderAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_BasisCurves_AddXformOp(usdGeom_BasisCurves_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddTranslateOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddScaleOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddRotateXOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddRotateYOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddRotateZOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddRotateXYZOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddRotateXZYOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddRotateYXZOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddRotateYZXOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddRotateZXYOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddRotateZYXOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddOrientOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_AddTransformOp(usdGeom_BasisCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_BasisCurves_SetResetXformStack(usdGeom_BasisCurves_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_BasisCurves_SetXformOpOrder(usdGeom_BasisCurves_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_BasisCurves_GetOrderedXformOps(usdGeom_BasisCurves_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_BasisCurves_ClearXformOpOrder(usdGeom_BasisCurves_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_BasisCurves_MakeMatrixXform(usdGeom_BasisCurves_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_BasisCurves_TransformMightBeTimeVarying(usdGeom_BasisCurves_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_BasisCurves_TransformMightBeTimeVarying_using(usdGeom_BasisCurves_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_BasisCurves_GetTimeSamples(usdGeom_BasisCurves_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_BasisCurves_GetTimeSamplesInInterval(usdGeom_BasisCurves_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_BasisCurves_GetLocalTransformation(usdGeom_BasisCurves_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_BasisCurves_GetLocalTransformation_with_ops(usdGeom_BasisCurves_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_BasisCurves_GetVisibilityAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_BasisCurves_CreateVisibilityAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetPurposeAttr(usdGeom_BasisCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_BasisCurves_CreatePurposeAttr(usdGeom_BasisCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_BasisCurves_GetProxyPrimRel(usdGeom_BasisCurves_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_BasisCurves_CreateProxyPrimRel(usdGeom_BasisCurves_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_BasisCurves_MakeVisible(usdGeom_BasisCurves_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_BasisCurves_MakeInvisible(usdGeom_BasisCurves_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_BasisCurves_ComputeVisibility(usdGeom_BasisCurves_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_BasisCurves_GetPurposeVisibilityAttr(usdGeom_BasisCurves_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_BasisCurves_ComputePurposeInfo(usdGeom_BasisCurves_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_BasisCurves_ComputePurposeInfo_given_parent(usdGeom_BasisCurves_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_BasisCurves_ComputePurpose(usdGeom_BasisCurves_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_BasisCurves_ComputeProxyPrim(usdGeom_BasisCurves_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_BasisCurves_SetProxyPrim(usdGeom_BasisCurves_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_BasisCurves_ComputeWorldBound(usdGeom_BasisCurves_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_BasisCurves_ComputeLocalBound(usdGeom_BasisCurves_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_BasisCurves_ComputeUntransformedBound(usdGeom_BasisCurves_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_BasisCurves_ComputeLocalToWorldTransform(usdGeom_BasisCurves_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_BasisCurves_ComputeParentToWorldTransform(usdGeom_BasisCurves_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_BasisCurves_IsConcrete(usdGeom_BasisCurves_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_BasisCurves_IsTyped(usdGeom_BasisCurves_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_BasisCurves_IsAPISchema(usdGeom_BasisCurves_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_BasisCurves_IsAppliedAPISchema(usdGeom_BasisCurves_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_BasisCurves_IsMultipleApplyAPISchema(usdGeom_BasisCurves_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_BasisCurves_GetSchemaKind(usdGeom_BasisCurves_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_BasisCurves_GetPrim(usdGeom_BasisCurves_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_BasisCurves_GetPath(usdGeom_BasisCurves_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_BasisCurves_GetSchemaClassPrimDefinition(usdGeom_BasisCurves_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_BasisCurves_new(usd_Prim_t const* param00, usdGeom_BasisCurves_t** _result) {
    *_result = new PXR_NS::UsdGeomBasisCurves(*param00);
    return 0;
}

int usdGeom_BasisCurves_dtor(usdGeom_BasisCurves_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_BBoxCache_ComputeWorldBound(usdGeom_BBoxCache_t* _this, usd_Prim_t const* prim, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*prim);
    return 0;
}

int usdGeom_BBoxCache_ComputeRelativeBound(usdGeom_BBoxCache_t* _this, usd_Prim_t const* prim, usd_Prim_t const* relativeToAncestorPrim, gf_BBox3d_t* _result) {
    *_result = _this->ComputeRelativeBound(*prim, *relativeToAncestorPrim);
    return 0;
}

int usdGeom_BBoxCache_ComputeLocalBound(usdGeom_BBoxCache_t* _this, usd_Prim_t const* prim, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*prim);
    return 0;
}

int usdGeom_BBoxCache_ComputeUntransformedBound(usdGeom_BBoxCache_t* _this, usd_Prim_t const* prim, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*prim);
    return 0;
}

int usdGeom_BBoxCache_ComputePointInstanceWorldBound(usdGeom_BBoxCache_t* _this, usdGeom_PointInstancer_t const* instancer, int64_t instanceId, gf_BBox3d_t* _result) {
    *_result = _this->ComputePointInstanceWorldBound(*instancer, instanceId);
    return 0;
}

int usdGeom_BBoxCache_ComputePointInstanceWorldBounds(usdGeom_BBoxCache_t* _this, usdGeom_PointInstancer_t const* instancer, int64_t const* instanceIdBegin, size_t numIds, gf_BBox3d_t* result, bool* _result) {
    *_result = _this->ComputePointInstanceWorldBounds(*instancer, instanceIdBegin, numIds, result);
    return 0;
}

int usdGeom_BBoxCache_ComputePointInstanceRelativeBounds(usdGeom_BBoxCache_t* _this, usdGeom_PointInstancer_t const* instancer, int64_t const* instanceIdBegin, size_t numIds, usd_Prim_t const* relativeToAncestorPrim, gf_BBox3d_t* result, bool* _result) {
    *_result = _this->ComputePointInstanceRelativeBounds(*instancer, instanceIdBegin, numIds, *relativeToAncestorPrim, result);
    return 0;
}

int usdGeom_BBoxCache_ComputePointInstanceRelativeBound(usdGeom_BBoxCache_t* _this, usdGeom_PointInstancer_t const* instancer, int64_t instanceId, usd_Prim_t const* relativeToAncestorPrim, gf_BBox3d_t* _result) {
    *_result = _this->ComputePointInstanceRelativeBound(*instancer, instanceId, *relativeToAncestorPrim);
    return 0;
}

int usdGeom_BBoxCache_ComputePointInstanceLocalBounds(usdGeom_BBoxCache_t* _this, usdGeom_PointInstancer_t const* instancer, int64_t const* instanceIdBegin, size_t numIds, gf_BBox3d_t* result, bool* _result) {
    *_result = _this->ComputePointInstanceLocalBounds(*instancer, instanceIdBegin, numIds, result);
    return 0;
}

int usdGeom_BBoxCache_ComputePointInstanceLocalBound(usdGeom_BBoxCache_t* _this, usdGeom_PointInstancer_t const* instancer, int64_t instanceId, gf_BBox3d_t* _result) {
    *_result = _this->ComputePointInstanceLocalBound(*instancer, instanceId);
    return 0;
}

int usdGeom_BBoxCache_ComputePointInstanceUntransformedBounds(usdGeom_BBoxCache_t* _this, usdGeom_PointInstancer_t const* instancer, int64_t const* instanceIdBegin, size_t numIds, gf_BBox3d_t* result, bool* _result) {
    *_result = _this->ComputePointInstanceUntransformedBounds(*instancer, instanceIdBegin, numIds, result);
    return 0;
}

int usdGeom_BBoxCache_ComputePointInstanceUntransformedBound(usdGeom_BBoxCache_t* _this, usdGeom_PointInstancer_t const* instancer, int64_t instanceId, gf_BBox3d_t* _result) {
    *_result = _this->ComputePointInstanceUntransformedBound(*instancer, instanceId);
    return 0;
}

int usdGeom_BBoxCache_Clear(usdGeom_BBoxCache_t* _this) {
    _this->Clear();
    return 0;
}

int usdGeom_BBoxCache_SetIncludedPurposes(usdGeom_BBoxCache_t* _this, tf_TokenVector_t const* includedPurposes) {
    _this->SetIncludedPurposes(*includedPurposes);
    return 0;
}

int usdGeom_BBoxCache_GetIncludedPurposes(usdGeom_BBoxCache_t* _this, tf_TokenVector_t const** _result) {
    *_result = &_this->GetIncludedPurposes();
    return 0;
}

int usdGeom_BBoxCache_GetUseExtentsHint(usdGeom_BBoxCache_t const* _this, bool* _result) {
    *_result = _this->GetUseExtentsHint();
    return 0;
}

int usdGeom_BBoxCache_GetIgnoreVisibility(usdGeom_BBoxCache_t const* _this, bool* _result) {
    *_result = _this->GetIgnoreVisibility();
    return 0;
}

int usdGeom_BBoxCache_SetTime(usdGeom_BBoxCache_t* _this, usd_TimeCode_t time) {
    _this->SetTime(time);
    return 0;
}

int usdGeom_BBoxCache_GetTime(usdGeom_BBoxCache_t const* _this, usd_TimeCode_t* _result) {
    *_result = _this->GetTime();
    return 0;
}

int usdGeom_BBoxCache_SetBaseTime(usdGeom_BBoxCache_t* _this, usd_TimeCode_t baseTime) {
    _this->SetBaseTime(baseTime);
    return 0;
}

int usdGeom_BBoxCache_GetBaseTime(usdGeom_BBoxCache_t const* _this, usd_TimeCode_t* _result) {
    *_result = _this->GetBaseTime();
    return 0;
}

int usdGeom_BBoxCache_ClearBaseTime(usdGeom_BBoxCache_t* _this) {
    _this->ClearBaseTime();
    return 0;
}

int usdGeom_BBoxCache_HasBaseTime(usdGeom_BBoxCache_t const* _this, bool* _result) {
    *_result = _this->HasBaseTime();
    return 0;
}

int usdGeom_BBoxCache_ctor(usd_TimeCode_t time, tf_TokenVector_t* includedPurposes, bool useExtentsHint, bool ignoreVisibility, usdGeom_BBoxCache_t** _result) {
    *_result = new PXR_NS::UsdGeomBBoxCache(time, *includedPurposes, useExtentsHint, ignoreVisibility);
    return 0;
}

int usdGeom_BBoxCache_dtor(usdGeom_BBoxCache_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_PathMatrixHashMap_dtor(usdGeom_PathMatrixHashMap_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Boundable_GetExtentAttr(usdGeom_Boundable_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_Boundable_CreateExtentAttr(usdGeom_Boundable_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Boundable_GetXformOpOrderAttr(usdGeom_Boundable_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Boundable_AddXformOp(usdGeom_Boundable_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddTranslateOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddScaleOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddRotateXOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddRotateYOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddRotateZOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddRotateXYZOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddRotateXZYOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddRotateYXZOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddRotateYZXOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddRotateZXYOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddRotateZYXOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddOrientOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_AddTransformOp(usdGeom_Boundable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Boundable_SetResetXformStack(usdGeom_Boundable_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Boundable_SetXformOpOrder(usdGeom_Boundable_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Boundable_GetOrderedXformOps(usdGeom_Boundable_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Boundable_ClearXformOpOrder(usdGeom_Boundable_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Boundable_MakeMatrixXform(usdGeom_Boundable_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Boundable_TransformMightBeTimeVarying(usdGeom_Boundable_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Boundable_TransformMightBeTimeVarying_using(usdGeom_Boundable_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Boundable_GetTimeSamples(usdGeom_Boundable_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Boundable_GetTimeSamplesInInterval(usdGeom_Boundable_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Boundable_GetLocalTransformation(usdGeom_Boundable_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Boundable_GetLocalTransformation_with_ops(usdGeom_Boundable_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Boundable_GetVisibilityAttr(usdGeom_Boundable_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Boundable_CreateVisibilityAttr(usdGeom_Boundable_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Boundable_GetPurposeAttr(usdGeom_Boundable_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Boundable_CreatePurposeAttr(usdGeom_Boundable_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Boundable_GetProxyPrimRel(usdGeom_Boundable_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Boundable_CreateProxyPrimRel(usdGeom_Boundable_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Boundable_MakeVisible(usdGeom_Boundable_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Boundable_MakeInvisible(usdGeom_Boundable_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Boundable_ComputeVisibility(usdGeom_Boundable_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Boundable_GetPurposeVisibilityAttr(usdGeom_Boundable_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Boundable_ComputePurposeInfo(usdGeom_Boundable_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Boundable_ComputePurposeInfo_given_parent(usdGeom_Boundable_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Boundable_ComputePurpose(usdGeom_Boundable_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Boundable_ComputeProxyPrim(usdGeom_Boundable_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Boundable_SetProxyPrim(usdGeom_Boundable_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Boundable_ComputeWorldBound(usdGeom_Boundable_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Boundable_ComputeLocalBound(usdGeom_Boundable_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Boundable_ComputeUntransformedBound(usdGeom_Boundable_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Boundable_ComputeLocalToWorldTransform(usdGeom_Boundable_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Boundable_ComputeParentToWorldTransform(usdGeom_Boundable_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Boundable_IsConcrete(usdGeom_Boundable_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Boundable_IsTyped(usdGeom_Boundable_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Boundable_IsAPISchema(usdGeom_Boundable_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Boundable_IsAppliedAPISchema(usdGeom_Boundable_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Boundable_IsMultipleApplyAPISchema(usdGeom_Boundable_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Boundable_GetSchemaKind(usdGeom_Boundable_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Boundable_GetPrim(usdGeom_Boundable_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Boundable_GetPath(usdGeom_Boundable_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Boundable_GetSchemaClassPrimDefinition(usdGeom_Boundable_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Boundable_new(usd_Prim_t const* param00, usdGeom_Boundable_t** _result) {
    *_result = new PXR_NS::UsdGeomBoundable(*param00);
    return 0;
}

int usdGeom_Boundable_dtor(usdGeom_Boundable_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Camera_GetProjectionAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetProjectionAttr();
    return 0;
}

int usdGeom_Camera_CreateProjectionAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateProjectionAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetHorizontalApertureAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetHorizontalApertureAttr();
    return 0;
}

int usdGeom_Camera_CreateHorizontalApertureAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateHorizontalApertureAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetVerticalApertureAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVerticalApertureAttr();
    return 0;
}

int usdGeom_Camera_CreateVerticalApertureAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVerticalApertureAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetHorizontalApertureOffsetAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetHorizontalApertureOffsetAttr();
    return 0;
}

int usdGeom_Camera_CreateHorizontalApertureOffsetAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateHorizontalApertureOffsetAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetVerticalApertureOffsetAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVerticalApertureOffsetAttr();
    return 0;
}

int usdGeom_Camera_CreateVerticalApertureOffsetAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVerticalApertureOffsetAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetFocalLengthAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetFocalLengthAttr();
    return 0;
}

int usdGeom_Camera_CreateFocalLengthAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateFocalLengthAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetClippingRangeAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetClippingRangeAttr();
    return 0;
}

int usdGeom_Camera_CreateClippingRangeAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateClippingRangeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetClippingPlanesAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetClippingPlanesAttr();
    return 0;
}

int usdGeom_Camera_CreateClippingPlanesAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateClippingPlanesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetFStopAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetFStopAttr();
    return 0;
}

int usdGeom_Camera_CreateFStopAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateFStopAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetFocusDistanceAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetFocusDistanceAttr();
    return 0;
}

int usdGeom_Camera_CreateFocusDistanceAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateFocusDistanceAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetStereoRoleAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetStereoRoleAttr();
    return 0;
}

int usdGeom_Camera_CreateStereoRoleAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateStereoRoleAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetShutterOpenAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetShutterOpenAttr();
    return 0;
}

int usdGeom_Camera_CreateShutterOpenAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateShutterOpenAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetShutterCloseAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetShutterCloseAttr();
    return 0;
}

int usdGeom_Camera_CreateShutterCloseAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateShutterCloseAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetExposureAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExposureAttr();
    return 0;
}

int usdGeom_Camera_CreateExposureAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExposureAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetCamera(usdGeom_Camera_t const* _this, usd_TimeCode_t const* time, gf_Camera_t* _result) {
    *_result = _this->GetCamera(*time);
    return 0;
}

int usdGeom_Camera_SetFromCamera(usdGeom_Camera_t* _this, gf_Camera_t const* camera, usd_TimeCode_t const* time) {
    _this->SetFromCamera(*camera, *time);
    return 0;
}

int usdGeom_Camera_GetXformOpOrderAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Camera_AddXformOp(usdGeom_Camera_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddTranslateOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddScaleOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddRotateXOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddRotateYOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddRotateZOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddRotateXYZOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddRotateXZYOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddRotateYXZOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddRotateYZXOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddRotateZXYOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddRotateZYXOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddOrientOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_AddTransformOp(usdGeom_Camera_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Camera_SetResetXformStack(usdGeom_Camera_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Camera_SetXformOpOrder(usdGeom_Camera_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Camera_GetOrderedXformOps(usdGeom_Camera_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Camera_ClearXformOpOrder(usdGeom_Camera_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Camera_MakeMatrixXform(usdGeom_Camera_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Camera_TransformMightBeTimeVarying(usdGeom_Camera_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Camera_TransformMightBeTimeVarying_using(usdGeom_Camera_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Camera_GetTimeSamples(usdGeom_Camera_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Camera_GetTimeSamplesInInterval(usdGeom_Camera_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Camera_GetLocalTransformation(usdGeom_Camera_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Camera_GetLocalTransformation_with_ops(usdGeom_Camera_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Camera_GetVisibilityAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Camera_CreateVisibilityAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetPurposeAttr(usdGeom_Camera_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Camera_CreatePurposeAttr(usdGeom_Camera_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Camera_GetProxyPrimRel(usdGeom_Camera_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Camera_CreateProxyPrimRel(usdGeom_Camera_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Camera_MakeVisible(usdGeom_Camera_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Camera_MakeInvisible(usdGeom_Camera_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Camera_ComputeVisibility(usdGeom_Camera_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Camera_GetPurposeVisibilityAttr(usdGeom_Camera_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Camera_ComputePurposeInfo(usdGeom_Camera_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Camera_ComputePurposeInfo_given_parent(usdGeom_Camera_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Camera_ComputePurpose(usdGeom_Camera_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Camera_ComputeProxyPrim(usdGeom_Camera_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Camera_SetProxyPrim(usdGeom_Camera_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Camera_ComputeWorldBound(usdGeom_Camera_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Camera_ComputeLocalBound(usdGeom_Camera_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Camera_ComputeUntransformedBound(usdGeom_Camera_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Camera_ComputeLocalToWorldTransform(usdGeom_Camera_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Camera_ComputeParentToWorldTransform(usdGeom_Camera_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Camera_IsConcrete(usdGeom_Camera_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Camera_IsTyped(usdGeom_Camera_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Camera_IsAPISchema(usdGeom_Camera_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Camera_IsAppliedAPISchema(usdGeom_Camera_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Camera_IsMultipleApplyAPISchema(usdGeom_Camera_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Camera_GetSchemaKind(usdGeom_Camera_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Camera_GetPrim(usdGeom_Camera_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Camera_GetPath(usdGeom_Camera_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Camera_GetSchemaClassPrimDefinition(usdGeom_Camera_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Camera_new(usd_Prim_t const* param00, usdGeom_Camera_t** _result) {
    *_result = new PXR_NS::UsdGeomCamera(*param00);
    return 0;
}

int usdGeom_Camera_dtor(usdGeom_Camera_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Capsule_GetHeightAttr(usdGeom_Capsule_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetHeightAttr();
    return 0;
}

int usdGeom_Capsule_CreateHeightAttr(usdGeom_Capsule_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateHeightAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Capsule_GetRadiusAttr(usdGeom_Capsule_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetRadiusAttr();
    return 0;
}

int usdGeom_Capsule_CreateRadiusAttr(usdGeom_Capsule_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateRadiusAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Capsule_GetAxisAttr(usdGeom_Capsule_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAxisAttr();
    return 0;
}

int usdGeom_Capsule_CreateAxisAttr(usdGeom_Capsule_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAxisAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Capsule_GetDisplayColorAttr(usdGeom_Capsule_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_Capsule_CreateDisplayColorAttr(usdGeom_Capsule_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Capsule_GetDisplayOpacityAttr(usdGeom_Capsule_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_Capsule_CreateDisplayOpacityAttr(usdGeom_Capsule_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Capsule_GetDoubleSidedAttr(usdGeom_Capsule_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_Capsule_CreateDoubleSidedAttr(usdGeom_Capsule_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Capsule_GetOrientationAttr(usdGeom_Capsule_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_Capsule_CreateOrientationAttr(usdGeom_Capsule_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Capsule_GetDisplayColorPrimvar(usdGeom_Capsule_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_Capsule_CreateDisplayColorPrimvar(usdGeom_Capsule_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_Capsule_GetExtentAttr(usdGeom_Capsule_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_Capsule_CreateExtentAttr(usdGeom_Capsule_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Capsule_GetXformOpOrderAttr(usdGeom_Capsule_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Capsule_AddXformOp(usdGeom_Capsule_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddTranslateOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddScaleOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddRotateXOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddRotateYOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddRotateZOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddRotateXYZOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddRotateXZYOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddRotateYXZOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddRotateYZXOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddRotateZXYOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddRotateZYXOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddOrientOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_AddTransformOp(usdGeom_Capsule_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Capsule_SetResetXformStack(usdGeom_Capsule_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Capsule_SetXformOpOrder(usdGeom_Capsule_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Capsule_GetOrderedXformOps(usdGeom_Capsule_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Capsule_ClearXformOpOrder(usdGeom_Capsule_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Capsule_MakeMatrixXform(usdGeom_Capsule_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Capsule_TransformMightBeTimeVarying(usdGeom_Capsule_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Capsule_TransformMightBeTimeVarying_using(usdGeom_Capsule_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Capsule_GetTimeSamples(usdGeom_Capsule_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Capsule_GetTimeSamplesInInterval(usdGeom_Capsule_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Capsule_GetLocalTransformation(usdGeom_Capsule_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Capsule_GetLocalTransformation_with_ops(usdGeom_Capsule_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Capsule_GetVisibilityAttr(usdGeom_Capsule_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Capsule_CreateVisibilityAttr(usdGeom_Capsule_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Capsule_GetPurposeAttr(usdGeom_Capsule_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Capsule_CreatePurposeAttr(usdGeom_Capsule_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Capsule_GetProxyPrimRel(usdGeom_Capsule_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Capsule_CreateProxyPrimRel(usdGeom_Capsule_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Capsule_MakeVisible(usdGeom_Capsule_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Capsule_MakeInvisible(usdGeom_Capsule_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Capsule_ComputeVisibility(usdGeom_Capsule_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Capsule_GetPurposeVisibilityAttr(usdGeom_Capsule_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Capsule_ComputePurposeInfo(usdGeom_Capsule_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Capsule_ComputePurposeInfo_given_parent(usdGeom_Capsule_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Capsule_ComputePurpose(usdGeom_Capsule_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Capsule_ComputeProxyPrim(usdGeom_Capsule_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Capsule_SetProxyPrim(usdGeom_Capsule_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Capsule_ComputeWorldBound(usdGeom_Capsule_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Capsule_ComputeLocalBound(usdGeom_Capsule_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Capsule_ComputeUntransformedBound(usdGeom_Capsule_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Capsule_ComputeLocalToWorldTransform(usdGeom_Capsule_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Capsule_ComputeParentToWorldTransform(usdGeom_Capsule_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Capsule_IsConcrete(usdGeom_Capsule_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Capsule_IsTyped(usdGeom_Capsule_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Capsule_IsAPISchema(usdGeom_Capsule_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Capsule_IsAppliedAPISchema(usdGeom_Capsule_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Capsule_IsMultipleApplyAPISchema(usdGeom_Capsule_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Capsule_GetSchemaKind(usdGeom_Capsule_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Capsule_GetPrim(usdGeom_Capsule_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Capsule_GetPath(usdGeom_Capsule_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Capsule_GetSchemaClassPrimDefinition(usdGeom_Capsule_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Capsule_new(usd_Prim_t const* param00, usdGeom_Capsule_t** _result) {
    *_result = new PXR_NS::UsdGeomCapsule(*param00);
    return 0;
}

int usdGeom_Capsule_dtor(usdGeom_Capsule_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Cone_GetHeightAttr(usdGeom_Cone_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetHeightAttr();
    return 0;
}

int usdGeom_Cone_CreateHeightAttr(usdGeom_Cone_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateHeightAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cone_GetRadiusAttr(usdGeom_Cone_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetRadiusAttr();
    return 0;
}

int usdGeom_Cone_CreateRadiusAttr(usdGeom_Cone_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateRadiusAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cone_GetAxisAttr(usdGeom_Cone_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAxisAttr();
    return 0;
}

int usdGeom_Cone_CreateAxisAttr(usdGeom_Cone_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAxisAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cone_GetDisplayColorAttr(usdGeom_Cone_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_Cone_CreateDisplayColorAttr(usdGeom_Cone_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cone_GetDisplayOpacityAttr(usdGeom_Cone_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_Cone_CreateDisplayOpacityAttr(usdGeom_Cone_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cone_GetDoubleSidedAttr(usdGeom_Cone_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_Cone_CreateDoubleSidedAttr(usdGeom_Cone_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cone_GetOrientationAttr(usdGeom_Cone_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_Cone_CreateOrientationAttr(usdGeom_Cone_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cone_GetDisplayColorPrimvar(usdGeom_Cone_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_Cone_CreateDisplayColorPrimvar(usdGeom_Cone_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_Cone_GetExtentAttr(usdGeom_Cone_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_Cone_CreateExtentAttr(usdGeom_Cone_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cone_GetXformOpOrderAttr(usdGeom_Cone_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Cone_AddXformOp(usdGeom_Cone_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddTranslateOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddScaleOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddRotateXOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddRotateYOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddRotateZOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddRotateXYZOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddRotateXZYOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddRotateYXZOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddRotateYZXOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddRotateZXYOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddRotateZYXOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddOrientOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_AddTransformOp(usdGeom_Cone_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cone_SetResetXformStack(usdGeom_Cone_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Cone_SetXformOpOrder(usdGeom_Cone_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Cone_GetOrderedXformOps(usdGeom_Cone_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Cone_ClearXformOpOrder(usdGeom_Cone_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Cone_MakeMatrixXform(usdGeom_Cone_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Cone_TransformMightBeTimeVarying(usdGeom_Cone_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Cone_TransformMightBeTimeVarying_using(usdGeom_Cone_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Cone_GetTimeSamples(usdGeom_Cone_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Cone_GetTimeSamplesInInterval(usdGeom_Cone_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Cone_GetLocalTransformation(usdGeom_Cone_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Cone_GetLocalTransformation_with_ops(usdGeom_Cone_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Cone_GetVisibilityAttr(usdGeom_Cone_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Cone_CreateVisibilityAttr(usdGeom_Cone_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cone_GetPurposeAttr(usdGeom_Cone_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Cone_CreatePurposeAttr(usdGeom_Cone_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cone_GetProxyPrimRel(usdGeom_Cone_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Cone_CreateProxyPrimRel(usdGeom_Cone_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Cone_MakeVisible(usdGeom_Cone_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Cone_MakeInvisible(usdGeom_Cone_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Cone_ComputeVisibility(usdGeom_Cone_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Cone_GetPurposeVisibilityAttr(usdGeom_Cone_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Cone_ComputePurposeInfo(usdGeom_Cone_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Cone_ComputePurposeInfo_given_parent(usdGeom_Cone_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Cone_ComputePurpose(usdGeom_Cone_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Cone_ComputeProxyPrim(usdGeom_Cone_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Cone_SetProxyPrim(usdGeom_Cone_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Cone_ComputeWorldBound(usdGeom_Cone_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Cone_ComputeLocalBound(usdGeom_Cone_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Cone_ComputeUntransformedBound(usdGeom_Cone_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Cone_ComputeLocalToWorldTransform(usdGeom_Cone_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Cone_ComputeParentToWorldTransform(usdGeom_Cone_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Cone_IsConcrete(usdGeom_Cone_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Cone_IsTyped(usdGeom_Cone_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Cone_IsAPISchema(usdGeom_Cone_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Cone_IsAppliedAPISchema(usdGeom_Cone_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Cone_IsMultipleApplyAPISchema(usdGeom_Cone_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Cone_GetSchemaKind(usdGeom_Cone_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Cone_GetPrim(usdGeom_Cone_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Cone_GetPath(usdGeom_Cone_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Cone_GetSchemaClassPrimDefinition(usdGeom_Cone_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Cone_new(usd_Prim_t const* param00, usdGeom_Cone_t** _result) {
    *_result = new PXR_NS::UsdGeomCone(*param00);
    return 0;
}

int usdGeom_Cone_dtor(usdGeom_Cone_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_ConstraintTarget_IsValid(usd_Attribute_t const* attr, bool* _result) {
    *_result = PXR_NS::UsdGeomConstraintTarget::IsValid(*attr);
    return 0;
}

int usdGeom_ConstraintTarget_GetAttr(usdGeom_ConstraintTarget_t const* _this, usd_Attribute_t const** _result) {
    *_result = &_this->GetAttr();
    return 0;
}

int usdGeom_ConstraintTarget_IsDefined(usdGeom_ConstraintTarget_t const* _this, bool* _result) {
    *_result = _this->IsDefined();
    return 0;
}

int usdGeom_ConstraintTarget_Get(usdGeom_ConstraintTarget_t const* _this, gf_Matrix4d_t* value, usd_TimeCode_t time, bool* _result) {
    *_result = _this->Get(value, time);
    return 0;
}

int usdGeom_ConstraintTarget_Set(usdGeom_ConstraintTarget_t const* _this, gf_Matrix4d_t const* value, usd_TimeCode_t time, bool* _result) {
    *_result = _this->Set(*value, time);
    return 0;
}

int usdGeom_ConstraintTarget_GetIdentifier(usdGeom_ConstraintTarget_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetIdentifier();
    return 0;
}

int usdGeom_ConstraintTarget_SetIdentifier(usdGeom_ConstraintTarget_t* _this, tf_Token_t const* identifier) {
    _this->SetIdentifier(*identifier);
    return 0;
}

int usdGeom_ConstraintTarget_ComputeInWorldSpace(usdGeom_ConstraintTarget_t const* _this, usd_TimeCode_t time, usdGeom_XformCache_t* xfCache, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeInWorldSpace(time, xfCache);
    return 0;
}

int usdGeom_ConstraintTarget_GetConstraintAttrName(std_String_t const* constraintName, tf_Token_t* _result) {
    *_result = PXR_NS::UsdGeomConstraintTarget::GetConstraintAttrName(*constraintName);
    return 0;
}

int usdGeom_ConstraintTarget_new(usd_Attribute_t const* param00, usdGeom_ConstraintTarget_t** _result) {
    *_result = new PXR_NS::UsdGeomConstraintTarget(*param00);
    return 0;
}

int usdGeom_ConstraintTarget_dtor(usdGeom_ConstraintTarget_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_ConstraintTargetVector_data(usdGeom_ConstraintTargetVector_t* _this, usdGeom_ConstraintTarget_t** _result) {
    *_result = _this->data();
    return 0;
}

int usdGeom_ConstraintTargetVector_data_const(usdGeom_ConstraintTargetVector_t const* _this, usdGeom_ConstraintTarget_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usdGeom_ConstraintTargetVector_empty(usdGeom_ConstraintTargetVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usdGeom_ConstraintTargetVector_size(usdGeom_ConstraintTargetVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usdGeom_ConstraintTargetVector_max_size(usdGeom_ConstraintTargetVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usdGeom_ConstraintTargetVector_reserve(usdGeom_ConstraintTargetVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usdGeom_ConstraintTargetVector_capacity(usdGeom_ConstraintTargetVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usdGeom_ConstraintTargetVector_clear(usdGeom_ConstraintTargetVector_t* _this) {
    _this->clear();
    return 0;
}

int usdGeom_ConstraintTargetVector_push_back(usdGeom_ConstraintTargetVector_t* _this, usdGeom_ConstraintTarget_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usdGeom_ConstraintTargetVector_pop_back(usdGeom_ConstraintTargetVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usdGeom_ConstraintTargetVector_resize(usdGeom_ConstraintTargetVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usdGeom_ConstraintTargetVector_resize_with(usdGeom_ConstraintTargetVector_t* _this, size_t const _Newsize, usdGeom_ConstraintTarget_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usdGeom_ConstraintTargetVector_op_index(usdGeom_ConstraintTargetVector_t const* _this, size_t const _Pos, usdGeom_ConstraintTarget_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int usdGeom_ConstraintTargetVector_default(usdGeom_ConstraintTargetVector_t** _result) {
    *_result = new std::vector<PXR_NS::UsdGeomConstraintTarget>();
    return 0;
}

int usdGeom_ConstraintTargetVector_dtor(usdGeom_ConstraintTargetVector_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Cube_GetSizeAttr(usdGeom_Cube_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetSizeAttr();
    return 0;
}

int usdGeom_Cube_CreateSizeAttr(usdGeom_Cube_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateSizeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cube_GetDisplayColorAttr(usdGeom_Cube_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_Cube_CreateDisplayColorAttr(usdGeom_Cube_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cube_GetDisplayOpacityAttr(usdGeom_Cube_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_Cube_CreateDisplayOpacityAttr(usdGeom_Cube_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cube_GetDoubleSidedAttr(usdGeom_Cube_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_Cube_CreateDoubleSidedAttr(usdGeom_Cube_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cube_GetOrientationAttr(usdGeom_Cube_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_Cube_CreateOrientationAttr(usdGeom_Cube_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cube_GetDisplayColorPrimvar(usdGeom_Cube_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_Cube_CreateDisplayColorPrimvar(usdGeom_Cube_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_Cube_GetExtentAttr(usdGeom_Cube_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_Cube_CreateExtentAttr(usdGeom_Cube_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cube_GetXformOpOrderAttr(usdGeom_Cube_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Cube_AddXformOp(usdGeom_Cube_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddTranslateOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddScaleOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddRotateXOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddRotateYOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddRotateZOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddRotateXYZOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddRotateXZYOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddRotateYXZOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddRotateYZXOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddRotateZXYOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddRotateZYXOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddOrientOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_AddTransformOp(usdGeom_Cube_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cube_SetResetXformStack(usdGeom_Cube_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Cube_SetXformOpOrder(usdGeom_Cube_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Cube_GetOrderedXformOps(usdGeom_Cube_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Cube_ClearXformOpOrder(usdGeom_Cube_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Cube_MakeMatrixXform(usdGeom_Cube_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Cube_TransformMightBeTimeVarying(usdGeom_Cube_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Cube_TransformMightBeTimeVarying_using(usdGeom_Cube_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Cube_GetTimeSamples(usdGeom_Cube_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Cube_GetTimeSamplesInInterval(usdGeom_Cube_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Cube_GetLocalTransformation(usdGeom_Cube_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Cube_GetLocalTransformation_with_ops(usdGeom_Cube_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Cube_GetVisibilityAttr(usdGeom_Cube_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Cube_CreateVisibilityAttr(usdGeom_Cube_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cube_GetPurposeAttr(usdGeom_Cube_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Cube_CreatePurposeAttr(usdGeom_Cube_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cube_GetProxyPrimRel(usdGeom_Cube_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Cube_CreateProxyPrimRel(usdGeom_Cube_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Cube_MakeVisible(usdGeom_Cube_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Cube_MakeInvisible(usdGeom_Cube_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Cube_ComputeVisibility(usdGeom_Cube_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Cube_GetPurposeVisibilityAttr(usdGeom_Cube_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Cube_ComputePurposeInfo(usdGeom_Cube_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Cube_ComputePurposeInfo_given_parent(usdGeom_Cube_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Cube_ComputePurpose(usdGeom_Cube_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Cube_ComputeProxyPrim(usdGeom_Cube_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Cube_SetProxyPrim(usdGeom_Cube_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Cube_ComputeWorldBound(usdGeom_Cube_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Cube_ComputeLocalBound(usdGeom_Cube_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Cube_ComputeUntransformedBound(usdGeom_Cube_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Cube_ComputeLocalToWorldTransform(usdGeom_Cube_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Cube_ComputeParentToWorldTransform(usdGeom_Cube_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Cube_IsConcrete(usdGeom_Cube_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Cube_IsTyped(usdGeom_Cube_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Cube_IsAPISchema(usdGeom_Cube_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Cube_IsAppliedAPISchema(usdGeom_Cube_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Cube_IsMultipleApplyAPISchema(usdGeom_Cube_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Cube_GetSchemaKind(usdGeom_Cube_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Cube_GetPrim(usdGeom_Cube_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Cube_GetPath(usdGeom_Cube_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Cube_GetSchemaClassPrimDefinition(usdGeom_Cube_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Cube_new(usd_Prim_t const* param00, usdGeom_Cube_t** _result) {
    *_result = new PXR_NS::UsdGeomCube(*param00);
    return 0;
}

int usdGeom_Cube_dtor(usdGeom_Cube_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Curves_GetCurveVertexCountsAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetCurveVertexCountsAttr();
    return 0;
}

int usdGeom_Curves_CreateCurveVertexCountsAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateCurveVertexCountsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetWidthsAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetWidthsAttr();
    return 0;
}

int usdGeom_Curves_CreateWidthsAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateWidthsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetWidthsInterpolation(usdGeom_Curves_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetWidthsInterpolation();
    return 0;
}

int usdGeom_Curves_SetWidthsInterpolation(usdGeom_Curves_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetWidthsInterpolation(*interpolation);
    return 0;
}

int usdGeom_Curves_GetCurveCount(usdGeom_Curves_t const* _this, usd_TimeCode_t timeCode, size_t* _result) {
    *_result = _this->GetCurveCount(timeCode);
    return 0;
}

int usdGeom_Curves_GetPointsAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPointsAttr();
    return 0;
}

int usdGeom_Curves_CreatePointsAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePointsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetVelocitiesAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVelocitiesAttr();
    return 0;
}

int usdGeom_Curves_CreateVelocitiesAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVelocitiesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetAccelerationsAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAccelerationsAttr();
    return 0;
}

int usdGeom_Curves_CreateAccelerationsAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAccelerationsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetNormalsAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetNormalsAttr();
    return 0;
}

int usdGeom_Curves_CreateNormalsAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateNormalsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetNormalsInterpolation(usdGeom_Curves_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNormalsInterpolation();
    return 0;
}

int usdGeom_Curves_SetNormalsInterpolation(usdGeom_Curves_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetNormalsInterpolation(*interpolation);
    return 0;
}

int usdGeom_Curves_ComputePointsAtTime(usdGeom_Curves_t const* _this, gf_Vec3fArray_t* points, usd_TimeCode_t const time, usd_TimeCode_t const baseTime, bool* _result) {
    *_result = _this->ComputePointsAtTime(points, time, baseTime);
    return 0;
}

int usdGeom_Curves_GetDisplayColorAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_Curves_CreateDisplayColorAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetDisplayOpacityAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_Curves_CreateDisplayOpacityAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetDoubleSidedAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_Curves_CreateDoubleSidedAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetOrientationAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_Curves_CreateOrientationAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetDisplayColorPrimvar(usdGeom_Curves_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_Curves_CreateDisplayColorPrimvar(usdGeom_Curves_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_Curves_GetExtentAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_Curves_CreateExtentAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetXformOpOrderAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Curves_AddXformOp(usdGeom_Curves_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddTranslateOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddScaleOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddRotateXOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddRotateYOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddRotateZOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddRotateXYZOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddRotateXZYOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddRotateYXZOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddRotateYZXOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddRotateZXYOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddRotateZYXOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddOrientOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_AddTransformOp(usdGeom_Curves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Curves_SetResetXformStack(usdGeom_Curves_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Curves_SetXformOpOrder(usdGeom_Curves_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Curves_GetOrderedXformOps(usdGeom_Curves_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Curves_ClearXformOpOrder(usdGeom_Curves_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Curves_MakeMatrixXform(usdGeom_Curves_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Curves_TransformMightBeTimeVarying(usdGeom_Curves_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Curves_TransformMightBeTimeVarying_using(usdGeom_Curves_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Curves_GetTimeSamples(usdGeom_Curves_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Curves_GetTimeSamplesInInterval(usdGeom_Curves_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Curves_GetLocalTransformation(usdGeom_Curves_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Curves_GetLocalTransformation_with_ops(usdGeom_Curves_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Curves_GetVisibilityAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Curves_CreateVisibilityAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetPurposeAttr(usdGeom_Curves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Curves_CreatePurposeAttr(usdGeom_Curves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Curves_GetProxyPrimRel(usdGeom_Curves_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Curves_CreateProxyPrimRel(usdGeom_Curves_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Curves_MakeVisible(usdGeom_Curves_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Curves_MakeInvisible(usdGeom_Curves_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Curves_ComputeVisibility(usdGeom_Curves_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Curves_GetPurposeVisibilityAttr(usdGeom_Curves_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Curves_ComputePurposeInfo(usdGeom_Curves_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Curves_ComputePurposeInfo_given_parent(usdGeom_Curves_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Curves_ComputePurpose(usdGeom_Curves_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Curves_ComputeProxyPrim(usdGeom_Curves_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Curves_SetProxyPrim(usdGeom_Curves_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Curves_ComputeWorldBound(usdGeom_Curves_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Curves_ComputeLocalBound(usdGeom_Curves_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Curves_ComputeUntransformedBound(usdGeom_Curves_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Curves_ComputeLocalToWorldTransform(usdGeom_Curves_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Curves_ComputeParentToWorldTransform(usdGeom_Curves_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Curves_IsConcrete(usdGeom_Curves_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Curves_IsTyped(usdGeom_Curves_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Curves_IsAPISchema(usdGeom_Curves_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Curves_IsAppliedAPISchema(usdGeom_Curves_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Curves_IsMultipleApplyAPISchema(usdGeom_Curves_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Curves_GetSchemaKind(usdGeom_Curves_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Curves_GetPrim(usdGeom_Curves_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Curves_GetPath(usdGeom_Curves_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Curves_GetSchemaClassPrimDefinition(usdGeom_Curves_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Curves_new(usd_Prim_t const* param00, usdGeom_Curves_t** _result) {
    *_result = new PXR_NS::UsdGeomCurves(*param00);
    return 0;
}

int usdGeom_Curves_dtor(usdGeom_Curves_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Cylinder_GetHeightAttr(usdGeom_Cylinder_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetHeightAttr();
    return 0;
}

int usdGeom_Cylinder_CreateHeightAttr(usdGeom_Cylinder_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateHeightAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cylinder_GetRadiusAttr(usdGeom_Cylinder_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetRadiusAttr();
    return 0;
}

int usdGeom_Cylinder_CreateRadiusAttr(usdGeom_Cylinder_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateRadiusAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cylinder_GetAxisAttr(usdGeom_Cylinder_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAxisAttr();
    return 0;
}

int usdGeom_Cylinder_CreateAxisAttr(usdGeom_Cylinder_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAxisAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cylinder_GetDisplayColorAttr(usdGeom_Cylinder_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_Cylinder_CreateDisplayColorAttr(usdGeom_Cylinder_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cylinder_GetDisplayOpacityAttr(usdGeom_Cylinder_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_Cylinder_CreateDisplayOpacityAttr(usdGeom_Cylinder_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cylinder_GetDoubleSidedAttr(usdGeom_Cylinder_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_Cylinder_CreateDoubleSidedAttr(usdGeom_Cylinder_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cylinder_GetOrientationAttr(usdGeom_Cylinder_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_Cylinder_CreateOrientationAttr(usdGeom_Cylinder_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cylinder_GetDisplayColorPrimvar(usdGeom_Cylinder_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_Cylinder_CreateDisplayColorPrimvar(usdGeom_Cylinder_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_Cylinder_GetExtentAttr(usdGeom_Cylinder_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_Cylinder_CreateExtentAttr(usdGeom_Cylinder_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cylinder_GetXformOpOrderAttr(usdGeom_Cylinder_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Cylinder_AddXformOp(usdGeom_Cylinder_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddTranslateOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddScaleOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddRotateXOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddRotateYOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddRotateZOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddRotateXYZOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddRotateXZYOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddRotateYXZOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddRotateYZXOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddRotateZXYOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddRotateZYXOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddOrientOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_AddTransformOp(usdGeom_Cylinder_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Cylinder_SetResetXformStack(usdGeom_Cylinder_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Cylinder_SetXformOpOrder(usdGeom_Cylinder_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Cylinder_GetOrderedXformOps(usdGeom_Cylinder_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Cylinder_ClearXformOpOrder(usdGeom_Cylinder_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Cylinder_MakeMatrixXform(usdGeom_Cylinder_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Cylinder_TransformMightBeTimeVarying(usdGeom_Cylinder_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Cylinder_TransformMightBeTimeVarying_using(usdGeom_Cylinder_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Cylinder_GetTimeSamples(usdGeom_Cylinder_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Cylinder_GetTimeSamplesInInterval(usdGeom_Cylinder_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Cylinder_GetLocalTransformation(usdGeom_Cylinder_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Cylinder_GetLocalTransformation_with_ops(usdGeom_Cylinder_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Cylinder_GetVisibilityAttr(usdGeom_Cylinder_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Cylinder_CreateVisibilityAttr(usdGeom_Cylinder_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cylinder_GetPurposeAttr(usdGeom_Cylinder_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Cylinder_CreatePurposeAttr(usdGeom_Cylinder_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Cylinder_GetProxyPrimRel(usdGeom_Cylinder_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Cylinder_CreateProxyPrimRel(usdGeom_Cylinder_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Cylinder_MakeVisible(usdGeom_Cylinder_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Cylinder_MakeInvisible(usdGeom_Cylinder_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Cylinder_ComputeVisibility(usdGeom_Cylinder_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Cylinder_GetPurposeVisibilityAttr(usdGeom_Cylinder_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Cylinder_ComputePurposeInfo(usdGeom_Cylinder_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Cylinder_ComputePurposeInfo_given_parent(usdGeom_Cylinder_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Cylinder_ComputePurpose(usdGeom_Cylinder_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Cylinder_ComputeProxyPrim(usdGeom_Cylinder_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Cylinder_SetProxyPrim(usdGeom_Cylinder_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Cylinder_ComputeWorldBound(usdGeom_Cylinder_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Cylinder_ComputeLocalBound(usdGeom_Cylinder_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Cylinder_ComputeUntransformedBound(usdGeom_Cylinder_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Cylinder_ComputeLocalToWorldTransform(usdGeom_Cylinder_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Cylinder_ComputeParentToWorldTransform(usdGeom_Cylinder_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Cylinder_IsConcrete(usdGeom_Cylinder_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Cylinder_IsTyped(usdGeom_Cylinder_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Cylinder_IsAPISchema(usdGeom_Cylinder_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Cylinder_IsAppliedAPISchema(usdGeom_Cylinder_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Cylinder_IsMultipleApplyAPISchema(usdGeom_Cylinder_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Cylinder_GetSchemaKind(usdGeom_Cylinder_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Cylinder_GetPrim(usdGeom_Cylinder_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Cylinder_GetPath(usdGeom_Cylinder_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Cylinder_GetSchemaClassPrimDefinition(usdGeom_Cylinder_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Cylinder_new(usd_Prim_t const* param00, usdGeom_Cylinder_t** _result) {
    *_result = new PXR_NS::UsdGeomCylinder(*param00);
    return 0;
}

int usdGeom_Cylinder_dtor(usdGeom_Cylinder_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Gprim_GetDisplayColorAttr(usdGeom_Gprim_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_Gprim_CreateDisplayColorAttr(usdGeom_Gprim_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Gprim_GetDisplayOpacityAttr(usdGeom_Gprim_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_Gprim_CreateDisplayOpacityAttr(usdGeom_Gprim_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Gprim_GetDoubleSidedAttr(usdGeom_Gprim_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_Gprim_CreateDoubleSidedAttr(usdGeom_Gprim_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Gprim_GetOrientationAttr(usdGeom_Gprim_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_Gprim_CreateOrientationAttr(usdGeom_Gprim_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Gprim_GetDisplayColorPrimvar(usdGeom_Gprim_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_Gprim_CreateDisplayColorPrimvar(usdGeom_Gprim_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_Gprim_GetExtentAttr(usdGeom_Gprim_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_Gprim_CreateExtentAttr(usdGeom_Gprim_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Gprim_GetXformOpOrderAttr(usdGeom_Gprim_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Gprim_AddXformOp(usdGeom_Gprim_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddTranslateOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddScaleOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddRotateXOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddRotateYOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddRotateZOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddRotateXYZOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddRotateXZYOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddRotateYXZOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddRotateYZXOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddRotateZXYOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddRotateZYXOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddOrientOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_AddTransformOp(usdGeom_Gprim_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Gprim_SetResetXformStack(usdGeom_Gprim_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Gprim_SetXformOpOrder(usdGeom_Gprim_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Gprim_GetOrderedXformOps(usdGeom_Gprim_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Gprim_ClearXformOpOrder(usdGeom_Gprim_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Gprim_MakeMatrixXform(usdGeom_Gprim_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Gprim_TransformMightBeTimeVarying(usdGeom_Gprim_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Gprim_TransformMightBeTimeVarying_using(usdGeom_Gprim_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Gprim_GetTimeSamples(usdGeom_Gprim_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Gprim_GetTimeSamplesInInterval(usdGeom_Gprim_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Gprim_GetLocalTransformation(usdGeom_Gprim_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Gprim_GetLocalTransformation_with_ops(usdGeom_Gprim_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Gprim_GetVisibilityAttr(usdGeom_Gprim_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Gprim_CreateVisibilityAttr(usdGeom_Gprim_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Gprim_GetPurposeAttr(usdGeom_Gprim_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Gprim_CreatePurposeAttr(usdGeom_Gprim_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Gprim_GetProxyPrimRel(usdGeom_Gprim_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Gprim_CreateProxyPrimRel(usdGeom_Gprim_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Gprim_MakeVisible(usdGeom_Gprim_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Gprim_MakeInvisible(usdGeom_Gprim_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Gprim_ComputeVisibility(usdGeom_Gprim_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Gprim_GetPurposeVisibilityAttr(usdGeom_Gprim_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Gprim_ComputePurposeInfo(usdGeom_Gprim_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Gprim_ComputePurposeInfo_given_parent(usdGeom_Gprim_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Gprim_ComputePurpose(usdGeom_Gprim_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Gprim_ComputeProxyPrim(usdGeom_Gprim_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Gprim_SetProxyPrim(usdGeom_Gprim_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Gprim_ComputeWorldBound(usdGeom_Gprim_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Gprim_ComputeLocalBound(usdGeom_Gprim_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Gprim_ComputeUntransformedBound(usdGeom_Gprim_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Gprim_ComputeLocalToWorldTransform(usdGeom_Gprim_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Gprim_ComputeParentToWorldTransform(usdGeom_Gprim_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Gprim_IsConcrete(usdGeom_Gprim_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Gprim_IsTyped(usdGeom_Gprim_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Gprim_IsAPISchema(usdGeom_Gprim_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Gprim_IsAppliedAPISchema(usdGeom_Gprim_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Gprim_IsMultipleApplyAPISchema(usdGeom_Gprim_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Gprim_GetSchemaKind(usdGeom_Gprim_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Gprim_GetPrim(usdGeom_Gprim_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Gprim_GetPath(usdGeom_Gprim_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Gprim_GetSchemaClassPrimDefinition(usdGeom_Gprim_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Gprim_new(usd_Prim_t const* param00, usdGeom_Gprim_t** _result) {
    *_result = new PXR_NS::UsdGeomGprim(*param00);
    return 0;
}

int usdGeom_Gprim_dtor(usdGeom_Gprim_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_HermiteCurves_GetTangentsAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetTangentsAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreateTangentsAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateTangentsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetCurveVertexCountsAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetCurveVertexCountsAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreateCurveVertexCountsAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateCurveVertexCountsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetWidthsAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetWidthsAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreateWidthsAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateWidthsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetWidthsInterpolation(usdGeom_HermiteCurves_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetWidthsInterpolation();
    return 0;
}

int usdGeom_HermiteCurves_SetWidthsInterpolation(usdGeom_HermiteCurves_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetWidthsInterpolation(*interpolation);
    return 0;
}

int usdGeom_HermiteCurves_GetCurveCount(usdGeom_HermiteCurves_t const* _this, usd_TimeCode_t timeCode, size_t* _result) {
    *_result = _this->GetCurveCount(timeCode);
    return 0;
}

int usdGeom_HermiteCurves_GetPointsAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPointsAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreatePointsAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePointsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetVelocitiesAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVelocitiesAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreateVelocitiesAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVelocitiesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetAccelerationsAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAccelerationsAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreateAccelerationsAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAccelerationsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetNormalsAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetNormalsAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreateNormalsAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateNormalsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetNormalsInterpolation(usdGeom_HermiteCurves_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNormalsInterpolation();
    return 0;
}

int usdGeom_HermiteCurves_SetNormalsInterpolation(usdGeom_HermiteCurves_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetNormalsInterpolation(*interpolation);
    return 0;
}

int usdGeom_HermiteCurves_ComputePointsAtTime(usdGeom_HermiteCurves_t const* _this, gf_Vec3fArray_t* points, usd_TimeCode_t const time, usd_TimeCode_t const baseTime, bool* _result) {
    *_result = _this->ComputePointsAtTime(points, time, baseTime);
    return 0;
}

int usdGeom_HermiteCurves_GetDisplayColorAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreateDisplayColorAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetDisplayOpacityAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreateDisplayOpacityAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetDoubleSidedAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreateDoubleSidedAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetOrientationAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreateOrientationAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetDisplayColorPrimvar(usdGeom_HermiteCurves_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_HermiteCurves_CreateDisplayColorPrimvar(usdGeom_HermiteCurves_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_HermiteCurves_GetExtentAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreateExtentAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetXformOpOrderAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_HermiteCurves_AddXformOp(usdGeom_HermiteCurves_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddTranslateOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddScaleOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddRotateXOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddRotateYOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddRotateZOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddRotateXYZOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddRotateXZYOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddRotateYXZOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddRotateYZXOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddRotateZXYOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddRotateZYXOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddOrientOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_AddTransformOp(usdGeom_HermiteCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_HermiteCurves_SetResetXformStack(usdGeom_HermiteCurves_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_HermiteCurves_SetXformOpOrder(usdGeom_HermiteCurves_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_HermiteCurves_GetOrderedXformOps(usdGeom_HermiteCurves_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_HermiteCurves_ClearXformOpOrder(usdGeom_HermiteCurves_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_HermiteCurves_MakeMatrixXform(usdGeom_HermiteCurves_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_HermiteCurves_TransformMightBeTimeVarying(usdGeom_HermiteCurves_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_HermiteCurves_TransformMightBeTimeVarying_using(usdGeom_HermiteCurves_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_HermiteCurves_GetTimeSamples(usdGeom_HermiteCurves_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_HermiteCurves_GetTimeSamplesInInterval(usdGeom_HermiteCurves_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_HermiteCurves_GetLocalTransformation(usdGeom_HermiteCurves_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_HermiteCurves_GetLocalTransformation_with_ops(usdGeom_HermiteCurves_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_HermiteCurves_GetVisibilityAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreateVisibilityAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetPurposeAttr(usdGeom_HermiteCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_HermiteCurves_CreatePurposeAttr(usdGeom_HermiteCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_HermiteCurves_GetProxyPrimRel(usdGeom_HermiteCurves_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_HermiteCurves_CreateProxyPrimRel(usdGeom_HermiteCurves_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_HermiteCurves_MakeVisible(usdGeom_HermiteCurves_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_HermiteCurves_MakeInvisible(usdGeom_HermiteCurves_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_HermiteCurves_ComputeVisibility(usdGeom_HermiteCurves_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_HermiteCurves_GetPurposeVisibilityAttr(usdGeom_HermiteCurves_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_HermiteCurves_ComputePurposeInfo(usdGeom_HermiteCurves_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_HermiteCurves_ComputePurposeInfo_given_parent(usdGeom_HermiteCurves_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_HermiteCurves_ComputePurpose(usdGeom_HermiteCurves_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_HermiteCurves_ComputeProxyPrim(usdGeom_HermiteCurves_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_HermiteCurves_SetProxyPrim(usdGeom_HermiteCurves_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_HermiteCurves_ComputeWorldBound(usdGeom_HermiteCurves_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_HermiteCurves_ComputeLocalBound(usdGeom_HermiteCurves_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_HermiteCurves_ComputeUntransformedBound(usdGeom_HermiteCurves_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_HermiteCurves_ComputeLocalToWorldTransform(usdGeom_HermiteCurves_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_HermiteCurves_ComputeParentToWorldTransform(usdGeom_HermiteCurves_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_HermiteCurves_IsConcrete(usdGeom_HermiteCurves_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_HermiteCurves_IsTyped(usdGeom_HermiteCurves_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_HermiteCurves_IsAPISchema(usdGeom_HermiteCurves_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_HermiteCurves_IsAppliedAPISchema(usdGeom_HermiteCurves_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_HermiteCurves_IsMultipleApplyAPISchema(usdGeom_HermiteCurves_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_HermiteCurves_GetSchemaKind(usdGeom_HermiteCurves_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_HermiteCurves_GetPrim(usdGeom_HermiteCurves_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_HermiteCurves_GetPath(usdGeom_HermiteCurves_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_HermiteCurves_GetSchemaClassPrimDefinition(usdGeom_HermiteCurves_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_HermiteCurves_new(usd_Prim_t const* param00, usdGeom_HermiteCurves_t** _result) {
    *_result = new PXR_NS::UsdGeomHermiteCurves(*param00);
    return 0;
}

int usdGeom_HermiteCurves_dtor(usdGeom_HermiteCurves_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Imageable_GetVisibilityAttr(usdGeom_Imageable_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Imageable_CreateVisibilityAttr(usdGeom_Imageable_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Imageable_GetPurposeAttr(usdGeom_Imageable_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Imageable_CreatePurposeAttr(usdGeom_Imageable_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Imageable_GetProxyPrimRel(usdGeom_Imageable_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Imageable_CreateProxyPrimRel(usdGeom_Imageable_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Imageable_MakeVisible(usdGeom_Imageable_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Imageable_MakeInvisible(usdGeom_Imageable_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Imageable_ComputeVisibility(usdGeom_Imageable_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Imageable_GetPurposeVisibilityAttr(usdGeom_Imageable_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Imageable_ComputePurposeInfo(usdGeom_Imageable_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Imageable_ComputePurposeInfo_given_parent(usdGeom_Imageable_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Imageable_ComputePurpose(usdGeom_Imageable_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Imageable_ComputeProxyPrim(usdGeom_Imageable_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Imageable_SetProxyPrim(usdGeom_Imageable_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Imageable_ComputeWorldBound(usdGeom_Imageable_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Imageable_ComputeLocalBound(usdGeom_Imageable_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Imageable_ComputeUntransformedBound(usdGeom_Imageable_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Imageable_ComputeLocalToWorldTransform(usdGeom_Imageable_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Imageable_ComputeParentToWorldTransform(usdGeom_Imageable_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Imageable_IsConcrete(usdGeom_Imageable_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Imageable_IsTyped(usdGeom_Imageable_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Imageable_IsAPISchema(usdGeom_Imageable_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Imageable_IsAppliedAPISchema(usdGeom_Imageable_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Imageable_IsMultipleApplyAPISchema(usdGeom_Imageable_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Imageable_GetSchemaKind(usdGeom_Imageable_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Imageable_GetPrim(usdGeom_Imageable_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Imageable_GetPath(usdGeom_Imageable_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Imageable_GetSchemaClassPrimDefinition(usdGeom_Imageable_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Imageable_new(usd_Prim_t const* param00, usdGeom_Imageable_t** _result) {
    *_result = new PXR_NS::UsdGeomImageable(*param00);
    return 0;
}

int usdGeom_Imageable_dtor(usdGeom_Imageable_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_ImageablePurposeInfo_dtor(usdGeom_ImageablePurposeInfo_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Mesh_GetFaceVertexIndicesAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetFaceVertexIndicesAttr();
    return 0;
}

int usdGeom_Mesh_CreateFaceVertexIndicesAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateFaceVertexIndicesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetFaceVertexCountsAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetFaceVertexCountsAttr();
    return 0;
}

int usdGeom_Mesh_CreateFaceVertexCountsAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateFaceVertexCountsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetSubdivisionSchemeAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetSubdivisionSchemeAttr();
    return 0;
}

int usdGeom_Mesh_CreateSubdivisionSchemeAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateSubdivisionSchemeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetInterpolateBoundaryAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetInterpolateBoundaryAttr();
    return 0;
}

int usdGeom_Mesh_CreateInterpolateBoundaryAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateInterpolateBoundaryAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetFaceVaryingLinearInterpolationAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetFaceVaryingLinearInterpolationAttr();
    return 0;
}

int usdGeom_Mesh_CreateFaceVaryingLinearInterpolationAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateFaceVaryingLinearInterpolationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetTriangleSubdivisionRuleAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetTriangleSubdivisionRuleAttr();
    return 0;
}

int usdGeom_Mesh_CreateTriangleSubdivisionRuleAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateTriangleSubdivisionRuleAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetHoleIndicesAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetHoleIndicesAttr();
    return 0;
}

int usdGeom_Mesh_CreateHoleIndicesAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateHoleIndicesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetCornerIndicesAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetCornerIndicesAttr();
    return 0;
}

int usdGeom_Mesh_CreateCornerIndicesAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateCornerIndicesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetCornerSharpnessesAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetCornerSharpnessesAttr();
    return 0;
}

int usdGeom_Mesh_CreateCornerSharpnessesAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateCornerSharpnessesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetCreaseIndicesAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetCreaseIndicesAttr();
    return 0;
}

int usdGeom_Mesh_CreateCreaseIndicesAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateCreaseIndicesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetCreaseLengthsAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetCreaseLengthsAttr();
    return 0;
}

int usdGeom_Mesh_CreateCreaseLengthsAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateCreaseLengthsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetCreaseSharpnessesAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetCreaseSharpnessesAttr();
    return 0;
}

int usdGeom_Mesh_CreateCreaseSharpnessesAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateCreaseSharpnessesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetFaceCount(usdGeom_Mesh_t const* _this, usd_TimeCode_t timeCode, size_t* _result) {
    *_result = _this->GetFaceCount(timeCode);
    return 0;
}

int usdGeom_Mesh_GetPointsAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPointsAttr();
    return 0;
}

int usdGeom_Mesh_CreatePointsAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePointsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetVelocitiesAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVelocitiesAttr();
    return 0;
}

int usdGeom_Mesh_CreateVelocitiesAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVelocitiesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetAccelerationsAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAccelerationsAttr();
    return 0;
}

int usdGeom_Mesh_CreateAccelerationsAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAccelerationsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetNormalsAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetNormalsAttr();
    return 0;
}

int usdGeom_Mesh_CreateNormalsAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateNormalsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetNormalsInterpolation(usdGeom_Mesh_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNormalsInterpolation();
    return 0;
}

int usdGeom_Mesh_SetNormalsInterpolation(usdGeom_Mesh_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetNormalsInterpolation(*interpolation);
    return 0;
}

int usdGeom_Mesh_ComputePointsAtTime(usdGeom_Mesh_t const* _this, gf_Vec3fArray_t* points, usd_TimeCode_t const time, usd_TimeCode_t const baseTime, bool* _result) {
    *_result = _this->ComputePointsAtTime(points, time, baseTime);
    return 0;
}

int usdGeom_Mesh_GetDisplayColorAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_Mesh_CreateDisplayColorAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetDisplayOpacityAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_Mesh_CreateDisplayOpacityAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetDoubleSidedAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_Mesh_CreateDoubleSidedAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetOrientationAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_Mesh_CreateOrientationAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetDisplayColorPrimvar(usdGeom_Mesh_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_Mesh_CreateDisplayColorPrimvar(usdGeom_Mesh_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_Mesh_GetExtentAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_Mesh_CreateExtentAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetXformOpOrderAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Mesh_AddXformOp(usdGeom_Mesh_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddTranslateOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddScaleOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddRotateXOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddRotateYOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddRotateZOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddRotateXYZOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddRotateXZYOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddRotateYXZOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddRotateYZXOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddRotateZXYOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddRotateZYXOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddOrientOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_AddTransformOp(usdGeom_Mesh_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Mesh_SetResetXformStack(usdGeom_Mesh_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Mesh_SetXformOpOrder(usdGeom_Mesh_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Mesh_GetOrderedXformOps(usdGeom_Mesh_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Mesh_ClearXformOpOrder(usdGeom_Mesh_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Mesh_MakeMatrixXform(usdGeom_Mesh_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Mesh_TransformMightBeTimeVarying(usdGeom_Mesh_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Mesh_TransformMightBeTimeVarying_using(usdGeom_Mesh_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Mesh_GetTimeSamples(usdGeom_Mesh_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Mesh_GetTimeSamplesInInterval(usdGeom_Mesh_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Mesh_GetLocalTransformation(usdGeom_Mesh_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Mesh_GetLocalTransformation_with_ops(usdGeom_Mesh_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Mesh_GetVisibilityAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Mesh_CreateVisibilityAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetPurposeAttr(usdGeom_Mesh_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Mesh_CreatePurposeAttr(usdGeom_Mesh_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Mesh_GetProxyPrimRel(usdGeom_Mesh_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Mesh_CreateProxyPrimRel(usdGeom_Mesh_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Mesh_MakeVisible(usdGeom_Mesh_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Mesh_MakeInvisible(usdGeom_Mesh_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Mesh_ComputeVisibility(usdGeom_Mesh_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Mesh_GetPurposeVisibilityAttr(usdGeom_Mesh_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Mesh_ComputePurposeInfo(usdGeom_Mesh_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Mesh_ComputePurposeInfo_given_parent(usdGeom_Mesh_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Mesh_ComputePurpose(usdGeom_Mesh_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Mesh_ComputeProxyPrim(usdGeom_Mesh_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Mesh_SetProxyPrim(usdGeom_Mesh_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Mesh_ComputeWorldBound(usdGeom_Mesh_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Mesh_ComputeLocalBound(usdGeom_Mesh_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Mesh_ComputeUntransformedBound(usdGeom_Mesh_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Mesh_ComputeLocalToWorldTransform(usdGeom_Mesh_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Mesh_ComputeParentToWorldTransform(usdGeom_Mesh_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Mesh_IsConcrete(usdGeom_Mesh_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Mesh_IsTyped(usdGeom_Mesh_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Mesh_IsAPISchema(usdGeom_Mesh_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Mesh_IsAppliedAPISchema(usdGeom_Mesh_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Mesh_IsMultipleApplyAPISchema(usdGeom_Mesh_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Mesh_GetSchemaKind(usdGeom_Mesh_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Mesh_GetPrim(usdGeom_Mesh_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Mesh_GetPath(usdGeom_Mesh_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Mesh_GetSchemaClassPrimDefinition(usdGeom_Mesh_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Mesh_new(usd_Prim_t const* param00, usdGeom_Mesh_t** _result) {
    *_result = new PXR_NS::UsdGeomMesh(*param00);
    return 0;
}

int usdGeom_Mesh_dtor(usdGeom_Mesh_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_ModelAPI_GetModelDrawModeAttr(usdGeom_ModelAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetModelDrawModeAttr();
    return 0;
}

int usdGeom_ModelAPI_CreateModelDrawModeAttr(usdGeom_ModelAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateModelDrawModeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_ModelAPI_GetModelDrawModeColorAttr(usdGeom_ModelAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetModelDrawModeColorAttr();
    return 0;
}

int usdGeom_ModelAPI_CreateModelDrawModeColorAttr(usdGeom_ModelAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateModelDrawModeColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_ModelAPI_GetModelCardGeometryAttr(usdGeom_ModelAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetModelCardGeometryAttr();
    return 0;
}

int usdGeom_ModelAPI_CreateModelCardGeometryAttr(usdGeom_ModelAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateModelCardGeometryAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_ModelAPI_GetModelCardTextureXPosAttr(usdGeom_ModelAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetModelCardTextureXPosAttr();
    return 0;
}

int usdGeom_ModelAPI_CreateModelCardTextureXPosAttr(usdGeom_ModelAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateModelCardTextureXPosAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_ModelAPI_GetModelCardTextureXNegAttr(usdGeom_ModelAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetModelCardTextureXNegAttr();
    return 0;
}

int usdGeom_ModelAPI_CreateModelCardTextureXNegAttr(usdGeom_ModelAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateModelCardTextureXNegAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_ModelAPI_GetModelCardTextureYPosAttr(usdGeom_ModelAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetModelCardTextureYPosAttr();
    return 0;
}

int usdGeom_ModelAPI_CreateModelCardTextureYPosAttr(usdGeom_ModelAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateModelCardTextureYPosAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_ModelAPI_GetModelCardTextureYNegAttr(usdGeom_ModelAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetModelCardTextureYNegAttr();
    return 0;
}

int usdGeom_ModelAPI_CreateModelCardTextureYNegAttr(usdGeom_ModelAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateModelCardTextureYNegAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_ModelAPI_GetModelCardTextureZPosAttr(usdGeom_ModelAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetModelCardTextureZPosAttr();
    return 0;
}

int usdGeom_ModelAPI_CreateModelCardTextureZPosAttr(usdGeom_ModelAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateModelCardTextureZPosAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_ModelAPI_GetModelCardTextureZNegAttr(usdGeom_ModelAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetModelCardTextureZNegAttr();
    return 0;
}

int usdGeom_ModelAPI_CreateModelCardTextureZNegAttr(usdGeom_ModelAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateModelCardTextureZNegAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_ModelAPI_ComputeModelDrawMode(usdGeom_ModelAPI_t const* _this, tf_Token_t const* parentDrawMode, tf_Token_t* _result) {
    *_result = _this->ComputeModelDrawMode(*parentDrawMode);
    return 0;
}

int usdGeom_ModelAPI_GetExtentsHint(usdGeom_ModelAPI_t const* _this, gf_Vec3fArray_t* extents, usd_TimeCode_t const* time, bool* _result) {
    *_result = _this->GetExtentsHint(extents, *time);
    return 0;
}

int usdGeom_ModelAPI_SetExtentsHint(usdGeom_ModelAPI_t const* _this, gf_Vec3fArray_t const* extents, usd_TimeCode_t const* time, bool* _result) {
    *_result = _this->SetExtentsHint(*extents, *time);
    return 0;
}

int usdGeom_ModelAPI_GetExtentsHintAttr(usdGeom_ModelAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentsHintAttr();
    return 0;
}

int usdGeom_ModelAPI_ComputeExtentsHint(usdGeom_ModelAPI_t const* _this, usdGeom_BBoxCache_t* bboxCache, gf_Vec3fArray_t* _result) {
    *_result = _this->ComputeExtentsHint(*bboxCache);
    return 0;
}

int usdGeom_ModelAPI_GetConstraintTarget(usdGeom_ModelAPI_t const* _this, std_String_t const* constraintName, usdGeom_ConstraintTarget_t* _result) {
    *_result = _this->GetConstraintTarget(*constraintName);
    return 0;
}

int usdGeom_ModelAPI_GetConstraintTargets(usdGeom_ModelAPI_t const* _this, usdGeom_ConstraintTargetVector_t* _result) {
    *_result = _this->GetConstraintTargets();
    return 0;
}

int usdGeom_ModelAPI_CreateConstraintTarget(usdGeom_ModelAPI_t const* _this, std_String_t const* constraintName, usdGeom_ConstraintTarget_t* _result) {
    *_result = _this->CreateConstraintTarget(*constraintName);
    return 0;
}

int usdGeom_ModelAPI_IsConcrete(usdGeom_ModelAPI_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_ModelAPI_IsTyped(usdGeom_ModelAPI_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_ModelAPI_IsAPISchema(usdGeom_ModelAPI_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_ModelAPI_IsAppliedAPISchema(usdGeom_ModelAPI_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_ModelAPI_IsMultipleApplyAPISchema(usdGeom_ModelAPI_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_ModelAPI_GetSchemaKind(usdGeom_ModelAPI_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_ModelAPI_GetPrim(usdGeom_ModelAPI_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_ModelAPI_GetPath(usdGeom_ModelAPI_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_ModelAPI_GetSchemaClassPrimDefinition(usdGeom_ModelAPI_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_ModelAPI_new(usd_Prim_t const* param00, usdGeom_ModelAPI_t** _result) {
    *_result = new PXR_NS::UsdGeomModelAPI(*param00);
    return 0;
}

int usdGeom_ModelAPI_dtor(usdGeom_ModelAPI_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_MotionAPI_GetMotionBlurScaleAttr(usdGeom_MotionAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetMotionBlurScaleAttr();
    return 0;
}

int usdGeom_MotionAPI_CreateMotionBlurScaleAttr(usdGeom_MotionAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateMotionBlurScaleAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_MotionAPI_GetVelocityScaleAttr(usdGeom_MotionAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVelocityScaleAttr();
    return 0;
}

int usdGeom_MotionAPI_CreateVelocityScaleAttr(usdGeom_MotionAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVelocityScaleAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_MotionAPI_GetNonlinearSampleCountAttr(usdGeom_MotionAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetNonlinearSampleCountAttr();
    return 0;
}

int usdGeom_MotionAPI_CreateNonlinearSampleCountAttr(usdGeom_MotionAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateNonlinearSampleCountAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_MotionAPI_ComputeVelocityScale(usdGeom_MotionAPI_t const* _this, usd_TimeCode_t time, float* _result) {
    *_result = _this->ComputeVelocityScale(time);
    return 0;
}

int usdGeom_MotionAPI_ComputeMotionBlurScale(usdGeom_MotionAPI_t const* _this, usd_TimeCode_t time, float* _result) {
    *_result = _this->ComputeMotionBlurScale(time);
    return 0;
}

int usdGeom_MotionAPI_ComputeNonlinearSampleCount(usdGeom_MotionAPI_t const* _this, usd_TimeCode_t time, int* _result) {
    *_result = _this->ComputeNonlinearSampleCount(time);
    return 0;
}

int usdGeom_MotionAPI_IsConcrete(usdGeom_MotionAPI_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_MotionAPI_IsTyped(usdGeom_MotionAPI_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_MotionAPI_IsAPISchema(usdGeom_MotionAPI_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_MotionAPI_IsAppliedAPISchema(usdGeom_MotionAPI_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_MotionAPI_IsMultipleApplyAPISchema(usdGeom_MotionAPI_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_MotionAPI_GetSchemaKind(usdGeom_MotionAPI_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_MotionAPI_GetPrim(usdGeom_MotionAPI_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_MotionAPI_GetPath(usdGeom_MotionAPI_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_MotionAPI_GetSchemaClassPrimDefinition(usdGeom_MotionAPI_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_MotionAPI_new(usd_Prim_t const* param00, usdGeom_MotionAPI_t** _result) {
    *_result = new PXR_NS::UsdGeomMotionAPI(*param00);
    return 0;
}

int usdGeom_MotionAPI_dtor(usdGeom_MotionAPI_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_NurbsCurves_GetOrderAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrderAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateOrderAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrderAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetKnotsAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetKnotsAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateKnotsAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateKnotsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetRangesAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetRangesAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateRangesAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateRangesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetPointWeightsAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPointWeightsAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreatePointWeightsAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePointWeightsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetCurveVertexCountsAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetCurveVertexCountsAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateCurveVertexCountsAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateCurveVertexCountsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetWidthsAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetWidthsAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateWidthsAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateWidthsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetWidthsInterpolation(usdGeom_NurbsCurves_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetWidthsInterpolation();
    return 0;
}

int usdGeom_NurbsCurves_SetWidthsInterpolation(usdGeom_NurbsCurves_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetWidthsInterpolation(*interpolation);
    return 0;
}

int usdGeom_NurbsCurves_GetCurveCount(usdGeom_NurbsCurves_t const* _this, usd_TimeCode_t timeCode, size_t* _result) {
    *_result = _this->GetCurveCount(timeCode);
    return 0;
}

int usdGeom_NurbsCurves_GetPointsAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPointsAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreatePointsAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePointsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetVelocitiesAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVelocitiesAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateVelocitiesAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVelocitiesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetAccelerationsAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAccelerationsAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateAccelerationsAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAccelerationsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetNormalsAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetNormalsAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateNormalsAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateNormalsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetNormalsInterpolation(usdGeom_NurbsCurves_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNormalsInterpolation();
    return 0;
}

int usdGeom_NurbsCurves_SetNormalsInterpolation(usdGeom_NurbsCurves_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetNormalsInterpolation(*interpolation);
    return 0;
}

int usdGeom_NurbsCurves_ComputePointsAtTime(usdGeom_NurbsCurves_t const* _this, gf_Vec3fArray_t* points, usd_TimeCode_t const time, usd_TimeCode_t const baseTime, bool* _result) {
    *_result = _this->ComputePointsAtTime(points, time, baseTime);
    return 0;
}

int usdGeom_NurbsCurves_GetDisplayColorAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateDisplayColorAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetDisplayOpacityAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateDisplayOpacityAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetDoubleSidedAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateDoubleSidedAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetOrientationAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateOrientationAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetDisplayColorPrimvar(usdGeom_NurbsCurves_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_NurbsCurves_CreateDisplayColorPrimvar(usdGeom_NurbsCurves_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_NurbsCurves_GetExtentAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateExtentAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetXformOpOrderAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_NurbsCurves_AddXformOp(usdGeom_NurbsCurves_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddTranslateOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddScaleOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddRotateXOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddRotateYOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddRotateZOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddRotateXYZOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddRotateXZYOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddRotateYXZOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddRotateYZXOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddRotateZXYOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddRotateZYXOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddOrientOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_AddTransformOp(usdGeom_NurbsCurves_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsCurves_SetResetXformStack(usdGeom_NurbsCurves_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_NurbsCurves_SetXformOpOrder(usdGeom_NurbsCurves_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_NurbsCurves_GetOrderedXformOps(usdGeom_NurbsCurves_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_NurbsCurves_ClearXformOpOrder(usdGeom_NurbsCurves_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_NurbsCurves_MakeMatrixXform(usdGeom_NurbsCurves_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_NurbsCurves_TransformMightBeTimeVarying(usdGeom_NurbsCurves_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_NurbsCurves_TransformMightBeTimeVarying_using(usdGeom_NurbsCurves_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_NurbsCurves_GetTimeSamples(usdGeom_NurbsCurves_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_NurbsCurves_GetTimeSamplesInInterval(usdGeom_NurbsCurves_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_NurbsCurves_GetLocalTransformation(usdGeom_NurbsCurves_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_NurbsCurves_GetLocalTransformation_with_ops(usdGeom_NurbsCurves_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_NurbsCurves_GetVisibilityAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreateVisibilityAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetPurposeAttr(usdGeom_NurbsCurves_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_NurbsCurves_CreatePurposeAttr(usdGeom_NurbsCurves_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsCurves_GetProxyPrimRel(usdGeom_NurbsCurves_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_NurbsCurves_CreateProxyPrimRel(usdGeom_NurbsCurves_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_NurbsCurves_MakeVisible(usdGeom_NurbsCurves_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_NurbsCurves_MakeInvisible(usdGeom_NurbsCurves_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_NurbsCurves_ComputeVisibility(usdGeom_NurbsCurves_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_NurbsCurves_GetPurposeVisibilityAttr(usdGeom_NurbsCurves_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_NurbsCurves_ComputePurposeInfo(usdGeom_NurbsCurves_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_NurbsCurves_ComputePurposeInfo_given_parent(usdGeom_NurbsCurves_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_NurbsCurves_ComputePurpose(usdGeom_NurbsCurves_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_NurbsCurves_ComputeProxyPrim(usdGeom_NurbsCurves_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_NurbsCurves_SetProxyPrim(usdGeom_NurbsCurves_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_NurbsCurves_ComputeWorldBound(usdGeom_NurbsCurves_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_NurbsCurves_ComputeLocalBound(usdGeom_NurbsCurves_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_NurbsCurves_ComputeUntransformedBound(usdGeom_NurbsCurves_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_NurbsCurves_ComputeLocalToWorldTransform(usdGeom_NurbsCurves_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_NurbsCurves_ComputeParentToWorldTransform(usdGeom_NurbsCurves_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_NurbsCurves_IsConcrete(usdGeom_NurbsCurves_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_NurbsCurves_IsTyped(usdGeom_NurbsCurves_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_NurbsCurves_IsAPISchema(usdGeom_NurbsCurves_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_NurbsCurves_IsAppliedAPISchema(usdGeom_NurbsCurves_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_NurbsCurves_IsMultipleApplyAPISchema(usdGeom_NurbsCurves_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_NurbsCurves_GetSchemaKind(usdGeom_NurbsCurves_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_NurbsCurves_GetPrim(usdGeom_NurbsCurves_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_NurbsCurves_GetPath(usdGeom_NurbsCurves_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_NurbsCurves_GetSchemaClassPrimDefinition(usdGeom_NurbsCurves_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_NurbsCurves_new(usd_Prim_t const* param00, usdGeom_NurbsCurves_t** _result) {
    *_result = new PXR_NS::UsdGeomNurbsCurves(*param00);
    return 0;
}

int usdGeom_NurbsCurves_dtor(usdGeom_NurbsCurves_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_NurbsPatch_GetUVertexCountAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetUVertexCountAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateUVertexCountAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateUVertexCountAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetVVertexCountAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVVertexCountAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateVVertexCountAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVVertexCountAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetUOrderAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetUOrderAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateUOrderAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateUOrderAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetVOrderAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVOrderAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateVOrderAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVOrderAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetUKnotsAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetUKnotsAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateUKnotsAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateUKnotsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetVKnotsAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVKnotsAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateVKnotsAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVKnotsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetUFormAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetUFormAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateUFormAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateUFormAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetVFormAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVFormAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateVFormAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVFormAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetURangeAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetURangeAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateURangeAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateURangeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetVRangeAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVRangeAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateVRangeAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVRangeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetPointWeightsAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPointWeightsAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreatePointWeightsAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePointWeightsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetTrimCurveCountsAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetTrimCurveCountsAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateTrimCurveCountsAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateTrimCurveCountsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetTrimCurveOrdersAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetTrimCurveOrdersAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateTrimCurveOrdersAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateTrimCurveOrdersAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetTrimCurveVertexCountsAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetTrimCurveVertexCountsAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateTrimCurveVertexCountsAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateTrimCurveVertexCountsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetTrimCurveKnotsAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetTrimCurveKnotsAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateTrimCurveKnotsAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateTrimCurveKnotsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetTrimCurveRangesAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetTrimCurveRangesAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateTrimCurveRangesAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateTrimCurveRangesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetTrimCurvePointsAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetTrimCurvePointsAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateTrimCurvePointsAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateTrimCurvePointsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetPointsAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPointsAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreatePointsAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePointsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetVelocitiesAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVelocitiesAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateVelocitiesAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVelocitiesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetAccelerationsAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAccelerationsAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateAccelerationsAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAccelerationsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetNormalsAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetNormalsAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateNormalsAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateNormalsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetNormalsInterpolation(usdGeom_NurbsPatch_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNormalsInterpolation();
    return 0;
}

int usdGeom_NurbsPatch_SetNormalsInterpolation(usdGeom_NurbsPatch_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetNormalsInterpolation(*interpolation);
    return 0;
}

int usdGeom_NurbsPatch_ComputePointsAtTime(usdGeom_NurbsPatch_t const* _this, gf_Vec3fArray_t* points, usd_TimeCode_t const time, usd_TimeCode_t const baseTime, bool* _result) {
    *_result = _this->ComputePointsAtTime(points, time, baseTime);
    return 0;
}

int usdGeom_NurbsPatch_GetDisplayColorAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateDisplayColorAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetDisplayOpacityAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateDisplayOpacityAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetDoubleSidedAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateDoubleSidedAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetOrientationAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateOrientationAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetDisplayColorPrimvar(usdGeom_NurbsPatch_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_NurbsPatch_CreateDisplayColorPrimvar(usdGeom_NurbsPatch_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_NurbsPatch_GetExtentAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateExtentAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetXformOpOrderAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_NurbsPatch_AddXformOp(usdGeom_NurbsPatch_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddTranslateOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddScaleOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddRotateXOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddRotateYOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddRotateZOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddRotateXYZOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddRotateXZYOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddRotateYXZOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddRotateYZXOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddRotateZXYOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddRotateZYXOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddOrientOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_AddTransformOp(usdGeom_NurbsPatch_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_NurbsPatch_SetResetXformStack(usdGeom_NurbsPatch_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_NurbsPatch_SetXformOpOrder(usdGeom_NurbsPatch_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_NurbsPatch_GetOrderedXformOps(usdGeom_NurbsPatch_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_NurbsPatch_ClearXformOpOrder(usdGeom_NurbsPatch_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_NurbsPatch_MakeMatrixXform(usdGeom_NurbsPatch_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_NurbsPatch_TransformMightBeTimeVarying(usdGeom_NurbsPatch_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_NurbsPatch_TransformMightBeTimeVarying_using(usdGeom_NurbsPatch_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_NurbsPatch_GetTimeSamples(usdGeom_NurbsPatch_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_NurbsPatch_GetTimeSamplesInInterval(usdGeom_NurbsPatch_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_NurbsPatch_GetLocalTransformation(usdGeom_NurbsPatch_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_NurbsPatch_GetLocalTransformation_with_ops(usdGeom_NurbsPatch_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_NurbsPatch_GetVisibilityAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreateVisibilityAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetPurposeAttr(usdGeom_NurbsPatch_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_NurbsPatch_CreatePurposeAttr(usdGeom_NurbsPatch_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_NurbsPatch_GetProxyPrimRel(usdGeom_NurbsPatch_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_NurbsPatch_CreateProxyPrimRel(usdGeom_NurbsPatch_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_NurbsPatch_MakeVisible(usdGeom_NurbsPatch_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_NurbsPatch_MakeInvisible(usdGeom_NurbsPatch_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_NurbsPatch_ComputeVisibility(usdGeom_NurbsPatch_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_NurbsPatch_GetPurposeVisibilityAttr(usdGeom_NurbsPatch_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_NurbsPatch_ComputePurposeInfo(usdGeom_NurbsPatch_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_NurbsPatch_ComputePurposeInfo_given_parent(usdGeom_NurbsPatch_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_NurbsPatch_ComputePurpose(usdGeom_NurbsPatch_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_NurbsPatch_ComputeProxyPrim(usdGeom_NurbsPatch_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_NurbsPatch_SetProxyPrim(usdGeom_NurbsPatch_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_NurbsPatch_ComputeWorldBound(usdGeom_NurbsPatch_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_NurbsPatch_ComputeLocalBound(usdGeom_NurbsPatch_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_NurbsPatch_ComputeUntransformedBound(usdGeom_NurbsPatch_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_NurbsPatch_ComputeLocalToWorldTransform(usdGeom_NurbsPatch_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_NurbsPatch_ComputeParentToWorldTransform(usdGeom_NurbsPatch_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_NurbsPatch_IsConcrete(usdGeom_NurbsPatch_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_NurbsPatch_IsTyped(usdGeom_NurbsPatch_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_NurbsPatch_IsAPISchema(usdGeom_NurbsPatch_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_NurbsPatch_IsAppliedAPISchema(usdGeom_NurbsPatch_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_NurbsPatch_IsMultipleApplyAPISchema(usdGeom_NurbsPatch_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_NurbsPatch_GetSchemaKind(usdGeom_NurbsPatch_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_NurbsPatch_GetPrim(usdGeom_NurbsPatch_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_NurbsPatch_GetPath(usdGeom_NurbsPatch_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_NurbsPatch_GetSchemaClassPrimDefinition(usdGeom_NurbsPatch_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_NurbsPatch_new(usd_Prim_t const* param00, usdGeom_NurbsPatch_t** _result) {
    *_result = new PXR_NS::UsdGeomNurbsPatch(*param00);
    return 0;
}

int usdGeom_NurbsPatch_dtor(usdGeom_NurbsPatch_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Plane_GetWidthAttr(usdGeom_Plane_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetWidthAttr();
    return 0;
}

int usdGeom_Plane_CreateWidthAttr(usdGeom_Plane_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateWidthAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Plane_GetLengthAttr(usdGeom_Plane_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetLengthAttr();
    return 0;
}

int usdGeom_Plane_CreateLengthAttr(usdGeom_Plane_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateLengthAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Plane_GetAxisAttr(usdGeom_Plane_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAxisAttr();
    return 0;
}

int usdGeom_Plane_CreateAxisAttr(usdGeom_Plane_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAxisAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Plane_GetDisplayColorAttr(usdGeom_Plane_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_Plane_CreateDisplayColorAttr(usdGeom_Plane_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Plane_GetDisplayOpacityAttr(usdGeom_Plane_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_Plane_CreateDisplayOpacityAttr(usdGeom_Plane_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Plane_GetDoubleSidedAttr(usdGeom_Plane_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_Plane_CreateDoubleSidedAttr(usdGeom_Plane_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Plane_GetOrientationAttr(usdGeom_Plane_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_Plane_CreateOrientationAttr(usdGeom_Plane_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Plane_GetDisplayColorPrimvar(usdGeom_Plane_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_Plane_CreateDisplayColorPrimvar(usdGeom_Plane_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_Plane_GetExtentAttr(usdGeom_Plane_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_Plane_CreateExtentAttr(usdGeom_Plane_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Plane_GetXformOpOrderAttr(usdGeom_Plane_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Plane_AddXformOp(usdGeom_Plane_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddTranslateOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddScaleOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddRotateXOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddRotateYOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddRotateZOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddRotateXYZOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddRotateXZYOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddRotateYXZOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddRotateYZXOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddRotateZXYOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddRotateZYXOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddOrientOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_AddTransformOp(usdGeom_Plane_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Plane_SetResetXformStack(usdGeom_Plane_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Plane_SetXformOpOrder(usdGeom_Plane_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Plane_GetOrderedXformOps(usdGeom_Plane_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Plane_ClearXformOpOrder(usdGeom_Plane_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Plane_MakeMatrixXform(usdGeom_Plane_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Plane_TransformMightBeTimeVarying(usdGeom_Plane_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Plane_TransformMightBeTimeVarying_using(usdGeom_Plane_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Plane_GetTimeSamples(usdGeom_Plane_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Plane_GetTimeSamplesInInterval(usdGeom_Plane_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Plane_GetLocalTransformation(usdGeom_Plane_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Plane_GetLocalTransformation_with_ops(usdGeom_Plane_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Plane_GetVisibilityAttr(usdGeom_Plane_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Plane_CreateVisibilityAttr(usdGeom_Plane_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Plane_GetPurposeAttr(usdGeom_Plane_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Plane_CreatePurposeAttr(usdGeom_Plane_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Plane_GetProxyPrimRel(usdGeom_Plane_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Plane_CreateProxyPrimRel(usdGeom_Plane_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Plane_MakeVisible(usdGeom_Plane_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Plane_MakeInvisible(usdGeom_Plane_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Plane_ComputeVisibility(usdGeom_Plane_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Plane_GetPurposeVisibilityAttr(usdGeom_Plane_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Plane_ComputePurposeInfo(usdGeom_Plane_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Plane_ComputePurposeInfo_given_parent(usdGeom_Plane_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Plane_ComputePurpose(usdGeom_Plane_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Plane_ComputeProxyPrim(usdGeom_Plane_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Plane_SetProxyPrim(usdGeom_Plane_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Plane_ComputeWorldBound(usdGeom_Plane_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Plane_ComputeLocalBound(usdGeom_Plane_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Plane_ComputeUntransformedBound(usdGeom_Plane_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Plane_ComputeLocalToWorldTransform(usdGeom_Plane_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Plane_ComputeParentToWorldTransform(usdGeom_Plane_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Plane_IsConcrete(usdGeom_Plane_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Plane_IsTyped(usdGeom_Plane_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Plane_IsAPISchema(usdGeom_Plane_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Plane_IsAppliedAPISchema(usdGeom_Plane_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Plane_IsMultipleApplyAPISchema(usdGeom_Plane_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Plane_GetSchemaKind(usdGeom_Plane_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Plane_GetPrim(usdGeom_Plane_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Plane_GetPath(usdGeom_Plane_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Plane_GetSchemaClassPrimDefinition(usdGeom_Plane_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Plane_new(usd_Prim_t const* param00, usdGeom_Plane_t** _result) {
    *_result = new PXR_NS::UsdGeomPlane(*param00);
    return 0;
}

int usdGeom_Plane_dtor(usdGeom_Plane_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_PointBased_GetPointsAttr(usdGeom_PointBased_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPointsAttr();
    return 0;
}

int usdGeom_PointBased_CreatePointsAttr(usdGeom_PointBased_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePointsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointBased_GetVelocitiesAttr(usdGeom_PointBased_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVelocitiesAttr();
    return 0;
}

int usdGeom_PointBased_CreateVelocitiesAttr(usdGeom_PointBased_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVelocitiesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointBased_GetAccelerationsAttr(usdGeom_PointBased_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAccelerationsAttr();
    return 0;
}

int usdGeom_PointBased_CreateAccelerationsAttr(usdGeom_PointBased_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAccelerationsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointBased_GetNormalsAttr(usdGeom_PointBased_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetNormalsAttr();
    return 0;
}

int usdGeom_PointBased_CreateNormalsAttr(usdGeom_PointBased_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateNormalsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointBased_GetNormalsInterpolation(usdGeom_PointBased_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNormalsInterpolation();
    return 0;
}

int usdGeom_PointBased_SetNormalsInterpolation(usdGeom_PointBased_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetNormalsInterpolation(*interpolation);
    return 0;
}

int usdGeom_PointBased_ComputePointsAtTime(usdGeom_PointBased_t const* _this, gf_Vec3fArray_t* points, usd_TimeCode_t const time, usd_TimeCode_t const baseTime, bool* _result) {
    *_result = _this->ComputePointsAtTime(points, time, baseTime);
    return 0;
}

int usdGeom_PointBased_GetDisplayColorAttr(usdGeom_PointBased_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_PointBased_CreateDisplayColorAttr(usdGeom_PointBased_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointBased_GetDisplayOpacityAttr(usdGeom_PointBased_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_PointBased_CreateDisplayOpacityAttr(usdGeom_PointBased_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointBased_GetDoubleSidedAttr(usdGeom_PointBased_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_PointBased_CreateDoubleSidedAttr(usdGeom_PointBased_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointBased_GetOrientationAttr(usdGeom_PointBased_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_PointBased_CreateOrientationAttr(usdGeom_PointBased_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointBased_GetDisplayColorPrimvar(usdGeom_PointBased_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_PointBased_CreateDisplayColorPrimvar(usdGeom_PointBased_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_PointBased_GetExtentAttr(usdGeom_PointBased_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_PointBased_CreateExtentAttr(usdGeom_PointBased_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointBased_GetXformOpOrderAttr(usdGeom_PointBased_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_PointBased_AddXformOp(usdGeom_PointBased_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddTranslateOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddScaleOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddRotateXOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddRotateYOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddRotateZOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddRotateXYZOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddRotateXZYOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddRotateYXZOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddRotateYZXOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddRotateZXYOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddRotateZYXOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddOrientOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_AddTransformOp(usdGeom_PointBased_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointBased_SetResetXformStack(usdGeom_PointBased_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_PointBased_SetXformOpOrder(usdGeom_PointBased_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_PointBased_GetOrderedXformOps(usdGeom_PointBased_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_PointBased_ClearXformOpOrder(usdGeom_PointBased_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_PointBased_MakeMatrixXform(usdGeom_PointBased_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_PointBased_TransformMightBeTimeVarying(usdGeom_PointBased_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_PointBased_TransformMightBeTimeVarying_using(usdGeom_PointBased_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_PointBased_GetTimeSamples(usdGeom_PointBased_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_PointBased_GetTimeSamplesInInterval(usdGeom_PointBased_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_PointBased_GetLocalTransformation(usdGeom_PointBased_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_PointBased_GetLocalTransformation_with_ops(usdGeom_PointBased_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_PointBased_GetVisibilityAttr(usdGeom_PointBased_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_PointBased_CreateVisibilityAttr(usdGeom_PointBased_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointBased_GetPurposeAttr(usdGeom_PointBased_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_PointBased_CreatePurposeAttr(usdGeom_PointBased_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointBased_GetProxyPrimRel(usdGeom_PointBased_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_PointBased_CreateProxyPrimRel(usdGeom_PointBased_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_PointBased_MakeVisible(usdGeom_PointBased_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_PointBased_MakeInvisible(usdGeom_PointBased_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_PointBased_ComputeVisibility(usdGeom_PointBased_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_PointBased_GetPurposeVisibilityAttr(usdGeom_PointBased_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_PointBased_ComputePurposeInfo(usdGeom_PointBased_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_PointBased_ComputePurposeInfo_given_parent(usdGeom_PointBased_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_PointBased_ComputePurpose(usdGeom_PointBased_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_PointBased_ComputeProxyPrim(usdGeom_PointBased_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_PointBased_SetProxyPrim(usdGeom_PointBased_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_PointBased_ComputeWorldBound(usdGeom_PointBased_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_PointBased_ComputeLocalBound(usdGeom_PointBased_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_PointBased_ComputeUntransformedBound(usdGeom_PointBased_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_PointBased_ComputeLocalToWorldTransform(usdGeom_PointBased_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_PointBased_ComputeParentToWorldTransform(usdGeom_PointBased_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_PointBased_IsConcrete(usdGeom_PointBased_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_PointBased_IsTyped(usdGeom_PointBased_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_PointBased_IsAPISchema(usdGeom_PointBased_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_PointBased_IsAppliedAPISchema(usdGeom_PointBased_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_PointBased_IsMultipleApplyAPISchema(usdGeom_PointBased_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_PointBased_GetSchemaKind(usdGeom_PointBased_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_PointBased_GetPrim(usdGeom_PointBased_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_PointBased_GetPath(usdGeom_PointBased_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_PointBased_GetSchemaClassPrimDefinition(usdGeom_PointBased_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_PointBased_new(usd_Prim_t const* param00, usdGeom_PointBased_t** _result) {
    *_result = new PXR_NS::UsdGeomPointBased(*param00);
    return 0;
}

int usdGeom_PointBased_dtor(usdGeom_PointBased_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_PointInstancer_GetProtoIndicesAttr(usdGeom_PointInstancer_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetProtoIndicesAttr();
    return 0;
}

int usdGeom_PointInstancer_CreateProtoIndicesAttr(usdGeom_PointInstancer_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateProtoIndicesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointInstancer_GetIdsAttr(usdGeom_PointInstancer_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetIdsAttr();
    return 0;
}

int usdGeom_PointInstancer_CreateIdsAttr(usdGeom_PointInstancer_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateIdsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointInstancer_GetPositionsAttr(usdGeom_PointInstancer_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPositionsAttr();
    return 0;
}

int usdGeom_PointInstancer_CreatePositionsAttr(usdGeom_PointInstancer_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePositionsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointInstancer_CreateOrientationsAttr(usdGeom_PointInstancer_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointInstancer_GetOrientationsAttr(usdGeom_PointInstancer_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationsAttr();
    return 0;
}

int usdGeom_PointInstancer_GetScalesAttr(usdGeom_PointInstancer_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetScalesAttr();
    return 0;
}

int usdGeom_PointInstancer_CreateScalesAttr(usdGeom_PointInstancer_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateScalesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointInstancer_GetVelocitiesAttr(usdGeom_PointInstancer_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVelocitiesAttr();
    return 0;
}

int usdGeom_PointInstancer_CreateVelocitiesAttr(usdGeom_PointInstancer_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVelocitiesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointInstancer_GetAccelerationsAttr(usdGeom_PointInstancer_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAccelerationsAttr();
    return 0;
}

int usdGeom_PointInstancer_CreateAccelerationsAttr(usdGeom_PointInstancer_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAccelerationsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointInstancer_GetAngularVelocitiesAttr(usdGeom_PointInstancer_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAngularVelocitiesAttr();
    return 0;
}

int usdGeom_PointInstancer_CreateAngularVelocitiesAttr(usdGeom_PointInstancer_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAngularVelocitiesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointInstancer_GetPrototypesRel(usdGeom_PointInstancer_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetPrototypesRel();
    return 0;
}

int usdGeom_PointInstancer_CreatePrototypesRel(usdGeom_PointInstancer_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreatePrototypesRel();
    return 0;
}

int usdGeom_PointInstancer_ComputeInstanceTransformsAtTime(usdGeom_PointInstancer_t const* _this, gf_Matrix4dArray_t* xforms, usd_TimeCode_t const time, usd_TimeCode_t const baseTime, int doProtoXforms, int applyMask, bool* _result) {
    *_result = _this->ComputeInstanceTransformsAtTime(xforms, time, baseTime, static_cast<PXR_NS::UsdGeomPointInstancer::ProtoXformInclusion>(doProtoXforms), static_cast<PXR_NS::UsdGeomPointInstancer::MaskApplication>(applyMask));
    return 0;
}

int usdGeom_PointInstancer_ComputeInstanceTransformsAtTimes(usdGeom_PointInstancer_t const* _this, gf_Matrix4dArrayVector_t* xformsArray, usd_TimeCodeVector_t const* times, usd_TimeCode_t const baseTime, int doProtoXforms, int applyMask, bool* _result) {
    *_result = _this->ComputeInstanceTransformsAtTimes(xformsArray, *times, baseTime, static_cast<PXR_NS::UsdGeomPointInstancer::ProtoXformInclusion>(doProtoXforms), static_cast<PXR_NS::UsdGeomPointInstancer::MaskApplication>(applyMask));
    return 0;
}

int usdGeom_PointInstancer_ComputeExtentAtTime(usdGeom_PointInstancer_t const* _this, gf_Vec3fArray_t* extent, usd_TimeCode_t const time, usd_TimeCode_t const baseTime, bool* _result) {
    *_result = _this->ComputeExtentAtTime(extent, time, baseTime);
    return 0;
}

int usdGeom_PointInstancer_ComputeExtentAtTime_with_transform(usdGeom_PointInstancer_t const* _this, gf_Vec3fArray_t* extent, usd_TimeCode_t const time, usd_TimeCode_t const baseTime, gf_Matrix4d_t const* transform, bool* _result) {
    *_result = _this->ComputeExtentAtTime(extent, time, baseTime, *transform);
    return 0;
}

int usdGeom_PointInstancer_ComputeExtentAtTimes(usdGeom_PointInstancer_t const* _this, gf_Vec3fArrayVector_t* extents, usd_TimeCodeVector_t const* times, usd_TimeCode_t const baseTime, bool* _result) {
    *_result = _this->ComputeExtentAtTimes(extents, *times, baseTime);
    return 0;
}

int usdGeom_PointInstancer_ComputeExtentAtTimes_with_transform(usdGeom_PointInstancer_t const* _this, gf_Vec3fArrayVector_t* extents, usd_TimeCodeVector_t const* times, usd_TimeCode_t const baseTime, gf_Matrix4d_t const* transform, bool* _result) {
    *_result = _this->ComputeExtentAtTimes(extents, *times, baseTime, *transform);
    return 0;
}

int usdGeom_PointInstancer_GetInstanceCount(usdGeom_PointInstancer_t const* _this, usd_TimeCode_t timeCode, size_t* _result) {
    *_result = _this->GetInstanceCount(timeCode);
    return 0;
}

int usdGeom_PointInstancer_GetExtentAttr(usdGeom_PointInstancer_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_PointInstancer_CreateExtentAttr(usdGeom_PointInstancer_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointInstancer_GetXformOpOrderAttr(usdGeom_PointInstancer_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_PointInstancer_AddXformOp(usdGeom_PointInstancer_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddTranslateOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddScaleOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddRotateXOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddRotateYOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddRotateZOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddRotateXYZOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddRotateXZYOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddRotateYXZOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddRotateYZXOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddRotateZXYOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddRotateZYXOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddOrientOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_AddTransformOp(usdGeom_PointInstancer_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_PointInstancer_SetResetXformStack(usdGeom_PointInstancer_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_PointInstancer_SetXformOpOrder(usdGeom_PointInstancer_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_PointInstancer_GetOrderedXformOps(usdGeom_PointInstancer_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_PointInstancer_ClearXformOpOrder(usdGeom_PointInstancer_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_PointInstancer_MakeMatrixXform(usdGeom_PointInstancer_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_PointInstancer_TransformMightBeTimeVarying(usdGeom_PointInstancer_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_PointInstancer_TransformMightBeTimeVarying_using(usdGeom_PointInstancer_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_PointInstancer_GetTimeSamples(usdGeom_PointInstancer_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_PointInstancer_GetTimeSamplesInInterval(usdGeom_PointInstancer_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_PointInstancer_GetLocalTransformation(usdGeom_PointInstancer_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_PointInstancer_GetLocalTransformation_with_ops(usdGeom_PointInstancer_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_PointInstancer_GetVisibilityAttr(usdGeom_PointInstancer_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_PointInstancer_CreateVisibilityAttr(usdGeom_PointInstancer_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointInstancer_GetPurposeAttr(usdGeom_PointInstancer_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_PointInstancer_CreatePurposeAttr(usdGeom_PointInstancer_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_PointInstancer_GetProxyPrimRel(usdGeom_PointInstancer_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_PointInstancer_CreateProxyPrimRel(usdGeom_PointInstancer_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_PointInstancer_MakeVisible(usdGeom_PointInstancer_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_PointInstancer_MakeInvisible(usdGeom_PointInstancer_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_PointInstancer_ComputeVisibility(usdGeom_PointInstancer_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_PointInstancer_GetPurposeVisibilityAttr(usdGeom_PointInstancer_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_PointInstancer_ComputePurposeInfo(usdGeom_PointInstancer_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_PointInstancer_ComputePurposeInfo_given_parent(usdGeom_PointInstancer_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_PointInstancer_ComputePurpose(usdGeom_PointInstancer_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_PointInstancer_ComputeProxyPrim(usdGeom_PointInstancer_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_PointInstancer_SetProxyPrim(usdGeom_PointInstancer_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_PointInstancer_ComputeWorldBound(usdGeom_PointInstancer_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_PointInstancer_ComputeLocalBound(usdGeom_PointInstancer_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_PointInstancer_ComputeUntransformedBound(usdGeom_PointInstancer_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_PointInstancer_ComputeLocalToWorldTransform(usdGeom_PointInstancer_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_PointInstancer_ComputeParentToWorldTransform(usdGeom_PointInstancer_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_PointInstancer_IsConcrete(usdGeom_PointInstancer_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_PointInstancer_IsTyped(usdGeom_PointInstancer_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_PointInstancer_IsAPISchema(usdGeom_PointInstancer_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_PointInstancer_IsAppliedAPISchema(usdGeom_PointInstancer_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_PointInstancer_IsMultipleApplyAPISchema(usdGeom_PointInstancer_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_PointInstancer_GetSchemaKind(usdGeom_PointInstancer_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_PointInstancer_GetPrim(usdGeom_PointInstancer_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_PointInstancer_GetPath(usdGeom_PointInstancer_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_PointInstancer_GetSchemaClassPrimDefinition(usdGeom_PointInstancer_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_PointInstancer_new(usd_Prim_t const* param00, usdGeom_PointInstancer_t** _result) {
    *_result = new PXR_NS::UsdGeomPointInstancer(*param00);
    return 0;
}

int usdGeom_PointInstancer_dtor(usdGeom_PointInstancer_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Points_GetWidthsAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetWidthsAttr();
    return 0;
}

int usdGeom_Points_CreateWidthsAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateWidthsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetIdsAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetIdsAttr();
    return 0;
}

int usdGeom_Points_CreateIdsAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateIdsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetWidthsInterpolation(usdGeom_Points_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetWidthsInterpolation();
    return 0;
}

int usdGeom_Points_SetWidthsInterpolation(usdGeom_Points_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetWidthsInterpolation(*interpolation);
    return 0;
}

int usdGeom_Points_GetPointCount(usdGeom_Points_t const* _this, usd_TimeCode_t timeCode, size_t* _result) {
    *_result = _this->GetPointCount(timeCode);
    return 0;
}

int usdGeom_Points_GetPointsAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPointsAttr();
    return 0;
}

int usdGeom_Points_CreatePointsAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePointsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetVelocitiesAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVelocitiesAttr();
    return 0;
}

int usdGeom_Points_CreateVelocitiesAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVelocitiesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetAccelerationsAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetAccelerationsAttr();
    return 0;
}

int usdGeom_Points_CreateAccelerationsAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateAccelerationsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetNormalsAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetNormalsAttr();
    return 0;
}

int usdGeom_Points_CreateNormalsAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateNormalsAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetNormalsInterpolation(usdGeom_Points_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNormalsInterpolation();
    return 0;
}

int usdGeom_Points_SetNormalsInterpolation(usdGeom_Points_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetNormalsInterpolation(*interpolation);
    return 0;
}

int usdGeom_Points_ComputePointsAtTime(usdGeom_Points_t const* _this, gf_Vec3fArray_t* points, usd_TimeCode_t const time, usd_TimeCode_t const baseTime, bool* _result) {
    *_result = _this->ComputePointsAtTime(points, time, baseTime);
    return 0;
}

int usdGeom_Points_GetDisplayColorAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_Points_CreateDisplayColorAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetDisplayOpacityAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_Points_CreateDisplayOpacityAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetDoubleSidedAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_Points_CreateDoubleSidedAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetOrientationAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_Points_CreateOrientationAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetDisplayColorPrimvar(usdGeom_Points_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_Points_CreateDisplayColorPrimvar(usdGeom_Points_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_Points_GetExtentAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_Points_CreateExtentAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetXformOpOrderAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Points_AddXformOp(usdGeom_Points_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddTranslateOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddScaleOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddRotateXOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddRotateYOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddRotateZOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddRotateXYZOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddRotateXZYOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddRotateYXZOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddRotateYZXOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddRotateZXYOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddRotateZYXOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddOrientOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_AddTransformOp(usdGeom_Points_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Points_SetResetXformStack(usdGeom_Points_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Points_SetXformOpOrder(usdGeom_Points_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Points_GetOrderedXformOps(usdGeom_Points_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Points_ClearXformOpOrder(usdGeom_Points_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Points_MakeMatrixXform(usdGeom_Points_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Points_TransformMightBeTimeVarying(usdGeom_Points_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Points_TransformMightBeTimeVarying_using(usdGeom_Points_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Points_GetTimeSamples(usdGeom_Points_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Points_GetTimeSamplesInInterval(usdGeom_Points_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Points_GetLocalTransformation(usdGeom_Points_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Points_GetLocalTransformation_with_ops(usdGeom_Points_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Points_GetVisibilityAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Points_CreateVisibilityAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetPurposeAttr(usdGeom_Points_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Points_CreatePurposeAttr(usdGeom_Points_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Points_GetProxyPrimRel(usdGeom_Points_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Points_CreateProxyPrimRel(usdGeom_Points_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Points_MakeVisible(usdGeom_Points_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Points_MakeInvisible(usdGeom_Points_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Points_ComputeVisibility(usdGeom_Points_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Points_GetPurposeVisibilityAttr(usdGeom_Points_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Points_ComputePurposeInfo(usdGeom_Points_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Points_ComputePurposeInfo_given_parent(usdGeom_Points_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Points_ComputePurpose(usdGeom_Points_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Points_ComputeProxyPrim(usdGeom_Points_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Points_SetProxyPrim(usdGeom_Points_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Points_ComputeWorldBound(usdGeom_Points_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Points_ComputeLocalBound(usdGeom_Points_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Points_ComputeUntransformedBound(usdGeom_Points_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Points_ComputeLocalToWorldTransform(usdGeom_Points_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Points_ComputeParentToWorldTransform(usdGeom_Points_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Points_IsConcrete(usdGeom_Points_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Points_IsTyped(usdGeom_Points_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Points_IsAPISchema(usdGeom_Points_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Points_IsAppliedAPISchema(usdGeom_Points_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Points_IsMultipleApplyAPISchema(usdGeom_Points_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Points_GetSchemaKind(usdGeom_Points_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Points_GetPrim(usdGeom_Points_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Points_GetPath(usdGeom_Points_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Points_GetSchemaClassPrimDefinition(usdGeom_Points_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Points_new(usd_Prim_t const* param00, usdGeom_Points_t** _result) {
    *_result = new PXR_NS::UsdGeomPoints(*param00);
    return 0;
}

int usdGeom_Points_dtor(usdGeom_Points_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Primvar_GetInterpolation(usdGeom_Primvar_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetInterpolation();
    return 0;
}

int usdGeom_Primvar_SetInterpolation(usdGeom_Primvar_t* _this, tf_Token_t const* interpolation, bool* _result) {
    *_result = _this->SetInterpolation(*interpolation);
    return 0;
}

int usdGeom_Primvar_HasAuthoredInterpolation(usdGeom_Primvar_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredInterpolation();
    return 0;
}

int usdGeom_Primvar_GetElementSize(usdGeom_Primvar_t const* _this, int* _result) {
    *_result = _this->GetElementSize();
    return 0;
}

int usdGeom_Primvar_SetElementSize(usdGeom_Primvar_t* _this, int eltSize, bool* _result) {
    *_result = _this->SetElementSize(eltSize);
    return 0;
}

int usdGeom_Primvar_HasAuthoredElementSize(usdGeom_Primvar_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredElementSize();
    return 0;
}

int usdGeom_Primvar_GetDeclarationInfo(usdGeom_Primvar_t const* _this, tf_Token_t* name, sdf_ValueTypeName_t* typeName, tf_Token_t* interpolation, int* elementSize) {
    _this->GetDeclarationInfo(name, typeName, interpolation, elementSize);
    return 0;
}

int usdGeom_Primvar_Get(usdGeom_Primvar_t const* _this, vt_Value_t* value, usd_TimeCode_t time, bool* _result) {
    *_result = _this->Get(value, time);
    return 0;
}

int usdGeom_Primvar_GetAttr(usdGeom_Primvar_t const* _this, usd_Attribute_t const** _result) {
    *_result = &_this->GetAttr();
    return 0;
}

int usdGeom_Primvar_IsDefined(usdGeom_Primvar_t const* _this, bool* _result) {
    *_result = _this->IsDefined();
    return 0;
}

int usdGeom_Primvar_HasValue(usdGeom_Primvar_t const* _this, bool* _result) {
    *_result = _this->HasValue();
    return 0;
}

int usdGeom_Primvar_HasAuthoredValue(usdGeom_Primvar_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredValue();
    return 0;
}

int usdGeom_Primvar_GetPrimvarName(usdGeom_Primvar_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetPrimvarName();
    return 0;
}

int usdGeom_Primvar_GetName(usdGeom_Primvar_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int usdGeom_Primvar_NameContainsNamespaces(usdGeom_Primvar_t const* _this, bool* _result) {
    *_result = _this->NameContainsNamespaces();
    return 0;
}

int usdGeom_Primvar_GetBaseName(usdGeom_Primvar_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetBaseName();
    return 0;
}

int usdGeom_Primvar_GetNamespace(usdGeom_Primvar_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNamespace();
    return 0;
}

int usdGeom_Primvar_GetTypeName(usdGeom_Primvar_t const* _this, sdf_ValueTypeName_t* _result) {
    *_result = _this->GetTypeName();
    return 0;
}

int usdGeom_Primvar_SplitName(usdGeom_Primvar_t const* _this, std_StringVector_t* _result) {
    *_result = _this->SplitName();
    return 0;
}

int usdGeom_Primvar_GetTimeSamples(usdGeom_Primvar_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Primvar_GetTimeSamplesInInterval(usdGeom_Primvar_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Primvar_ValueMightBeTimeVarying(usdGeom_Primvar_t const* _this, bool* _result) {
    *_result = _this->ValueMightBeTimeVarying();
    return 0;
}

int usdGeom_Primvar_IsIdTarget(usdGeom_Primvar_t const* _this, bool* _result) {
    *_result = _this->IsIdTarget();
    return 0;
}

int usdGeom_Primvar_SetIdTarget(usdGeom_Primvar_t const* _this, sdf_Path_t const* path, bool* _result) {
    *_result = _this->SetIdTarget(*path);
    return 0;
}

int usdGeom_Primvar_IsPrimvar(usd_Attribute_t const* attr, bool* _result) {
    *_result = PXR_NS::UsdGeomPrimvar::IsPrimvar(*attr);
    return 0;
}

int usdGeom_Primvar_IsValidPrimvarName(tf_Token_t const* name, bool* _result) {
    *_result = PXR_NS::UsdGeomPrimvar::IsValidPrimvarName(*name);
    return 0;
}

int usdGeom_Primvar_StripPrimvarsName(tf_Token_t const* name, tf_Token_t* _result) {
    *_result = PXR_NS::UsdGeomPrimvar::StripPrimvarsName(*name);
    return 0;
}

int usdGeom_Primvar_IsValidInterpolation(tf_Token_t const* interpolation, bool* _result) {
    *_result = PXR_NS::UsdGeomPrimvar::IsValidInterpolation(*interpolation);
    return 0;
}

int usdGeom_Primvar_new(usd_Attribute_t const* param00, usdGeom_Primvar_t** _result) {
    *_result = new PXR_NS::UsdGeomPrimvar(*param00);
    return 0;
}

int usdGeom_Primvar_dtor(usdGeom_Primvar_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_PrimvarVector_data(usdGeom_PrimvarVector_t* _this, usdGeom_Primvar_t** _result) {
    *_result = _this->data();
    return 0;
}

int usdGeom_PrimvarVector_data_const(usdGeom_PrimvarVector_t const* _this, usdGeom_Primvar_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usdGeom_PrimvarVector_empty(usdGeom_PrimvarVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usdGeom_PrimvarVector_size(usdGeom_PrimvarVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usdGeom_PrimvarVector_max_size(usdGeom_PrimvarVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usdGeom_PrimvarVector_reserve(usdGeom_PrimvarVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usdGeom_PrimvarVector_capacity(usdGeom_PrimvarVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usdGeom_PrimvarVector_clear(usdGeom_PrimvarVector_t* _this) {
    _this->clear();
    return 0;
}

int usdGeom_PrimvarVector_push_back(usdGeom_PrimvarVector_t* _this, usdGeom_Primvar_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usdGeom_PrimvarVector_pop_back(usdGeom_PrimvarVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usdGeom_PrimvarVector_resize(usdGeom_PrimvarVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usdGeom_PrimvarVector_resize_with(usdGeom_PrimvarVector_t* _this, size_t const _Newsize, usdGeom_Primvar_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usdGeom_PrimvarVector_op_index(usdGeom_PrimvarVector_t const* _this, size_t const _Pos, usdGeom_Primvar_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int usdGeom_PrimvarVector_default(usdGeom_PrimvarVector_t** _result) {
    *_result = new std::vector<PXR_NS::UsdGeomPrimvar>();
    return 0;
}

int usdGeom_PrimvarVector_dtor(usdGeom_PrimvarVector_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_PrimvarsAPI_CreatePrimvar(usdGeom_PrimvarsAPI_t const* _this, tf_Token_t const* name, sdf_ValueTypeName_t const* typeName, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreatePrimvar(*name, *typeName, *interpolation, elementSize);
    return 0;
}

int usdGeom_PrimvarsAPI_RemovePrimvar(usdGeom_PrimvarsAPI_t* _this, tf_Token_t const* name, bool* _result) {
    *_result = _this->RemovePrimvar(*name);
    return 0;
}

int usdGeom_PrimvarsAPI_BlockPrimvar(usdGeom_PrimvarsAPI_t* _this, tf_Token_t const* name) {
    _this->BlockPrimvar(*name);
    return 0;
}

int usdGeom_PrimvarsAPI_GetPrimvar(usdGeom_PrimvarsAPI_t const* _this, tf_Token_t const* name, usdGeom_Primvar_t* _result) {
    *_result = _this->GetPrimvar(*name);
    return 0;
}

int usdGeom_PrimvarsAPI_GetPrimvars(usdGeom_PrimvarsAPI_t const* _this, usdGeom_PrimvarVector_t* _result) {
    *_result = _this->GetPrimvars();
    return 0;
}

int usdGeom_PrimvarsAPI_GetAuthoredPrimvars(usdGeom_PrimvarsAPI_t const* _this, usdGeom_PrimvarVector_t* _result) {
    *_result = _this->GetAuthoredPrimvars();
    return 0;
}

int usdGeom_PrimvarsAPI_GetPrimvarsWithValues(usdGeom_PrimvarsAPI_t const* _this, usdGeom_PrimvarVector_t* _result) {
    *_result = _this->GetPrimvarsWithValues();
    return 0;
}

int usdGeom_PrimvarsAPI_GetPrimvarsWithAuthoredValues(usdGeom_PrimvarsAPI_t const* _this, usdGeom_PrimvarVector_t* _result) {
    *_result = _this->GetPrimvarsWithAuthoredValues();
    return 0;
}

int usdGeom_PrimvarsAPI_FindInheritablePrimvars(usdGeom_PrimvarsAPI_t const* _this, usdGeom_PrimvarVector_t* _result) {
    *_result = _this->FindInheritablePrimvars();
    return 0;
}

int usdGeom_PrimvarsAPI_FindIncrementallyInheritablePrimvars(usdGeom_PrimvarsAPI_t const* _this, usdGeom_PrimvarVector_t const* inheritedFromAncestors, usdGeom_PrimvarVector_t* _result) {
    *_result = _this->FindIncrementallyInheritablePrimvars(*inheritedFromAncestors);
    return 0;
}

int usdGeom_PrimvarsAPI_FindPrimvarWithInheritance(usdGeom_PrimvarsAPI_t const* _this, tf_Token_t const* name, usdGeom_Primvar_t* _result) {
    *_result = _this->FindPrimvarWithInheritance(*name);
    return 0;
}

int usdGeom_PrimvarsAPI_FindPrimvarWIthInheritance_from(usdGeom_PrimvarsAPI_t const* _this, tf_Token_t const* name, usdGeom_PrimvarVector_t const* inheritedFromAncestors, usdGeom_Primvar_t* _result) {
    *_result = _this->FindPrimvarWithInheritance(*name, *inheritedFromAncestors);
    return 0;
}

int usdGeom_PrimvarsAPI_FindPrimvarsWithInheritance(usdGeom_PrimvarsAPI_t const* _this, usdGeom_PrimvarVector_t* _result) {
    *_result = _this->FindPrimvarsWithInheritance();
    return 0;
}

int usdGeom_PrimvarsAPI_FindPrimvarsWIthInheritance_from(usdGeom_PrimvarsAPI_t const* _this, usdGeom_PrimvarVector_t const* inheritedFromAncestors, usdGeom_PrimvarVector_t* _result) {
    *_result = _this->FindPrimvarsWithInheritance(*inheritedFromAncestors);
    return 0;
}

int usdGeom_PrimvarsAPI_HasPrimvar(usdGeom_PrimvarsAPI_t const* _this, tf_Token_t const* name, bool* _result) {
    *_result = _this->HasPrimvar(*name);
    return 0;
}

int usdGeom_PrimvarsAPI_HasPossiblyInheritedPrimvar(usdGeom_PrimvarsAPI_t const* _this, tf_Token_t const* name, bool* _result) {
    *_result = _this->HasPossiblyInheritedPrimvar(*name);
    return 0;
}

int usdGeom_PrimvarsAPI_GetSchemaAttributeNames(bool includeInherited, tf_TokenVector_t const** _result) {
    *_result = &PXR_NS::UsdGeomPrimvarsAPI::GetSchemaAttributeNames(includeInherited);
    return 0;
}

int usdGeom_PrimvarsAPI_Get(usd_StageWeakPtr_t const* stage, sdf_Path_t const* path, usdGeom_PrimvarsAPI_t* _result) {
    *_result = PXR_NS::UsdGeomPrimvarsAPI::Get(*stage, *path);
    return 0;
}

int usdGeom_PrimvarsAPI_CanContainPropertyName(tf_Token_t const* name, bool* _result) {
    *_result = PXR_NS::UsdGeomPrimvarsAPI::CanContainPropertyName(*name);
    return 0;
}

int usdGeom_PrimvarsAPI_IsConcrete(usdGeom_PrimvarsAPI_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_PrimvarsAPI_IsTyped(usdGeom_PrimvarsAPI_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_PrimvarsAPI_IsAPISchema(usdGeom_PrimvarsAPI_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_PrimvarsAPI_IsAppliedAPISchema(usdGeom_PrimvarsAPI_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_PrimvarsAPI_IsMultipleApplyAPISchema(usdGeom_PrimvarsAPI_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_PrimvarsAPI_GetSchemaKind(usdGeom_PrimvarsAPI_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_PrimvarsAPI_GetPrim(usdGeom_PrimvarsAPI_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_PrimvarsAPI_GetPath(usdGeom_PrimvarsAPI_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_PrimvarsAPI_GetSchemaClassPrimDefinition(usdGeom_PrimvarsAPI_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_PrimvarsAPI_new(usd_Prim_t const* param00, usdGeom_PrimvarsAPI_t** _result) {
    *_result = new PXR_NS::UsdGeomPrimvarsAPI(*param00);
    return 0;
}

int usdGeom_PrimvarsAPI_dtor(usdGeom_PrimvarsAPI_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Scope_Get(usd_StageWeakPtr_t const* stage, sdf_Path_t const* path, usdGeom_Scope_t* _result) {
    *_result = PXR_NS::UsdGeomScope::Get(*stage, *path);
    return 0;
}

int usdGeom_Scope_GetSchemaAttributeNames(bool includeInherited, tf_TokenVector_t const** _result) {
    *_result = &PXR_NS::UsdGeomScope::GetSchemaAttributeNames(includeInherited);
    return 0;
}

int usdGeom_Scope_Define(usd_StageWeakPtr_t const* stage, sdf_Path_t const* path, usdGeom_Scope_t* _result) {
    *_result = PXR_NS::UsdGeomScope::Define(*stage, *path);
    return 0;
}

int usdGeom_Scope_GetVisibilityAttr(usdGeom_Scope_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Scope_CreateVisibilityAttr(usdGeom_Scope_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Scope_GetPurposeAttr(usdGeom_Scope_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Scope_CreatePurposeAttr(usdGeom_Scope_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Scope_GetProxyPrimRel(usdGeom_Scope_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Scope_CreateProxyPrimRel(usdGeom_Scope_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Scope_MakeVisible(usdGeom_Scope_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Scope_MakeInvisible(usdGeom_Scope_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Scope_ComputeVisibility(usdGeom_Scope_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Scope_GetPurposeVisibilityAttr(usdGeom_Scope_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Scope_ComputePurposeInfo(usdGeom_Scope_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Scope_ComputePurposeInfo_given_parent(usdGeom_Scope_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Scope_ComputePurpose(usdGeom_Scope_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Scope_ComputeProxyPrim(usdGeom_Scope_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Scope_SetProxyPrim(usdGeom_Scope_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Scope_ComputeWorldBound(usdGeom_Scope_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Scope_ComputeLocalBound(usdGeom_Scope_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Scope_ComputeUntransformedBound(usdGeom_Scope_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Scope_ComputeLocalToWorldTransform(usdGeom_Scope_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Scope_ComputeParentToWorldTransform(usdGeom_Scope_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Scope_IsConcrete(usdGeom_Scope_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Scope_IsTyped(usdGeom_Scope_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Scope_IsAPISchema(usdGeom_Scope_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Scope_IsAppliedAPISchema(usdGeom_Scope_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Scope_IsMultipleApplyAPISchema(usdGeom_Scope_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Scope_GetSchemaKind(usdGeom_Scope_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Scope_GetPrim(usdGeom_Scope_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Scope_GetPath(usdGeom_Scope_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Scope_GetSchemaClassPrimDefinition(usdGeom_Scope_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Scope_new(usd_Prim_t const* param00, usdGeom_Scope_t** _result) {
    *_result = new PXR_NS::UsdGeomScope(*param00);
    return 0;
}

int usdGeom_Scope_dtor(usdGeom_Scope_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Sphere_GetRadiusAttr(usdGeom_Sphere_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetRadiusAttr();
    return 0;
}

int usdGeom_Sphere_CreateRadiusAttr(usdGeom_Sphere_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateRadiusAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Sphere_GetDisplayColorAttr(usdGeom_Sphere_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayColorAttr();
    return 0;
}

int usdGeom_Sphere_CreateDisplayColorAttr(usdGeom_Sphere_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayColorAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Sphere_GetDisplayOpacityAttr(usdGeom_Sphere_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDisplayOpacityAttr();
    return 0;
}

int usdGeom_Sphere_CreateDisplayOpacityAttr(usdGeom_Sphere_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDisplayOpacityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Sphere_GetDoubleSidedAttr(usdGeom_Sphere_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetDoubleSidedAttr();
    return 0;
}

int usdGeom_Sphere_CreateDoubleSidedAttr(usdGeom_Sphere_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateDoubleSidedAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Sphere_GetOrientationAttr(usdGeom_Sphere_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetOrientationAttr();
    return 0;
}

int usdGeom_Sphere_CreateOrientationAttr(usdGeom_Sphere_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateOrientationAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Sphere_GetDisplayColorPrimvar(usdGeom_Sphere_t const* _this, usdGeom_Primvar_t* _result) {
    *_result = _this->GetDisplayColorPrimvar();
    return 0;
}

int usdGeom_Sphere_CreateDisplayColorPrimvar(usdGeom_Sphere_t const* _this, tf_Token_t const* interpolation, int elementSize, usdGeom_Primvar_t* _result) {
    *_result = _this->CreateDisplayColorPrimvar(*interpolation, elementSize);
    return 0;
}

int usdGeom_Sphere_GetExtentAttr(usdGeom_Sphere_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExtentAttr();
    return 0;
}

int usdGeom_Sphere_CreateExtentAttr(usdGeom_Sphere_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExtentAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Sphere_GetXformOpOrderAttr(usdGeom_Sphere_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Sphere_AddXformOp(usdGeom_Sphere_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddTranslateOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddScaleOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddRotateXOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddRotateYOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddRotateZOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddRotateXYZOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddRotateXZYOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddRotateYXZOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddRotateYZXOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddRotateZXYOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddRotateZYXOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddOrientOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_AddTransformOp(usdGeom_Sphere_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Sphere_SetResetXformStack(usdGeom_Sphere_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Sphere_SetXformOpOrder(usdGeom_Sphere_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Sphere_GetOrderedXformOps(usdGeom_Sphere_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Sphere_ClearXformOpOrder(usdGeom_Sphere_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Sphere_MakeMatrixXform(usdGeom_Sphere_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Sphere_TransformMightBeTimeVarying(usdGeom_Sphere_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Sphere_TransformMightBeTimeVarying_using(usdGeom_Sphere_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Sphere_GetTimeSamples(usdGeom_Sphere_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Sphere_GetTimeSamplesInInterval(usdGeom_Sphere_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Sphere_GetLocalTransformation(usdGeom_Sphere_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Sphere_GetLocalTransformation_with_ops(usdGeom_Sphere_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Sphere_GetVisibilityAttr(usdGeom_Sphere_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Sphere_CreateVisibilityAttr(usdGeom_Sphere_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Sphere_GetPurposeAttr(usdGeom_Sphere_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Sphere_CreatePurposeAttr(usdGeom_Sphere_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Sphere_GetProxyPrimRel(usdGeom_Sphere_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Sphere_CreateProxyPrimRel(usdGeom_Sphere_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Sphere_MakeVisible(usdGeom_Sphere_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Sphere_MakeInvisible(usdGeom_Sphere_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Sphere_ComputeVisibility(usdGeom_Sphere_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Sphere_GetPurposeVisibilityAttr(usdGeom_Sphere_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Sphere_ComputePurposeInfo(usdGeom_Sphere_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Sphere_ComputePurposeInfo_given_parent(usdGeom_Sphere_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Sphere_ComputePurpose(usdGeom_Sphere_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Sphere_ComputeProxyPrim(usdGeom_Sphere_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Sphere_SetProxyPrim(usdGeom_Sphere_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Sphere_ComputeWorldBound(usdGeom_Sphere_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Sphere_ComputeLocalBound(usdGeom_Sphere_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Sphere_ComputeUntransformedBound(usdGeom_Sphere_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Sphere_ComputeLocalToWorldTransform(usdGeom_Sphere_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Sphere_ComputeParentToWorldTransform(usdGeom_Sphere_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Sphere_IsConcrete(usdGeom_Sphere_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Sphere_IsTyped(usdGeom_Sphere_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Sphere_IsAPISchema(usdGeom_Sphere_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Sphere_IsAppliedAPISchema(usdGeom_Sphere_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Sphere_IsMultipleApplyAPISchema(usdGeom_Sphere_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Sphere_GetSchemaKind(usdGeom_Sphere_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Sphere_GetPrim(usdGeom_Sphere_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Sphere_GetPath(usdGeom_Sphere_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Sphere_GetSchemaClassPrimDefinition(usdGeom_Sphere_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Sphere_new(usd_Prim_t const* param00, usdGeom_Sphere_t** _result) {
    *_result = new PXR_NS::UsdGeomSphere(*param00);
    return 0;
}

int usdGeom_Sphere_dtor(usdGeom_Sphere_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Subset_GetElementTypeAttr(usdGeom_Subset_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetElementTypeAttr();
    return 0;
}

int usdGeom_Subset_CreateElementTypeAttr(usdGeom_Subset_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateElementTypeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Subset_GetIndicesAttr(usdGeom_Subset_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetIndicesAttr();
    return 0;
}

int usdGeom_Subset_CreateIndicesAttr(usdGeom_Subset_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateIndicesAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Subset_GetFamilyNameAttr(usdGeom_Subset_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetFamilyNameAttr();
    return 0;
}

int usdGeom_Subset_CreateFamilyNameAttr(usdGeom_Subset_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateFamilyNameAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Subset_Get(usd_StageWeakPtr_t const* stage, sdf_Path_t const* path, usdGeom_Subset_t* _result) {
    *_result = PXR_NS::UsdGeomSubset::Get(*stage, *path);
    return 0;
}

int usdGeom_Subset_GetSchemaAttributeNames(bool includeInherited, tf_TokenVector_t const** _result) {
    *_result = &PXR_NS::UsdGeomSubset::GetSchemaAttributeNames(includeInherited);
    return 0;
}

int usdGeom_Subset_Define(usd_StageWeakPtr_t const* stage, sdf_Path_t const* path, usdGeom_Subset_t* _result) {
    *_result = PXR_NS::UsdGeomSubset::Define(*stage, *path);
    return 0;
}

int usdGeom_Subset_CreateGeomSubset(usdGeom_Imageable_t const* geom, tf_Token_t const* subsetName, tf_Token_t const* elementType, vt_IntArray_t const* indices, tf_Token_t const* familyName, tf_Token_t const* familyType, usdGeom_Subset_t* _result) {
    *_result = PXR_NS::UsdGeomSubset::CreateGeomSubset(*geom, *subsetName, *elementType, *indices, *familyName, *familyType);
    return 0;
}

int usdGeom_Subset_CreateUniqueGeomSubset(usdGeom_Imageable_t const* geom, tf_Token_t const* subsetName, tf_Token_t const* elementType, vt_IntArray_t const* indices, tf_Token_t const* familyName, tf_Token_t const* familyType, usdGeom_Subset_t* _result) {
    *_result = PXR_NS::UsdGeomSubset::CreateUniqueGeomSubset(*geom, *subsetName, *elementType, *indices, *familyName, *familyType);
    return 0;
}

int usdGeom_Subset_GetAllGeomSubsets(usdGeom_Imageable_t const* geom, usdGeom_SubsetVector_t* _result) {
    *_result = PXR_NS::UsdGeomSubset::GetAllGeomSubsets(*geom);
    return 0;
}

int usdGeom_Subset_GetGeomSubsets(usdGeom_Imageable_t const* geom, tf_Token_t const* elementType, tf_Token_t const* familyName, usdGeom_SubsetVector_t* _result) {
    *_result = PXR_NS::UsdGeomSubset::GetGeomSubsets(*geom, *elementType, *familyName);
    return 0;
}

int usdGeom_Subset_SetFamilyType(usdGeom_Imageable_t const* geom, tf_Token_t const* familyName, tf_Token_t const* familyType, bool* _result) {
    *_result = PXR_NS::UsdGeomSubset::SetFamilyType(*geom, *familyName, *familyType);
    return 0;
}

int usdGeom_Subset_GetFamilyType(usdGeom_Imageable_t const* geom, tf_Token_t const* familyName, tf_Token_t* _result) {
    *_result = PXR_NS::UsdGeomSubset::GetFamilyType(*geom, *familyName);
    return 0;
}

int usdGeom_Subset_GetUnassignedIndices(usdGeom_SubsetVector_t const* subsets, size_t const elementCount, usd_TimeCode_t const* time, vt_IntArray_t* _result) {
    *_result = PXR_NS::UsdGeomSubset::GetUnassignedIndices(*subsets, elementCount, *time);
    return 0;
}

int usdGeom_Subset_ValidateSubsets(usdGeom_SubsetVector_t const* subsets, size_t const elementCount, tf_Token_t const* familyType, std_String_t* const reason, bool* _result) {
    *_result = PXR_NS::UsdGeomSubset::ValidateSubsets(*subsets, elementCount, *familyType, reason);
    return 0;
}

int usdGeom_Subset_ValidateFamily(usdGeom_Imageable_t const* geom, tf_Token_t const* elementType, tf_Token_t const* familyName, std_String_t* const reason, bool* _result) {
    *_result = PXR_NS::UsdGeomSubset::ValidateFamily(*geom, *elementType, *familyName, reason);
    return 0;
}

int usdGeom_Subset_IsConcrete(usdGeom_Subset_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Subset_IsTyped(usdGeom_Subset_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Subset_IsAPISchema(usdGeom_Subset_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Subset_IsAppliedAPISchema(usdGeom_Subset_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Subset_IsMultipleApplyAPISchema(usdGeom_Subset_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Subset_GetSchemaKind(usdGeom_Subset_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Subset_GetPrim(usdGeom_Subset_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Subset_GetPath(usdGeom_Subset_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Subset_GetSchemaClassPrimDefinition(usdGeom_Subset_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Subset_new(usd_Prim_t const* param00, usdGeom_Subset_t** _result) {
    *_result = new PXR_NS::UsdGeomSubset(*param00);
    return 0;
}

int usdGeom_Subset_dtor(usdGeom_Subset_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_SubsetVector_data(usdGeom_SubsetVector_t* _this, usdGeom_Subset_t** _result) {
    *_result = _this->data();
    return 0;
}

int usdGeom_SubsetVector_data_const(usdGeom_SubsetVector_t const* _this, usdGeom_Subset_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usdGeom_SubsetVector_empty(usdGeom_SubsetVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usdGeom_SubsetVector_size(usdGeom_SubsetVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usdGeom_SubsetVector_max_size(usdGeom_SubsetVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usdGeom_SubsetVector_reserve(usdGeom_SubsetVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usdGeom_SubsetVector_capacity(usdGeom_SubsetVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usdGeom_SubsetVector_clear(usdGeom_SubsetVector_t* _this) {
    _this->clear();
    return 0;
}

int usdGeom_SubsetVector_push_back(usdGeom_SubsetVector_t* _this, usdGeom_Subset_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usdGeom_SubsetVector_pop_back(usdGeom_SubsetVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usdGeom_SubsetVector_resize(usdGeom_SubsetVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usdGeom_SubsetVector_resize_with(usdGeom_SubsetVector_t* _this, size_t const _Newsize, usdGeom_Subset_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usdGeom_SubsetVector_op_index(usdGeom_SubsetVector_t const* _this, size_t const _Pos, usdGeom_Subset_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int usdGeom_SubsetVector_default(usdGeom_SubsetVector_t** _result) {
    *_result = new std::vector<PXR_NS::UsdGeomSubset>();
    return 0;
}

int usdGeom_SubsetVector_dtor(usdGeom_SubsetVector_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_VisibilityAPI_GetGuideVisibilityAttr(usdGeom_VisibilityAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetGuideVisibilityAttr();
    return 0;
}

int usdGeom_VisibilityAPI_CreateGuideVisibilityAttr(usdGeom_VisibilityAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateGuideVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_VisibilityAPI_GetProxyVisibilityAttr(usdGeom_VisibilityAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetProxyVisibilityAttr();
    return 0;
}

int usdGeom_VisibilityAPI_CreateProxyVisibilityAttr(usdGeom_VisibilityAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateProxyVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_VisibilityAPI_GetRenderVisibilityAttr(usdGeom_VisibilityAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetRenderVisibilityAttr();
    return 0;
}

int usdGeom_VisibilityAPI_CreateRenderVisibilityAttr(usdGeom_VisibilityAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateRenderVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_VisibilityAPI_GetPurposeVisibilityAttr(usdGeom_VisibilityAPI_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_VisibilityAPI_GetSchemaAttributeNames(bool includeInherited, tf_TokenVector_t const** _result) {
    *_result = &PXR_NS::UsdGeomVisibilityAPI::GetSchemaAttributeNames(includeInherited);
    return 0;
}

int usdGeom_VisibilityAPI_Get(usd_StageWeakPtr_t const* stage, sdf_Path_t const* path, usdGeom_VisibilityAPI_t* _result) {
    *_result = PXR_NS::UsdGeomVisibilityAPI::Get(*stage, *path);
    return 0;
}

int usdGeom_VisibilityAPI_CanApply(usd_Prim_t const* prim, std_String_t* whyNot, bool* _result) {
    *_result = PXR_NS::UsdGeomVisibilityAPI::CanApply(*prim, whyNot);
    return 0;
}

int usdGeom_VisibilityAPI_Apply(usd_Prim_t const* prim, usdGeom_VisibilityAPI_t* _result) {
    *_result = PXR_NS::UsdGeomVisibilityAPI::Apply(*prim);
    return 0;
}

int usdGeom_VisibilityAPI_IsConcrete(usdGeom_VisibilityAPI_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_VisibilityAPI_IsTyped(usdGeom_VisibilityAPI_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_VisibilityAPI_IsAPISchema(usdGeom_VisibilityAPI_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_VisibilityAPI_IsAppliedAPISchema(usdGeom_VisibilityAPI_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_VisibilityAPI_IsMultipleApplyAPISchema(usdGeom_VisibilityAPI_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_VisibilityAPI_GetSchemaKind(usdGeom_VisibilityAPI_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_VisibilityAPI_GetPrim(usdGeom_VisibilityAPI_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_VisibilityAPI_GetPath(usdGeom_VisibilityAPI_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_VisibilityAPI_GetSchemaClassPrimDefinition(usdGeom_VisibilityAPI_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_VisibilityAPI_new(usd_Prim_t const* param00, usdGeom_VisibilityAPI_t** _result) {
    *_result = new PXR_NS::UsdGeomVisibilityAPI(*param00);
    return 0;
}

int usdGeom_VisibilityAPI_dtor(usdGeom_VisibilityAPI_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Xform_GetSchemaAttributeNames(bool includeInherited, tf_TokenVector_t const** _result) {
    *_result = &PXR_NS::UsdGeomXform::GetSchemaAttributeNames(includeInherited);
    return 0;
}

int usdGeom_Xform_Get(usd_StageWeakPtr_t const* stage, sdf_Path_t const* path, usdGeom_Xform_t* _result) {
    *_result = PXR_NS::UsdGeomXform::Get(*stage, *path);
    return 0;
}

int usdGeom_Xform_Define(usd_StageWeakPtr_t const* stage, sdf_Path_t const* path, usdGeom_Xform_t* _result) {
    *_result = PXR_NS::UsdGeomXform::Define(*stage, *path);
    return 0;
}

int usdGeom_Xform_GetXformOpOrderAttr(usdGeom_Xform_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Xform_AddXformOp(usdGeom_Xform_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddTranslateOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddScaleOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddRotateXOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddRotateYOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddRotateZOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddRotateXYZOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddRotateXZYOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddRotateYXZOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddRotateYZXOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddRotateZXYOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddRotateZYXOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddOrientOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_AddTransformOp(usdGeom_Xform_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xform_SetResetXformStack(usdGeom_Xform_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Xform_SetXformOpOrder(usdGeom_Xform_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Xform_GetOrderedXformOps(usdGeom_Xform_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Xform_ClearXformOpOrder(usdGeom_Xform_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Xform_MakeMatrixXform(usdGeom_Xform_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Xform_TransformMightBeTimeVarying(usdGeom_Xform_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Xform_TransformMightBeTimeVarying_using(usdGeom_Xform_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Xform_GetTimeSamples(usdGeom_Xform_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Xform_GetTimeSamplesInInterval(usdGeom_Xform_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Xform_GetLocalTransformation(usdGeom_Xform_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Xform_GetLocalTransformation_with_ops(usdGeom_Xform_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Xform_GetVisibilityAttr(usdGeom_Xform_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Xform_CreateVisibilityAttr(usdGeom_Xform_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Xform_GetPurposeAttr(usdGeom_Xform_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Xform_CreatePurposeAttr(usdGeom_Xform_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Xform_GetProxyPrimRel(usdGeom_Xform_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Xform_CreateProxyPrimRel(usdGeom_Xform_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Xform_MakeVisible(usdGeom_Xform_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Xform_MakeInvisible(usdGeom_Xform_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Xform_ComputeVisibility(usdGeom_Xform_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Xform_GetPurposeVisibilityAttr(usdGeom_Xform_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Xform_ComputePurposeInfo(usdGeom_Xform_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Xform_ComputePurposeInfo_given_parent(usdGeom_Xform_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Xform_ComputePurpose(usdGeom_Xform_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Xform_ComputeProxyPrim(usdGeom_Xform_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Xform_SetProxyPrim(usdGeom_Xform_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Xform_ComputeWorldBound(usdGeom_Xform_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Xform_ComputeLocalBound(usdGeom_Xform_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Xform_ComputeUntransformedBound(usdGeom_Xform_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Xform_ComputeLocalToWorldTransform(usdGeom_Xform_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Xform_ComputeParentToWorldTransform(usdGeom_Xform_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Xform_IsConcrete(usdGeom_Xform_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Xform_IsTyped(usdGeom_Xform_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Xform_IsAPISchema(usdGeom_Xform_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Xform_IsAppliedAPISchema(usdGeom_Xform_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Xform_IsMultipleApplyAPISchema(usdGeom_Xform_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Xform_GetSchemaKind(usdGeom_Xform_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Xform_GetPrim(usdGeom_Xform_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Xform_GetPath(usdGeom_Xform_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Xform_GetSchemaClassPrimDefinition(usdGeom_Xform_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Xform_new(usdGeom_Xform_t** _result) {
    *_result = new PXR_NS::UsdGeomXform();
    return 0;
}

int usdGeom_Xform_dtor(usdGeom_Xform_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_XformCache_GetLocalToWorldTransform(usdGeom_XformCache_t* _this, usd_Prim_t const* prim, gf_Matrix4d_t* _result) {
    *_result = _this->GetLocalToWorldTransform(*prim);
    return 0;
}

int usdGeom_XformCache_GetParentToWorldTransform(usdGeom_XformCache_t* _this, usd_Prim_t const* prim, gf_Matrix4d_t* _result) {
    *_result = _this->GetParentToWorldTransform(*prim);
    return 0;
}

int usdGeom_XformCache_GetLocalTransformation(usdGeom_XformCache_t* _this, usd_Prim_t const* prim, bool* resetsXformStack, gf_Matrix4d_t* _result) {
    *_result = _this->GetLocalTransformation(*prim, resetsXformStack);
    return 0;
}

int usdGeom_XformCache_ComputeRelativeTransform(usdGeom_XformCache_t* _this, usd_Prim_t const* prim, usd_Prim_t const* ancestor, bool* resetXformStack, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeRelativeTransform(*prim, *ancestor, resetXformStack);
    return 0;
}

int usdGeom_XformCache_IsAttributeIncludedInLocalTransform(usdGeom_XformCache_t* _this, usd_Prim_t const* prim, tf_Token_t const* attrName, bool* _result) {
    *_result = _this->IsAttributeIncludedInLocalTransform(*prim, *attrName);
    return 0;
}

int usdGeom_XformCache_TransformMightBeTimeVarying(usdGeom_XformCache_t* _this, usd_Prim_t const* prim, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*prim);
    return 0;
}

int usdGeom_XformCache_GetResetXformStack(usdGeom_XformCache_t* _this, usd_Prim_t const* prim, bool* _result) {
    *_result = _this->GetResetXformStack(*prim);
    return 0;
}

int usdGeom_XformCache_Clear(usdGeom_XformCache_t* _this) {
    _this->Clear();
    return 0;
}

int usdGeom_XformCache_GetTime(usdGeom_XformCache_t* _this, usd_TimeCode_t* _result) {
    *_result = _this->GetTime();
    return 0;
}

int usdGeom_XformCache_SetTime(usdGeom_XformCache_t* _this, usd_TimeCode_t time) {
    _this->SetTime(time);
    return 0;
}

int usdGeom_XformCache_Swap(usdGeom_XformCache_t* _this, usdGeom_XformCache_t* other) {
    _this->Swap(*other);
    return 0;
}

int usdGeom_XformCache_new(usd_TimeCode_t time, usdGeom_XformCache_t** _result) {
    *_result = new PXR_NS::UsdGeomXformCache(time);
    return 0;
}

int usdGeom_XformCache_dtor(usdGeom_XformCache_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_XformCommonAPI_SetXformVectors(usdGeom_XformCommonAPI_t const* _this, gf_Vec3d_t const* translation, gf_Vec3f_t const* rotation, gf_Vec3f_t const* scale, gf_Vec3f_t const* pivot, int rotOrder, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->SetXformVectors(*translation, *rotation, *scale, *pivot, static_cast<PXR_NS::UsdGeomXformCommonAPI::RotationOrder>(rotOrder), time);
    return 0;
}

int usdGeom_XformCommonAPI_GetResetXformStack(usdGeom_XformCommonAPI_t const* _this, bool* _result) {
    *_result = _this->GetResetXformStack();
    return 0;
}

int usdGeom_XformCommonAPI_SetResetXformStack(usdGeom_XformCommonAPI_t const* _this, bool resetXformStack, bool* _result) {
    *_result = _this->SetResetXformStack(resetXformStack);
    return 0;
}

int usdGeom_XformCommonAPI_SetTranslate(usdGeom_XformCommonAPI_t const* _this, gf_Vec3d_t const* translation, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->SetTranslate(*translation, time);
    return 0;
}

int usdGeom_XformCommonAPI_SetPivot(usdGeom_XformCommonAPI_t const* _this, gf_Vec3f_t const* pivot, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->SetPivot(*pivot, time);
    return 0;
}

int usdGeom_XformCommonAPI_SetRotate(usdGeom_XformCommonAPI_t const* _this, gf_Vec3f_t const* rotation, int rotOrder, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->SetRotate(*rotation, static_cast<PXR_NS::UsdGeomXformCommonAPI::RotationOrder>(rotOrder), time);
    return 0;
}

int usdGeom_XformCommonAPI_SetScale(usdGeom_XformCommonAPI_t const* _this, gf_Vec3f_t const* scale, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->SetScale(*scale, time);
    return 0;
}

int usdGeom_XformCommonAPI_CreateXformOps_with_rotation_order(usdGeom_XformCommonAPI_t const* _this, int rotOrder, int op1, int op2, int op3, int op4, usdGeom_XformCommonAPIOps_t* _result) {
    *_result = _this->CreateXformOps(static_cast<PXR_NS::UsdGeomXformCommonAPI::RotationOrder>(rotOrder), static_cast<PXR_NS::UsdGeomXformCommonAPI::OpFlags>(op1), static_cast<PXR_NS::UsdGeomXformCommonAPI::OpFlags>(op2), static_cast<PXR_NS::UsdGeomXformCommonAPI::OpFlags>(op3), static_cast<PXR_NS::UsdGeomXformCommonAPI::OpFlags>(op4));
    return 0;
}

int usdGeom_XformCommonAPI_CreateXformOps(usdGeom_XformCommonAPI_t const* _this, int op1, int op2, int op3, int op4, usdGeom_XformCommonAPIOps_t* _result) {
    *_result = _this->CreateXformOps(static_cast<PXR_NS::UsdGeomXformCommonAPI::OpFlags>(op1), static_cast<PXR_NS::UsdGeomXformCommonAPI::OpFlags>(op2), static_cast<PXR_NS::UsdGeomXformCommonAPI::OpFlags>(op3), static_cast<PXR_NS::UsdGeomXformCommonAPI::OpFlags>(op4));
    return 0;
}

int usdGeom_XformCommonAPI_GetSchemaAttributeNames(bool includeInherited, tf_TokenVector_t const** _result) {
    *_result = &PXR_NS::UsdGeomXformCommonAPI::GetSchemaAttributeNames(includeInherited);
    return 0;
}

int usdGeom_XformCommonAPI_Get(usd_StageWeakPtr_t const* stage, sdf_Path_t const* path, usdGeom_XformCommonAPI_t* _result) {
    *_result = PXR_NS::UsdGeomXformCommonAPI::Get(*stage, *path);
    return 0;
}

int usdGeom_XformCommonAPI_ConvertRotationOrderToOpType(int rotOrder, int* _result) {
    *_result = static_cast<int>(PXR_NS::UsdGeomXformCommonAPI::ConvertRotationOrderToOpType(static_cast<PXR_NS::UsdGeomXformCommonAPI::RotationOrder>(rotOrder)));
    return 0;
}

int usdGeom_XformCommonAPI_ConvertOpTypeToRotationOrder(int opType, int* _result) {
    *_result = static_cast<int>(PXR_NS::UsdGeomXformCommonAPI::ConvertOpTypeToRotationOrder(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType)));
    return 0;
}

int usdGeom_XformCommonAPI_CanConvertOpTypeToRotationOrder(int opType, bool* _result) {
    *_result = PXR_NS::UsdGeomXformCommonAPI::CanConvertOpTypeToRotationOrder(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType));
    return 0;
}

int usdGeom_XformCommonAPI_GetRotationTransform(gf_Vec3f_t const* rotation, int rotationOrder, gf_Matrix4d_t* _result) {
    *_result = PXR_NS::UsdGeomXformCommonAPI::GetRotationTransform(*rotation, static_cast<PXR_NS::UsdGeomXformCommonAPI::RotationOrder>(rotationOrder));
    return 0;
}

int usdGeom_XformCommonAPI_IsConcrete(usdGeom_XformCommonAPI_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_XformCommonAPI_IsTyped(usdGeom_XformCommonAPI_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_XformCommonAPI_IsAPISchema(usdGeom_XformCommonAPI_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_XformCommonAPI_IsAppliedAPISchema(usdGeom_XformCommonAPI_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_XformCommonAPI_IsMultipleApplyAPISchema(usdGeom_XformCommonAPI_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_XformCommonAPI_GetSchemaKind(usdGeom_XformCommonAPI_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_XformCommonAPI_GetPrim(usdGeom_XformCommonAPI_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_XformCommonAPI_GetPath(usdGeom_XformCommonAPI_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_XformCommonAPI_GetSchemaClassPrimDefinition(usdGeom_XformCommonAPI_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_XformCommonAPI_new(usd_Prim_t const* param00, usdGeom_XformCommonAPI_t** _result) {
    *_result = new PXR_NS::UsdGeomXformCommonAPI(*param00);
    return 0;
}

int usdGeom_XformCommonAPI_dtor(usdGeom_XformCommonAPI_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_XformCommonAPIOps_dtor(usdGeom_XformCommonAPIOps_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_Xformable_GetXformOpOrderAttr(usdGeom_Xformable_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetXformOpOrderAttr();
    return 0;
}

int usdGeom_Xformable_AddXformOp(usdGeom_Xformable_t const* _this, int opType, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddXformOp(static_cast<PXR_NS::UsdGeomXformOp::Type>(opType), static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddTranslateOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTranslateOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddScaleOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddScaleOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddRotateXOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddRotateYOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddRotateZOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddRotateXYZOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXYZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddRotateXZYOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateXZYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddRotateYXZOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYXZOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddRotateYZXOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateYZXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddRotateZXYOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZXYOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddRotateZYXOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddRotateZYXOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddOrientOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddOrientOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_AddTransformOp(usdGeom_Xformable_t const* _this, int precision, tf_Token_t const* opSuffix, bool isInverseOp, usdGeom_XformOp_t* _result) {
    *_result = _this->AddTransformOp(static_cast<PXR_NS::UsdGeomXformOp::Precision>(precision), *opSuffix, isInverseOp);
    return 0;
}

int usdGeom_Xformable_SetResetXformStack(usdGeom_Xformable_t const* _this, bool resetXform, bool* _result) {
    *_result = _this->SetResetXformStack(resetXform);
    return 0;
}

int usdGeom_Xformable_SetXformOpOrder(usdGeom_Xformable_t const* _this, usdGeom_XformOpVector_t const* orderedXformOps, bool resetXformStack, bool* _result) {
    *_result = _this->SetXformOpOrder(*orderedXformOps, resetXformStack);
    return 0;
}

int usdGeom_Xformable_GetOrderedXformOps(usdGeom_Xformable_t const* _this, bool* resetsXformStack, usdGeom_XformOpVector_t* _result) {
    *_result = _this->GetOrderedXformOps(resetsXformStack);
    return 0;
}

int usdGeom_Xformable_ClearXformOpOrder(usdGeom_Xformable_t const* _this, bool* _result) {
    *_result = _this->ClearXformOpOrder();
    return 0;
}

int usdGeom_Xformable_MakeMatrixXform(usdGeom_Xformable_t const* _this, usdGeom_XformOp_t* _result) {
    *_result = _this->MakeMatrixXform();
    return 0;
}

int usdGeom_Xformable_TransformMightBeTimeVarying(usdGeom_Xformable_t const* _this, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying();
    return 0;
}

int usdGeom_Xformable_TransformMightBeTimeVarying_using(usdGeom_Xformable_t const* _this, usdGeom_XformOpVector_t const* ops, bool* _result) {
    *_result = _this->TransformMightBeTimeVarying(*ops);
    return 0;
}

int usdGeom_Xformable_GetTimeSamples(usdGeom_Xformable_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_Xformable_GetTimeSamplesInInterval(usdGeom_Xformable_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_Xformable_GetLocalTransformation(usdGeom_Xformable_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, time);
    return 0;
}

int usdGeom_Xformable_GetLocalTransformation_with_ops(usdGeom_Xformable_t const* _this, gf_Matrix4d_t* transform, bool* resetsXformStack, usdGeom_XformOpVector_t const* ops, usd_TimeCode_t const time, bool* _result) {
    *_result = _this->GetLocalTransformation(transform, resetsXformStack, *ops, time);
    return 0;
}

int usdGeom_Xformable_GetVisibilityAttr(usdGeom_Xformable_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetVisibilityAttr();
    return 0;
}

int usdGeom_Xformable_CreateVisibilityAttr(usdGeom_Xformable_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateVisibilityAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Xformable_GetPurposeAttr(usdGeom_Xformable_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeAttr();
    return 0;
}

int usdGeom_Xformable_CreatePurposeAttr(usdGeom_Xformable_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreatePurposeAttr(*defaultValue, writeSparsely);
    return 0;
}

int usdGeom_Xformable_GetProxyPrimRel(usdGeom_Xformable_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetProxyPrimRel();
    return 0;
}

int usdGeom_Xformable_CreateProxyPrimRel(usdGeom_Xformable_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateProxyPrimRel();
    return 0;
}

int usdGeom_Xformable_MakeVisible(usdGeom_Xformable_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeVisible(*time);
    return 0;
}

int usdGeom_Xformable_MakeInvisible(usdGeom_Xformable_t const* _this, usd_TimeCode_t const* time) {
    _this->MakeInvisible(*time);
    return 0;
}

int usdGeom_Xformable_ComputeVisibility(usdGeom_Xformable_t const* _this, usd_TimeCode_t const* time, tf_Token_t* _result) {
    *_result = _this->ComputeVisibility(*time);
    return 0;
}

int usdGeom_Xformable_GetPurposeVisibilityAttr(usdGeom_Xformable_t const* _this, tf_Token_t const* purpose, usd_Attribute_t* _result) {
    *_result = _this->GetPurposeVisibilityAttr(*purpose);
    return 0;
}

int usdGeom_Xformable_ComputePurposeInfo(usdGeom_Xformable_t const* _this, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo();
    return 0;
}

int usdGeom_Xformable_ComputePurposeInfo_given_parent(usdGeom_Xformable_t const* _this, usdGeom_ImageablePurposeInfo_t const* parentPurposeInfo, usdGeom_ImageablePurposeInfo_t* _result) {
    *_result = _this->ComputePurposeInfo(*parentPurposeInfo);
    return 0;
}

int usdGeom_Xformable_ComputePurpose(usdGeom_Xformable_t const* _this, tf_Token_t* _result) {
    *_result = _this->ComputePurpose();
    return 0;
}

int usdGeom_Xformable_ComputeProxyPrim(usdGeom_Xformable_t const* _this, usd_Prim_t* renderPrim, usd_Prim_t* _result) {
    *_result = _this->ComputeProxyPrim(renderPrim);
    return 0;
}

int usdGeom_Xformable_SetProxyPrim(usdGeom_Xformable_t const* _this, usd_Prim_t const* proxy, bool* _result) {
    *_result = _this->SetProxyPrim(*proxy);
    return 0;
}

int usdGeom_Xformable_ComputeWorldBound(usdGeom_Xformable_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeWorldBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Xformable_ComputeLocalBound(usdGeom_Xformable_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeLocalBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Xformable_ComputeUntransformedBound(usdGeom_Xformable_t const* _this, usd_TimeCode_t const* time, tf_Token_t const* purpose1, tf_Token_t const* purpose2, tf_Token_t const* purpose3, tf_Token_t const* purpose4, gf_BBox3d_t* _result) {
    *_result = _this->ComputeUntransformedBound(*time, *purpose1, *purpose2, *purpose3, *purpose4);
    return 0;
}

int usdGeom_Xformable_ComputeLocalToWorldTransform(usdGeom_Xformable_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeLocalToWorldTransform(*time);
    return 0;
}

int usdGeom_Xformable_ComputeParentToWorldTransform(usdGeom_Xformable_t const* _this, usd_TimeCode_t const* time, gf_Matrix4d_t* _result) {
    *_result = _this->ComputeParentToWorldTransform(*time);
    return 0;
}

int usdGeom_Xformable_IsConcrete(usdGeom_Xformable_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usdGeom_Xformable_IsTyped(usdGeom_Xformable_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usdGeom_Xformable_IsAPISchema(usdGeom_Xformable_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usdGeom_Xformable_IsAppliedAPISchema(usdGeom_Xformable_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usdGeom_Xformable_IsMultipleApplyAPISchema(usdGeom_Xformable_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usdGeom_Xformable_GetSchemaKind(usdGeom_Xformable_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usdGeom_Xformable_GetPrim(usdGeom_Xformable_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usdGeom_Xformable_GetPath(usdGeom_Xformable_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usdGeom_Xformable_GetSchemaClassPrimDefinition(usdGeom_Xformable_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usdGeom_Xformable_new(usdGeom_Xformable_t** _result) {
    *_result = new PXR_NS::UsdGeomXformable();
    return 0;
}

int usdGeom_Xformable_dtor(usdGeom_Xformable_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_XformOp_GetOpType(usdGeom_XformOp_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetOpType());
    return 0;
}

int usdGeom_XformOp_GetPrecision(usdGeom_XformOp_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetPrecision());
    return 0;
}

int usdGeom_XformOp_IsInverseOp(usdGeom_XformOp_t const* _this, bool* _result) {
    *_result = _this->IsInverseOp();
    return 0;
}

int usdGeom_XformOp_GetOpName(usdGeom_XformOp_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetOpName();
    return 0;
}

int usdGeom_XformOp_HasSuffix(usdGeom_XformOp_t const* _this, tf_Token_t const* suffix, bool* _result) {
    *_result = _this->HasSuffix(*suffix);
    return 0;
}

int usdGeom_XformOp_GetAttr(usdGeom_XformOp_t const* _this, usd_Attribute_t const** _result) {
    *_result = &_this->GetAttr();
    return 0;
}

int usdGeom_XformOp_IsDefined(usdGeom_XformOp_t const* _this, bool* _result) {
    *_result = _this->IsDefined();
    return 0;
}

int usdGeom_XformOp_GetName(usdGeom_XformOp_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int usdGeom_XformOp_GetBaseName(usdGeom_XformOp_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetBaseName();
    return 0;
}

int usdGeom_XformOp_GetNamespace(usdGeom_XformOp_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNamespace();
    return 0;
}

int usdGeom_XformOp_SplitName(usdGeom_XformOp_t const* _this, std_StringVector_t* _result) {
    *_result = _this->SplitName();
    return 0;
}

int usdGeom_XformOp_GetTypeName(usdGeom_XformOp_t const* _this, sdf_ValueTypeName_t* _result) {
    *_result = _this->GetTypeName();
    return 0;
}

int usdGeom_XformOp_GetTimeSamples(usdGeom_XformOp_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usdGeom_XformOp_GetTimeSamplesInInterval(usdGeom_XformOp_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usdGeom_XformOp_GetNumTimeSamples(usdGeom_XformOp_t const* _this, size_t* _result) {
    *_result = _this->GetNumTimeSamples();
    return 0;
}

int usdGeom_XformOp_GetOpTransform(usdGeom_XformOp_t const* _this, usd_TimeCode_t time, gf_Matrix4d_t* _result) {
    *_result = _this->GetOpTransform(time);
    return 0;
}

int usdGeom_XformOp_MightBeTimeVarying(usdGeom_XformOp_t const* _this, bool* _result) {
    *_result = _this->MightBeTimeVarying();
    return 0;
}

int usdGeom_XformOp_new(usd_Attribute_t const* attr, bool isInverseOp, usdGeom_XformOp_t** _result) {
    *_result = new PXR_NS::UsdGeomXformOp(*attr, isInverseOp);
    return 0;
}

int usdGeom_XformOp_dtor(usdGeom_XformOp_t* _this) {
    delete _this;
    return 0;
}

int usdGeom_XformOpVector_data(usdGeom_XformOpVector_t* _this, usdGeom_XformOp_t** _result) {
    *_result = _this->data();
    return 0;
}

int usdGeom_XformOpVector_data_const(usdGeom_XformOpVector_t const* _this, usdGeom_XformOp_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usdGeom_XformOpVector_empty(usdGeom_XformOpVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usdGeom_XformOpVector_size(usdGeom_XformOpVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usdGeom_XformOpVector_max_size(usdGeom_XformOpVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usdGeom_XformOpVector_reserve(usdGeom_XformOpVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usdGeom_XformOpVector_capacity(usdGeom_XformOpVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usdGeom_XformOpVector_clear(usdGeom_XformOpVector_t* _this) {
    _this->clear();
    return 0;
}

int usdGeom_XformOpVector_push_back(usdGeom_XformOpVector_t* _this, usdGeom_XformOp_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usdGeom_XformOpVector_pop_back(usdGeom_XformOpVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usdGeom_XformOpVector_resize(usdGeom_XformOpVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usdGeom_XformOpVector_resize_with(usdGeom_XformOpVector_t* _this, size_t const _Newsize, usdGeom_XformOp_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usdGeom_XformOpVector_op_index(usdGeom_XformOpVector_t const* _this, size_t const _Pos, usdGeom_XformOp_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int usdGeom_XformOpVector_default(usdGeom_XformOpVector_t** _result) {
    *_result = new std::vector<PXR_NS::UsdGeomXformOp>();
    return 0;
}

int usdGeom_XformOpVector_dtor(usdGeom_XformOpVector_t* _this) {
    delete _this;
    return 0;
}

int usd_Prim_GetPrimTypeInfo(usd_Prim_t const* _this, usd_PrimTypeInfo_t const** _result) {
    *_result = &_this->GetPrimTypeInfo();
    return 0;
}

int usd_Prim_GetPrimDefinition(usd_Prim_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = &_this->GetPrimDefinition();
    return 0;
}

int usd_Prim_GetSpecifier(usd_Prim_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSpecifier());
    return 0;
}

int usd_Prim_SetSpecifier(usd_Prim_t const* _this, int specifier, bool* _result) {
    *_result = _this->SetSpecifier(static_cast<PXR_NS::SdfSpecifier>(specifier));
    return 0;
}

int usd_Prim_GetPrimStack(usd_Prim_t const* _this, sdf_PrimSpecHandleVector_t* _result) {
    *_result = _this->GetPrimStack();
    return 0;
}

int usd_Prim_GetPrimStackWithLayerOffsets(usd_Prim_t const* _this, sdf_PrimSpecHandleOffsetPairVector_t* _result) {
    *_result = _this->GetPrimStackWithLayerOffsets();
    return 0;
}

int usd_Prim_GetTypeName(usd_Prim_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetTypeName();
    return 0;
}

int usd_Prim_SetTypeName(usd_Prim_t const* _this, tf_Token_t const* typeName, bool* _result) {
    *_result = _this->SetTypeName(*typeName);
    return 0;
}

int usd_Prim_ClearTypeName(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->ClearTypeName();
    return 0;
}

int usd_Prim_HasAuthoredTypeName(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredTypeName();
    return 0;
}

int usd_Prim_IsActive(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->IsActive();
    return 0;
}

int usd_Prim_SetActive(usd_Prim_t const* _this, bool active, bool* _result) {
    *_result = _this->SetActive(active);
    return 0;
}

int usd_Prim_ClearActive(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->ClearActive();
    return 0;
}

int usd_Prim_HasAuthoredActive(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredActive();
    return 0;
}

int usd_Prim_IsLoaded(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->IsLoaded();
    return 0;
}

int usd_Prim_IsModel(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->IsModel();
    return 0;
}

int usd_Prim_IsGroup(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->IsGroup();
    return 0;
}

int usd_Prim_IsAbstract(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->IsAbstract();
    return 0;
}

int usd_Prim_IsDefined(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->IsDefined();
    return 0;
}

int usd_Prim_GetName(usd_Prim_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int usd_Prim_HasDefiningSpecifier(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->HasDefiningSpecifier();
    return 0;
}

int usd_Prim_GetAppliedSchemas(usd_Prim_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->GetAppliedSchemas();
    return 0;
}

int usd_Prim_GetPropertiesInNamespace(usd_Prim_t const* _this, std_String_t const* namespaces, usd_PropertyVector_t* _result) {
    *_result = _this->GetPropertiesInNamespace(*namespaces);
    return 0;
}

int usd_Prim_GetAuthoredPropertiesInNamespace(usd_Prim_t const* _this, std_String_t const* namespaces, usd_PropertyVector_t* _result) {
    *_result = _this->GetAuthoredPropertiesInNamespace(*namespaces);
    return 0;
}

int usd_Prim_GetPropertyOrder(usd_Prim_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->GetPropertyOrder();
    return 0;
}

int usd_Prim_SetPropertyOrder(usd_Prim_t const* _this, tf_TokenVector_t const* order) {
    _this->SetPropertyOrder(*order);
    return 0;
}

int usd_Prim_ClearPropertyOrder(usd_Prim_t const* _this) {
    _this->ClearPropertyOrder();
    return 0;
}

int usd_Prim_RemoveProperty(usd_Prim_t* _this, tf_Token_t const* propName, bool* _result) {
    *_result = _this->RemoveProperty(*propName);
    return 0;
}

int usd_Prim_GetProperty(usd_Prim_t const* _this, tf_Token_t const* propName, usd_Property_t* _result) {
    *_result = _this->GetProperty(*propName);
    return 0;
}

int usd_Prim_HasProperty(usd_Prim_t const* _this, tf_Token_t const* propName, bool* _result) {
    *_result = _this->HasProperty(*propName);
    return 0;
}

int usd_Prim_AddAppliedSchema(usd_Prim_t const* _this, tf_Token_t const* appliedSchemaName, bool* _result) {
    *_result = _this->AddAppliedSchema(*appliedSchemaName);
    return 0;
}

int usd_Prim_RemoveAppliedSchema(usd_Prim_t const* _this, tf_Token_t const* appliedSchemaName, bool* _result) {
    *_result = _this->RemoveAppliedSchema(*appliedSchemaName);
    return 0;
}

int usd_Prim_IsA(usd_Prim_t const* _this, tf_Token_t const* schemaIdentifier, bool* _result) {
    *_result = _this->IsA(*schemaIdentifier);
    return 0;
}

int usd_Prim_IsA_with_version(usd_Prim_t const* _this, tf_Token_t const* schemaFamily, unsigned int schemaVersion, bool* _result) {
    *_result = _this->IsA(*schemaFamily, schemaVersion);
    return 0;
}

int usd_Prim_IsInFamily(usd_Prim_t const* _this, tf_Token_t const* schemaFamily, bool* _result) {
    *_result = _this->IsInFamily(*schemaFamily);
    return 0;
}

int usd_Prim_IsInFamily_with_policy(usd_Prim_t const* _this, tf_Token_t const* schemaIdentifier, int versionPolicy, bool* _result) {
    *_result = _this->IsInFamily(*schemaIdentifier, static_cast<PXR_NS::UsdSchemaRegistry::VersionPolicy>(versionPolicy));
    return 0;
}

int usd_Prim_IsInFamily_with_policy_and_version(usd_Prim_t const* _this, tf_Token_t const* schemaFamily, unsigned int schemaVersion, int versionPolicy, bool* _result) {
    *_result = _this->IsInFamily(*schemaFamily, schemaVersion, static_cast<PXR_NS::UsdSchemaRegistry::VersionPolicy>(versionPolicy));
    return 0;
}

int usd_Prim_IsInFamily_with_type(usd_Prim_t const* _this, tf_Type_t const* schemaType, int versionPolicy, bool* _result) {
    *_result = _this->IsInFamily(*schemaType, static_cast<PXR_NS::UsdSchemaRegistry::VersionPolicy>(versionPolicy));
    return 0;
}

int usd_Prim_GetVersionIfIsInFamily(usd_Prim_t const* _this, tf_Token_t const* schemaFamily, unsigned int* schemaVersion, bool* _result) {
    *_result = _this->GetVersionIfIsInFamily(*schemaFamily, schemaVersion);
    return 0;
}

int usd_Prim_HasAPI(usd_Prim_t const* _this, tf_Type_t const* schemaType, bool* _result) {
    *_result = _this->HasAPI(*schemaType);
    return 0;
}

int usd_Prim_HasAPI_with_instance_name(usd_Prim_t const* _this, tf_Type_t const* schemaType, tf_Token_t const* instanceName, bool* _result) {
    *_result = _this->HasAPI(*schemaType, *instanceName);
    return 0;
}

int usd_Prim_HasAPI_with_schema_identifier(usd_Prim_t const* _this, tf_Token_t const* schemaIdentifier, bool* _result) {
    *_result = _this->HasAPI(*schemaIdentifier);
    return 0;
}

int usd_Prim_HasAPI_with_instance_name_and_schema_identifier(usd_Prim_t const* _this, tf_Token_t const* schemaIdentifier, tf_Token_t const* instanceName, bool* _result) {
    *_result = _this->HasAPI(*schemaIdentifier, *instanceName);
    return 0;
}

int usd_Prim_HasAPIInFamily(usd_Prim_t const* _this, tf_Token_t const* schemaFamily, bool* _result) {
    *_result = _this->HasAPIInFamily(*schemaFamily);
    return 0;
}

int usd_Prim_GetVersionIfHasAPIInFamily(usd_Prim_t const* _this, tf_Token_t const* schemaFamily, tf_Token_t const* instanceName, unsigned int* schemaVersion, bool* _result) {
    *_result = _this->GetVersionIfHasAPIInFamily(*schemaFamily, *instanceName, schemaVersion);
    return 0;
}

int usd_Prim_CanApplyAPI(usd_Prim_t const* _this, tf_Type_t const* schemaType, std_String_t* whyNot, bool* _result) {
    *_result = _this->CanApplyAPI(*schemaType, whyNot);
    return 0;
}

int usd_Prim_ApplyAPI(usd_Prim_t const* _this, tf_Type_t const* schemaType, bool* _result) {
    *_result = _this->ApplyAPI(*schemaType);
    return 0;
}

int usd_Prim_RemoveAPI(usd_Prim_t const* _this, tf_Type_t const* schemaType, bool* _result) {
    *_result = _this->RemoveAPI(*schemaType);
    return 0;
}

int usd_Prim_GetChild(usd_Prim_t const* _this, tf_Token_t const* name, usd_Prim_t* _result) {
    *_result = _this->GetChild(*name);
    return 0;
}

int usd_Prim_GetChildren(usd_Prim_t const* _this, usd_PrimSiblingRange_t* _result) {
    *_result = _this->GetChildren();
    return 0;
}

int usd_Prim_GetAllChildren(usd_Prim_t const* _this, usd_PrimSiblingRange_t* _result) {
    *_result = _this->GetAllChildren();
    return 0;
}

int usd_Prim_GetFilteredChildren(usd_Prim_t const* _this, usd_PrimFlagsPredicate_t const* pred, usd_PrimSiblingRange_t* _result) {
    *_result = _this->GetFilteredChildren(*pred);
    return 0;
}

int usd_Prim_GetChildrenNames(usd_Prim_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->GetChildrenNames();
    return 0;
}

int usd_Prim_GetFilteredChildrenNames(usd_Prim_t const* _this, usd_PrimFlagsPredicate_t const* predicate, tf_TokenVector_t* _result) {
    *_result = _this->GetFilteredChildrenNames(*predicate);
    return 0;
}

int usd_Prim_GetDescendants(usd_Prim_t const* _this, usd_PrimSubtreeRange_t* _result) {
    *_result = _this->GetDescendants();
    return 0;
}

int usd_Prim_GetAllDescendants(usd_Prim_t const* _this, usd_PrimSubtreeRange_t* _result) {
    *_result = _this->GetAllDescendants();
    return 0;
}

int usd_Prim_GetFilteredDescendants(usd_Prim_t const* _this, usd_PrimFlagsPredicate_t const* pred, usd_PrimSubtreeRange_t* _result) {
    *_result = _this->GetFilteredDescendants(*pred);
    return 0;
}

int usd_Prim_GetChildrenReorder(usd_Prim_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->GetChildrenReorder();
    return 0;
}

int usd_Prim_GetParent(usd_Prim_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetParent();
    return 0;
}

int usd_Prim_GetNextSibling(usd_Prim_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetNextSibling();
    return 0;
}

int usd_Prim_GetFilteredNextSibling(usd_Prim_t const* _this, usd_PrimFlagsPredicate_t const* predicate, usd_Prim_t* _result) {
    *_result = _this->GetFilteredNextSibling(*predicate);
    return 0;
}

int usd_Prim_IsPseudoRoot(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->IsPseudoRoot();
    return 0;
}

int usd_Prim_GetPrimAtPath(usd_Prim_t const* _this, sdf_Path_t const* path, usd_Prim_t* _result) {
    *_result = _this->GetPrimAtPath(*path);
    return 0;
}

int usd_Prim_GetObjectAtPath(usd_Prim_t const* _this, sdf_Path_t const* path, usd_Object_t* _result) {
    *_result = _this->GetObjectAtPath(*path);
    return 0;
}

int usd_Prim_GetPropertyAtPath(usd_Prim_t const* _this, sdf_Path_t const* path, usd_Property_t* _result) {
    *_result = _this->GetPropertyAtPath(*path);
    return 0;
}

int usd_Prim_GetAttributeAtPath(usd_Prim_t const* _this, sdf_Path_t const* path, usd_Attribute_t* _result) {
    *_result = _this->GetAttributeAtPath(*path);
    return 0;
}

int usd_Prim_GetRelationshipAtPath(usd_Prim_t const* _this, sdf_Path_t const* path, usd_Relationship_t* _result) {
    *_result = _this->GetRelationshipAtPath(*path);
    return 0;
}

int usd_Prim_GetVariantSets(usd_Prim_t const* _this, usd_VariantSets_t* _result) {
    *_result = _this->GetVariantSets();
    return 0;
}

int usd_Prim_GetVariantSet(usd_Prim_t const* _this, std_String_t const* variantSetName, usd_VariantSet_t* _result) {
    *_result = _this->GetVariantSet(*variantSetName);
    return 0;
}

int usd_Prim_HasVariantSets(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->HasVariantSets();
    return 0;
}

int usd_Prim_CreateAttribute(usd_Prim_t const* _this, tf_Token_t const* name, sdf_ValueTypeName_t const* typeName, bool custom, int variability, usd_Attribute_t* _result) {
    *_result = _this->CreateAttribute(*name, *typeName, custom, static_cast<PXR_NS::SdfVariability>(variability));
    return 0;
}

int usd_Prim_GetAttributes(usd_Prim_t const* _this, usd_AttributeVector_t* _result) {
    *_result = _this->GetAttributes();
    return 0;
}

int usd_Prim_GetAuthoredAttributes(usd_Prim_t const* _this, usd_AttributeVector_t* _result) {
    *_result = _this->GetAuthoredAttributes();
    return 0;
}

int usd_Prim_GetAttribute(usd_Prim_t const* _this, tf_Token_t const* attrName, usd_Attribute_t* _result) {
    *_result = _this->GetAttribute(*attrName);
    return 0;
}

int usd_Prim_HasAttribute(usd_Prim_t const* _this, tf_Token_t const* attrName, bool* _result) {
    *_result = _this->HasAttribute(*attrName);
    return 0;
}

int usd_Prim_CreateRelationship(usd_Prim_t const* _this, tf_Token_t const* relName, bool custom, usd_Relationship_t* _result) {
    *_result = _this->CreateRelationship(*relName, custom);
    return 0;
}

int usd_Prim_GetRelationships(usd_Prim_t const* _this, usd_RelationshipVector_t* _result) {
    *_result = _this->GetRelationships();
    return 0;
}

int usd_Prim_GetAuthoredRelationships(usd_Prim_t const* _this, usd_RelationshipVector_t* _result) {
    *_result = _this->GetAuthoredRelationships();
    return 0;
}

int usd_Prim_GetRelationship(usd_Prim_t const* _this, tf_Token_t const* relName, usd_Relationship_t* _result) {
    *_result = _this->GetRelationship(*relName);
    return 0;
}

int usd_Prim_HasRelationship(usd_Prim_t const* _this, tf_Token_t const* relName, bool* _result) {
    *_result = _this->HasRelationship(*relName);
    return 0;
}

int usd_Prim_GetPayloads(usd_Prim_t const* _this, usd_Payloads_t* _result) {
    *_result = _this->GetPayloads();
    return 0;
}

int usd_Prim_HasAuthoredPayloads(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredPayloads();
    return 0;
}

int usd_Prim_Load(usd_Prim_t const* _this, int policy) {
    _this->Load(static_cast<PXR_NS::UsdLoadPolicy>(policy));
    return 0;
}

int usd_Prim_Unload(usd_Prim_t const* _this) {
    _this->Unload();
    return 0;
}

int usd_Prim_GetReferences(usd_Prim_t const* _this, usd_References_t* _result) {
    *_result = _this->GetReferences();
    return 0;
}

int usd_Prim_HasAuthoredReferences(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredReferences();
    return 0;
}

int usd_Prim_GetInherits(usd_Prim_t const* _this, usd_Inherits_t* _result) {
    *_result = _this->GetInherits();
    return 0;
}

int usd_Prim_HasAuthoredInherits(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredInherits();
    return 0;
}

int usd_Prim_GetSpecializes(usd_Prim_t const* _this, usd_Specializes_t* _result) {
    *_result = _this->GetSpecializes();
    return 0;
}

int usd_Prim_HasAuthoredSpecializes(usd_Prim_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredSpecializes();
    return 0;
}

int usd_Prim_GetPrimIndex(usd_Prim_t const* _this, pcp_PrimIndex_t const** _result) {
    *_result = &_this->GetPrimIndex();
    return 0;
}

int usd_Prim_ComputeExpandedPrimIndex(usd_Prim_t const* _this, pcp_PrimIndex_t* _result) {
    *_result = _this->ComputeExpandedPrimIndex();
    return 0;
}

int usd_Prim_MakeResolveTargetUpToEditTarget(usd_Prim_t const* _this, usd_EditTarget_t const* editTarget, usd_ResolveTarget_t* _result) {
    *_result = _this->MakeResolveTargetUpToEditTarget(*editTarget);
    return 0;
}

int usd_Prim_MakeResolveTargetStrongerThanEditTarget(usd_Prim_t const* _this, usd_EditTarget_t const* editTarget, usd_ResolveTarget_t* _result) {
    *_result = _this->MakeResolveTargetStrongerThanEditTarget(*editTarget);
    return 0;
}

int usd_Prim_new(usd_Prim_t** _result) {
    *_result = new PXR_NS::UsdPrim();
    return 0;
}

int usd_Prim_dtor(usd_Prim_t* _this) {
    delete _this;
    return 0;
}

int usd_PrimCompositionQuery_SetFilter(usd_PrimCompositionQuery_t* _this, usd_PrimCompositionQueryFilter_t const* filter) {
    _this->SetFilter(*filter);
    return 0;
}

int usd_PrimCompositionQuery_GetFilter(usd_PrimCompositionQuery_t const* _this, usd_PrimCompositionQueryFilter_t* _result) {
    *_result = _this->GetFilter();
    return 0;
}

int usd_PrimCompositionQuery_GetCompositionArcs(usd_PrimCompositionQuery_t* _this, usd_PrimCompositionQueryArcVector_t* _result) {
    *_result = _this->GetCompositionArcs();
    return 0;
}

int usd_PrimCompositionQuery_GetDirectInherits(usd_Prim_t const* prim, usd_PrimCompositionQuery_t* _result) {
    *_result = PXR_NS::UsdPrimCompositionQuery::GetDirectInherits(*prim);
    return 0;
}

int usd_PrimCompositionQuery_GetDirectReferences(usd_Prim_t const* prim, usd_PrimCompositionQuery_t* _result) {
    *_result = PXR_NS::UsdPrimCompositionQuery::GetDirectReferences(*prim);
    return 0;
}

int usd_PrimCompositionQuery_GetDirectRootLayerArcs(usd_Prim_t const* prim, usd_PrimCompositionQuery_t* _result) {
    *_result = PXR_NS::UsdPrimCompositionQuery::GetDirectRootLayerArcs(*prim);
    return 0;
}

int usd_PrimCompositionQuery_ctor(usd_Prim_t const* prim, usd_PrimCompositionQueryFilter_t const* filter, usd_PrimCompositionQuery_t** _result) {
    *_result = new PXR_NS::UsdPrimCompositionQuery(*prim, *filter);
    return 0;
}

int usd_PrimCompositionQuery_dtor(usd_PrimCompositionQuery_t* _this) {
    delete _this;
    return 0;
}

int usd_PrimCompositionQueryArc_GetTargetNode(usd_PrimCompositionQueryArc_t const* _this, pcp_NodeRef_t* _result) {
    *_result = _this->GetTargetNode();
    return 0;
}

int usd_PrimCompositionQueryArc_GetIntroducingNode(usd_PrimCompositionQueryArc_t const* _this, pcp_NodeRef_t* _result) {
    *_result = _this->GetIntroducingNode();
    return 0;
}

int usd_PrimCompositionQueryArc_GetTargetLayer(usd_PrimCompositionQueryArc_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetTargetLayer();
    return 0;
}

int usd_PrimCompositionQueryArc_GetTargetPrimPath(usd_PrimCompositionQueryArc_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetTargetPrimPath();
    return 0;
}

int usd_PrimCompositionQueryArc_MakeResolveTargetUpTo(usd_PrimCompositionQueryArc_t const* _this, sdf_LayerHandle_t const* subLayer, usd_ResolveTarget_t* _result) {
    *_result = _this->MakeResolveTargetUpTo(*subLayer);
    return 0;
}

int usd_PrimCompositionQueryArc_MakeResolveTargetStrongerThan(usd_PrimCompositionQueryArc_t const* _this, sdf_LayerHandle_t const* subLayer, usd_ResolveTarget_t* _result) {
    *_result = _this->MakeResolveTargetStrongerThan(*subLayer);
    return 0;
}

int usd_PrimCompositionQueryArc_GetIntroducingLayer(usd_PrimCompositionQueryArc_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetIntroducingLayer();
    return 0;
}

int usd_PrimCompositionQueryArc_GetIntroducingPrimPath(usd_PrimCompositionQueryArc_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetIntroducingPrimPath();
    return 0;
}

int usd_PrimCompositionQueryArc_GetIntroducingListEditor_reference(usd_PrimCompositionQueryArc_t const* _this, sdf_ReferenceEditorProxy_t* editor, sdf_Reference_t* ref, bool* _result) {
    *_result = _this->GetIntroducingListEditor(editor, ref);
    return 0;
}

int usd_PrimCompositionQueryArc_GetIntroducingListEditor_payload(usd_PrimCompositionQueryArc_t const* _this, sdf_PayloadEditorProxy_t* editor, sdf_Payload_t* payload, bool* _result) {
    *_result = _this->GetIntroducingListEditor(editor, payload);
    return 0;
}

int usd_PrimCompositionQueryArc_GetIntroducingListEditor_path(usd_PrimCompositionQueryArc_t const* _this, sdf_InheritsProxy_t* editor, sdf_Path_t* path, bool* _result) {
    *_result = _this->GetIntroducingListEditor(editor, path);
    return 0;
}

int usd_PrimCompositionQueryArc_GetIntroducingListEditor_name(usd_PrimCompositionQueryArc_t const* _this, sdf_NameEditorProxy_t* editor, std_String_t* name, bool* _result) {
    *_result = _this->GetIntroducingListEditor(editor, name);
    return 0;
}

int usd_PrimCompositionQueryArc_GetArcType(usd_PrimCompositionQueryArc_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetArcType());
    return 0;
}

int usd_PrimCompositionQueryArc_IsImplicit(usd_PrimCompositionQueryArc_t const* _this, bool* _result) {
    *_result = _this->IsImplicit();
    return 0;
}

int usd_PrimCompositionQueryArc_IsAncestral(usd_PrimCompositionQueryArc_t const* _this, bool* _result) {
    *_result = _this->IsAncestral();
    return 0;
}

int usd_PrimCompositionQueryArc_HasSpecs(usd_PrimCompositionQueryArc_t const* _this, bool* _result) {
    *_result = _this->HasSpecs();
    return 0;
}

int usd_PrimCompositionQueryArc_IsIntroducedInRootLayerStack(usd_PrimCompositionQueryArc_t const* _this, bool* _result) {
    *_result = _this->IsIntroducedInRootLayerStack();
    return 0;
}

int usd_PrimCompositionQueryArc_IsIntroducedInRootLayerPrimSpec(usd_PrimCompositionQueryArc_t const* _this, bool* _result) {
    *_result = _this->IsIntroducedInRootLayerPrimSpec();
    return 0;
}

int usd_PrimCompositionQueryArc_dtor(usd_PrimCompositionQueryArc_t* _this) {
    delete _this;
    return 0;
}

int usd_PrimCompositionQueryArcVector_dtor(usd_PrimCompositionQueryArcVector_t* _this) {
    delete _this;
    return 0;
}

int usd_PrimDefinition_GetPropertyNames(usd_PrimDefinition_t const* _this, tf_TokenVector_t const** _result) {
    *_result = &_this->GetPropertyNames();
    return 0;
}

int usd_PrimDefinition_GetAppliedAPISchemas(usd_PrimDefinition_t const* _this, tf_TokenVector_t const** _result) {
    *_result = &_this->GetAppliedAPISchemas();
    return 0;
}

int usd_PrimDefinition_GetSpecType(usd_PrimDefinition_t const* _this, tf_Token_t const* propName, int* _result) {
    *_result = static_cast<int>(_this->GetSpecType(*propName));
    return 0;
}

int usd_PrimDefinition_GetSchemaPropertySpec(usd_PrimDefinition_t const* _this, tf_Token_t const* propName, sdf_PropertySpecHandle_t* _result) {
    *_result = _this->GetSchemaPropertySpec(*propName);
    return 0;
}

int usd_PrimDefinition_GetSchemaAttributeSpec(usd_PrimDefinition_t const* _this, tf_Token_t const* attrName, sdf_AttributeSpecHandle_t* _result) {
    *_result = _this->GetSchemaAttributeSpec(*attrName);
    return 0;
}

int usd_PrimDefinition_GetSchemaRelationshipSpec(usd_PrimDefinition_t const* _this, tf_Token_t const* relName, sdf_RelationshipSpecHandle_t* _result) {
    *_result = _this->GetSchemaRelationshipSpec(*relName);
    return 0;
}

int usd_PrimDefinition_ListMetadataFields(usd_PrimDefinition_t const* _this, tf_TokenVector_t* _result) {
    *_result = _this->ListMetadataFields();
    return 0;
}

int usd_PrimDefinition_GetDocumentation(usd_PrimDefinition_t const* _this, std_String_t* _result) {
    *_result = _this->GetDocumentation();
    return 0;
}

int usd_PrimDefinition_ListPropertyMetadataFields(usd_PrimDefinition_t const* _this, tf_Token_t const* propName, tf_TokenVector_t* _result) {
    *_result = _this->ListPropertyMetadataFields(*propName);
    return 0;
}

int usd_PrimDefinition_GetPropertyDocumentation(usd_PrimDefinition_t const* _this, tf_Token_t const* propName, std_String_t* _result) {
    *_result = _this->GetPropertyDocumentation(*propName);
    return 0;
}

int usd_PrimDefinition_FlattenTo(usd_PrimDefinition_t const* _this, sdf_LayerHandle_t const* layer, sdf_Path_t const* path, int newSpecSpecifier, bool* _result) {
    *_result = _this->FlattenTo(*layer, *path, static_cast<PXR_NS::SdfSpecifier>(newSpecSpecifier));
    return 0;
}

int usd_PrimDefinition_FlattenTo_prim_under_parent(usd_PrimDefinition_t const* _this, usd_Prim_t const* parent, tf_Token_t const* name, int newSpecSpecifier, usd_Prim_t* _result) {
    *_result = _this->FlattenTo(*parent, *name, static_cast<PXR_NS::SdfSpecifier>(newSpecSpecifier));
    return 0;
}

int usd_PrimDefinition_FlattenTo_prim(usd_PrimDefinition_t const* _this, usd_Prim_t const* prim, int newSpecSpecifier, usd_Prim_t* _result) {
    *_result = _this->FlattenTo(*prim, static_cast<PXR_NS::SdfSpecifier>(newSpecSpecifier));
    return 0;
}

int usd_PrimDefinition_dtor(usd_PrimDefinition_t* _this) {
    delete _this;
    return 0;
}

int usd_PrimFlagsPredicate_dtor(usd_PrimFlagsPredicate_t* _this) {
    delete _this;
    return 0;
}

int usd_PrimRange_begin(usd_PrimRange_t const* _this, usd_PrimRangeIterator_t* _result) {
    *_result = _this->begin();
    return 0;
}

int usd_PrimRange_end(usd_PrimRange_t const* _this, usd_PrimRangeIterator_t* _result) {
    *_result = _this->end();
    return 0;
}

int usd_PrimRange_from_prim(usd_Prim_t* start, usd_PrimRange_t** _result) {
    *_result = new PXR_NS::UsdPrimRange(*start);
    return 0;
}

int usd_PrimRange_dtor(usd_PrimRange_t* _this) {
    delete _this;
    return 0;
}

int usd_PrimRangeIterator_op_inc(usd_PrimRangeIterator_t* _this, usd_PrimRangeIterator_t** _result) {
    *_result = &_this->operator++();
    return 0;
}

int usd_PrimRangeIterator_op_assign(usd_PrimRangeIterator_t* _this, usd_PrimRangeIterator_t const* param00, usd_PrimRangeIterator_t** _result) {
    *_result = &_this->operator=(*param00);
    return 0;
}

int usd_PrimRangeIterator_op_eq(usd_PrimRangeIterator_t const* _this, usd_PrimRangeIterator_t const* other, bool* _result) {
    *_result = _this->operator==(*other);
    return 0;
}

int usd_PrimRangeIterator_deref(usd_PrimRangeIterator_t const* _this, usd_Prim_t* _result) {
    *_result = _this->operator*();
    return 0;
}

int usd_PrimRangeIterator_new(usd_PrimRangeIterator_t** _result) {
    *_result = new PXR_NS::UsdPrimRange::iterator();
    return 0;
}

int usd_PrimRangeIterator_dtor(usd_PrimRangeIterator_t* _this) {
    delete _this;
    return 0;
}

int usd_PrimSiblingRange_begin(usd_PrimSiblingRange_t const* _this, usd_PrimSiblingIterator_t* _result) {
    *_result = _this->begin();
    return 0;
}

int usd_PrimSiblingRange_end(usd_PrimSiblingRange_t const* _this, usd_PrimSiblingIterator_t* _result) {
    *_result = _this->end();
    return 0;
}

int usd_PrimSiblingRange_dtor(usd_PrimSiblingRange_t* _this) {
    delete _this;
    return 0;
}

int usd_PrimSiblingIterator_deref(usd_PrimSiblingIterator_t const* _this, usd_Prim_t* _result) {
    *_result = _this->operator*();
    return 0;
}

int usd_PrimSiblingIterator_op_inc(usd_PrimSiblingIterator_t* _this, usd_PrimSiblingIterator_t** _result) {
    *_result = &_this->operator++();
    return 0;
}

int usd_PrimSiblingIterator_dtor(usd_PrimSiblingIterator_t* _this) {
    delete _this;
    return 0;
}

int usd_PrimSubtreeRange_dtor(usd_PrimSubtreeRange_t* _this) {
    delete _this;
    return 0;
}

int usd_PrimTypeInfo_GetTypeName(usd_PrimTypeInfo_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetTypeName();
    return 0;
}

int usd_PrimTypeInfo_GetAppliedAPISchemas(usd_PrimTypeInfo_t const* _this, tf_TokenVector_t const** _result) {
    *_result = &_this->GetAppliedAPISchemas();
    return 0;
}

int usd_PrimTypeInfo_GetSchemaType(usd_PrimTypeInfo_t const* _this, tf_Type_t const** _result) {
    *_result = &_this->GetSchemaType();
    return 0;
}

int usd_PrimTypeInfo_GetSchemaTypeName(usd_PrimTypeInfo_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetSchemaTypeName();
    return 0;
}

int usd_PrimTypeInfo_GetPrimDefinition(usd_PrimTypeInfo_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = &_this->GetPrimDefinition();
    return 0;
}

int usd_PrimTypeInfo_GetEmptyPrimType(usd_PrimTypeInfo_t const** _result) {
    *_result = &PXR_NS::UsdPrimTypeInfo::GetEmptyPrimType();
    return 0;
}

int usd_PrimTypeInfo_dtor(usd_PrimTypeInfo_t* _this) {
    delete _this;
    return 0;
}

int usd_PrimVector_data(usd_PrimVector_t* _this, usd_Prim_t** _result) {
    *_result = _this->data();
    return 0;
}

int usd_PrimVector_data_const(usd_PrimVector_t const* _this, usd_Prim_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usd_PrimVector_empty(usd_PrimVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usd_PrimVector_size(usd_PrimVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usd_PrimVector_max_size(usd_PrimVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usd_PrimVector_reserve(usd_PrimVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usd_PrimVector_capacity(usd_PrimVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usd_PrimVector_clear(usd_PrimVector_t* _this) {
    _this->clear();
    return 0;
}

int usd_PrimVector_push_back(usd_PrimVector_t* _this, usd_Prim_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usd_PrimVector_pop_back(usd_PrimVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usd_PrimVector_resize(usd_PrimVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usd_PrimVector_resize_with(usd_PrimVector_t* _this, size_t const _Newsize, usd_Prim_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usd_PrimVector_op_index(usd_PrimVector_t const* _this, size_t const _Pos, usd_Prim_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int usd_PrimVector_default(usd_PrimVector_t** _result) {
    *_result = new std::vector<PXR_NS::UsdPrim>();
    return 0;
}

int usd_PrimVector_dtor(usd_PrimVector_t* _this) {
    delete _this;
    return 0;
}

int usd_ClipsAPI_IsConcrete(usd_ClipsAPI_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usd_ClipsAPI_IsTyped(usd_ClipsAPI_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usd_ClipsAPI_IsAPISchema(usd_ClipsAPI_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usd_ClipsAPI_IsAppliedAPISchema(usd_ClipsAPI_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usd_ClipsAPI_IsMultipleApplyAPISchema(usd_ClipsAPI_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usd_ClipsAPI_GetSchemaKind(usd_ClipsAPI_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usd_ClipsAPI_GetPrim(usd_ClipsAPI_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usd_ClipsAPI_GetPath(usd_ClipsAPI_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usd_ClipsAPI_GetSchemaClassPrimDefinition(usd_ClipsAPI_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usd_ClipsAPI_Get(usd_StageWeakPtr_t const* stage, sdf_Path_t const* path, usd_ClipsAPI_t* _result) {
    *_result = PXR_NS::UsdClipsAPI::Get(*stage, *path);
    return 0;
}

int usd_ClipsAPI_GetSchemaAttributeNames(bool includeInherited, tf_TokenVector_t const** _result) {
    *_result = &PXR_NS::UsdClipsAPI::GetSchemaAttributeNames(includeInherited);
    return 0;
}

int usd_ClipsAPI_GetClips(usd_ClipsAPI_t const* _this, vt_Dictionary_t* clips, bool* _result) {
    *_result = _this->GetClips(clips);
    return 0;
}

int usd_ClipsAPI_SetClips(usd_ClipsAPI_t* _this, vt_Dictionary_t const* clips, bool* _result) {
    *_result = _this->SetClips(*clips);
    return 0;
}

int usd_ClipsAPI_GetClipSets(usd_ClipsAPI_t const* _this, sdf_StringListOp_t* clipSets, bool* _result) {
    *_result = _this->GetClipSets(clipSets);
    return 0;
}

int usd_ClipsAPI_SetClipSets(usd_ClipsAPI_t* _this, sdf_StringListOp_t const* clipSets, bool* _result) {
    *_result = _this->SetClipSets(*clipSets);
    return 0;
}

int usd_ClipsAPI_ComputeClipAssetPaths_on_clip_set(usd_ClipsAPI_t const* _this, std_String_t const* clipSet, sdf_AssetPathArray_t* _result) {
    *_result = _this->ComputeClipAssetPaths(*clipSet);
    return 0;
}

int usd_ClipsAPI_ComputeClipAssetPaths(usd_ClipsAPI_t const* _this, sdf_AssetPathArray_t* _result) {
    *_result = _this->ComputeClipAssetPaths();
    return 0;
}

int usd_ClipsAPI_GetClipAssetPaths_from_clip_set(usd_ClipsAPI_t const* _this, sdf_AssetPathArray_t* assetPaths, std_String_t const* clipSet, bool* _result) {
    *_result = _this->GetClipAssetPaths(assetPaths, *clipSet);
    return 0;
}

int usd_ClipsAPI_GetClipAssetPaths(usd_ClipsAPI_t const* _this, sdf_AssetPathArray_t* assetPaths, bool* _result) {
    *_result = _this->GetClipAssetPaths(assetPaths);
    return 0;
}

int usd_ClipsAPI_SetClipAssetPaths_on_clip_set(usd_ClipsAPI_t* _this, sdf_AssetPathArray_t const* assetPaths, std_String_t const* clipSet, bool* _result) {
    *_result = _this->SetClipAssetPaths(*assetPaths, *clipSet);
    return 0;
}

int usd_ClipsAPI_SetClipAssetPaths(usd_ClipsAPI_t* _this, sdf_AssetPathArray_t const* assetPaths, bool* _result) {
    *_result = _this->SetClipAssetPaths(*assetPaths);
    return 0;
}

int usd_ClipsAPI_GetClipPrimPath_from_clip_set(usd_ClipsAPI_t const* _this, std_String_t* primPath, std_String_t const* clipSet, bool* _result) {
    *_result = _this->GetClipPrimPath(primPath, *clipSet);
    return 0;
}

int usd_ClipsAPI_GetClipPrimPath(usd_ClipsAPI_t const* _this, std_String_t* primPath, bool* _result) {
    *_result = _this->GetClipPrimPath(primPath);
    return 0;
}

int usd_ClipsAPI_SetClipPrimPath_from_clip_set(usd_ClipsAPI_t* _this, std_String_t const* primPath, std_String_t const* clipSet, bool* _result) {
    *_result = _this->SetClipPrimPath(*primPath, *clipSet);
    return 0;
}

int usd_ClipsAPI_SetClipPrimPath(usd_ClipsAPI_t* _this, std_String_t const* primPath, bool* _result) {
    *_result = _this->SetClipPrimPath(*primPath);
    return 0;
}

int usd_ClipsAPI_on_schema_obj(usd_Prim_t const* param00, usd_ClipsAPI_t** _result) {
    *_result = new PXR_NS::UsdClipsAPI(*param00);
    return 0;
}

int usd_ClipsAPI_on_prim(usd_Prim_t const* param00, usd_ClipsAPI_t** _result) {
    *_result = new PXR_NS::UsdClipsAPI(*param00);
    return 0;
}

int usd_ClipsAPI_dtor(usd_ClipsAPI_t* _this) {
    delete _this;
    return 0;
}

int usd_CollectionAPI_GetName(usd_CollectionAPI_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetName();
    return 0;
}

int usd_CollectionAPI_GetExpansionRuleAttr(usd_CollectionAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetExpansionRuleAttr();
    return 0;
}

int usd_CollectionAPI_CreateExpansionRuleAttr(usd_CollectionAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateExpansionRuleAttr(*defaultValue, writeSparsely);
    return 0;
}

int usd_CollectionAPI_GetIncludeRootAttr(usd_CollectionAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetIncludeRootAttr();
    return 0;
}

int usd_CollectionAPI_CreateIncludeRootAttr(usd_CollectionAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateIncludeRootAttr(*defaultValue, writeSparsely);
    return 0;
}

int usd_CollectionAPI_GetCollectionAttr(usd_CollectionAPI_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->GetCollectionAttr();
    return 0;
}

int usd_CollectionAPI_CreateCollectionAttr(usd_CollectionAPI_t const* _this, vt_Value_t const* defaultValue, bool writeSparsely, usd_Attribute_t* _result) {
    *_result = _this->CreateCollectionAttr(*defaultValue, writeSparsely);
    return 0;
}

int usd_CollectionAPI_GetIncludesRel(usd_CollectionAPI_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetIncludesRel();
    return 0;
}

int usd_CollectionAPI_CreateIncludesRel(usd_CollectionAPI_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateIncludesRel();
    return 0;
}

int usd_CollectionAPI_GetExcludesRel(usd_CollectionAPI_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->GetExcludesRel();
    return 0;
}

int usd_CollectionAPI_CreateExcludesRel(usd_CollectionAPI_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->CreateExcludesRel();
    return 0;
}

int usd_CollectionAPI_GetCollectionPath(usd_CollectionAPI_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetCollectionPath();
    return 0;
}

int usd_CollectionAPI_ComputeMembershipQuery(usd_CollectionAPI_t const* _this, usd_CollectionMembershipQuery_t* _result) {
    *_result = _this->ComputeMembershipQuery();
    return 0;
}

int usd_CollectionAPI_ComputeMembershipQuery_in(usd_CollectionAPI_t const* _this, usd_CollectionMembershipQuery_t* query) {
    _this->ComputeMembershipQuery(query);
    return 0;
}

int usd_CollectionAPI_HasNoIncludedPaths(usd_CollectionAPI_t const* _this, bool* _result) {
    *_result = _this->HasNoIncludedPaths();
    return 0;
}

int usd_CollectionAPI_Validate(usd_CollectionAPI_t const* _this, std_String_t* reason, bool* _result) {
    *_result = _this->Validate(reason);
    return 0;
}

int usd_CollectionAPI_ResetCollection(usd_CollectionAPI_t const* _this, bool* _result) {
    *_result = _this->ResetCollection();
    return 0;
}

int usd_CollectionAPI_BlockCollection(usd_CollectionAPI_t const* _this, bool* _result) {
    *_result = _this->BlockCollection();
    return 0;
}

int usd_CollectionAPI_IncludePath(usd_CollectionAPI_t const* _this, sdf_Path_t const* pathToInclude, bool* _result) {
    *_result = _this->IncludePath(*pathToInclude);
    return 0;
}

int usd_CollectionAPI_ExcludePath(usd_CollectionAPI_t const* _this, sdf_Path_t const* pathToExclude, bool* _result) {
    *_result = _this->ExcludePath(*pathToExclude);
    return 0;
}

int usd_CollectionAPI_IsConcrete(usd_CollectionAPI_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usd_CollectionAPI_IsTyped(usd_CollectionAPI_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usd_CollectionAPI_IsAPISchema(usd_CollectionAPI_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usd_CollectionAPI_IsAppliedAPISchema(usd_CollectionAPI_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usd_CollectionAPI_IsMultipleApplyAPISchema(usd_CollectionAPI_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usd_CollectionAPI_GetSchemaKind(usd_CollectionAPI_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usd_CollectionAPI_GetPrim(usd_CollectionAPI_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usd_CollectionAPI_GetPath(usd_CollectionAPI_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usd_CollectionAPI_GetSchemaClassPrimDefinition(usd_CollectionAPI_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usd_CollectionAPI_GetSchemaAttributeNames(bool includeInherited, tf_TokenVector_t const** _result) {
    *_result = &PXR_NS::UsdCollectionAPI::GetSchemaAttributeNames(includeInherited);
    return 0;
}

int usd_CollectionAPI_GetSchemaAttributeNames_for_instance(bool includeInherited, tf_Token_t const* instanceName, tf_TokenVector_t* _result) {
    *_result = PXR_NS::UsdCollectionAPI::GetSchemaAttributeNames(includeInherited, *instanceName);
    return 0;
}

int usd_CollectionAPI_Get(usd_StageWeakPtr_t const* stage, sdf_Path_t const* path, usd_CollectionAPI_t* _result) {
    *_result = PXR_NS::UsdCollectionAPI::Get(*stage, *path);
    return 0;
}

int usd_CollectionAPI_Get_from_prim(usd_Prim_t const* prim, tf_Token_t const* name, usd_CollectionAPI_t* _result) {
    *_result = PXR_NS::UsdCollectionAPI::Get(*prim, *name);
    return 0;
}

int usd_CollectionAPI_GetAll(usd_Prim_t const* prim, usd_CollectionAPIVector_t* _result) {
    *_result = PXR_NS::UsdCollectionAPI::GetAll(*prim);
    return 0;
}

int usd_CollectionAPI_IsSchemaPropertyBaseName(tf_Token_t const* baseName, bool* _result) {
    *_result = PXR_NS::UsdCollectionAPI::IsSchemaPropertyBaseName(*baseName);
    return 0;
}

int usd_CollectionAPI_IsCollectionAPIPath(sdf_Path_t const* path, tf_Token_t* name, bool* _result) {
    *_result = PXR_NS::UsdCollectionAPI::IsCollectionAPIPath(*path, name);
    return 0;
}

int usd_CollectionAPI_CanApply(usd_Prim_t const* prim, tf_Token_t const* name, std_String_t* whyNot, bool* _result) {
    *_result = PXR_NS::UsdCollectionAPI::CanApply(*prim, *name, whyNot);
    return 0;
}

int usd_CollectionAPI_Apply(usd_Prim_t const* prim, tf_Token_t const* name, usd_CollectionAPI_t* _result) {
    *_result = PXR_NS::UsdCollectionAPI::Apply(*prim, *name);
    return 0;
}

int usd_CollectionAPI_GetCollection(usd_StageWeakPtr_t const* stage, sdf_Path_t const* collectionPath, usd_CollectionAPI_t* _result) {
    *_result = PXR_NS::UsdCollectionAPI::GetCollection(*stage, *collectionPath);
    return 0;
}

int usd_CollectionAPI_GetCollection_from_prim(usd_Prim_t const* prim, tf_Token_t const* name, usd_CollectionAPI_t* _result) {
    *_result = PXR_NS::UsdCollectionAPI::GetCollection(*prim, *name);
    return 0;
}

int usd_CollectionAPI_GetAllCollections(usd_Prim_t const* prim, usd_CollectionAPIVector_t* _result) {
    *_result = PXR_NS::UsdCollectionAPI::GetAllCollections(*prim);
    return 0;
}

int usd_CollectionAPI_GetNamedCollectionPath(usd_Prim_t const* prim, tf_Token_t const* collectionName, sdf_Path_t* _result) {
    *_result = PXR_NS::UsdCollectionAPI::GetNamedCollectionPath(*prim, *collectionName);
    return 0;
}

int usd_CollectionAPI_ComputeIncludedObjects(usd_CollectionMembershipQuery_t const* query, usd_StageWeakPtr_t const* stage, usd_PrimFlagsPredicate_t const* pred, usd_ObjectSet_t* _result) {
    *_result = PXR_NS::UsdCollectionAPI::ComputeIncludedObjects(*query, *stage, *pred);
    return 0;
}

int usd_CollectionAPI_ComputeIncludedPaths(usd_CollectionMembershipQuery_t const* query, usd_StageWeakPtr_t const* stage, usd_PrimFlagsPredicate_t const* pred, sdf_PathSet_t* _result) {
    *_result = PXR_NS::UsdCollectionAPI::ComputeIncludedPaths(*query, *stage, *pred);
    return 0;
}

int usd_CollectionAPI_CanContainPropertyName(tf_Token_t const* name, bool* _result) {
    *_result = PXR_NS::UsdCollectionAPI::CanContainPropertyName(*name);
    return 0;
}

int usd_CollectionAPI_on_schema_obj(usd_Prim_t const* param00, tf_Token_t const* param01, usd_CollectionAPI_t** _result) {
    *_result = new PXR_NS::UsdCollectionAPI(*param00, *param01);
    return 0;
}

int usd_CollectionAPI_on_prim(usd_Prim_t const* param00, tf_Token_t const* param01, usd_CollectionAPI_t** _result) {
    *_result = new PXR_NS::UsdCollectionAPI(*param00, *param01);
    return 0;
}

int usd_CollectionAPI_dtor(usd_CollectionAPI_t* _this) {
    delete _this;
    return 0;
}

int usd_CollectionAPIVector_data(usd_CollectionAPIVector_t* _this, usd_CollectionAPI_t** _result) {
    *_result = _this->data();
    return 0;
}

int usd_CollectionAPIVector_data_const(usd_CollectionAPIVector_t const* _this, usd_CollectionAPI_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usd_CollectionAPIVector_empty(usd_CollectionAPIVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usd_CollectionAPIVector_size(usd_CollectionAPIVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usd_CollectionAPIVector_max_size(usd_CollectionAPIVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usd_CollectionAPIVector_reserve(usd_CollectionAPIVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usd_CollectionAPIVector_capacity(usd_CollectionAPIVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usd_CollectionAPIVector_clear(usd_CollectionAPIVector_t* _this) {
    _this->clear();
    return 0;
}

int usd_CollectionAPIVector_push_back(usd_CollectionAPIVector_t* _this, usd_CollectionAPI_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usd_CollectionAPIVector_pop_back(usd_CollectionAPIVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usd_CollectionAPIVector_resize(usd_CollectionAPIVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usd_CollectionAPIVector_resize_with(usd_CollectionAPIVector_t* _this, size_t const _Newsize, usd_CollectionAPI_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usd_CollectionAPIVector_op_index(usd_CollectionAPIVector_t const* _this, size_t const _Pos, usd_CollectionAPI_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int usd_CollectionAPIVector_default(usd_CollectionAPIVector_t** _result) {
    *_result = new std::vector<PXR_NS::UsdCollectionAPI>();
    return 0;
}

int usd_CollectionAPIVector_dtor(usd_CollectionAPIVector_t* _this) {
    delete _this;
    return 0;
}

int usd_CollectionMembershipQuery_IsPathIncluded(usd_CollectionMembershipQuery_t const* _this, sdf_Path_t const* path, tf_Token_t* expansionRule, bool* _result) {
    *_result = _this->IsPathIncluded(*path, expansionRule);
    return 0;
}

int usd_CollectionMembershipQuery_IsPathIncluded_with_parent_expansion_rule(usd_CollectionMembershipQuery_t const* _this, sdf_Path_t const* path, tf_Token_t const* parentExpansionRule, tf_Token_t* expansionRule, bool* _result) {
    *_result = _this->IsPathIncluded(*path, *parentExpansionRule, expansionRule);
    return 0;
}

int usd_CollectionMembershipQuery_HasExcludes(usd_CollectionMembershipQuery_t const* _this, bool* _result) {
    *_result = _this->HasExcludes();
    return 0;
}

int usd_CollectionMembershipQuery_op_eq(usd_CollectionMembershipQuery_t const* _this, usd_CollectionMembershipQuery_t const* rhs, bool* _result) {
    *_result = _this->operator==(*rhs);
    return 0;
}

int usd_CollectionMembershipQuery_GetAsPathExpansionRuleMap(usd_CollectionMembershipQuery_t const* _this, usd_CollectionMembershipQueryPathExpansionRuleMap_t const** _result) {
    *_result = &_this->GetAsPathExpansionRuleMap();
    return 0;
}

int usd_CollectionMembershipQuery_GetIncludedCollections(usd_CollectionMembershipQuery_t const* _this, sdf_PathSet_t const** _result) {
    *_result = &_this->GetIncludedCollections();
    return 0;
}

int usd_CollectionMembershipQuery_default(usd_CollectionMembershipQuery_t** _result) {
    *_result = new PXR_NS::UsdCollectionMembershipQuery();
    return 0;
}

int usd_CollectionMembershipQuery_dtor(usd_CollectionMembershipQuery_t* _this) {
    delete _this;
    return 0;
}

int usd_CollectionMembershipQueryPathExpansionRuleMap_at(usd_CollectionMembershipQueryPathExpansionRuleMap_t* _this, sdf_Path_t const* _Keyval, tf_Token_t** _result) {
    *_result = &_this->at(*_Keyval);
    return 0;
}

int usd_CollectionMembershipQueryPathExpansionRuleMap_default(usd_CollectionMembershipQueryPathExpansionRuleMap_t** _result) {
    *_result = new PXR_NS::UsdCollectionMembershipQuery::PathExpansionRuleMap();
    return 0;
}

int usd_CollectionMembershipQueryPathExpansionRuleMap_dtor(usd_CollectionMembershipQueryPathExpansionRuleMap_t* _this) {
    delete _this;
    return 0;
}

int usd_ModelAPI_IsConcrete(usd_ModelAPI_t const* _this, bool* _result) {
    *_result = _this->IsConcrete();
    return 0;
}

int usd_ModelAPI_IsTyped(usd_ModelAPI_t const* _this, bool* _result) {
    *_result = _this->IsTyped();
    return 0;
}

int usd_ModelAPI_IsAPISchema(usd_ModelAPI_t const* _this, bool* _result) {
    *_result = _this->IsAPISchema();
    return 0;
}

int usd_ModelAPI_IsAppliedAPISchema(usd_ModelAPI_t const* _this, bool* _result) {
    *_result = _this->IsAppliedAPISchema();
    return 0;
}

int usd_ModelAPI_IsMultipleApplyAPISchema(usd_ModelAPI_t const* _this, bool* _result) {
    *_result = _this->IsMultipleApplyAPISchema();
    return 0;
}

int usd_ModelAPI_GetSchemaKind(usd_ModelAPI_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSchemaKind());
    return 0;
}

int usd_ModelAPI_GetPrim(usd_ModelAPI_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usd_ModelAPI_GetPath(usd_ModelAPI_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usd_ModelAPI_GetSchemaClassPrimDefinition(usd_ModelAPI_t const* _this, usd_PrimDefinition_t const** _result) {
    *_result = _this->GetSchemaClassPrimDefinition();
    return 0;
}

int usd_ModelAPI_GetAssetIdentifier(usd_ModelAPI_t const* _this, sdf_AssetPath_t* identifier, bool* _result) {
    *_result = _this->GetAssetIdentifier(identifier);
    return 0;
}

int usd_ModelAPI_SetAssetIdentifier(usd_ModelAPI_t const* _this, sdf_AssetPath_t const* identifier) {
    _this->SetAssetIdentifier(*identifier);
    return 0;
}

int usd_ModelAPI_GetAssetName(usd_ModelAPI_t const* _this, std_String_t* assetName, bool* _result) {
    *_result = _this->GetAssetName(assetName);
    return 0;
}

int usd_ModelAPI_SetAssetName(usd_ModelAPI_t const* _this, std_String_t const* assetName) {
    _this->SetAssetName(*assetName);
    return 0;
}

int usd_ModelAPI_GetAssetVersion(usd_ModelAPI_t const* _this, std_String_t* version, bool* _result) {
    *_result = _this->GetAssetVersion(version);
    return 0;
}

int usd_ModelAPI_SetAssetVersion(usd_ModelAPI_t const* _this, std_String_t const* version) {
    _this->SetAssetVersion(*version);
    return 0;
}

int usd_ModelAPI_GetPayloadAssetDependencies(usd_ModelAPI_t const* _this, sdf_AssetPathArray_t* assetDeps, bool* _result) {
    *_result = _this->GetPayloadAssetDependencies(assetDeps);
    return 0;
}

int usd_ModelAPI_SetPayloadAssetDependencies(usd_ModelAPI_t const* _this, sdf_AssetPathArray_t const* assetDeps) {
    _this->SetPayloadAssetDependencies(*assetDeps);
    return 0;
}

int usd_ModelAPI_GetAssetInfo(usd_ModelAPI_t const* _this, vt_Dictionary_t* info, bool* _result) {
    *_result = _this->GetAssetInfo(info);
    return 0;
}

int usd_ModelAPI_SetAssetInfo(usd_ModelAPI_t const* _this, vt_Dictionary_t const* info) {
    _this->SetAssetInfo(*info);
    return 0;
}

int usd_ModelAPI_Get(usd_StageWeakPtr_t const* stage, sdf_Path_t const* path, usd_ModelAPI_t* _result) {
    *_result = PXR_NS::UsdModelAPI::Get(*stage, *path);
    return 0;
}

int usd_ModelAPI_GetSchemaAttributeNames(bool includeInherited, tf_TokenVector_t const** _result) {
    *_result = &PXR_NS::UsdModelAPI::GetSchemaAttributeNames(includeInherited);
    return 0;
}

int usd_ModelAPI_GetKind(usd_ModelAPI_t const* _this, tf_Token_t* kind, bool* _result) {
    *_result = _this->GetKind(kind);
    return 0;
}

int usd_ModelAPI_SetKind(usd_ModelAPI_t const* _this, tf_Token_t const* kind, bool* _result) {
    *_result = _this->SetKind(*kind);
    return 0;
}

int usd_ModelAPI_IsKind(usd_ModelAPI_t const* _this, tf_Token_t const* baseKind, int validation, bool* _result) {
    *_result = _this->IsKind(*baseKind, static_cast<PXR_NS::UsdModelAPI::KindValidation>(validation));
    return 0;
}

int usd_ModelAPI_IsModel(usd_ModelAPI_t const* _this, bool* _result) {
    *_result = _this->IsModel();
    return 0;
}

int usd_ModelAPI_IsGroup(usd_ModelAPI_t const* _this, bool* _result) {
    *_result = _this->IsGroup();
    return 0;
}

int usd_ModelAPI_on_schema_obj(usd_Prim_t const* param00, usd_ModelAPI_t** _result) {
    *_result = new PXR_NS::UsdModelAPI(*param00);
    return 0;
}

int usd_ModelAPI_on_prim(usd_Prim_t const* param00, usd_ModelAPI_t** _result) {
    *_result = new PXR_NS::UsdModelAPI(*param00);
    return 0;
}

int usd_ModelAPI_dtor(usd_ModelAPI_t* _this) {
    delete _this;
    return 0;
}

int usd_Attribute_GetVariability(usd_Attribute_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetVariability());
    return 0;
}

int usd_Attribute_SetVariability(usd_Attribute_t const* _this, int variability, bool* _result) {
    *_result = _this->SetVariability(static_cast<PXR_NS::SdfVariability>(variability));
    return 0;
}

int usd_Attribute_GetTypeName(usd_Attribute_t const* _this, sdf_ValueTypeName_t* _result) {
    *_result = _this->GetTypeName();
    return 0;
}

int usd_Attribute_SetTypeName(usd_Attribute_t const* _this, sdf_ValueTypeName_t const* typeName, bool* _result) {
    *_result = _this->SetTypeName(*typeName);
    return 0;
}

int usd_Attribute_GetRoleName(usd_Attribute_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetRoleName();
    return 0;
}

int usd_Attribute_AddConnection(usd_Attribute_t const* _this, sdf_Path_t const* source, int position, bool* _result) {
    *_result = _this->AddConnection(*source, static_cast<PXR_NS::UsdListPosition>(position));
    return 0;
}

int usd_Attribute_RemoveConnection(usd_Attribute_t const* _this, sdf_Path_t const* source, bool* _result) {
    *_result = _this->RemoveConnection(*source);
    return 0;
}

int usd_Attribute_SetConnections(usd_Attribute_t const* _this, sdf_PathVector_t const* sources, bool* _result) {
    *_result = _this->SetConnections(*sources);
    return 0;
}

int usd_Attribute_ClearConnections(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->ClearConnections();
    return 0;
}

int usd_Attribute_GetConnections(usd_Attribute_t const* _this, sdf_PathVector_t* sources, bool* _result) {
    *_result = _this->GetConnections(sources);
    return 0;
}

int usd_Attribute_HasAuthoredConnections(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredConnections();
    return 0;
}

int usd_Attribute_GetColorSpace(usd_Attribute_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetColorSpace();
    return 0;
}

int usd_Attribute_SetColorSpace(usd_Attribute_t const* _this, tf_Token_t const* colorSpace) {
    _this->SetColorSpace(*colorSpace);
    return 0;
}

int usd_Attribute_HasColorSpace(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasColorSpace();
    return 0;
}

int usd_Attribute_ClearColorSpace(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->ClearColorSpace();
    return 0;
}

int usd_Attribute_GetTimeSamples(usd_Attribute_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usd_Attribute_GetTimeSamplesInInterval(usd_Attribute_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usd_Attribute_GetNumTimeSamples(usd_Attribute_t const* _this, size_t* _result) {
    *_result = _this->GetNumTimeSamples();
    return 0;
}

int usd_Attribute_GetBracketingTimeSamples(usd_Attribute_t const* _this, double desiredTime, double* lower, double* upper, bool* hasTimeSamples, bool* _result) {
    *_result = _this->GetBracketingTimeSamples(desiredTime, lower, upper, hasTimeSamples);
    return 0;
}

int usd_Attribute_HasValue(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasValue();
    return 0;
}

int usd_Attribute_HasAuthoredValueOpinion(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredValueOpinion();
    return 0;
}

int usd_Attribute_HasAuthoredValue(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredValue();
    return 0;
}

int usd_Attribute_HasFallbackValue(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasFallbackValue();
    return 0;
}

int usd_Attribute_ValueMightBeTimeVarying(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->ValueMightBeTimeVarying();
    return 0;
}

int usd_Attribute_Get(usd_Attribute_t const* _this, vt_Value_t* value, usd_TimeCode_t time, bool* _result) {
    *_result = _this->Get(value, time);
    return 0;
}

int usd_Attribute_GetResolveInfo_at_time(usd_Attribute_t const* _this, usd_TimeCode_t time, usd_ResolveInfo_t* _result) {
    *_result = _this->GetResolveInfo(time);
    return 0;
}

int usd_Attribute_GetResolveInfo(usd_Attribute_t const* _this, usd_ResolveInfo_t* _result) {
    *_result = _this->GetResolveInfo();
    return 0;
}

int usd_Attribute_Set(usd_Attribute_t const* _this, vt_Value_t const* value, usd_TimeCode_t time, bool* _result) {
    *_result = _this->Set(*value, time);
    return 0;
}

int usd_Attribute_Clear(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->Clear();
    return 0;
}

int usd_Attribute_ClearAtTime(usd_Attribute_t const* _this, usd_TimeCode_t time, bool* _result) {
    *_result = _this->ClearAtTime(time);
    return 0;
}

int usd_Attribute_ClearDefault(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->ClearDefault();
    return 0;
}

int usd_Attribute_Block(usd_Attribute_t const* _this) {
    _this->Block();
    return 0;
}

int usd_Attribute_GetUnionedTimeSamples(usd_AttributeVector_t const* attrs, std_DoubleVector_t* times, bool* _result) {
    *_result = PXR_NS::UsdAttribute::GetUnionedTimeSamples(*attrs, times);
    return 0;
}

int usd_Attribute_GetUnionedTimeSamplesInInterval(usd_AttributeVector_t const* attrs, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = PXR_NS::UsdAttribute::GetUnionedTimeSamplesInInterval(*attrs, *interval, times);
    return 0;
}

int usd_Attribute_GetPropertyStack(usd_Attribute_t const* _this, usd_TimeCode_t time, sdf_PropertySpecHandleVector_t* _result) {
    *_result = _this->GetPropertyStack(time);
    return 0;
}

int usd_Attribute_GetPropertyStackWithLayerOffsets(usd_Attribute_t const* _this, usd_TimeCode_t time, sdf_PropertySpecHandleOffsetPairVector_t* _result) {
    *_result = _this->GetPropertyStackWithLayerOffsets(time);
    return 0;
}

int usd_Attribute_GetBaseName(usd_Attribute_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetBaseName();
    return 0;
}

int usd_Attribute_GetNamespace(usd_Attribute_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNamespace();
    return 0;
}

int usd_Attribute_SplitName(usd_Attribute_t const* _this, std_StringVector_t* _result) {
    *_result = _this->SplitName();
    return 0;
}

int usd_Attribute_GetDisplayGroup(usd_Attribute_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayGroup();
    return 0;
}

int usd_Attribute_SetDisplayGroup(usd_Attribute_t const* _this, std_String_t const* displayGroup, bool* _result) {
    *_result = _this->SetDisplayGroup(*displayGroup);
    return 0;
}

int usd_Attribute_ClearDisplayGroup(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->ClearDisplayGroup();
    return 0;
}

int usd_Attribute_HasAuthoredDisplayGroup(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredDisplayGroup();
    return 0;
}

int usd_Attribute_GetNestedDisplayGroups(usd_Attribute_t const* _this, std_StringVector_t* _result) {
    *_result = _this->GetNestedDisplayGroups();
    return 0;
}

int usd_Attribute_SetNestedDisplayGroups(usd_Attribute_t const* _this, std_StringVector_t const* nestedGroups, bool* _result) {
    *_result = _this->SetNestedDisplayGroups(*nestedGroups);
    return 0;
}

int usd_Attribute_IsCustom(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->IsCustom();
    return 0;
}

int usd_Attribute_SetCustom(usd_Attribute_t const* _this, bool isCustom, bool* _result) {
    *_result = _this->SetCustom(isCustom);
    return 0;
}

int usd_Attribute_IsDefined(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->IsDefined();
    return 0;
}

int usd_Attribute_IsAuthored(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->IsAuthored();
    return 0;
}

int usd_Attribute_IsAuthoredAt(usd_Attribute_t const* _this, usd_EditTarget_t const* editTarget, bool* _result) {
    *_result = _this->IsAuthoredAt(*editTarget);
    return 0;
}

int usd_Attribute_FlattenTo_prim(usd_Attribute_t const* _this, usd_Prim_t const* parent, usd_Property_t* _result) {
    *_result = _this->FlattenTo(*parent);
    return 0;
}

int usd_Attribute_FlattenTo_named_property(usd_Attribute_t const* _this, usd_Prim_t const* parent, tf_Token_t const* propName, usd_Property_t* _result) {
    *_result = _this->FlattenTo(*parent, *propName);
    return 0;
}

int usd_Attribute_FlattenTo_property(usd_Attribute_t const* _this, usd_Property_t const* property, usd_Property_t* _result) {
    *_result = _this->FlattenTo(*property);
    return 0;
}

int usd_Attribute_GetMetadata_value(usd_Attribute_t const* _this, tf_Token_t const* key, vt_Value_t* value, bool* _result) {
    *_result = _this->GetMetadata(*key, value);
    return 0;
}

int usd_Attribute_SetMetadata_value(usd_Attribute_t const* _this, tf_Token_t const* key, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetMetadata(*key, *value);
    return 0;
}

int usd_Attribute_ClearMetadata(usd_Attribute_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->ClearMetadata(*key);
    return 0;
}

int usd_Attribute_HasMetadata(usd_Attribute_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasMetadata(*key);
    return 0;
}

int usd_Attribute_HasAuthoredMetadata(usd_Attribute_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasAuthoredMetadata(*key);
    return 0;
}

int usd_Attribute_GetMetadataByDictKey_value(usd_Attribute_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, vt_Value_t* value, bool* _result) {
    *_result = _this->GetMetadataByDictKey(*key, *keyPath, value);
    return 0;
}

int usd_Attribute_SetMetadataByDictKey_value(usd_Attribute_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetMetadataByDictKey(*key, *keyPath, *value);
    return 0;
}

int usd_Attribute_ClearMetadataByDictKey(usd_Attribute_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->ClearMetadataByDictKey(*key, *keyPath);
    return 0;
}

int usd_Attribute_HasMetadataDictKey(usd_Attribute_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasMetadataDictKey(*key, *keyPath);
    return 0;
}

int usd_Attribute_HasAuthoredMetadataDictKey(usd_Attribute_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredMetadataDictKey(*key, *keyPath);
    return 0;
}

int usd_Attribute_GetAllMetadata(usd_Attribute_t const* _this, usd_MetadataValueMap_t* _result) {
    *_result = _this->GetAllMetadata();
    return 0;
}

int usd_Attribute_GetAllAuthoredMetadata(usd_Attribute_t const* _this, usd_MetadataValueMap_t* _result) {
    *_result = _this->GetAllAuthoredMetadata();
    return 0;
}

int usd_Attribute_IsHidden(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->IsHidden();
    return 0;
}

int usd_Attribute_SetHidden(usd_Attribute_t const* _this, bool hidden, bool* _result) {
    *_result = _this->SetHidden(hidden);
    return 0;
}

int usd_Attribute_ClearHidden(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->ClearHidden();
    return 0;
}

int usd_Attribute_HasAuthoredHidden(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredHidden();
    return 0;
}

int usd_Attribute_GetCustomData(usd_Attribute_t const* _this, vt_Dictionary_t* _result) {
    *_result = _this->GetCustomData();
    return 0;
}

int usd_Attribute_GetCustomDataByKey(usd_Attribute_t const* _this, tf_Token_t const* keyPath, vt_Value_t* _result) {
    *_result = _this->GetCustomDataByKey(*keyPath);
    return 0;
}

int usd_Attribute_SetCustomData(usd_Attribute_t const* _this, vt_Dictionary_t const* customData) {
    _this->SetCustomData(*customData);
    return 0;
}

int usd_Attribute_SetCustomDataByKey(usd_Attribute_t const* _this, tf_Token_t const* keyPath, vt_Value_t const* value) {
    _this->SetCustomDataByKey(*keyPath, *value);
    return 0;
}

int usd_Attribute_ClearCustomData(usd_Attribute_t const* _this) {
    _this->ClearCustomData();
    return 0;
}

int usd_Attribute_ClearCustomDataByKey(usd_Attribute_t const* _this, tf_Token_t const* keyPath) {
    _this->ClearCustomDataByKey(*keyPath);
    return 0;
}

int usd_Attribute_HasCustomData(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasCustomData();
    return 0;
}

int usd_Attribute_HasCustomDataKey(usd_Attribute_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasCustomDataKey(*keyPath);
    return 0;
}

int usd_Attribute_HasAuthoredCustomData(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredCustomData();
    return 0;
}

int usd_Attribute_HasAuthoredCustomDataKey(usd_Attribute_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredCustomDataKey(*keyPath);
    return 0;
}

int usd_Attribute_GetAssetInfo(usd_Attribute_t const* _this, vt_Dictionary_t* _result) {
    *_result = _this->GetAssetInfo();
    return 0;
}

int usd_Attribute_GetAssetInfoByKey(usd_Attribute_t const* _this, tf_Token_t const* keyPath, vt_Value_t* _result) {
    *_result = _this->GetAssetInfoByKey(*keyPath);
    return 0;
}

int usd_Attribute_SetAssetInfo(usd_Attribute_t const* _this, vt_Dictionary_t const* customData) {
    _this->SetAssetInfo(*customData);
    return 0;
}

int usd_Attribute_SetAssetInfoByKey(usd_Attribute_t const* _this, tf_Token_t const* keyPath, vt_Value_t const* value) {
    _this->SetAssetInfoByKey(*keyPath, *value);
    return 0;
}

int usd_Attribute_ClearAssetInfo(usd_Attribute_t const* _this) {
    _this->ClearAssetInfo();
    return 0;
}

int usd_Attribute_ClearAssetInfoByKey(usd_Attribute_t const* _this, tf_Token_t const* keyPath) {
    _this->ClearAssetInfoByKey(*keyPath);
    return 0;
}

int usd_Attribute_HasAssetInfo(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasAssetInfo();
    return 0;
}

int usd_Attribute_HasAssetInfoKey(usd_Attribute_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAssetInfoKey(*keyPath);
    return 0;
}

int usd_Attribute_HasAuthoredAssetInfo(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredAssetInfo();
    return 0;
}

int usd_Attribute_HasAuthoredAssetInfoKey(usd_Attribute_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredAssetInfoKey(*keyPath);
    return 0;
}

int usd_Attribute_GetDocumentation(usd_Attribute_t const* _this, std_String_t* _result) {
    *_result = _this->GetDocumentation();
    return 0;
}

int usd_Attribute_SetDocumentation(usd_Attribute_t const* _this, std_String_t const* doc, bool* _result) {
    *_result = _this->SetDocumentation(*doc);
    return 0;
}

int usd_Attribute_ClearDocumentation(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->ClearDocumentation();
    return 0;
}

int usd_Attribute_HasAuthoredDocumentation(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredDocumentation();
    return 0;
}

int usd_Attribute_GetDisplayName(usd_Attribute_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayName();
    return 0;
}

int usd_Attribute_SetDisplayName(usd_Attribute_t const* _this, std_String_t const* name, bool* _result) {
    *_result = _this->SetDisplayName(*name);
    return 0;
}

int usd_Attribute_ClearDisplayName(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->ClearDisplayName();
    return 0;
}

int usd_Attribute_HasAuthoredDisplayName(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredDisplayName();
    return 0;
}

int usd_Attribute_IsValid(usd_Attribute_t const* _this, bool* _result) {
    *_result = _this->IsValid();
    return 0;
}

int usd_Attribute_GetStage(usd_Attribute_t const* _this, usd_StageWeakPtr_t* _result) {
    *_result = _this->GetStage();
    return 0;
}

int usd_Attribute_GetPath(usd_Attribute_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usd_Attribute_GetPrimPath(usd_Attribute_t const* _this, sdf_Path_t const** _result) {
    *_result = &_this->GetPrimPath();
    return 0;
}

int usd_Attribute_GetPrim(usd_Attribute_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usd_Attribute_GetName(usd_Attribute_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int usd_Attribute_GetNamespaceDelimiter(char* _result) {
    *_result = PXR_NS::UsdAttribute::GetNamespaceDelimiter();
    return 0;
}

int usd_Attribute_GetDescription(usd_Attribute_t const* _this, std_String_t* _result) {
    *_result = _this->GetDescription();
    return 0;
}

int usd_Attribute_default(usd_Attribute_t** _result) {
    *_result = new PXR_NS::UsdAttribute();
    return 0;
}

int usd_Attribute_dtor(usd_Attribute_t* _this) {
    delete _this;
    return 0;
}

int usd_AttributeQuery_GetAttribute(usd_AttributeQuery_t const* _this, usd_Attribute_t const** _result) {
    *_result = &_this->GetAttribute();
    return 0;
}

int usd_AttributeQuery_IsValid(usd_AttributeQuery_t const* _this, bool* _result) {
    *_result = _this->IsValid();
    return 0;
}

int usd_AttributeQuery_Get_value(usd_AttributeQuery_t const* _this, vt_Value_t* value, usd_TimeCode_t time, bool* _result) {
    *_result = _this->Get(value, time);
    return 0;
}

int usd_AttributeQuery_GetTimeSamples(usd_AttributeQuery_t const* _this, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamples(times);
    return 0;
}

int usd_AttributeQuery_GetTimeSamplesInInterval(usd_AttributeQuery_t const* _this, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = _this->GetTimeSamplesInInterval(*interval, times);
    return 0;
}

int usd_AttributeQuery_GetNumTimeSamples(usd_AttributeQuery_t const* _this, size_t* _result) {
    *_result = _this->GetNumTimeSamples();
    return 0;
}

int usd_AttributeQuery_GetBracketingTimeSamples(usd_AttributeQuery_t const* _this, double desiredTime, double* lower, double* upper, bool* hasTimeSamples, bool* _result) {
    *_result = _this->GetBracketingTimeSamples(desiredTime, lower, upper, hasTimeSamples);
    return 0;
}

int usd_AttributeQuery_HasValue(usd_AttributeQuery_t const* _this, bool* _result) {
    *_result = _this->HasValue();
    return 0;
}

int usd_AttributeQuery_HasAuthoredValueOpinion(usd_AttributeQuery_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredValueOpinion();
    return 0;
}

int usd_AttributeQuery_HasAuthoredValue(usd_AttributeQuery_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredValue();
    return 0;
}

int usd_AttributeQuery_HasFallbackValue(usd_AttributeQuery_t const* _this, bool* _result) {
    *_result = _this->HasFallbackValue();
    return 0;
}

int usd_AttributeQuery_ValueMightBeTimeVarying(usd_AttributeQuery_t const* _this, bool* _result) {
    *_result = _this->ValueMightBeTimeVarying();
    return 0;
}

int usd_AttributeQuery_GetUnionedTimeSamples(usd_AttributeQueryVector_t const* attrQueries, std_DoubleVector_t* times, bool* _result) {
    *_result = PXR_NS::UsdAttributeQuery::GetUnionedTimeSamples(*attrQueries, times);
    return 0;
}

int usd_AttributeQuery_GetUnionedTimeSamplesInInterval(usd_AttributeQueryVector_t const* attrQueries, gf_Interval_t const* interval, std_DoubleVector_t* times, bool* _result) {
    *_result = PXR_NS::UsdAttributeQuery::GetUnionedTimeSamplesInInterval(*attrQueries, *interval, times);
    return 0;
}

int usd_AttributeQuery_from_attribute_and_resolve_target(usd_AttributeQuery_t** _result) {
    *_result = new PXR_NS::UsdAttributeQuery();
    return 0;
}

int usd_AttributeQuery_from_prim_and_attribute_name(usd_AttributeQuery_t** _result) {
    *_result = new PXR_NS::UsdAttributeQuery();
    return 0;
}

int usd_AttributeQuery_default(usd_AttributeQuery_t** _result) {
    *_result = new PXR_NS::UsdAttributeQuery();
    return 0;
}

int usd_AttributeQuery_dtor(usd_AttributeQuery_t* _this) {
    delete _this;
    return 0;
}

int usd_AttributeQueryVector_data(usd_AttributeQueryVector_t* _this, usd_AttributeQuery_t** _result) {
    *_result = _this->data();
    return 0;
}

int usd_AttributeQueryVector_data_const(usd_AttributeQueryVector_t const* _this, usd_AttributeQuery_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usd_AttributeQueryVector_empty(usd_AttributeQueryVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usd_AttributeQueryVector_size(usd_AttributeQueryVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usd_AttributeQueryVector_max_size(usd_AttributeQueryVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usd_AttributeQueryVector_reserve(usd_AttributeQueryVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usd_AttributeQueryVector_capacity(usd_AttributeQueryVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usd_AttributeQueryVector_clear(usd_AttributeQueryVector_t* _this) {
    _this->clear();
    return 0;
}

int usd_AttributeQueryVector_push_back(usd_AttributeQueryVector_t* _this, usd_AttributeQuery_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usd_AttributeQueryVector_pop_back(usd_AttributeQueryVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usd_AttributeQueryVector_resize(usd_AttributeQueryVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usd_AttributeQueryVector_resize_with(usd_AttributeQueryVector_t* _this, size_t const _Newsize, usd_AttributeQuery_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usd_AttributeQueryVector_op_index(usd_AttributeQueryVector_t const* _this, size_t const _Pos, usd_AttributeQuery_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int usd_AttributeQueryVector_default(usd_AttributeQueryVector_t** _result) {
    *_result = new std::vector<PXR_NS::UsdAttributeQuery>();
    return 0;
}

int usd_AttributeQueryVector_dtor(usd_AttributeQueryVector_t* _this) {
    delete _this;
    return 0;
}

int usd_AttributeVector_data(usd_AttributeVector_t* _this, usd_Attribute_t** _result) {
    *_result = _this->data();
    return 0;
}

int usd_AttributeVector_data_const(usd_AttributeVector_t const* _this, usd_Attribute_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usd_AttributeVector_empty(usd_AttributeVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usd_AttributeVector_size(usd_AttributeVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usd_AttributeVector_max_size(usd_AttributeVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usd_AttributeVector_reserve(usd_AttributeVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usd_AttributeVector_capacity(usd_AttributeVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usd_AttributeVector_clear(usd_AttributeVector_t* _this) {
    _this->clear();
    return 0;
}

int usd_AttributeVector_push_back(usd_AttributeVector_t* _this, usd_Attribute_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usd_AttributeVector_pop_back(usd_AttributeVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usd_AttributeVector_resize(usd_AttributeVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usd_AttributeVector_resize_with(usd_AttributeVector_t* _this, size_t const _Newsize, usd_Attribute_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usd_AttributeVector_op_index(usd_AttributeVector_t const* _this, size_t const _Pos, usd_Attribute_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int usd_AttributeVector_default(usd_AttributeVector_t** _result) {
    *_result = new std::vector<PXR_NS::UsdAttribute>();
    return 0;
}

int usd_AttributeVector_dtor(usd_AttributeVector_t* _this) {
    delete _this;
    return 0;
}

int usd_EditTarget_op_eq(usd_EditTarget_t const* _this, usd_EditTarget_t const* other, bool* _result) {
    *_result = _this->operator==(*other);
    return 0;
}

int usd_EditTarget_IsNull(usd_EditTarget_t const* _this, bool* _result) {
    *_result = _this->IsNull();
    return 0;
}

int usd_EditTarget_IsValid(usd_EditTarget_t const* _this, bool* _result) {
    *_result = _this->IsValid();
    return 0;
}

int usd_EditTarget_GetLayer(usd_EditTarget_t const* _this, sdf_LayerHandle_t const** _result) {
    *_result = &_this->GetLayer();
    return 0;
}

int usd_EditTarget_MapToSpecPath(usd_EditTarget_t const* _this, sdf_Path_t const* scenePath, sdf_Path_t* _result) {
    *_result = _this->MapToSpecPath(*scenePath);
    return 0;
}

int usd_EditTarget_GetPrimSpecForScenePath(usd_EditTarget_t const* _this, sdf_Path_t const* scenePath, sdf_PrimSpecHandle_t* _result) {
    *_result = _this->GetPrimSpecForScenePath(*scenePath);
    return 0;
}

int usd_EditTarget_GetPropertySpecForScenePath(usd_EditTarget_t const* _this, sdf_Path_t const* scenePath, sdf_PropertySpecHandle_t* _result) {
    *_result = _this->GetPropertySpecForScenePath(*scenePath);
    return 0;
}

int usd_EditTarget_GetSpecForScenePath(usd_EditTarget_t const* _this, sdf_Path_t const* scenePath, sdf_SpecHandle_t* _result) {
    *_result = _this->GetSpecForScenePath(*scenePath);
    return 0;
}

int usd_EditTarget_GetMapFunction(usd_EditTarget_t const* _this, pcp_MapFunction_t const** _result) {
    *_result = &_this->GetMapFunction();
    return 0;
}

int usd_EditTarget_ComposeOver(usd_EditTarget_t const* _this, usd_EditTarget_t const* weaker, usd_EditTarget_t* _result) {
    *_result = _this->ComposeOver(*weaker);
    return 0;
}

int usd_EditTarget_ForLocalDirectVariant(sdf_LayerHandle_t const* layer, sdf_Path_t const* varSelPath, usd_EditTarget_t* _result) {
    *_result = PXR_NS::UsdEditTarget::ForLocalDirectVariant(*layer, *varSelPath);
    return 0;
}

int usd_EditTarget_from_layer_and_node(usd_EditTarget_t** _result) {
    *_result = new PXR_NS::UsdEditTarget();
    return 0;
}

int usd_EditTarget_from_layer(usd_EditTarget_t** _result) {
    *_result = new PXR_NS::UsdEditTarget();
    return 0;
}

int usd_EditTarget_default(usd_EditTarget_t** _result) {
    *_result = new PXR_NS::UsdEditTarget();
    return 0;
}

int usd_EditTarget_dtor(usd_EditTarget_t* _this) {
    delete _this;
    return 0;
}

int usd_Inherits_AddInherit(usd_Inherits_t* _this, sdf_Path_t const* primPath, int position, bool* _result) {
    *_result = _this->AddInherit(*primPath, static_cast<PXR_NS::UsdListPosition>(position));
    return 0;
}

int usd_Inherits_RemoveInherit(usd_Inherits_t* _this, sdf_Path_t const* primPath, bool* _result) {
    *_result = _this->RemoveInherit(*primPath);
    return 0;
}

int usd_Inherits_ClearInherits(usd_Inherits_t* _this, bool* _result) {
    *_result = _this->ClearInherits();
    return 0;
}

int usd_Inherits_SetInherits(usd_Inherits_t* _this, sdf_PathVector_t const* items, bool* _result) {
    *_result = _this->SetInherits(*items);
    return 0;
}

int usd_Inherits_GetAllDirectInherits(usd_Inherits_t const* _this, sdf_PathVector_t* _result) {
    *_result = _this->GetAllDirectInherits();
    return 0;
}

int usd_Inherits_GetPrim(usd_Inherits_t const* _this, usd_Prim_t const** _result) {
    *_result = &_this->GetPrim();
    return 0;
}

int usd_Inherits_dtor(usd_Inherits_t* _this) {
    delete _this;
    return 0;
}

int usd_MetadataValueMap_at_const(usd_MetadataValueMap_t const* _this, tf_Token_t const* _Keyval, vt_Value_t const** _result) {
    *_result = &_this->at(*_Keyval);
    return 0;
}

int usd_MetadataValueMap_at(usd_MetadataValueMap_t* _this, tf_Token_t const* _Keyval, vt_Value_t** _result) {
    *_result = &_this->at(*_Keyval);
    return 0;
}

int usd_MetadataValueMap_dtor(usd_MetadataValueMap_t* _this) {
    delete _this;
    return 0;
}

int usd_Object_Is_Attribute(usd_Object_t const* _this, bool* _result) {
    *_result = _this->Is<PXR_NS::UsdAttribute>();
    return 0;
}

int usd_Object_Is_Relationship(usd_Object_t const* _this, bool* _result) {
    *_result = _this->Is<PXR_NS::UsdRelationship>();
    return 0;
}

int usd_Object_Is_Property(usd_Object_t const* _this, bool* _result) {
    *_result = _this->Is<PXR_NS::UsdProperty>();
    return 0;
}

int usd_Object_Is_Prim(usd_Object_t const* _this, bool* _result) {
    *_result = _this->Is<PXR_NS::UsdPrim>();
    return 0;
}

int usd_Object_As_Attribute(usd_Object_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->As<PXR_NS::UsdAttribute>();
    return 0;
}

int usd_Object_As_Relationship(usd_Object_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->As<PXR_NS::UsdRelationship>();
    return 0;
}

int usd_Object_As_Property(usd_Object_t const* _this, usd_Property_t* _result) {
    *_result = _this->As<PXR_NS::UsdProperty>();
    return 0;
}

int usd_Object_As_Prim(usd_Object_t const* _this, usd_Prim_t* _result) {
    *_result = _this->As<PXR_NS::UsdPrim>();
    return 0;
}

int usd_Object_GetMetadata_value(usd_Object_t const* _this, tf_Token_t const* key, vt_Value_t* value, bool* _result) {
    *_result = _this->GetMetadata(*key, value);
    return 0;
}

int usd_Object_SetMetadata_value(usd_Object_t const* _this, tf_Token_t const* key, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetMetadata(*key, *value);
    return 0;
}

int usd_Object_ClearMetadata(usd_Object_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->ClearMetadata(*key);
    return 0;
}

int usd_Object_HasMetadata(usd_Object_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasMetadata(*key);
    return 0;
}

int usd_Object_HasAuthoredMetadata(usd_Object_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasAuthoredMetadata(*key);
    return 0;
}

int usd_Object_GetMetadataByDictKey_value(usd_Object_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, vt_Value_t* value, bool* _result) {
    *_result = _this->GetMetadataByDictKey(*key, *keyPath, value);
    return 0;
}

int usd_Object_SetMetadataByDictKey_value(usd_Object_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetMetadataByDictKey(*key, *keyPath, *value);
    return 0;
}

int usd_Object_ClearMetadataByDictKey(usd_Object_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->ClearMetadataByDictKey(*key, *keyPath);
    return 0;
}

int usd_Object_HasMetadataDictKey(usd_Object_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasMetadataDictKey(*key, *keyPath);
    return 0;
}

int usd_Object_HasAuthoredMetadataDictKey(usd_Object_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredMetadataDictKey(*key, *keyPath);
    return 0;
}

int usd_Object_GetAllMetadata(usd_Object_t const* _this, usd_MetadataValueMap_t* _result) {
    *_result = _this->GetAllMetadata();
    return 0;
}

int usd_Object_GetAllAuthoredMetadata(usd_Object_t const* _this, usd_MetadataValueMap_t* _result) {
    *_result = _this->GetAllAuthoredMetadata();
    return 0;
}

int usd_Object_IsHidden(usd_Object_t const* _this, bool* _result) {
    *_result = _this->IsHidden();
    return 0;
}

int usd_Object_SetHidden(usd_Object_t const* _this, bool hidden, bool* _result) {
    *_result = _this->SetHidden(hidden);
    return 0;
}

int usd_Object_ClearHidden(usd_Object_t const* _this, bool* _result) {
    *_result = _this->ClearHidden();
    return 0;
}

int usd_Object_HasAuthoredHidden(usd_Object_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredHidden();
    return 0;
}

int usd_Object_GetCustomData(usd_Object_t const* _this, vt_Dictionary_t* _result) {
    *_result = _this->GetCustomData();
    return 0;
}

int usd_Object_GetCustomDataByKey(usd_Object_t const* _this, tf_Token_t const* keyPath, vt_Value_t* _result) {
    *_result = _this->GetCustomDataByKey(*keyPath);
    return 0;
}

int usd_Object_SetCustomData(usd_Object_t const* _this, vt_Dictionary_t const* customData) {
    _this->SetCustomData(*customData);
    return 0;
}

int usd_Object_SetCustomDataByKey(usd_Object_t const* _this, tf_Token_t const* keyPath, vt_Value_t const* value) {
    _this->SetCustomDataByKey(*keyPath, *value);
    return 0;
}

int usd_Object_ClearCustomData(usd_Object_t const* _this) {
    _this->ClearCustomData();
    return 0;
}

int usd_Object_ClearCustomDataByKey(usd_Object_t const* _this, tf_Token_t const* keyPath) {
    _this->ClearCustomDataByKey(*keyPath);
    return 0;
}

int usd_Object_HasCustomData(usd_Object_t const* _this, bool* _result) {
    *_result = _this->HasCustomData();
    return 0;
}

int usd_Object_HasCustomDataKey(usd_Object_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasCustomDataKey(*keyPath);
    return 0;
}

int usd_Object_HasAuthoredCustomData(usd_Object_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredCustomData();
    return 0;
}

int usd_Object_HasAuthoredCustomDataKey(usd_Object_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredCustomDataKey(*keyPath);
    return 0;
}

int usd_Object_GetAssetInfo(usd_Object_t const* _this, vt_Dictionary_t* _result) {
    *_result = _this->GetAssetInfo();
    return 0;
}

int usd_Object_GetAssetInfoByKey(usd_Object_t const* _this, tf_Token_t const* keyPath, vt_Value_t* _result) {
    *_result = _this->GetAssetInfoByKey(*keyPath);
    return 0;
}

int usd_Object_SetAssetInfo(usd_Object_t const* _this, vt_Dictionary_t const* customData) {
    _this->SetAssetInfo(*customData);
    return 0;
}

int usd_Object_SetAssetInfoByKey(usd_Object_t const* _this, tf_Token_t const* keyPath, vt_Value_t const* value) {
    _this->SetAssetInfoByKey(*keyPath, *value);
    return 0;
}

int usd_Object_ClearAssetInfo(usd_Object_t const* _this) {
    _this->ClearAssetInfo();
    return 0;
}

int usd_Object_ClearAssetInfoByKey(usd_Object_t const* _this, tf_Token_t const* keyPath) {
    _this->ClearAssetInfoByKey(*keyPath);
    return 0;
}

int usd_Object_HasAssetInfo(usd_Object_t const* _this, bool* _result) {
    *_result = _this->HasAssetInfo();
    return 0;
}

int usd_Object_HasAssetInfoKey(usd_Object_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAssetInfoKey(*keyPath);
    return 0;
}

int usd_Object_HasAuthoredAssetInfo(usd_Object_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredAssetInfo();
    return 0;
}

int usd_Object_HasAuthoredAssetInfoKey(usd_Object_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredAssetInfoKey(*keyPath);
    return 0;
}

int usd_Object_GetDocumentation(usd_Object_t const* _this, std_String_t* _result) {
    *_result = _this->GetDocumentation();
    return 0;
}

int usd_Object_SetDocumentation(usd_Object_t const* _this, std_String_t const* doc, bool* _result) {
    *_result = _this->SetDocumentation(*doc);
    return 0;
}

int usd_Object_ClearDocumentation(usd_Object_t const* _this, bool* _result) {
    *_result = _this->ClearDocumentation();
    return 0;
}

int usd_Object_HasAuthoredDocumentation(usd_Object_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredDocumentation();
    return 0;
}

int usd_Object_GetDisplayName(usd_Object_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayName();
    return 0;
}

int usd_Object_SetDisplayName(usd_Object_t const* _this, std_String_t const* name, bool* _result) {
    *_result = _this->SetDisplayName(*name);
    return 0;
}

int usd_Object_ClearDisplayName(usd_Object_t const* _this, bool* _result) {
    *_result = _this->ClearDisplayName();
    return 0;
}

int usd_Object_HasAuthoredDisplayName(usd_Object_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredDisplayName();
    return 0;
}

int usd_Object_IsValid(usd_Object_t const* _this, bool* _result) {
    *_result = _this->IsValid();
    return 0;
}

int usd_Object_GetStage(usd_Object_t const* _this, usd_StageWeakPtr_t* _result) {
    *_result = _this->GetStage();
    return 0;
}

int usd_Object_GetPath(usd_Object_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usd_Object_GetPrimPath(usd_Object_t const* _this, sdf_Path_t const** _result) {
    *_result = &_this->GetPrimPath();
    return 0;
}

int usd_Object_GetPrim(usd_Object_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usd_Object_GetName(usd_Object_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int usd_Object_GetNamespaceDelimiter(char* _result) {
    *_result = PXR_NS::UsdObject::GetNamespaceDelimiter();
    return 0;
}

int usd_Object_GetDescription(usd_Object_t const* _this, std_String_t* _result) {
    *_result = _this->GetDescription();
    return 0;
}

int usd_Object_default(usd_Object_t** _result) {
    *_result = new PXR_NS::UsdObject();
    return 0;
}

int usd_Object_dtor(usd_Object_t* _this) {
    delete _this;
    return 0;
}

int usd_ObjectSet_default(usd_ObjectSet_t** _result) {
    *_result = new std::set<PXR_NS::UsdObject>();
    return 0;
}

int usd_ObjectSet_dtor(usd_ObjectSet_t* _this) {
    delete _this;
    return 0;
}

int usd_Payloads_AddPayload(usd_Payloads_t* _this, sdf_Payload_t const* payload, int position, bool* _result) {
    *_result = _this->AddPayload(*payload, static_cast<PXR_NS::UsdListPosition>(position));
    return 0;
}

int usd_Payloads_AddPayload_with_identifier_and_path(usd_Payloads_t* _this, std_String_t const* identifier, sdf_Path_t const* primPath, sdf_LayerOffset_t const* layerOffset, int position, bool* _result) {
    *_result = _this->AddPayload(*identifier, *primPath, *layerOffset, static_cast<PXR_NS::UsdListPosition>(position));
    return 0;
}

int usd_Payloads_AddPayload_with_identifier(usd_Payloads_t* _this, std_String_t const* identifier, sdf_LayerOffset_t const* layerOffset, int position, bool* _result) {
    *_result = _this->AddPayload(*identifier, *layerOffset, static_cast<PXR_NS::UsdListPosition>(position));
    return 0;
}

int usd_Payloads_AddInternalPayload(usd_Payloads_t* _this, sdf_Path_t const* primPath, sdf_LayerOffset_t const* layerOffset, int position, bool* _result) {
    *_result = _this->AddInternalPayload(*primPath, *layerOffset, static_cast<PXR_NS::UsdListPosition>(position));
    return 0;
}

int usd_Payloads_RemovePayload(usd_Payloads_t* _this, sdf_Payload_t const* ref, bool* _result) {
    *_result = _this->RemovePayload(*ref);
    return 0;
}

int usd_Payloads_ClearPayloads(usd_Payloads_t* _this, bool* _result) {
    *_result = _this->ClearPayloads();
    return 0;
}

int usd_Payloads_SetPayloads(usd_Payloads_t* _this, sdf_PayloadVector_t const* items, bool* _result) {
    *_result = _this->SetPayloads(*items);
    return 0;
}

int usd_Payloads_GetPrim(usd_Payloads_t const* _this, usd_Prim_t const** _result) {
    *_result = &_this->GetPrim();
    return 0;
}

int usd_Payloads_dtor(usd_Payloads_t* _this) {
    delete _this;
    return 0;
}

int usd_Property_Is_Attribute(usd_Property_t const* _this, bool* _result) {
    *_result = _this->Is<PXR_NS::UsdAttribute>();
    return 0;
}

int usd_Property_Is_Relationship(usd_Property_t const* _this, bool* _result) {
    *_result = _this->Is<PXR_NS::UsdRelationship>();
    return 0;
}

int usd_Property_Is_Property(usd_Property_t const* _this, bool* _result) {
    *_result = _this->Is<PXR_NS::UsdProperty>();
    return 0;
}

int usd_Property_Is_Prim(usd_Property_t const* _this, bool* _result) {
    *_result = _this->Is<PXR_NS::UsdPrim>();
    return 0;
}

int usd_Property_As_Attribute(usd_Property_t const* _this, usd_Attribute_t* _result) {
    *_result = _this->As<PXR_NS::UsdAttribute>();
    return 0;
}

int usd_Property_As_Relationship(usd_Property_t const* _this, usd_Relationship_t* _result) {
    *_result = _this->As<PXR_NS::UsdRelationship>();
    return 0;
}

int usd_Property_As_Property(usd_Property_t const* _this, usd_Property_t* _result) {
    *_result = _this->As<PXR_NS::UsdProperty>();
    return 0;
}

int usd_Property_As_Prim(usd_Property_t const* _this, usd_Prim_t* _result) {
    *_result = _this->As<PXR_NS::UsdPrim>();
    return 0;
}

int usd_Property_GetPropertyStack(usd_Property_t const* _this, usd_TimeCode_t time, sdf_PropertySpecHandleVector_t* _result) {
    *_result = _this->GetPropertyStack(time);
    return 0;
}

int usd_Property_GetPropertyStackWithLayerOffsets(usd_Property_t const* _this, usd_TimeCode_t time, sdf_PropertySpecHandleOffsetPairVector_t* _result) {
    *_result = _this->GetPropertyStackWithLayerOffsets(time);
    return 0;
}

int usd_Property_GetBaseName(usd_Property_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetBaseName();
    return 0;
}

int usd_Property_GetNamespace(usd_Property_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNamespace();
    return 0;
}

int usd_Property_SplitName(usd_Property_t const* _this, std_StringVector_t* _result) {
    *_result = _this->SplitName();
    return 0;
}

int usd_Property_GetDisplayGroup(usd_Property_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayGroup();
    return 0;
}

int usd_Property_SetDisplayGroup(usd_Property_t const* _this, std_String_t const* displayGroup, bool* _result) {
    *_result = _this->SetDisplayGroup(*displayGroup);
    return 0;
}

int usd_Property_ClearDisplayGroup(usd_Property_t const* _this, bool* _result) {
    *_result = _this->ClearDisplayGroup();
    return 0;
}

int usd_Property_HasAuthoredDisplayGroup(usd_Property_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredDisplayGroup();
    return 0;
}

int usd_Property_GetNestedDisplayGroups(usd_Property_t const* _this, std_StringVector_t* _result) {
    *_result = _this->GetNestedDisplayGroups();
    return 0;
}

int usd_Property_SetNestedDisplayGroups(usd_Property_t const* _this, std_StringVector_t const* nestedGroups, bool* _result) {
    *_result = _this->SetNestedDisplayGroups(*nestedGroups);
    return 0;
}

int usd_Property_IsCustom(usd_Property_t const* _this, bool* _result) {
    *_result = _this->IsCustom();
    return 0;
}

int usd_Property_SetCustom(usd_Property_t const* _this, bool isCustom, bool* _result) {
    *_result = _this->SetCustom(isCustom);
    return 0;
}

int usd_Property_IsDefined(usd_Property_t const* _this, bool* _result) {
    *_result = _this->IsDefined();
    return 0;
}

int usd_Property_IsAuthored(usd_Property_t const* _this, bool* _result) {
    *_result = _this->IsAuthored();
    return 0;
}

int usd_Property_IsAuthoredAt(usd_Property_t const* _this, usd_EditTarget_t const* editTarget, bool* _result) {
    *_result = _this->IsAuthoredAt(*editTarget);
    return 0;
}

int usd_Property_FlattenTo_prim(usd_Property_t const* _this, usd_Prim_t const* parent, usd_Property_t* _result) {
    *_result = _this->FlattenTo(*parent);
    return 0;
}

int usd_Property_FlattenTo_named_property(usd_Property_t const* _this, usd_Prim_t const* parent, tf_Token_t const* propName, usd_Property_t* _result) {
    *_result = _this->FlattenTo(*parent, *propName);
    return 0;
}

int usd_Property_FlattenTo_property(usd_Property_t const* _this, usd_Property_t const* property, usd_Property_t* _result) {
    *_result = _this->FlattenTo(*property);
    return 0;
}

int usd_Property_GetMetadata_value(usd_Property_t const* _this, tf_Token_t const* key, vt_Value_t* value, bool* _result) {
    *_result = _this->GetMetadata(*key, value);
    return 0;
}

int usd_Property_SetMetadata_value(usd_Property_t const* _this, tf_Token_t const* key, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetMetadata(*key, *value);
    return 0;
}

int usd_Property_ClearMetadata(usd_Property_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->ClearMetadata(*key);
    return 0;
}

int usd_Property_HasMetadata(usd_Property_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasMetadata(*key);
    return 0;
}

int usd_Property_HasAuthoredMetadata(usd_Property_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasAuthoredMetadata(*key);
    return 0;
}

int usd_Property_GetMetadataByDictKey_value(usd_Property_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, vt_Value_t* value, bool* _result) {
    *_result = _this->GetMetadataByDictKey(*key, *keyPath, value);
    return 0;
}

int usd_Property_SetMetadataByDictKey_value(usd_Property_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetMetadataByDictKey(*key, *keyPath, *value);
    return 0;
}

int usd_Property_ClearMetadataByDictKey(usd_Property_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->ClearMetadataByDictKey(*key, *keyPath);
    return 0;
}

int usd_Property_HasMetadataDictKey(usd_Property_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasMetadataDictKey(*key, *keyPath);
    return 0;
}

int usd_Property_HasAuthoredMetadataDictKey(usd_Property_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredMetadataDictKey(*key, *keyPath);
    return 0;
}

int usd_Property_GetAllMetadata(usd_Property_t const* _this, usd_MetadataValueMap_t* _result) {
    *_result = _this->GetAllMetadata();
    return 0;
}

int usd_Property_GetAllAuthoredMetadata(usd_Property_t const* _this, usd_MetadataValueMap_t* _result) {
    *_result = _this->GetAllAuthoredMetadata();
    return 0;
}

int usd_Property_IsHidden(usd_Property_t const* _this, bool* _result) {
    *_result = _this->IsHidden();
    return 0;
}

int usd_Property_SetHidden(usd_Property_t const* _this, bool hidden, bool* _result) {
    *_result = _this->SetHidden(hidden);
    return 0;
}

int usd_Property_ClearHidden(usd_Property_t const* _this, bool* _result) {
    *_result = _this->ClearHidden();
    return 0;
}

int usd_Property_HasAuthoredHidden(usd_Property_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredHidden();
    return 0;
}

int usd_Property_GetCustomData(usd_Property_t const* _this, vt_Dictionary_t* _result) {
    *_result = _this->GetCustomData();
    return 0;
}

int usd_Property_GetCustomDataByKey(usd_Property_t const* _this, tf_Token_t const* keyPath, vt_Value_t* _result) {
    *_result = _this->GetCustomDataByKey(*keyPath);
    return 0;
}

int usd_Property_SetCustomData(usd_Property_t const* _this, vt_Dictionary_t const* customData) {
    _this->SetCustomData(*customData);
    return 0;
}

int usd_Property_SetCustomDataByKey(usd_Property_t const* _this, tf_Token_t const* keyPath, vt_Value_t const* value) {
    _this->SetCustomDataByKey(*keyPath, *value);
    return 0;
}

int usd_Property_ClearCustomData(usd_Property_t const* _this) {
    _this->ClearCustomData();
    return 0;
}

int usd_Property_ClearCustomDataByKey(usd_Property_t const* _this, tf_Token_t const* keyPath) {
    _this->ClearCustomDataByKey(*keyPath);
    return 0;
}

int usd_Property_HasCustomData(usd_Property_t const* _this, bool* _result) {
    *_result = _this->HasCustomData();
    return 0;
}

int usd_Property_HasCustomDataKey(usd_Property_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasCustomDataKey(*keyPath);
    return 0;
}

int usd_Property_HasAuthoredCustomData(usd_Property_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredCustomData();
    return 0;
}

int usd_Property_HasAuthoredCustomDataKey(usd_Property_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredCustomDataKey(*keyPath);
    return 0;
}

int usd_Property_GetAssetInfo(usd_Property_t const* _this, vt_Dictionary_t* _result) {
    *_result = _this->GetAssetInfo();
    return 0;
}

int usd_Property_GetAssetInfoByKey(usd_Property_t const* _this, tf_Token_t const* keyPath, vt_Value_t* _result) {
    *_result = _this->GetAssetInfoByKey(*keyPath);
    return 0;
}

int usd_Property_SetAssetInfo(usd_Property_t const* _this, vt_Dictionary_t const* customData) {
    _this->SetAssetInfo(*customData);
    return 0;
}

int usd_Property_SetAssetInfoByKey(usd_Property_t const* _this, tf_Token_t const* keyPath, vt_Value_t const* value) {
    _this->SetAssetInfoByKey(*keyPath, *value);
    return 0;
}

int usd_Property_ClearAssetInfo(usd_Property_t const* _this) {
    _this->ClearAssetInfo();
    return 0;
}

int usd_Property_ClearAssetInfoByKey(usd_Property_t const* _this, tf_Token_t const* keyPath) {
    _this->ClearAssetInfoByKey(*keyPath);
    return 0;
}

int usd_Property_HasAssetInfo(usd_Property_t const* _this, bool* _result) {
    *_result = _this->HasAssetInfo();
    return 0;
}

int usd_Property_HasAssetInfoKey(usd_Property_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAssetInfoKey(*keyPath);
    return 0;
}

int usd_Property_HasAuthoredAssetInfo(usd_Property_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredAssetInfo();
    return 0;
}

int usd_Property_HasAuthoredAssetInfoKey(usd_Property_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredAssetInfoKey(*keyPath);
    return 0;
}

int usd_Property_GetDocumentation(usd_Property_t const* _this, std_String_t* _result) {
    *_result = _this->GetDocumentation();
    return 0;
}

int usd_Property_SetDocumentation(usd_Property_t const* _this, std_String_t const* doc, bool* _result) {
    *_result = _this->SetDocumentation(*doc);
    return 0;
}

int usd_Property_ClearDocumentation(usd_Property_t const* _this, bool* _result) {
    *_result = _this->ClearDocumentation();
    return 0;
}

int usd_Property_HasAuthoredDocumentation(usd_Property_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredDocumentation();
    return 0;
}

int usd_Property_GetDisplayName(usd_Property_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayName();
    return 0;
}

int usd_Property_SetDisplayName(usd_Property_t const* _this, std_String_t const* name, bool* _result) {
    *_result = _this->SetDisplayName(*name);
    return 0;
}

int usd_Property_ClearDisplayName(usd_Property_t const* _this, bool* _result) {
    *_result = _this->ClearDisplayName();
    return 0;
}

int usd_Property_HasAuthoredDisplayName(usd_Property_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredDisplayName();
    return 0;
}

int usd_Property_IsValid(usd_Property_t const* _this, bool* _result) {
    *_result = _this->IsValid();
    return 0;
}

int usd_Property_GetStage(usd_Property_t const* _this, usd_StageWeakPtr_t* _result) {
    *_result = _this->GetStage();
    return 0;
}

int usd_Property_GetPath(usd_Property_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usd_Property_GetPrimPath(usd_Property_t const* _this, sdf_Path_t const** _result) {
    *_result = &_this->GetPrimPath();
    return 0;
}

int usd_Property_GetPrim(usd_Property_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usd_Property_GetName(usd_Property_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int usd_Property_GetNamespaceDelimiter(char* _result) {
    *_result = PXR_NS::UsdProperty::GetNamespaceDelimiter();
    return 0;
}

int usd_Property_GetDescription(usd_Property_t const* _this, std_String_t* _result) {
    *_result = _this->GetDescription();
    return 0;
}

int usd_Property_default(usd_Property_t** _result) {
    *_result = new PXR_NS::UsdProperty();
    return 0;
}

int usd_Property_dtor(usd_Property_t* _this) {
    delete _this;
    return 0;
}

int usd_PropertyVector_data(usd_PropertyVector_t* _this, usd_Property_t** _result) {
    *_result = _this->data();
    return 0;
}

int usd_PropertyVector_data_const(usd_PropertyVector_t const* _this, usd_Property_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usd_PropertyVector_empty(usd_PropertyVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usd_PropertyVector_size(usd_PropertyVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usd_PropertyVector_max_size(usd_PropertyVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usd_PropertyVector_reserve(usd_PropertyVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usd_PropertyVector_capacity(usd_PropertyVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usd_PropertyVector_clear(usd_PropertyVector_t* _this) {
    _this->clear();
    return 0;
}

int usd_PropertyVector_push_back(usd_PropertyVector_t* _this, usd_Property_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usd_PropertyVector_pop_back(usd_PropertyVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usd_PropertyVector_resize(usd_PropertyVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usd_PropertyVector_resize_with(usd_PropertyVector_t* _this, size_t const _Newsize, usd_Property_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usd_PropertyVector_op_index(usd_PropertyVector_t const* _this, size_t const _Pos, usd_Property_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int usd_PropertyVector_default(usd_PropertyVector_t** _result) {
    *_result = new std::vector<PXR_NS::UsdProperty>();
    return 0;
}

int usd_PropertyVector_dtor(usd_PropertyVector_t* _this) {
    delete _this;
    return 0;
}

int usd_References_AddReference(usd_References_t* _this, sdf_Reference_t const* ref, int position, bool* _result) {
    *_result = _this->AddReference(*ref, static_cast<PXR_NS::UsdListPosition>(position));
    return 0;
}

int usd_References_AddReference_with_identifier_and_path(usd_References_t* _this, std_String_t const* identifier, sdf_Path_t const* primPath, sdf_LayerOffset_t const* layerOffset, int position, bool* _result) {
    *_result = _this->AddReference(*identifier, *primPath, *layerOffset, static_cast<PXR_NS::UsdListPosition>(position));
    return 0;
}

int usd_References_AddReference_with_identifier(usd_References_t* _this, std_String_t const* identifier, sdf_LayerOffset_t const* layerOffset, int position, bool* _result) {
    *_result = _this->AddReference(*identifier, *layerOffset, static_cast<PXR_NS::UsdListPosition>(position));
    return 0;
}

int usd_References_AddInternalReference(usd_References_t* _this, sdf_Path_t const* primPath, sdf_LayerOffset_t const* layerOffset, int position, bool* _result) {
    *_result = _this->AddInternalReference(*primPath, *layerOffset, static_cast<PXR_NS::UsdListPosition>(position));
    return 0;
}

int usd_References_RemoveReference(usd_References_t* _this, sdf_Reference_t const* ref, bool* _result) {
    *_result = _this->RemoveReference(*ref);
    return 0;
}

int usd_References_ClearReferences(usd_References_t* _this, bool* _result) {
    *_result = _this->ClearReferences();
    return 0;
}

int usd_References_SetReferences(usd_References_t* _this, sdf_ReferenceVector_t const* items, bool* _result) {
    *_result = _this->SetReferences(*items);
    return 0;
}

int usd_References_GetPrim(usd_References_t const* _this, usd_Prim_t const** _result) {
    *_result = &_this->GetPrim();
    return 0;
}

int usd_References_dtor(usd_References_t* _this) {
    delete _this;
    return 0;
}

int usd_Relationship_AddTarget(usd_Relationship_t const* _this, sdf_Path_t const* target, int position, bool* _result) {
    *_result = _this->AddTarget(*target, static_cast<PXR_NS::UsdListPosition>(position));
    return 0;
}

int usd_Relationship_RemoveTarget(usd_Relationship_t const* _this, sdf_Path_t const* target, bool* _result) {
    *_result = _this->RemoveTarget(*target);
    return 0;
}

int usd_Relationship_SetTargets(usd_Relationship_t const* _this, sdf_PathVector_t const* targets, bool* _result) {
    *_result = _this->SetTargets(*targets);
    return 0;
}

int usd_Relationship_ClearTargets(usd_Relationship_t const* _this, bool removeSpec, bool* _result) {
    *_result = _this->ClearTargets(removeSpec);
    return 0;
}

int usd_Relationship_GetTargets(usd_Relationship_t const* _this, sdf_PathVector_t* targets, bool* _result) {
    *_result = _this->GetTargets(targets);
    return 0;
}

int usd_Relationship_GetForwardedTargets(usd_Relationship_t const* _this, sdf_PathVector_t* targets, bool* _result) {
    *_result = _this->GetForwardedTargets(targets);
    return 0;
}

int usd_Relationship_HasAuthoredTargets(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredTargets();
    return 0;
}

int usd_Relationship_GetPropertyStack(usd_Relationship_t const* _this, usd_TimeCode_t time, sdf_PropertySpecHandleVector_t* _result) {
    *_result = _this->GetPropertyStack(time);
    return 0;
}

int usd_Relationship_GetPropertyStackWithLayerOffsets(usd_Relationship_t const* _this, usd_TimeCode_t time, sdf_PropertySpecHandleOffsetPairVector_t* _result) {
    *_result = _this->GetPropertyStackWithLayerOffsets(time);
    return 0;
}

int usd_Relationship_GetBaseName(usd_Relationship_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetBaseName();
    return 0;
}

int usd_Relationship_GetNamespace(usd_Relationship_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetNamespace();
    return 0;
}

int usd_Relationship_SplitName(usd_Relationship_t const* _this, std_StringVector_t* _result) {
    *_result = _this->SplitName();
    return 0;
}

int usd_Relationship_GetDisplayGroup(usd_Relationship_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayGroup();
    return 0;
}

int usd_Relationship_SetDisplayGroup(usd_Relationship_t const* _this, std_String_t const* displayGroup, bool* _result) {
    *_result = _this->SetDisplayGroup(*displayGroup);
    return 0;
}

int usd_Relationship_ClearDisplayGroup(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->ClearDisplayGroup();
    return 0;
}

int usd_Relationship_HasAuthoredDisplayGroup(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredDisplayGroup();
    return 0;
}

int usd_Relationship_GetNestedDisplayGroups(usd_Relationship_t const* _this, std_StringVector_t* _result) {
    *_result = _this->GetNestedDisplayGroups();
    return 0;
}

int usd_Relationship_SetNestedDisplayGroups(usd_Relationship_t const* _this, std_StringVector_t const* nestedGroups, bool* _result) {
    *_result = _this->SetNestedDisplayGroups(*nestedGroups);
    return 0;
}

int usd_Relationship_IsCustom(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->IsCustom();
    return 0;
}

int usd_Relationship_SetCustom(usd_Relationship_t const* _this, bool isCustom, bool* _result) {
    *_result = _this->SetCustom(isCustom);
    return 0;
}

int usd_Relationship_IsDefined(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->IsDefined();
    return 0;
}

int usd_Relationship_IsAuthored(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->IsAuthored();
    return 0;
}

int usd_Relationship_IsAuthoredAt(usd_Relationship_t const* _this, usd_EditTarget_t const* editTarget, bool* _result) {
    *_result = _this->IsAuthoredAt(*editTarget);
    return 0;
}

int usd_Relationship_FlattenTo_prim(usd_Relationship_t const* _this, usd_Prim_t const* parent, usd_Property_t* _result) {
    *_result = _this->FlattenTo(*parent);
    return 0;
}

int usd_Relationship_FlattenTo_named_property(usd_Relationship_t const* _this, usd_Prim_t const* parent, tf_Token_t const* propName, usd_Property_t* _result) {
    *_result = _this->FlattenTo(*parent, *propName);
    return 0;
}

int usd_Relationship_FlattenTo_property(usd_Relationship_t const* _this, usd_Property_t const* property, usd_Property_t* _result) {
    *_result = _this->FlattenTo(*property);
    return 0;
}

int usd_Relationship_GetMetadata_value(usd_Relationship_t const* _this, tf_Token_t const* key, vt_Value_t* value, bool* _result) {
    *_result = _this->GetMetadata(*key, value);
    return 0;
}

int usd_Relationship_SetMetadata_value(usd_Relationship_t const* _this, tf_Token_t const* key, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetMetadata(*key, *value);
    return 0;
}

int usd_Relationship_ClearMetadata(usd_Relationship_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->ClearMetadata(*key);
    return 0;
}

int usd_Relationship_HasMetadata(usd_Relationship_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasMetadata(*key);
    return 0;
}

int usd_Relationship_HasAuthoredMetadata(usd_Relationship_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasAuthoredMetadata(*key);
    return 0;
}

int usd_Relationship_GetMetadataByDictKey_value(usd_Relationship_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, vt_Value_t* value, bool* _result) {
    *_result = _this->GetMetadataByDictKey(*key, *keyPath, value);
    return 0;
}

int usd_Relationship_SetMetadataByDictKey_value(usd_Relationship_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetMetadataByDictKey(*key, *keyPath, *value);
    return 0;
}

int usd_Relationship_ClearMetadataByDictKey(usd_Relationship_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->ClearMetadataByDictKey(*key, *keyPath);
    return 0;
}

int usd_Relationship_HasMetadataDictKey(usd_Relationship_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasMetadataDictKey(*key, *keyPath);
    return 0;
}

int usd_Relationship_HasAuthoredMetadataDictKey(usd_Relationship_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredMetadataDictKey(*key, *keyPath);
    return 0;
}

int usd_Relationship_GetAllMetadata(usd_Relationship_t const* _this, usd_MetadataValueMap_t* _result) {
    *_result = _this->GetAllMetadata();
    return 0;
}

int usd_Relationship_GetAllAuthoredMetadata(usd_Relationship_t const* _this, usd_MetadataValueMap_t* _result) {
    *_result = _this->GetAllAuthoredMetadata();
    return 0;
}

int usd_Relationship_IsHidden(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->IsHidden();
    return 0;
}

int usd_Relationship_SetHidden(usd_Relationship_t const* _this, bool hidden, bool* _result) {
    *_result = _this->SetHidden(hidden);
    return 0;
}

int usd_Relationship_ClearHidden(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->ClearHidden();
    return 0;
}

int usd_Relationship_HasAuthoredHidden(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredHidden();
    return 0;
}

int usd_Relationship_GetCustomData(usd_Relationship_t const* _this, vt_Dictionary_t* _result) {
    *_result = _this->GetCustomData();
    return 0;
}

int usd_Relationship_GetCustomDataByKey(usd_Relationship_t const* _this, tf_Token_t const* keyPath, vt_Value_t* _result) {
    *_result = _this->GetCustomDataByKey(*keyPath);
    return 0;
}

int usd_Relationship_SetCustomData(usd_Relationship_t const* _this, vt_Dictionary_t const* customData) {
    _this->SetCustomData(*customData);
    return 0;
}

int usd_Relationship_SetCustomDataByKey(usd_Relationship_t const* _this, tf_Token_t const* keyPath, vt_Value_t const* value) {
    _this->SetCustomDataByKey(*keyPath, *value);
    return 0;
}

int usd_Relationship_ClearCustomData(usd_Relationship_t const* _this) {
    _this->ClearCustomData();
    return 0;
}

int usd_Relationship_ClearCustomDataByKey(usd_Relationship_t const* _this, tf_Token_t const* keyPath) {
    _this->ClearCustomDataByKey(*keyPath);
    return 0;
}

int usd_Relationship_HasCustomData(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->HasCustomData();
    return 0;
}

int usd_Relationship_HasCustomDataKey(usd_Relationship_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasCustomDataKey(*keyPath);
    return 0;
}

int usd_Relationship_HasAuthoredCustomData(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredCustomData();
    return 0;
}

int usd_Relationship_HasAuthoredCustomDataKey(usd_Relationship_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredCustomDataKey(*keyPath);
    return 0;
}

int usd_Relationship_GetAssetInfo(usd_Relationship_t const* _this, vt_Dictionary_t* _result) {
    *_result = _this->GetAssetInfo();
    return 0;
}

int usd_Relationship_GetAssetInfoByKey(usd_Relationship_t const* _this, tf_Token_t const* keyPath, vt_Value_t* _result) {
    *_result = _this->GetAssetInfoByKey(*keyPath);
    return 0;
}

int usd_Relationship_SetAssetInfo(usd_Relationship_t const* _this, vt_Dictionary_t const* customData) {
    _this->SetAssetInfo(*customData);
    return 0;
}

int usd_Relationship_SetAssetInfoByKey(usd_Relationship_t const* _this, tf_Token_t const* keyPath, vt_Value_t const* value) {
    _this->SetAssetInfoByKey(*keyPath, *value);
    return 0;
}

int usd_Relationship_ClearAssetInfo(usd_Relationship_t const* _this) {
    _this->ClearAssetInfo();
    return 0;
}

int usd_Relationship_ClearAssetInfoByKey(usd_Relationship_t const* _this, tf_Token_t const* keyPath) {
    _this->ClearAssetInfoByKey(*keyPath);
    return 0;
}

int usd_Relationship_HasAssetInfo(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->HasAssetInfo();
    return 0;
}

int usd_Relationship_HasAssetInfoKey(usd_Relationship_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAssetInfoKey(*keyPath);
    return 0;
}

int usd_Relationship_HasAuthoredAssetInfo(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredAssetInfo();
    return 0;
}

int usd_Relationship_HasAuthoredAssetInfoKey(usd_Relationship_t const* _this, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredAssetInfoKey(*keyPath);
    return 0;
}

int usd_Relationship_GetDocumentation(usd_Relationship_t const* _this, std_String_t* _result) {
    *_result = _this->GetDocumentation();
    return 0;
}

int usd_Relationship_SetDocumentation(usd_Relationship_t const* _this, std_String_t const* doc, bool* _result) {
    *_result = _this->SetDocumentation(*doc);
    return 0;
}

int usd_Relationship_ClearDocumentation(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->ClearDocumentation();
    return 0;
}

int usd_Relationship_HasAuthoredDocumentation(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredDocumentation();
    return 0;
}

int usd_Relationship_GetDisplayName(usd_Relationship_t const* _this, std_String_t* _result) {
    *_result = _this->GetDisplayName();
    return 0;
}

int usd_Relationship_SetDisplayName(usd_Relationship_t const* _this, std_String_t const* name, bool* _result) {
    *_result = _this->SetDisplayName(*name);
    return 0;
}

int usd_Relationship_ClearDisplayName(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->ClearDisplayName();
    return 0;
}

int usd_Relationship_HasAuthoredDisplayName(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredDisplayName();
    return 0;
}

int usd_Relationship_IsValid(usd_Relationship_t const* _this, bool* _result) {
    *_result = _this->IsValid();
    return 0;
}

int usd_Relationship_GetStage(usd_Relationship_t const* _this, usd_StageWeakPtr_t* _result) {
    *_result = _this->GetStage();
    return 0;
}

int usd_Relationship_GetPath(usd_Relationship_t const* _this, sdf_Path_t* _result) {
    *_result = _this->GetPath();
    return 0;
}

int usd_Relationship_GetPrimPath(usd_Relationship_t const* _this, sdf_Path_t const** _result) {
    *_result = &_this->GetPrimPath();
    return 0;
}

int usd_Relationship_GetPrim(usd_Relationship_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPrim();
    return 0;
}

int usd_Relationship_GetName(usd_Relationship_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->GetName();
    return 0;
}

int usd_Relationship_GetNamespaceDelimiter(char* _result) {
    *_result = PXR_NS::UsdRelationship::GetNamespaceDelimiter();
    return 0;
}

int usd_Relationship_GetDescription(usd_Relationship_t const* _this, std_String_t* _result) {
    *_result = _this->GetDescription();
    return 0;
}

int usd_Relationship_default(usd_Relationship_t** _result) {
    *_result = new PXR_NS::UsdRelationship();
    return 0;
}

int usd_Relationship_dtor(usd_Relationship_t* _this) {
    delete _this;
    return 0;
}

int usd_RelationshipVector_data(usd_RelationshipVector_t* _this, usd_Relationship_t** _result) {
    *_result = _this->data();
    return 0;
}

int usd_RelationshipVector_data_const(usd_RelationshipVector_t const* _this, usd_Relationship_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usd_RelationshipVector_empty(usd_RelationshipVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usd_RelationshipVector_size(usd_RelationshipVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usd_RelationshipVector_max_size(usd_RelationshipVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usd_RelationshipVector_reserve(usd_RelationshipVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usd_RelationshipVector_capacity(usd_RelationshipVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usd_RelationshipVector_clear(usd_RelationshipVector_t* _this) {
    _this->clear();
    return 0;
}

int usd_RelationshipVector_push_back(usd_RelationshipVector_t* _this, usd_Relationship_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usd_RelationshipVector_pop_back(usd_RelationshipVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usd_RelationshipVector_resize(usd_RelationshipVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usd_RelationshipVector_resize_with(usd_RelationshipVector_t* _this, size_t const _Newsize, usd_Relationship_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usd_RelationshipVector_op_index(usd_RelationshipVector_t const* _this, size_t const _Pos, usd_Relationship_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int usd_RelationshipVector_default(usd_RelationshipVector_t** _result) {
    *_result = new std::vector<PXR_NS::UsdRelationship>();
    return 0;
}

int usd_RelationshipVector_dtor(usd_RelationshipVector_t* _this) {
    delete _this;
    return 0;
}

int usd_ResolveInfo_GetSource(usd_ResolveInfo_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetSource());
    return 0;
}

int usd_ResolveInfo_HasAuthoredValueOpinion(usd_ResolveInfo_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredValueOpinion();
    return 0;
}

int usd_ResolveInfo_HasAuthoredValue(usd_ResolveInfo_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredValue();
    return 0;
}

int usd_ResolveInfo_GetNode(usd_ResolveInfo_t const* _this, pcp_NodeRef_t* _result) {
    *_result = _this->GetNode();
    return 0;
}

int usd_ResolveInfo_ValueIsBlocked(usd_ResolveInfo_t const* _this, bool* _result) {
    *_result = _this->ValueIsBlocked();
    return 0;
}

int usd_ResolveInfo_dtor(usd_ResolveInfo_t* _this) {
    delete _this;
    return 0;
}

int usd_ResolveTarget_GetPrimIndex(usd_ResolveTarget_t const* _this, pcp_PrimIndex_t const** _result) {
    *_result = _this->GetPrimIndex();
    return 0;
}

int usd_ResolveTarget_GetStartNode(usd_ResolveTarget_t const* _this, pcp_NodeRef_t* _result) {
    *_result = _this->GetStartNode();
    return 0;
}

int usd_ResolveTarget_GetStartLayer(usd_ResolveTarget_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetStartLayer();
    return 0;
}

int usd_ResolveTarget_GetStopNode(usd_ResolveTarget_t const* _this, pcp_NodeRef_t* _result) {
    *_result = _this->GetStopNode();
    return 0;
}

int usd_ResolveTarget_GetStopLayer(usd_ResolveTarget_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetStopLayer();
    return 0;
}

int usd_ResolveTarget_IsNull(usd_ResolveTarget_t const* _this, bool* _result) {
    *_result = _this->IsNull();
    return 0;
}

int usd_ResolveTarget_dtor(usd_ResolveTarget_t* _this) {
    delete _this;
    return 0;
}

int usd_Specializes_AddSpecialize(usd_Specializes_t* _this, sdf_Path_t const* primPath, int position, bool* _result) {
    *_result = _this->AddSpecialize(*primPath, static_cast<PXR_NS::UsdListPosition>(position));
    return 0;
}

int usd_Specializes_RemoveSpecialize(usd_Specializes_t* _this, sdf_Path_t const* primPath, bool* _result) {
    *_result = _this->RemoveSpecialize(*primPath);
    return 0;
}

int usd_Specializes_ClearSpecializes(usd_Specializes_t* _this, bool* _result) {
    *_result = _this->ClearSpecializes();
    return 0;
}

int usd_Specializes_SetSpecializes(usd_Specializes_t* _this, sdf_PathVector_t const* items, bool* _result) {
    *_result = _this->SetSpecializes(*items);
    return 0;
}

int usd_Specializes_GetPrim(usd_Specializes_t const* _this, usd_Prim_t const** _result) {
    *_result = &_this->GetPrim();
    return 0;
}

int usd_Specializes_dtor(usd_Specializes_t* _this) {
    delete _this;
    return 0;
}

int usd_VariantSet_dtor(usd_VariantSet_t* _this) {
    delete _this;
    return 0;
}

int usd_VariantSets_dtor(usd_VariantSets_t* _this) {
    delete _this;
    return 0;
}

int usd_Stage_Save(usd_Stage_t* _this) {
    _this->Save();
    return 0;
}

int usd_Stage_SaveSessionLayers(usd_Stage_t* _this) {
    _this->SaveSessionLayers();
    return 0;
}

int usd_Stage_Load(usd_Stage_t* _this, sdf_Path_t const* path, int policy, usd_Prim_t* _result) {
    *_result = _this->Load(*path, static_cast<PXR_NS::UsdLoadPolicy>(policy));
    return 0;
}

int usd_Stage_Unload(usd_Stage_t* _this, sdf_Path_t const* path) {
    _this->Unload(*path);
    return 0;
}

int usd_Stage_LoadAndUnload(usd_Stage_t* _this, sdf_PathSet_t const* loadSet, sdf_PathSet_t const* unloadSet, int policy) {
    _this->LoadAndUnload(*loadSet, *unloadSet, static_cast<PXR_NS::UsdLoadPolicy>(policy));
    return 0;
}

int usd_Stage_GetLoadSet(usd_Stage_t* _this, sdf_PathSet_t* _result) {
    *_result = _this->GetLoadSet();
    return 0;
}

int usd_Stage_FindLoadable(usd_Stage_t* _this, sdf_Path_t const* rootPath, sdf_PathSet_t* _result) {
    *_result = _this->FindLoadable(*rootPath);
    return 0;
}

int usd_Stage_GetLoadRules(usd_Stage_t const* _this, usd_StageLoadRules_t const** _result) {
    *_result = &_this->GetLoadRules();
    return 0;
}

int usd_Stage_SetLoadRules(usd_Stage_t* _this, usd_StageLoadRules_t const* rules) {
    _this->SetLoadRules(*rules);
    return 0;
}

int usd_Stage_GetPopulationMask(usd_Stage_t const* _this, usd_StagePopulationMask_t* _result) {
    *_result = _this->GetPopulationMask();
    return 0;
}

int usd_Stage_SetPopulationMask(usd_Stage_t* _this, usd_StagePopulationMask_t const* mask) {
    _this->SetPopulationMask(*mask);
    return 0;
}

int usd_Stage_GetSessionLayer(usd_Stage_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetSessionLayer();
    return 0;
}

int usd_Stage_GetRootLayer(usd_Stage_t const* _this, sdf_LayerHandle_t* _result) {
    *_result = _this->GetRootLayer();
    return 0;
}

int usd_Stage_GetPathResolverContext(usd_Stage_t const* _this, ar_ResolverContext_t* _result) {
    *_result = _this->GetPathResolverContext();
    return 0;
}

int usd_Stage_ResolveIdentifierToEditTarget(usd_Stage_t const* _this, std_String_t const* identifier, std_String_t* _result) {
    *_result = _this->ResolveIdentifierToEditTarget(*identifier);
    return 0;
}

int usd_Stage_GetLayerStack(usd_Stage_t const* _this, bool includeSessionLayers, sdf_LayerHandleVector_t* _result) {
    *_result = _this->GetLayerStack(includeSessionLayers);
    return 0;
}

int usd_Stage_GetUsedLayers(usd_Stage_t const* _this, bool includeClipLayers, sdf_LayerHandleVector_t* _result) {
    *_result = _this->GetUsedLayers(includeClipLayers);
    return 0;
}

int usd_Stage_HasLocalLayer(usd_Stage_t const* _this, sdf_LayerHandle_t const* layer, bool* _result) {
    *_result = _this->HasLocalLayer(*layer);
    return 0;
}

int usd_Stage_GetEditTarget(usd_Stage_t const* _this, usd_EditTarget_t const** _result) {
    *_result = &_this->GetEditTarget();
    return 0;
}

int usd_Stage_GetEditTargetForLocalLayer(usd_Stage_t* _this, sdf_LayerHandle_t const* layer, usd_EditTarget_t* _result) {
    *_result = _this->GetEditTargetForLocalLayer(*layer);
    return 0;
}

int usd_Stage_GetEditTargetForLocalLayer_with_index(usd_Stage_t* _this, size_t i, usd_EditTarget_t* _result) {
    *_result = _this->GetEditTargetForLocalLayer(i);
    return 0;
}

int usd_Stage_SetEditTarget(usd_Stage_t* _this, usd_EditTarget_t const* editTarget) {
    _this->SetEditTarget(*editTarget);
    return 0;
}

int usd_Stage_MuteLayer(usd_Stage_t* _this, std_String_t const* layerIdentifier) {
    _this->MuteLayer(*layerIdentifier);
    return 0;
}

int usd_Stage_UnmuteLayer(usd_Stage_t* _this, std_String_t const* layerIdentifier) {
    _this->UnmuteLayer(*layerIdentifier);
    return 0;
}

int usd_Stage_MuteAndUnmuteLayers(usd_Stage_t* _this, std_StringVector_t const* muteLayers, std_StringVector_t const* unmuteLayers) {
    _this->MuteAndUnmuteLayers(*muteLayers, *unmuteLayers);
    return 0;
}

int usd_Stage_GetMutedLayers(usd_Stage_t const* _this, std_StringVector_t const** _result) {
    *_result = &_this->GetMutedLayers();
    return 0;
}

int usd_Stage_IsLayerMuted(usd_Stage_t const* _this, std_String_t const* layerIdentifier, bool* _result) {
    *_result = _this->IsLayerMuted(*layerIdentifier);
    return 0;
}

int usd_Stage_Export(usd_Stage_t const* _this, std_String_t const* filename, bool addSourceFileComment, sdf_LayerFileFormatArguments_t const* args, bool* _result) {
    *_result = _this->Export(*filename, addSourceFileComment, *args);
    return 0;
}

int usd_Stage_ExportToString(usd_Stage_t const* _this, std_String_t* result, bool addSourceFileComment, bool* _result) {
    *_result = _this->ExportToString(result, addSourceFileComment);
    return 0;
}

int usd_Stage_Flatten(usd_Stage_t const* _this, bool addSourceFileComment, sdf_LayerRefPtr_t* _result) {
    *_result = _this->Flatten(addSourceFileComment);
    return 0;
}

int usd_Stage_GetMetadata(usd_Stage_t const* _this, tf_Token_t const* key, vt_Value_t* value, bool* _result) {
    *_result = _this->GetMetadata(*key, value);
    return 0;
}

int usd_Stage_HasMetadata(usd_Stage_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasMetadata(*key);
    return 0;
}

int usd_Stage_HasAuthoredMetadata(usd_Stage_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->HasAuthoredMetadata(*key);
    return 0;
}

int usd_Stage_SetMetadata(usd_Stage_t const* _this, tf_Token_t const* key, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetMetadata(*key, *value);
    return 0;
}

int usd_Stage_ClearMetadata(usd_Stage_t const* _this, tf_Token_t const* key, bool* _result) {
    *_result = _this->ClearMetadata(*key);
    return 0;
}

int usd_Stage_GetMetadataByDictKey(usd_Stage_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, vt_Value_t* value, bool* _result) {
    *_result = _this->GetMetadataByDictKey(*key, *keyPath, value);
    return 0;
}

int usd_Stage_HasMetadataDictKey(usd_Stage_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasMetadataDictKey(*key, *keyPath);
    return 0;
}

int usd_Stage_HasAuthoredMetadataDictKey(usd_Stage_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->HasAuthoredMetadataDictKey(*key, *keyPath);
    return 0;
}

int usd_Stage_SetMetadataByDictKey(usd_Stage_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, vt_Value_t const* value, bool* _result) {
    *_result = _this->SetMetadataByDictKey(*key, *keyPath, *value);
    return 0;
}

int usd_Stage_ClearMetadataByDictKey(usd_Stage_t const* _this, tf_Token_t const* key, tf_Token_t const* keyPath, bool* _result) {
    *_result = _this->ClearMetadataByDictKey(*key, *keyPath);
    return 0;
}

int usd_Stage_WriteFallbackPrimTypes(usd_Stage_t* _this) {
    _this->WriteFallbackPrimTypes();
    return 0;
}

int usd_Stage_GetStartTimeCode(usd_Stage_t const* _this, double* _result) {
    *_result = _this->GetStartTimeCode();
    return 0;
}

int usd_Stage_SetStartTimeCode(usd_Stage_t* _this, double param00) {
    _this->SetStartTimeCode(param00);
    return 0;
}

int usd_Stage_GetEndTimeCode(usd_Stage_t const* _this, double* _result) {
    *_result = _this->GetEndTimeCode();
    return 0;
}

int usd_Stage_SetEndTimeCode(usd_Stage_t* _this, double param00) {
    _this->SetEndTimeCode(param00);
    return 0;
}

int usd_Stage_HasAuthoredTimeCodeRange(usd_Stage_t const* _this, bool* _result) {
    *_result = _this->HasAuthoredTimeCodeRange();
    return 0;
}

int usd_Stage_GetTimeCodesPerSecond(usd_Stage_t const* _this, double* _result) {
    *_result = _this->GetTimeCodesPerSecond();
    return 0;
}

int usd_Stage_SetTimeCodesPerSecond(usd_Stage_t const* _this, double timeCodesPerSecond) {
    _this->SetTimeCodesPerSecond(timeCodesPerSecond);
    return 0;
}

int usd_Stage_GetFramesPerSecond(usd_Stage_t const* _this, double* _result) {
    *_result = _this->GetFramesPerSecond();
    return 0;
}

int usd_Stage_SetFramesPerSecond(usd_Stage_t const* _this, double framesPerSecond) {
    _this->SetFramesPerSecond(framesPerSecond);
    return 0;
}

int usd_Stage_SetInterpolationType(usd_Stage_t* _this, int interpolationType) {
    _this->SetInterpolationType(static_cast<PXR_NS::UsdInterpolationType>(interpolationType));
    return 0;
}

int usd_Stage_GetInterpolationType(usd_Stage_t const* _this, int* _result) {
    *_result = static_cast<int>(_this->GetInterpolationType());
    return 0;
}

int usd_Stage_GetPrototypes(usd_Stage_t const* _this, usd_PrimVector_t* _result) {
    *_result = _this->GetPrototypes();
    return 0;
}

int usd_Stage_GetGlobalVariantFallbacks(pcp_VariantFallbackMap_t* _result) {
    *_result = PXR_NS::UsdStage::GetGlobalVariantFallbacks();
    return 0;
}

int usd_Stage_SetGlobalVariantFallbacks(pcp_VariantFallbackMap_t const* fallbacks) {
    PXR_NS::UsdStage::SetGlobalVariantFallbacks(*fallbacks);
    return 0;
}

int usd_Stage_CreateNew(std_String_t const* identifier, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::CreateNew(*identifier, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_CreateNew_with_session_layer(std_String_t const* identifier, sdf_LayerHandle_t const* sessionLayer, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::CreateNew(*identifier, *sessionLayer, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_CreateNew_with_session_layer_and_resolver_context(std_String_t const* identifier, sdf_LayerHandle_t const* sessionLayer, ar_ResolverContext_t const* pathResolverContext, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::CreateNew(*identifier, *sessionLayer, *pathResolverContext, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_CreateNew_with_resolver_context(std_String_t const* identifier, ar_ResolverContext_t const* pathResolverContext, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::CreateNew(*identifier, *pathResolverContext, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_CreateInMemory(int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::CreateInMemory(static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_CreateInMemory_with_identifier(std_String_t const* identifier, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::CreateInMemory(*identifier, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_CreateInMemory_with_resolver_context(std_String_t const* identifier, ar_ResolverContext_t const* pathResolverContext, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::CreateInMemory(*identifier, *pathResolverContext, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_CreateInMemory_with_session_layer(std_String_t const* identifier, sdf_LayerHandle_t const* sessionLayer, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::CreateInMemory(*identifier, *sessionLayer, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_CreateInMemory_with_session_layer_and_resolver_context(std_String_t const* identifier, sdf_LayerHandle_t const* sessionLayer, ar_ResolverContext_t const* pathResolverContext, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::CreateInMemory(*identifier, *sessionLayer, *pathResolverContext, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_Open(std_String_t const* filePath, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::Open(*filePath, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_Open_with_resolver_context(std_String_t const* filePath, ar_ResolverContext_t const* pathResolverContext, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::Open(*filePath, *pathResolverContext, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_Open_at_root(sdf_LayerHandle_t const* rootLayer, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::Open(*rootLayer, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_Open_at_root_with_resolver_context(sdf_LayerHandle_t const* rootLayer, ar_ResolverContext_t const* pathResolverContext, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::Open(*rootLayer, *pathResolverContext, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_Open_at_root_with_session_layer_and_resolver_context(sdf_LayerHandle_t const* rootLayer, sdf_LayerHandle_t const* sessionLayer, ar_ResolverContext_t const* pathResolverContext, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::Open(*rootLayer, *sessionLayer, *pathResolverContext, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_Open_at_root_with_session_layer(sdf_LayerHandle_t const* rootLayer, sdf_LayerHandle_t const* sessionLayer, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::Open(*rootLayer, *sessionLayer, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_OpenMasked(std_String_t const* filePath, usd_StagePopulationMask_t const* mask, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::OpenMasked(*filePath, *mask, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_OpenMasked_with_resolver_context(std_String_t const* filePath, ar_ResolverContext_t const* pathResolverContext, usd_StagePopulationMask_t const* mask, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::OpenMasked(*filePath, *pathResolverContext, *mask, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_OpenMasked_at_root(sdf_LayerHandle_t const* rootLayer, usd_StagePopulationMask_t const* mask, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::OpenMasked(*rootLayer, *mask, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_OpenMasked_at_root_with_resolver_context(sdf_LayerHandle_t const* rootLayer, ar_ResolverContext_t const* pathResolverContext, usd_StagePopulationMask_t const* mask, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::OpenMasked(*rootLayer, *pathResolverContext, *mask, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_OpenMasked_at_root_with_session_layer_and_resolver_context(sdf_LayerHandle_t const* rootLayer, sdf_LayerHandle_t const* sessionLayer, ar_ResolverContext_t const* pathResolverContext, usd_StagePopulationMask_t const* mask, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::OpenMasked(*rootLayer, *sessionLayer, *pathResolverContext, *mask, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_OpenMasked_at_root_with_session_layer(sdf_LayerHandle_t const* rootLayer, sdf_LayerHandle_t const* sessionLayer, usd_StagePopulationMask_t const* mask, int load, usd_StageRefPtr_t* _result) {
    *_result = PXR_NS::UsdStage::OpenMasked(*rootLayer, *sessionLayer, *mask, static_cast<PXR_NS::UsdStage::InitialLoadSet>(load));
    return 0;
}

int usd_Stage_IsSupportedFile(std_String_t const* filePath, bool* _result) {
    *_result = PXR_NS::UsdStage::IsSupportedFile(*filePath);
    return 0;
}

int usd_Stage_Reload(usd_Stage_t* _this) {
    _this->Reload();
    return 0;
}

int usd_Stage_GetColorConfigFallbacks(sdf_AssetPath_t* colorConfiguration, tf_Token_t* colorManagementSystem) {
    PXR_NS::UsdStage::GetColorConfigFallbacks(colorConfiguration, colorManagementSystem);
    return 0;
}

int usd_Stage_SetColorConfigFallbacks(sdf_AssetPath_t const* colorConfiguration, tf_Token_t const* colorManagementSystem) {
    PXR_NS::UsdStage::SetColorConfigFallbacks(*colorConfiguration, *colorManagementSystem);
    return 0;
}

int usd_Stage_SetColorConfiguration(usd_Stage_t const* _this, sdf_AssetPath_t const* colorConfig) {
    _this->SetColorConfiguration(*colorConfig);
    return 0;
}

int usd_Stage_GetColorConfiguration(usd_Stage_t const* _this, sdf_AssetPath_t* _result) {
    *_result = _this->GetColorConfiguration();
    return 0;
}

int usd_Stage_SetColorManagementSystem(usd_Stage_t const* _this, tf_Token_t const* cms) {
    _this->SetColorManagementSystem(*cms);
    return 0;
}

int usd_Stage_GetColorManagementSystem(usd_Stage_t const* _this, tf_Token_t* _result) {
    *_result = _this->GetColorManagementSystem();
    return 0;
}

int usd_Stage_GetPseudoRoot(usd_Stage_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetPseudoRoot();
    return 0;
}

int usd_Stage_GetDefaultPrim(usd_Stage_t const* _this, usd_Prim_t* _result) {
    *_result = _this->GetDefaultPrim();
    return 0;
}

int usd_Stage_SetDefaultPrim(usd_Stage_t* _this, usd_Prim_t const* prim) {
    _this->SetDefaultPrim(*prim);
    return 0;
}

int usd_Stage_ClearDefaultPrim(usd_Stage_t* _this) {
    _this->ClearDefaultPrim();
    return 0;
}

int usd_Stage_HasDefaultPrim(usd_Stage_t const* _this, bool* _result) {
    *_result = _this->HasDefaultPrim();
    return 0;
}

int usd_Stage_GetPrimAtPath(usd_Stage_t const* _this, sdf_Path_t const* path, usd_Prim_t* _result) {
    *_result = _this->GetPrimAtPath(*path);
    return 0;
}

int usd_Stage_GetObjectAtPath(usd_Stage_t const* _this, sdf_Path_t const* path, usd_Object_t* _result) {
    *_result = _this->GetObjectAtPath(*path);
    return 0;
}

int usd_Stage_GetPropertyAtPath(usd_Stage_t const* _this, sdf_Path_t const* path, usd_Property_t* _result) {
    *_result = _this->GetPropertyAtPath(*path);
    return 0;
}

int usd_Stage_GetAttributeAtPath(usd_Stage_t const* _this, sdf_Path_t const* path, usd_Attribute_t* _result) {
    *_result = _this->GetAttributeAtPath(*path);
    return 0;
}

int usd_Stage_GetRelationshipAtPath(usd_Stage_t const* _this, sdf_Path_t const* path, usd_Relationship_t* _result) {
    *_result = _this->GetRelationshipAtPath(*path);
    return 0;
}

int usd_Stage_OverridePrim(usd_Stage_t* _this, sdf_Path_t const* path, usd_Prim_t* _result) {
    *_result = _this->OverridePrim(*path);
    return 0;
}

int usd_Stage_DefinePrim(usd_Stage_t* _this, sdf_Path_t const* path, tf_Token_t const* typeName, usd_Prim_t* _result) {
    *_result = _this->DefinePrim(*path, *typeName);
    return 0;
}

int usd_Stage_CreateClassPrim(usd_Stage_t* _this, sdf_Path_t const* rootPrimPath, usd_Prim_t* _result) {
    *_result = _this->CreateClassPrim(*rootPrimPath);
    return 0;
}

int usd_Stage_RemovePrim(usd_Stage_t* _this, sdf_Path_t const* path, bool* _result) {
    *_result = _this->RemovePrim(*path);
    return 0;
}

int usd_Stage_dtor(usd_Stage_t* _this) {
    delete _this;
    return 0;
}

int usd_StageCache_GetAllStages(usd_StageCache_t const* _this, usd_StageRefPtrVector_t* _result) {
    *_result = _this->GetAllStages();
    return 0;
}

int usd_StageCache_Size(usd_StageCache_t const* _this, size_t* _result) {
    *_result = _this->Size();
    return 0;
}

int usd_StageCache_IsEmpty(usd_StageCache_t const* _this, bool* _result) {
    *_result = _this->IsEmpty();
    return 0;
}

int usd_StageCache_Find(usd_StageCache_t const* _this, usd_StageCacheId_t id, usd_StageRefPtr_t* _result) {
    *_result = _this->Find(id);
    return 0;
}

int usd_StageCache_FindOneMatching_with_root_layer(usd_StageCache_t const* _this, sdf_LayerHandle_t const* rootLayer, usd_StageRefPtr_t* _result) {
    *_result = _this->FindOneMatching(*rootLayer);
    return 0;
}

int usd_StageCache_FindOneMatching_with_root_and_session_layer(usd_StageCache_t const* _this, sdf_LayerHandle_t const* rootLayer, sdf_LayerHandle_t const* sessionLayer, usd_StageRefPtr_t* _result) {
    *_result = _this->FindOneMatching(*rootLayer, *sessionLayer);
    return 0;
}

int usd_StageCache_FindOneMatching_with_root_layer_and_resolver(usd_StageCache_t const* _this, sdf_LayerHandle_t const* rootLayer, ar_ResolverContext_t const* pathResolverContext, usd_StageRefPtr_t* _result) {
    *_result = _this->FindOneMatching(*rootLayer, *pathResolverContext);
    return 0;
}

int usd_StageCache_FindOneMatching_with_root_and_session_layer_and_resolver(usd_StageCache_t const* _this, sdf_LayerHandle_t const* rootLayer, sdf_LayerHandle_t const* sessionLayer, ar_ResolverContext_t const* pathResolverContext, usd_StageRefPtr_t* _result) {
    *_result = _this->FindOneMatching(*rootLayer, *sessionLayer, *pathResolverContext);
    return 0;
}

int usd_StageCache_FindAllMatching_with_root_layer(usd_StageCache_t const* _this, sdf_LayerHandle_t const* rootLayer, usd_StageRefPtrVector_t* _result) {
    *_result = _this->FindAllMatching(*rootLayer);
    return 0;
}

int usd_StageCache_FindAllMatching_with_root_and_session_layer(usd_StageCache_t const* _this, sdf_LayerHandle_t const* rootLayer, sdf_LayerHandle_t const* sessionLayer, usd_StageRefPtrVector_t* _result) {
    *_result = _this->FindAllMatching(*rootLayer, *sessionLayer);
    return 0;
}

int usd_StageCache_FindAllMatching_with_root_layer_and_resolver(usd_StageCache_t const* _this, sdf_LayerHandle_t const* rootLayer, ar_ResolverContext_t const* pathResolverContext, usd_StageRefPtrVector_t* _result) {
    *_result = _this->FindAllMatching(*rootLayer, *pathResolverContext);
    return 0;
}

int usd_StageCache_FindAllMatching_with_root_and_session_layer_and_resolver(usd_StageCache_t const* _this, sdf_LayerHandle_t const* rootLayer, sdf_LayerHandle_t const* sessionLayer, ar_ResolverContext_t const* pathResolverContext, usd_StageRefPtrVector_t* _result) {
    *_result = _this->FindAllMatching(*rootLayer, *sessionLayer, *pathResolverContext);
    return 0;
}

int usd_StageCache_GetId(usd_StageCache_t const* _this, usd_StageRefPtr_t const* stage, usd_StageCacheId_t* _result) {
    *_result = _this->GetId(*stage);
    return 0;
}

int usd_StageCache_Contains(usd_StageCache_t const* _this, usd_StageRefPtr_t const* stage, bool* _result) {
    *_result = _this->Contains(*stage);
    return 0;
}

int usd_StageCache_Contains_id(usd_StageCache_t const* _this, usd_StageCacheId_t id, bool* _result) {
    *_result = _this->Contains(id);
    return 0;
}

int usd_StageCache_Insert(usd_StageCache_t* _this, usd_StageRefPtr_t const* stage, usd_StageCacheId_t* _result) {
    *_result = _this->Insert(*stage);
    return 0;
}

int usd_StageCache_Erase(usd_StageCache_t* _this, usd_StageRefPtr_t const* stage, bool* _result) {
    *_result = _this->Erase(*stage);
    return 0;
}

int usd_StageCache_Erase_id(usd_StageCache_t* _this, usd_StageCacheId_t id, bool* _result) {
    *_result = _this->Erase(id);
    return 0;
}

int usd_StageCache_EraseAll_with_root_layer(usd_StageCache_t* _this, sdf_LayerHandle_t const* rootLayer, size_t* _result) {
    *_result = _this->EraseAll(*rootLayer);
    return 0;
}

int usd_StageCache_EraseAll_with_root_and_session_layer(usd_StageCache_t* _this, sdf_LayerHandle_t const* rootLayer, sdf_LayerHandle_t const* sessionLayer, size_t* _result) {
    *_result = _this->EraseAll(*rootLayer, *sessionLayer);
    return 0;
}

int usd_StageCache_EraseAll_with_root_and_session_layer_and_resolver(usd_StageCache_t* _this, sdf_LayerHandle_t const* rootLayer, sdf_LayerHandle_t const* sessionLayer, ar_ResolverContext_t const* pathResolverContext, size_t* _result) {
    *_result = _this->EraseAll(*rootLayer, *sessionLayer, *pathResolverContext);
    return 0;
}

int usd_StageCache_Clear(usd_StageCache_t* _this) {
    _this->Clear();
    return 0;
}

int usd_StageCache_SetDebugName(usd_StageCache_t* _this, std_String_t const* debugName) {
    _this->SetDebugName(*debugName);
    return 0;
}

int usd_StageCache_GetDebugName(usd_StageCache_t const* _this, std_String_t* _result) {
    *_result = _this->GetDebugName();
    return 0;
}

int usd_StageCache_default(usd_StageCache_t** _result) {
    *_result = new PXR_NS::UsdStageCache();
    return 0;
}

int usd_StageCache_dtor(usd_StageCache_t* _this) {
    delete _this;
    return 0;
}

int usd_StageCacheId_ToLongInt(usd_StageCacheId_t const* _this, long* _result) {
    *_result = _this->ToLongInt();
    return 0;
}

int usd_StageCacheId_ToString(usd_StageCacheId_t const* _this, std_String_t* _result) {
    *_result = _this->ToString();
    return 0;
}

int usd_StageCacheId_IsValid(usd_StageCacheId_t const* _this, bool* _result) {
    *_result = _this->IsValid();
    return 0;
}

int usd_StageCacheId_FromLongInt(long val, usd_StageCacheId_t* _result) {
    *_result = PXR_NS::UsdStageCache::Id::FromLongInt(val);
    return 0;
}

int usd_StageCacheId_FromString(std_String_t const* s, usd_StageCacheId_t* _result) {
    *_result = PXR_NS::UsdStageCache::Id::FromString(*s);
    return 0;
}

int usd_StageLoadRules_LoadWithDescendants(usd_StageLoadRules_t* _this, sdf_Path_t const* path) {
    _this->LoadWithDescendants(*path);
    return 0;
}

int usd_StageLoadRules_LoadWithoutDescendants(usd_StageLoadRules_t* _this, sdf_Path_t const* path) {
    _this->LoadWithoutDescendants(*path);
    return 0;
}

int usd_StageLoadRules_Unload(usd_StageLoadRules_t* _this, sdf_Path_t const* path) {
    _this->Unload(*path);
    return 0;
}

int usd_StageLoadRules_LoadAndUnload(usd_StageLoadRules_t* _this, sdf_PathSet_t const* loadSet, sdf_PathSet_t const* unloadSet, int policy) {
    _this->LoadAndUnload(*loadSet, *unloadSet, static_cast<PXR_NS::UsdLoadPolicy>(policy));
    return 0;
}

int usd_StageLoadRules_AddRule(usd_StageLoadRules_t* _this, sdf_Path_t const* path, int rule) {
    _this->AddRule(*path, static_cast<PXR_NS::UsdStageLoadRules::Rule>(rule));
    return 0;
}

int usd_StageLoadRules_Minimize(usd_StageLoadRules_t* _this) {
    _this->Minimize();
    return 0;
}

int usd_StageLoadRules_IsLoaded(usd_StageLoadRules_t const* _this, sdf_Path_t const* path, bool* _result) {
    *_result = _this->IsLoaded(*path);
    return 0;
}

int usd_StageLoadRules_IsLoadedWithAllDescendants(usd_StageLoadRules_t const* _this, sdf_Path_t const* path, bool* _result) {
    *_result = _this->IsLoadedWithAllDescendants(*path);
    return 0;
}

int usd_StageLoadRules_IsLoadedWithNoDescendants(usd_StageLoadRules_t const* _this, sdf_Path_t const* path, bool* _result) {
    *_result = _this->IsLoadedWithNoDescendants(*path);
    return 0;
}

int usd_StageLoadRules_GetEffectiveRuleForPath(usd_StageLoadRules_t const* _this, sdf_Path_t const* path, int* _result) {
    *_result = static_cast<int>(_this->GetEffectiveRuleForPath(*path));
    return 0;
}

int usd_StageLoadRules_GetRules(usd_StageLoadRules_t const* _this, usd_PathStageLoadRulesRulePairVector_t const** _result) {
    *_result = &_this->GetRules();
    return 0;
}

int usd_StageLoadRules_LoadAll(usd_StageLoadRules_t* _result) {
    *_result = PXR_NS::UsdStageLoadRules::LoadAll();
    return 0;
}

int usd_StageLoadRules_LoadNone(usd_StageLoadRules_t* _result) {
    *_result = PXR_NS::UsdStageLoadRules::LoadNone();
    return 0;
}

int usd_StageLoadRules_default(usd_StageLoadRules_t** _result) {
    *_result = new PXR_NS::UsdStageLoadRules();
    return 0;
}

int usd_StageLoadRules_dtor(usd_StageLoadRules_t* _this) {
    delete _this;
    return 0;
}

int usd_PathStageLoadRulesRulePair_dtor(usd_PathStageLoadRulesRulePair_t* _this) {
    delete _this;
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_data(usd_PathStageLoadRulesRulePairVector_t* _this, usd_PathStageLoadRulesRulePair_t** _result) {
    *_result = _this->data();
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_data_const(usd_PathStageLoadRulesRulePairVector_t const* _this, usd_PathStageLoadRulesRulePair_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_empty(usd_PathStageLoadRulesRulePairVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_size(usd_PathStageLoadRulesRulePairVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_max_size(usd_PathStageLoadRulesRulePairVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_reserve(usd_PathStageLoadRulesRulePairVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_capacity(usd_PathStageLoadRulesRulePairVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_clear(usd_PathStageLoadRulesRulePairVector_t* _this) {
    _this->clear();
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_push_back(usd_PathStageLoadRulesRulePairVector_t* _this, usd_PathStageLoadRulesRulePair_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_pop_back(usd_PathStageLoadRulesRulePairVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_resize(usd_PathStageLoadRulesRulePairVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_resize_with(usd_PathStageLoadRulesRulePairVector_t* _this, size_t const _Newsize, usd_PathStageLoadRulesRulePair_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_op_index(usd_PathStageLoadRulesRulePairVector_t const* _this, size_t const _Pos, usd_PathStageLoadRulesRulePair_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_default(usd_PathStageLoadRulesRulePairVector_t** _result) {
    *_result = new std::vector<std::pair<PXR_NS::SdfPath, PXR_NS::UsdStageLoadRules::Rule>>();
    return 0;
}

int usd_PathStageLoadRulesRulePairVector_dtor(usd_PathStageLoadRulesRulePairVector_t* _this) {
    delete _this;
    return 0;
}

int usd_StagePopulationMask_from_path_vector(usd_StagePopulationMask_t** _result) {
    *_result = new PXR_NS::UsdStagePopulationMask();
    return 0;
}

int usd_StagePopulationMask_default(usd_StagePopulationMask_t** _result) {
    *_result = new PXR_NS::UsdStagePopulationMask();
    return 0;
}

int usd_StagePopulationMask_dtor(usd_StagePopulationMask_t* _this) {
    delete _this;
    return 0;
}

int usd_StageRefPtr_get(usd_StageRefPtr_t const* _this, usd_Stage_t** _result) {
    *_result = _this->operator->();
    return 0;
}

int usd_StageRefPtr_ctor(usd_StageRefPtr_t** _result) {
    *_result = new PXR_NS::UsdStageRefPtr();
    return 0;
}

int usd_StageRefPtr_dtor(usd_StageRefPtr_t* _this) {
    delete _this;
    return 0;
}

int usd_StageRefPtrVector_data(usd_StageRefPtrVector_t* _this, usd_StageRefPtr_t** _result) {
    *_result = _this->data();
    return 0;
}

int usd_StageRefPtrVector_data_const(usd_StageRefPtrVector_t const* _this, usd_StageRefPtr_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usd_StageRefPtrVector_empty(usd_StageRefPtrVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usd_StageRefPtrVector_size(usd_StageRefPtrVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usd_StageRefPtrVector_max_size(usd_StageRefPtrVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usd_StageRefPtrVector_reserve(usd_StageRefPtrVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usd_StageRefPtrVector_capacity(usd_StageRefPtrVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usd_StageRefPtrVector_clear(usd_StageRefPtrVector_t* _this) {
    _this->clear();
    return 0;
}

int usd_StageRefPtrVector_push_back(usd_StageRefPtrVector_t* _this, usd_StageRefPtr_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usd_StageRefPtrVector_pop_back(usd_StageRefPtrVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usd_StageRefPtrVector_resize(usd_StageRefPtrVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usd_StageRefPtrVector_resize_with(usd_StageRefPtrVector_t* _this, size_t const _Newsize, usd_StageRefPtr_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usd_StageRefPtrVector_op_index(usd_StageRefPtrVector_t const* _this, size_t const _Pos, usd_StageRefPtr_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int usd_StageRefPtrVector_default(usd_StageRefPtrVector_t** _result) {
    *_result = new PXR_NS::UsdStageRefPtrVector();
    return 0;
}

int usd_StageRefPtrVector_dtor(usd_StageRefPtrVector_t* _this) {
    delete _this;
    return 0;
}

int usd_StageWeakPtr_get(usd_StageWeakPtr_t const* _this, usd_Stage_t** _result) {
    *_result = _this->operator->();
    return 0;
}

int usd_StageWeakPtr_ctor(usd_StageWeakPtr_t** _result) {
    *_result = new PXR_NS::UsdStageWeakPtr();
    return 0;
}

int usd_StageWeakPtr_dtor(usd_StageWeakPtr_t* _this) {
    delete _this;
    return 0;
}

int usd_TimeCode_IsEarliestTime(usd_TimeCode_t const* _this, bool* _result) {
    *_result = _this->IsEarliestTime();
    return 0;
}

int usd_TimeCode_IsDefault(usd_TimeCode_t const* _this, bool* _result) {
    *_result = _this->IsDefault();
    return 0;
}

int usd_TimeCode_IsNumeric(usd_TimeCode_t const* _this, bool* _result) {
    *_result = _this->IsNumeric();
    return 0;
}

int usd_TimeCode_GetValue(usd_TimeCode_t const* _this, double* _result) {
    *_result = _this->GetValue();
    return 0;
}

int usd_TimeCode_EarliestTime(usd_TimeCode_t* _result) {
    *_result = PXR_NS::UsdTimeCode::EarliestTime();
    return 0;
}

int usd_TimeCode_Default(usd_TimeCode_t* _result) {
    *_result = PXR_NS::UsdTimeCode::Default();
    return 0;
}

int usd_TimeCode_SafeStep(double maxValue, double maxCompression, double* _result) {
    *_result = PXR_NS::UsdTimeCode::SafeStep(maxValue, maxCompression);
    return 0;
}

int usd_TimeCode_from_sdf_timecode(double param00, usd_TimeCode_t* _result) {
    new (_result) PXR_NS::UsdTimeCode(param00);
    return 0;
}

int usd_TimeCode_from_time(double param00, usd_TimeCode_t* _result) {
    new (_result) PXR_NS::UsdTimeCode(param00);
    return 0;
}

int usd_TimeCodeVector_data(usd_TimeCodeVector_t* _this, usd_TimeCode_t** _result) {
    *_result = _this->data();
    return 0;
}

int usd_TimeCodeVector_data_const(usd_TimeCodeVector_t const* _this, usd_TimeCode_t const** _result) {
    *_result = _this->data();
    return 0;
}

int usd_TimeCodeVector_empty(usd_TimeCodeVector_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int usd_TimeCodeVector_size(usd_TimeCodeVector_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int usd_TimeCodeVector_max_size(usd_TimeCodeVector_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int usd_TimeCodeVector_reserve(usd_TimeCodeVector_t* _this, size_t const _Newcapacity) {
    _this->reserve(_Newcapacity);
    return 0;
}

int usd_TimeCodeVector_capacity(usd_TimeCodeVector_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int usd_TimeCodeVector_clear(usd_TimeCodeVector_t* _this) {
    _this->clear();
    return 0;
}

int usd_TimeCodeVector_push_back(usd_TimeCodeVector_t* _this, usd_TimeCode_t const* _Val) {
    _this->push_back(*_Val);
    return 0;
}

int usd_TimeCodeVector_pop_back(usd_TimeCodeVector_t* _this) {
    _this->pop_back();
    return 0;
}

int usd_TimeCodeVector_resize(usd_TimeCodeVector_t* _this, size_t const _Newsize) {
    _this->resize(_Newsize);
    return 0;
}

int usd_TimeCodeVector_resize_with(usd_TimeCodeVector_t* _this, size_t const _Newsize, usd_TimeCode_t const* _Val) {
    _this->resize(_Newsize, *_Val);
    return 0;
}

int usd_TimeCodeVector_op_index(usd_TimeCodeVector_t const* _this, size_t const _Pos, usd_TimeCode_t* _result) {
    *_result = _this->operator[](_Pos);
    return 0;
}

int usd_TimeCodeVector_default(usd_TimeCodeVector_t** _result) {
    *_result = new std::vector<PXR_NS::UsdTimeCode>();
    return 0;
}

int usd_TimeCodeVector_dtor(usd_TimeCodeVector_t* _this) {
    delete _this;
    return 0;
}

int usd_Prim_GetProperties(usd_Prim_t* prim, usd_PropertyVector_t* _result) {
    *_result = UsdPrim_GetProperties(*prim);
    return 0;
}

int vt_Value_IsArrayValued(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsArrayValued();
    return 0;
}

int vt_Value_IsEmpty(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsEmpty();
    return 0;
}

int vt_Value_GetArraySize(vt_Value_t const* _this, size_t* _result) {
    *_result = _this->GetArraySize();
    return 0;
}

int vt_Value_GetType(vt_Value_t const* _this, tf_Type_t* _result) {
    *_result = _this->GetType();
    return 0;
}

int vt_Value_GetTypeName(vt_Value_t const* _this, std_String_t* _result) {
    *_result = _this->GetTypeName();
    return 0;
}

int vt_Value_IsHolding_bool(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<bool>();
    return 0;
}

int vt_Value_Get_bool(vt_Value_t const* _this, bool* _result) {
    *_result = _this->Get<bool>();
    return 0;
}

int vt_Value_IsHolding_int(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<int>();
    return 0;
}

int vt_Value_Get_int(vt_Value_t const* _this, int* _result) {
    *_result = _this->Get<int>();
    return 0;
}

int vt_Value_IsHolding_float(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<float>();
    return 0;
}

int vt_Value_Get_float(vt_Value_t const* _this, float* _result) {
    *_result = _this->Get<float>();
    return 0;
}

int vt_Value_IsHolding_double(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<double>();
    return 0;
}

int vt_Value_Get_double(vt_Value_t const* _this, double* _result) {
    *_result = _this->Get<double>();
    return 0;
}

int vt_Value_IsHolding_TfToken(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::TfToken>();
    return 0;
}

int vt_Value_Get_TfToken(vt_Value_t const* _this, tf_Token_t const** _result) {
    *_result = &_this->Get<PXR_NS::TfToken>();
    return 0;
}

int vt_Value_IsHolding_SdfAssetPath(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::SdfAssetPath>();
    return 0;
}

int vt_Value_Get_SdfAssetPath(vt_Value_t const* _this, sdf_AssetPath_t const** _result) {
    *_result = &_this->Get<PXR_NS::SdfAssetPath>();
    return 0;
}

int vt_Value_IsHolding_string(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<std::string>();
    return 0;
}

int vt_Value_Get_string(vt_Value_t const* _this, std_String_t const** _result) {
    *_result = &_this->Get<std::string>();
    return 0;
}

int vt_Value_IsHolding_GfVec2d(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfVec2d>();
    return 0;
}

int vt_Value_Get_GfVec2d(vt_Value_t const* _this, gf_Vec2d_t* _result) {
    *_result = _this->Get<PXR_NS::GfVec2d>();
    return 0;
}

int vt_Value_IsHolding_GfVec2h(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfVec2h>();
    return 0;
}

int vt_Value_Get_GfVec2h(vt_Value_t const* _this, gf_Vec2h_t* _result) {
    *_result = _this->Get<PXR_NS::GfVec2h>();
    return 0;
}

int vt_Value_IsHolding_GfVec2f(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfVec2f>();
    return 0;
}

int vt_Value_Get_GfVec2f(vt_Value_t const* _this, gf_Vec2f_t* _result) {
    *_result = _this->Get<PXR_NS::GfVec2f>();
    return 0;
}

int vt_Value_IsHolding_GfVec2i(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfVec2i>();
    return 0;
}

int vt_Value_Get_GfVec2i(vt_Value_t const* _this, gf_Vec2i_t* _result) {
    *_result = _this->Get<PXR_NS::GfVec2i>();
    return 0;
}

int vt_Value_IsHolding_GfVec3d(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfVec3d>();
    return 0;
}

int vt_Value_Get_GfVec3d(vt_Value_t const* _this, gf_Vec3d_t* _result) {
    *_result = _this->Get<PXR_NS::GfVec3d>();
    return 0;
}

int vt_Value_IsHolding_GfVec3h(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfVec3h>();
    return 0;
}

int vt_Value_Get_GfVec3h(vt_Value_t const* _this, gf_Vec3h_t* _result) {
    *_result = _this->Get<PXR_NS::GfVec3h>();
    return 0;
}

int vt_Value_IsHolding_GfVec3f(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfVec3f>();
    return 0;
}

int vt_Value_Get_GfVec3f(vt_Value_t const* _this, gf_Vec3f_t* _result) {
    *_result = _this->Get<PXR_NS::GfVec3f>();
    return 0;
}

int vt_Value_IsHolding_GfVec3i(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfVec3i>();
    return 0;
}

int vt_Value_Get_GfVec3i(vt_Value_t const* _this, gf_Vec3i_t* _result) {
    *_result = _this->Get<PXR_NS::GfVec3i>();
    return 0;
}

int vt_Value_IsHolding_GfVec4d(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfVec4d>();
    return 0;
}

int vt_Value_Get_GfVec4d(vt_Value_t const* _this, gf_Vec4d_t* _result) {
    *_result = _this->Get<PXR_NS::GfVec4d>();
    return 0;
}

int vt_Value_IsHolding_GfVec4h(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfVec4h>();
    return 0;
}

int vt_Value_Get_GfVec4h(vt_Value_t const* _this, gf_Vec4h_t* _result) {
    *_result = _this->Get<PXR_NS::GfVec4h>();
    return 0;
}

int vt_Value_IsHolding_GfVec4f(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfVec4f>();
    return 0;
}

int vt_Value_Get_GfVec4f(vt_Value_t const* _this, gf_Vec4f_t* _result) {
    *_result = _this->Get<PXR_NS::GfVec4f>();
    return 0;
}

int vt_Value_IsHolding_GfVec4i(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfVec4i>();
    return 0;
}

int vt_Value_Get_GfVec4i(vt_Value_t const* _this, gf_Vec4i_t* _result) {
    *_result = _this->Get<PXR_NS::GfVec4i>();
    return 0;
}

int vt_Value_IsHolding_GfQuatd(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfQuatd>();
    return 0;
}

int vt_Value_Get_GfQuatd(vt_Value_t const* _this, gf_Quatd_t* _result) {
    *_result = _this->Get<PXR_NS::GfQuatd>();
    return 0;
}

int vt_Value_IsHolding_GfQuatf(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfQuatf>();
    return 0;
}

int vt_Value_Get_GfQuatf(vt_Value_t const* _this, gf_Quatf_t* _result) {
    *_result = _this->Get<PXR_NS::GfQuatf>();
    return 0;
}

int vt_Value_IsHolding_GfQuath(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfQuath>();
    return 0;
}

int vt_Value_Get_GfQuath(vt_Value_t const* _this, gf_Quath_t* _result) {
    *_result = _this->Get<PXR_NS::GfQuath>();
    return 0;
}

int vt_Value_IsHolding_GfMatrix2d(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfMatrix2d>();
    return 0;
}

int vt_Value_Get_GfMatrix2d(vt_Value_t const* _this, gf_Matrix2d_t* _result) {
    *_result = _this->Get<PXR_NS::GfMatrix2d>();
    return 0;
}

int vt_Value_IsHolding_GfMatrix2f(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfMatrix2f>();
    return 0;
}

int vt_Value_Get_GfMatrix2f(vt_Value_t const* _this, gf_Matrix2f_t* _result) {
    *_result = _this->Get<PXR_NS::GfMatrix2f>();
    return 0;
}

int vt_Value_IsHolding_GfMatrix3d(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfMatrix3d>();
    return 0;
}

int vt_Value_Get_GfMatrix3d(vt_Value_t const* _this, gf_Matrix3d_t* _result) {
    *_result = _this->Get<PXR_NS::GfMatrix3d>();
    return 0;
}

int vt_Value_IsHolding_GfMatrix3f(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfMatrix3f>();
    return 0;
}

int vt_Value_Get_GfMatrix3f(vt_Value_t const* _this, gf_Matrix3f_t* _result) {
    *_result = _this->Get<PXR_NS::GfMatrix3f>();
    return 0;
}

int vt_Value_IsHolding_GfMatrix4d(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfMatrix4d>();
    return 0;
}

int vt_Value_Get_GfMatrix4d(vt_Value_t const* _this, gf_Matrix4d_t* _result) {
    *_result = _this->Get<PXR_NS::GfMatrix4d>();
    return 0;
}

int vt_Value_IsHolding_GfMatrix4f(vt_Value_t const* _this, bool* _result) {
    *_result = _this->IsHolding<PXR_NS::GfMatrix4f>();
    return 0;
}

int vt_Value_Get_GfMatrix4f(vt_Value_t const* _this, gf_Matrix4f_t* _result) {
    *_result = _this->Get<PXR_NS::GfMatrix4f>();
    return 0;
}

int vt_Value_from_GfMatrix4f(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfMatrix4d(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfMatrix3f(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfMatrix3d(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfMatrix2f(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfMatrix2d(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfQuath(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfQuatf(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfQuatd(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfVec4i(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfVec4f(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfVec4h(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfVec4d(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfVec3i(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfVec3f(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfVec3h(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfVec3d(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfVec2i(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfVec2f(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfVec2h(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_GfVec2d(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_string(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_SdfAssetPath(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_TfToken(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_double(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_float(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_int(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_from_bool(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_ctor(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_new(vt_Value_t** _result) {
    *_result = new PXR_NS::VtValue();
    return 0;
}

int vt_Value_dtor(vt_Value_t* _this) {
    delete _this;
    return 0;
}

int vt_TokenArray_data(vt_TokenArray_t* _this, tf_Token_t** _result) {
    *_result = _this->data();
    return 0;
}

int vt_TokenArray_data_const(vt_TokenArray_t const* _this, tf_Token_t const** _result) {
    *_result = _this->data();
    return 0;
}

int vt_TokenArray_size(vt_TokenArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int vt_TokenArray_op_index(vt_TokenArray_t* _this, size_t index, tf_Token_t** _result) {
    *_result = &_this->operator[](index);
    return 0;
}

int vt_TokenArray_op_index_const(vt_TokenArray_t const* _this, size_t index, tf_Token_t const** _result) {
    *_result = &_this->operator[](index);
    return 0;
}

int vt_TokenArray_new(vt_TokenArray_t** _result) {
    *_result = new PXR_NS::VtTokenArray();
    return 0;
}

int vt_TokenArray_dtor(vt_TokenArray_t* _this) {
    delete _this;
    return 0;
}

int vt_BoolArray_data(vt_BoolArray_t* _this, bool** _result) {
    *_result = _this->data();
    return 0;
}

int vt_BoolArray_data_const(vt_BoolArray_t const* _this, bool const** _result) {
    *_result = _this->data();
    return 0;
}

int vt_BoolArray_size(vt_BoolArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int vt_BoolArray_op_index(vt_BoolArray_t* _this, size_t index, bool* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int vt_BoolArray_op_index_const(vt_BoolArray_t const* _this, size_t index, bool* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int vt_BoolArray_new(vt_BoolArray_t** _result) {
    *_result = new PXR_NS::VtBoolArray();
    return 0;
}

int vt_BoolArray_dtor(vt_BoolArray_t* _this) {
    delete _this;
    return 0;
}

int vt_IntArray_data(vt_IntArray_t* _this, int** _result) {
    *_result = _this->data();
    return 0;
}

int vt_IntArray_data_const(vt_IntArray_t const* _this, int const** _result) {
    *_result = _this->data();
    return 0;
}

int vt_IntArray_size(vt_IntArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int vt_IntArray_op_index(vt_IntArray_t* _this, size_t index, int* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int vt_IntArray_op_index_const(vt_IntArray_t const* _this, size_t index, int* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int vt_IntArray_new(vt_IntArray_t** _result) {
    *_result = new PXR_NS::VtIntArray();
    return 0;
}

int vt_IntArray_dtor(vt_IntArray_t* _this) {
    delete _this;
    return 0;
}

int vt_FloatArray_data(vt_FloatArray_t* _this, float** _result) {
    *_result = _this->data();
    return 0;
}

int vt_FloatArray_data_const(vt_FloatArray_t const* _this, float const** _result) {
    *_result = _this->data();
    return 0;
}

int vt_FloatArray_size(vt_FloatArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int vt_FloatArray_op_index(vt_FloatArray_t* _this, size_t index, float* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int vt_FloatArray_op_index_const(vt_FloatArray_t const* _this, size_t index, float* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int vt_FloatArray_new(vt_FloatArray_t** _result) {
    *_result = new PXR_NS::VtFloatArray();
    return 0;
}

int vt_FloatArray_dtor(vt_FloatArray_t* _this) {
    delete _this;
    return 0;
}

int vt_DoubleArray_data(vt_DoubleArray_t* _this, double** _result) {
    *_result = _this->data();
    return 0;
}

int vt_DoubleArray_data_const(vt_DoubleArray_t const* _this, double const** _result) {
    *_result = _this->data();
    return 0;
}

int vt_DoubleArray_size(vt_DoubleArray_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int vt_DoubleArray_op_index(vt_DoubleArray_t* _this, size_t index, double* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int vt_DoubleArray_op_index_const(vt_DoubleArray_t const* _this, size_t index, double* _result) {
    *_result = _this->operator[](index);
    return 0;
}

int vt_DoubleArray_new(vt_DoubleArray_t** _result) {
    *_result = new PXR_NS::VtDoubleArray();
    return 0;
}

int vt_DoubleArray_dtor(vt_DoubleArray_t* _this) {
    delete _this;
    return 0;
}

int vt_Dictionary_dtor(vt_Dictionary_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
