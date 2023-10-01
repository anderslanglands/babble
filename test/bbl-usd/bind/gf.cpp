#include <pxr/base/gf/interval.h>
#if defined(__clang__)

#include "babble"
#include "babble-std"

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

struct Vec2f {
    float x;
    float y;
};

struct Vec3f {
    float x;
    float y;
    float z;
};

struct Vec4f {
    float x;
    float y;
    float z;
    float w;
};

struct Vec2h {
    short x;
    short y;
};

struct Vec3h {
    short x;
    short y;
    short z;
};

struct Vec4h {
    short x;
    short y;
    short z;
    short w;
};

struct Vec2d {
    double x;
    double y;
};

struct Vec3d {
    double x;
    double y;
    double z;
};

struct Vec4d {
    double x;
    double y;
    double z;
    double w;
};

struct Vec2i {
    int x;
    int y;
};

struct Vec3i {
    int x;
    int y;
    int z;
};

struct Vec4i {
    int x;
    int y;
    int z;
    int w;
};

struct Matrix2d {
    double m[4];
};

struct Matrix3d {
    double m[9];
};

struct Matrix4d {
    double m[16];
};

struct Matrix2f {
    float m[4];
};

struct Matrix3f {
    float m[9];
};

struct Matrix4f {
    float m[16];
};

BBL_MODULE(gf) {
    // clang-format off

    // We don't bind methods for simple classes like this and will poke at the members directly or build that 
    // functionality in higher-level language bindings
    bbl::Class<PXR_NS::GfVec2f>("Vec2f")
        .replace_with<Vec2f>()
        ;
    bbl::Class<PXR_NS::GfVec3f>("Vec3f")
        .replace_with<Vec3f>()
        ;
    bbl::Class<PXR_NS::GfVec4f>("Vec4f")
        .replace_with<Vec4f>()
        ;

    bbl::Class<PXR_NS::GfVec2h>("Vec2h")
        .replace_with<Vec2h>()
        ;
    bbl::Class<PXR_NS::GfVec3h>("Vec3h")
        .replace_with<Vec3h>()
        ;
    bbl::Class<PXR_NS::GfVec4h>("Vec4h")
        .replace_with<Vec4h>()
        ;

    bbl::Class<PXR_NS::GfVec2d>("Vec2d")
        .replace_with<Vec2d>()
        ;
    bbl::Class<PXR_NS::GfVec3d>("Vec3d")
        .replace_with<Vec3d>()
        ;
    bbl::Class<PXR_NS::GfVec4d>("Vec4d")
        .replace_with<Vec4d>()
        ;

    bbl::Class<PXR_NS::GfVec2i>("Vec2i")
        .replace_with<Vec2i>()
        ;
    bbl::Class<PXR_NS::GfVec3i>("Vec3i")
        .replace_with<Vec3i>()
        ;
    bbl::Class<PXR_NS::GfVec4i>("Vec4i")
        .replace_with<Vec4i>()
        ;

    bbl::Class<PXR_NS::GfMatrix2d>("Matrix2d")
        .replace_with<Matrix2d>()
        ;
    bbl::Class<PXR_NS::GfMatrix3d>("Matrix3d")
        .replace_with<Matrix3d>()
        ;
    bbl::Class<PXR_NS::GfMatrix4d>("Matrix4d")
        .replace_with<Matrix4d>()
        ;

    
    bbl::Class<PXR_NS::GfMatrix2f>("Matrix2f")
        .replace_with<Matrix2f>()
        ;
    bbl::Class<PXR_NS::GfMatrix3f>("Matrix3f")
        .replace_with<Matrix3f>()
        ;
    bbl::Class<PXR_NS::GfMatrix4f>("Matrix4f")
        .replace_with<Matrix4f>()
        ;

    bbl::Class<PXR_NS::GfQuatd>("Quatd")
        .replace_with<Vec4d>()
        ;
    bbl::Class<PXR_NS::GfQuatf>("Quatf")
        .replace_with<Vec4f>()
        ;
    bbl::Class<PXR_NS::GfQuath>("Quath")
        .replace_with<Vec4h>()
        ;

    bbl::Class<PXR_NS::GfInterval>("Interval")
        .opaque_bytes()
        .ctor(bbl::Ctor<PXR_NS::GfInterval>(), "default")
        .m(&PXR_NS::GfInterval::GetMin)
        .m(&PXR_NS::GfInterval::GetMax)
        .m(&PXR_NS::GfInterval::GetSize)
        ;

}


#endif

