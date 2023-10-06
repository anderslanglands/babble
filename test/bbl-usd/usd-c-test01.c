#include <usd-c.h>
#include <stdio.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("usage:\n\t%s <usdlayer>", argv[0]);
        return 1;
    }

    struct std_String_t* str_path = NULL;
    std_String_from_char_ptr(argv[1], &str_path);

    struct usd_StageRefPtr_t* stage_ref = NULL;
    usd_StageRefPtr_ctor(&stage_ref);
    usd_Stage_Open(str_path, usd_StageInitialLoadSet_LoadAll, stage_ref);

    // Note: make sure stage_ref outlives any use of stage
    struct usd_Stage_t* stage = NULL;
    usd_StageRefPtr_get(stage_ref, &stage);
    if (stage == NULL) {
        printf("error: could not open stage\n");
    }

    struct usd_Prim_t* pseudo_root;
    usd_Prim_new(&pseudo_root);
    usd_Stage_GetPseudoRoot(stage, pseudo_root);

    // Constructing a prim range and its iterators is a bit of a long-winded dance
    struct usd_PrimRange_t* prim_range;
    usd_PrimRange_from_prim(pseudo_root, &prim_range);

    struct usd_PrimRangeIterator_t* current;
    usd_PrimRangeIterator_new(&current);
    struct usd_PrimRangeIterator_t* end;
    usd_PrimRangeIterator_new(&end);
    struct usd_PrimRangeIterator_t* dummy;
    usd_PrimRangeIterator_new(&dummy);

    usd_PrimRange_begin(prim_range, current);
    usd_PrimRange_end(prim_range, end);

    bool done = false;
    usd_PrimRangeIterator_op_eq(current, end, &done);
    struct usd_Prim_t* prim;
    usd_Prim_new(&prim);
    while (!done) {
        usd_PrimRangeIterator_deref(current, prim);

        // print out the prim's name
        struct tf_Token_t const* tok_name;
        usd_Prim_GetName(prim, &tok_name);
        char const* name_str = NULL;
        tf_Token_GetText(tok_name, &name_str);
        printf("%s\n", name_str);

        // Get all the properties and print out their names
        struct usd_PropertyVector_t* properties;
        usd_PropertyVector_default(&properties);
        usd_Prim_GetProperties(prim, properties);

        size_t num_props = 0;
        usd_PropertyVector_size(properties, &num_props);

        for (int i = 0; i < num_props; ++i) {
            struct usd_Property_t const* prop;
            usd_PropertyVector_op_index(properties, i, &prop);

            struct tf_Token_t const* tok_prop_name;
            usd_Property_GetName(prop, &tok_prop_name);

            char const* prop_name_str = NULL;
            tf_Token_GetText(tok_prop_name, &prop_name_str);
            printf("  %s", prop_name_str);

            bool is_attribute = false;
            bool is_relationship = false;
            usd_Property_Is_Attribute(prop, &is_attribute);
            usd_Property_Is_Relationship(prop, &is_relationship);

            if (is_attribute) {
                printf(" A");

                struct usd_Attribute_t* attr;
                usd_Attribute_default(&attr);
                usd_Property_As_Attribute(prop, attr);

                struct vt_Value_t* val;

                usd_Attribute_dtor(attr);

            } else if (is_relationship) {
                printf(" R");

                struct usd_Relationship_t* rel;
                usd_Relationship_default(&rel);
                usd_Property_As_Relationship(prop, rel);

                struct sdf_PathVector_t* targets;
                sdf_PathVector_default(&targets);

                bool result = false;
                usd_Relationship_GetTargets(rel, targets, &result);

                size_t num_targets = 0;
                sdf_PathVector_size(targets, &num_targets);

                for (int t = 0; t < num_targets; ++t) {
                    struct sdf_Path_t const* path;
                    sdf_PathVector_op_index(targets, t, &path);
                    char const* path_str = NULL;
                    sdf_Path_GetText(path, &path_str);

                    printf("\n    \"%s\"", path_str);
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