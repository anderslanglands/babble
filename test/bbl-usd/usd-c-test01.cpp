#include <usd-c.h>
#include <stdio.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("usage:\n\t%s <usdlayer>", argv[0]);
    }

    String_t* str_path = nullptr;
    String_from_char_ptr(argv[1], &str_path);

    StageRefPtr_t* stage_ref = nullptr;
    StageRefPtr_ctor(&stage_ref);
    Stage_Open(str_path, LoadAll, stage_ref);

    // Note: make sure stage_ref outlives any use of stage
    Stage_t* stage = nullptr;
    StageRefPtr_get(stage_ref, &stage);
    if (stage == nullptr) {
        printf("error: could not open stage\n");
    }

    Prim_t* pseudo_root;
    Prim_new(&pseudo_root);
    Stage_GetPseudoRoot(stage, pseudo_root);

    PrimRange_t* prim_range;
    PrimRange_from_prim(pseudo_root, &prim_range);

    PrimRangeIterator_t* current;
    PrimRangeIterator_new(&current);
    PrimRangeIterator_t* end;
    PrimRangeIterator_new(&end);
    PrimRangeIterator_t* dummy;
    PrimRangeIterator_new(&dummy);

    PrimRange_begin(prim_range, current);
    PrimRange_end(prim_range, end);

    bool done = false;
    PrimRangeIterator_op_eq(current, end, &done);
    Prim_t* prim;
    Prim_new(&prim);
    while (!done) {
        PrimRangeIterator_deref(current, prim);

        Token_t const* tok_name;
        Prim_GetName(prim, &tok_name);
        char const* name_str = nullptr;
        Token_GetText(tok_name, &name_str);

        printf("%s\n", name_str);

        PrimRangeIterator_op_inc(current, &dummy);
        PrimRangeIterator_op_eq(current, end, &done);
    }

    Prim_dtor(prim);

    PrimRangeIterator_dtor(end);
    PrimRangeIterator_dtor(current);
    PrimRange_dtor(prim_range);

    Prim_dtor(pseudo_root);

    StageRefPtr_dtor(stage_ref);
    String_dtor(str_path);
}