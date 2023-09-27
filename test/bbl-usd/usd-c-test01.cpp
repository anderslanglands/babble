#include <usd-c.h>
#include <stdio.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("usage:\n\t%s <usdlayer>", argv[0]);
    }

    std_String_t* str_path = nullptr;
    std_String_from_char_ptr(argv[1], &str_path);

    usd_StageRefPtr_t* stage_ref = nullptr;
    usd_StageRefPtr_ctor(&stage_ref);
    usd_Stage_Open(str_path, LoadAll, stage_ref);

    // Note: make sure stage_ref outlives any use of stage
    usd_Stage_t* stage = nullptr;
    usd_StageRefPtr_get(stage_ref, &stage);
    if (stage == nullptr) {
        printf("error: could not open stage\n");
    }

    usd_Prim_t* pseudo_root;
    usd_Prim_new(&pseudo_root);
    usd_Stage_GetPseudoRoot(stage, pseudo_root);

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

        tf_Token_t const* tok_name;
        usd_Prim_GetName(prim, &tok_name);
        char const* name_str = nullptr;
        tf_Token_GetText(tok_name, &name_str);

        printf("%s\n", name_str);

        usd_PrimRangeIterator_op_inc(current, &dummy);
        usd_PrimRangeIterator_op_eq(current, end, &done);
    }

    usd_Prim_dtor(prim);

    usd_PrimRangeIterator_dtor(end);
    usd_PrimRangeIterator_dtor(current);
    usd_PrimRange_dtor(prim_range);

    usd_Prim_dtor(pseudo_root);

    usd_StageRefPtr_dtor(stage_ref);
    std_String_dtor(str_path);
}