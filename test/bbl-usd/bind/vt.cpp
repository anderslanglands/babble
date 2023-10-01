#include <pxr/base/vt/dictionary.h>
#if defined(__clang__)

#include "babble"
#include "babble-std"

#include <pxr/base/vt/value.h>

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


#include <pxr/base/tf/token.h>
#include <pxr/usd/sdf/assetPath.h>

BBL_MODULE(vt) {
    // clang-format off

    using Value = PXR_NS::VtValue;

    #define VT_VALUE_TYPE(TY, NAME) \
        .ctor(bbl::Ctor<Value, TY>("value"), "from_" NAME) \
        .m((bool (Value::*)() const) \
            &Value::IsHolding<TY>, "IsHolding_" NAME \
        ) \
        .m((TY const& (Value::*)() const&) \
            &Value::Get<TY>, "Get_" NAME \
        ) 

    bbl::Class<PXR_NS::VtValue>("Value")
        .ctor(bbl::Ctor<Value, float>("obj"))
        .m(&Value::IsArrayValued)
        .m(&Value::IsEmpty)
        .m(&Value::GetArraySize)
        .m(&Value::GetType)
        .m(&Value::GetTypeName)

        // VT_VALUE_TYPE(bool, "bool")
        // VT_VALUE_TYPE(int, "int")
        // VT_VALUE_TYPE(float, "float")
        // VT_VALUE_TYPE(double, "double")
        // VT_VALUE_TYPE(PXR_NS::TfToken, "TfToken")
        // VT_VALUE_TYPE(PXR_NS::SdfAssetPath, "SdfAssetPath")
        // VT_VALUE_TYPE(std::string, "string")

        // VT_VALUE_TYPE(PXR_NS::GfVec2d, "GfVec2d")
        // VT_VALUE_TYPE(PXR_NS::GfVec2f, "GfVec2f")
        // VT_VALUE_TYPE(PXR_NS::GfVec2h, "GfVec2h")
        // VT_VALUE_TYPE(PXR_NS::GfVec2i, "GfVec2i")
        // VT_VALUE_TYPE(PXR_NS::GfVec3d, "GfVec3d")
        // VT_VALUE_TYPE(PXR_NS::GfVec3f, "GfVec3f")
        // VT_VALUE_TYPE(PXR_NS::GfVec3h, "GfVec3h")
        // VT_VALUE_TYPE(PXR_NS::GfVec3i, "GfVec3i")
        // VT_VALUE_TYPE(PXR_NS::GfVec4d, "GfVec4d")
        // VT_VALUE_TYPE(PXR_NS::GfVec4f, "GfVec4f")
        // VT_VALUE_TYPE(PXR_NS::GfVec4h, "GfVec4h")
        // VT_VALUE_TYPE(PXR_NS::GfVec4i, "GfVec4i")

        // VT_VALUE_TYPE(PXR_NS::GfQuatd, "GfQuatd")
        // VT_VALUE_TYPE(PXR_NS::GfQuatf, "GfQuatf")
        // VT_VALUE_TYPE(PXR_NS::GfQuath, "GfQuath")

        // VT_VALUE_TYPE(PXR_NS::GfMatrix2d, "GfMatrix2d")
        // VT_VALUE_TYPE(PXR_NS::GfMatrix2f, "GfMatrix2f")
        // VT_VALUE_TYPE(PXR_NS::GfMatrix3d, "GfMatrix3d")
        // VT_VALUE_TYPE(PXR_NS::GfMatrix3f, "GfMatrix3f")
        // VT_VALUE_TYPE(PXR_NS::GfMatrix4d, "GfMatrix4d")
        // VT_VALUE_TYPE(PXR_NS::GfMatrix4f, "GfMatrix4f")

        ;

    bbl::Class<PXR_NS::VtDictionary>("Dictionary")
        ;
}


#endif

