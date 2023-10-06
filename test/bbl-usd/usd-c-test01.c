#include <usd-c.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("usage:\n\t%s <usdlayer>", argv[0]);
        return 1;
    }

    std_String_t* str_path = NULL;
    std_String_from_char_ptr(argv[1], &str_path);

    usd_StageRefPtr_t* stage_ref = NULL;
    usd_StageRefPtr_ctor(&stage_ref);
    usd_Stage_Open(str_path, usd_StageInitialLoadSet_LoadAll, stage_ref);

    // Note: make sure stage_ref outlives any use of stage
    usd_Stage_t* stage = NULL;
    usd_StageRefPtr_get(stage_ref, &stage);
    if (stage == NULL) {
        printf("error: could not open stage\n");
    }

    usd_Prim_t* pseudo_root;
    usd_Prim_new(&pseudo_root);
    usd_Stage_GetPseudoRoot(stage, pseudo_root);

    // Constructing a prim range and its iterators is a bit of a long-winded dance
    usd_PrimRange_t* prim_range;
    usd_PrimRange_from_prim(pseudo_root, &prim_range);

    usd_PrimRangeIterator_t* current;
    usd_PrimRangeIterator_new(&current);
    usd_PrimRangeIterator_t* end;
    usd_PrimRangeIterator_new(&end);
    usd_PrimRangeIterator_t* dummy;
    usd_PrimRangeIterator_new(&dummy);

    usd_PrimRange_begin(prim_range, current);
    usd_PrimRange_end(prim_range, end);

    bool done = false;
    usd_PrimRangeIterator_op_eq(current, end, &done);
    usd_Prim_t* prim;
    usd_Prim_new(&prim);
    while (!done) {
        usd_PrimRangeIterator_deref(current, prim);

        // print out the prim's name
        tf_Token_t const* tok_name;
        usd_Prim_GetName(prim, &tok_name);
        char const* name_str = NULL;
        tf_Token_GetText(tok_name, &name_str);
        printf("%s\n", name_str);

        // Get all the properties and print out their names
        usd_PropertyVector_t* properties;
        usd_PropertyVector_default(&properties);
        usd_Prim_GetProperties(prim, properties);

        size_t num_props = 0;
        usd_PropertyVector_size(properties, &num_props);

        // time code to get attrs at
        usd_TimeCode_t tc;
        usd_TimeCode_Default(&tc);

        for (int i = 0; i < num_props; ++i) {
            usd_Property_t const* prop;
            usd_PropertyVector_op_index(properties, i, &prop);

            tf_Token_t const* tok_prop_name;
            usd_Property_GetName(prop, &tok_prop_name);

            char const* prop_name_str = NULL;
            tf_Token_GetText(tok_prop_name, &prop_name_str);
            printf("  %s", prop_name_str);

            bool is_attribute = false;
            bool is_relationship = false;
            usd_Property_Is_Attribute(prop, &is_attribute);
            usd_Property_Is_Relationship(prop, &is_relationship);

            if (is_attribute) {

                usd_Attribute_t* attr;
                usd_Attribute_default(&attr);
                usd_Property_As_Attribute(prop, attr);

                vt_Value_t* val;
                vt_Value_new(&val);

                bool result = false;
                usd_Attribute_Get(attr, val, tc, &result);

                vt_Value_IsHolding_bool(val, &result);
                if (result) {
                    bool v;
                    vt_Value_Get_bool(val, &v);

                    printf(" bool %d\n", v);
                    continue;
                }

                vt_Value_IsHolding_int(val, &result);
                if (result) {
                    int v;
                    vt_Value_Get_int(val, &v);

                    printf(" int %d\n", v);
                    continue;
                }

                vt_Value_IsHolding_float(val, &result);
                if (result) {
                    float v;
                    vt_Value_Get_float(val, &v);
                    printf(" float %f\n", v);
                    continue;
                }

                vt_Value_IsHolding_double(val, &result);
                if (result) {
                    double v;
                    vt_Value_Get_double(val, &v);
                    printf(" double %f\n", v);
                    continue;
                }

                vt_Value_IsHolding_TfToken(val, &result);
                if (result) {
                    tf_Token_t const* v;
                    vt_Value_Get_TfToken(val, &v);
                    char const* str;
                    tf_Token_GetText(v, &str);
                    printf(" TfToken \"%s\"\n", str);
                    continue;
                }

                vt_Value_IsHolding_SdfAssetPath(val, &result);
                if (result) {
                    sdf_AssetPath_t const* v;
                    vt_Value_Get_SdfAssetPath(val, &v);
                    char const* str = "";
                    sdf_AssetPath_GetResolvedPath(v, &str);
                    if (strlen(str) == 0) {
                        sdf_AssetPath_GetAssetPath(v, &str);
                    }

                    printf(" SdfAssetPath @%s@\n", str);
                    continue;
                }

                vt_Value_IsHolding_string(val, &result);
                if (result) {
                    std_String_t const* stdstr;
                    vt_Value_Get_string(val, &stdstr);
                    char const* str = "";
                    std_String_c_str(stdstr, &str);
                    printf(" string\"%s\"\n", str);
                    continue;
                }

                vt_Value_IsHolding_GfVec2d(val, &result);
                if (result) {
                    gf_Vec2d_t v;
                    vt_Value_Get_GfVec2d(val, &v);
                    printf(" GfVec2d (%f, %f)\n", v.x, v.y);
                    continue;
                }

                vt_Value_IsHolding_GfVec2f(val, &result);
                if (result) {
                    gf_Vec2f_t v;
                    vt_Value_Get_GfVec2f(val, &v);
                    printf(" GfVec2f (%f, %f)\n", v.x, v.y);
                    continue;
                }

                vt_Value_IsHolding_GfVec2h(val, &result);
                if (result) {
                    gf_Vec2h_t v;
                    vt_Value_Get_GfVec2h(val, &v);
                    // printf(" GfVec2h (%f, %f)\n", v.x, v.y);
                    printf(" GfVec2h\n");
                    continue;
                }

                vt_Value_IsHolding_GfVec2i(val, &result);
                if (result) {
                    gf_Vec2i_t v;
                    vt_Value_Get_GfVec2i(val, &v);
                    printf(" GfVec2i (%d, %d)\n", v.x, v.y);
                    continue;
                }

                vt_Value_IsHolding_GfVec3d(val, &result);
                if (result) {
                    gf_Vec3d_t v;
                    vt_Value_Get_GfVec3d(val, &v);
                    printf(" GfVec3d (%f, %f, %f)\n", v.x, v.y, v.z);
                    continue;
                }

                vt_Value_IsHolding_GfVec3f(val, &result);
                if (result) {
                    gf_Vec3f_t v;
                    vt_Value_Get_GfVec3f(val, &v);
                    printf(" GfVec3f (%f, %f, %f)\n", v.x, v.y, v.z);
                    continue;
                }

                vt_Value_IsHolding_GfVec3h(val, &result);
                if (result) {
                    printf(" GfVec3h\n");
                    continue;
                }

                vt_Value_IsHolding_GfVec3i(val, &result);
                if (result) {
                    gf_Vec3i_t v;
                    vt_Value_Get_GfVec3i(val, &v);
                    printf(" GfVec3i (%d, %d, %d)\n", v.x, v.y, v.z);
                    continue;
                }

                vt_Value_IsHolding_GfVec4d(val, &result);
                if (result) {
                    gf_Vec4d_t v;
                    vt_Value_Get_GfVec4d(val, &v);
                    printf(" GfVec4d (%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
                    continue;
                }

                vt_Value_IsHolding_GfVec4f(val, &result);
                if (result) {
                    gf_Vec4f_t v;
                    vt_Value_Get_GfVec4f(val, &v);
                    printf(" GfVec4f (%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
                    continue;
                }

                vt_Value_IsHolding_GfVec4h(val, &result);
                if (result) {
                    printf(" GfVec4h\n");
                    continue;
                }

                vt_Value_IsHolding_GfVec4i(val, &result);
                if (result) {
                    gf_Vec4i_t v;
                    vt_Value_Get_GfVec4i(val, &v);
                    printf(" GfVec4i (%d, %d, %d, %d)\n", v.x, v.y, v.z, v.w);
                    continue;
                }

                vt_Value_IsHolding_GfQuatd(val, &result);
                if (result) {
                    gf_Quatd_t v;
                    vt_Value_Get_GfQuatd(val, &v);
                    printf(" GfQuatd (%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
                    continue;
                }

                vt_Value_IsHolding_GfQuath(val, &result);
                if (result) {
                    printf(" GfQuath\n");
                    continue;
                }

                vt_Value_IsHolding_GfQuatf(val, &result);
                if (result) {
                    gf_Quatf_t v;
                    vt_Value_Get_GfQuatf(val, &v);
                    printf(" GfQuatf (%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
                    continue;
                }

                vt_Value_IsHolding_GfMatrix2d(val, &result);
                if (result) {
                    gf_Matrix2d_t v;
                    vt_Value_Get_GfMatrix2d(val, &v);
                    printf(" GfMatrix2d [[%f, %f], [%f, %f]]\n", v.m[0], v.m[1], v.m[2], v.m[3]);
                    continue;
                }

                vt_Value_IsHolding_GfMatrix2f(val, &result);
                if (result) {
                    gf_Matrix2f_t v;
                    vt_Value_Get_GfMatrix2f(val, &v);
                    printf(" GfMatrix2f [[%f, %f], [%f, %f]]\n", v.m[0], v.m[1], v.m[2], v.m[3]);
                    continue;
                }

                vt_Value_IsHolding_GfMatrix3d(val, &result);
                if (result) {
                    gf_Matrix3d_t v;
                    vt_Value_Get_GfMatrix3d(val, &v);
                    printf(" GfMatrix3d [[%f, %f, %f], [%f, %f, %f], [%f, %f, %f]]\n", v.m[0], v.m[1], v.m[2], v.m[3], v.m[4], v.m[5], v.m[6], v.m[7], v.m[8]);
                    continue;
                }

                vt_Value_IsHolding_GfMatrix3f(val, &result);
                if (result) {
                    gf_Matrix3f_t v;
                    vt_Value_Get_GfMatrix3f(val, &v);
                    printf(" GfMatrix3f [[%f, %f, %f], [%f, %f, %f], [%f, %f, %f]]\n", v.m[0], v.m[1], v.m[2], v.m[3], v.m[4], v.m[5], v.m[6], v.m[7], v.m[8]);
                    continue;
                }

                vt_Value_IsHolding_GfMatrix4d(val, &result);
                if (result) {
                    gf_Matrix4d_t v;
                    vt_Value_Get_GfMatrix4d(val, &v);
                    printf(" GfMatrix4d [[%f, %f, %f, %f], [%f, %f, %f, %f], [%f, %f, %f, %f], [%f, %f, %f, %f]]\n", v.m[0], v.m[1], v.m[2], v.m[3], v.m[4], v.m[5], v.m[6], v.m[7], v.m[8], v.m[9], v.m[10], v.m[11], v.m[12], v.m[13], v.m[14], v.m[15]);
                    continue;
                }

                vt_Value_IsHolding_GfMatrix4f(val, &result);
                if (result) {
                    gf_Matrix4f_t v;
                    vt_Value_Get_GfMatrix4f(val, &v);
                    printf(" GfMatrix4f [[%f, %f, %f, %f], [%f, %f, %f, %f], [%f, %f, %f, %f], [%f, %f, %f, %f]]\n", v.m[0], v.m[1], v.m[2], v.m[3], v.m[4], v.m[5], v.m[6], v.m[7], v.m[8], v.m[9], v.m[10], v.m[11], v.m[12], v.m[13], v.m[14], v.m[15]);
                    continue;
                }

                usd_Attribute_dtor(attr);

            } else if (is_relationship) {
                usd_Relationship_t* rel;
                usd_Relationship_default(&rel);
                usd_Property_As_Relationship(prop, rel);

                sdf_PathVector_t* targets;
                sdf_PathVector_default(&targets);

                bool result = false;
                usd_Relationship_GetTargets(rel, targets, &result);

                size_t num_targets = 0;
                sdf_PathVector_size(targets, &num_targets);

                for (int t = 0; t < num_targets; ++t) {
                    sdf_Path_t const* path;
                    sdf_PathVector_op_index(targets, t, &path);
                    char const* path_str = NULL;
                    sdf_Path_GetText(path, &path_str);

                    printf(" <%s>", path_str);
                }

                usd_Relationship_dtor(rel);
            } else {
                // oh oh...
                printf("WHAT IS THIS?\n");
                return 2;
            }

            printf("\n");
        }

        // cleanup
        usd_PropertyVector_dtor(properties);

        // advance iterator
        usd_PrimRangeIterator_op_inc(current, &dummy);
        usd_PrimRangeIterator_op_eq(current, end, &done);
    }

    // cleanup
    usd_Prim_dtor(prim);

    usd_PrimRangeIterator_dtor(end);
    usd_PrimRangeIterator_dtor(current);
    usd_PrimRange_dtor(prim_range);

    usd_Prim_dtor(pseudo_root);

    usd_StageRefPtr_dtor(stage_ref);
    std_String_dtor(str_path);

    return 0;
}