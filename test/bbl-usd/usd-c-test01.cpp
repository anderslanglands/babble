#include <usd-c.h>
#include <stdio.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("usage:\n\t%s <usdlayer>", argv[0]);
        return 1;
    }

    std_String_t* str_path = nullptr;
    std_String_from_char_ptr(argv[1], &str_path);

    usd_StageRefPtr_t* stage_ref = nullptr;
    usd_StageRefPtr_ctor(&stage_ref);
    usd_Stage_Open(str_path, usd_StageInitialLoadSet_LoadAll, stage_ref);

    // Note: make sure stage_ref outlives any use of stage
    usd_Stage_t* stage = nullptr;
    usd_StageRefPtr_get(stage_ref, &stage);
    if (stage == nullptr) {
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
        char const* name_str = nullptr;
        tf_Token_GetText(tok_name, &name_str);
        printf("%s\n", name_str);

        // Get all the properties and print out their names
        usd_PropertyVector_t* properties;
        usd_PropertyVector_default(&properties);
        usd_Prim_GetProperties(prim, properties);

        size_t num_props = 0;
        usd_PropertyVector_size(properties, &num_props);

        for (int i = 0; i < num_props; ++i) {
            usd_Property_t const* prop;
            usd_PropertyVector_op_index(properties, i, &prop);

            tf_Token_t const* tok_prop_name;
            usd_Property_GetName(prop, &tok_prop_name);

            char const* prop_name_str = nullptr;
            tf_Token_GetText(tok_prop_name, &prop_name_str);
            printf("  %s\n", prop_name_str);

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
}