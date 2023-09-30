#ifndef BBL_DETAIL_H
#define BBL_DETAIL_H

/**
 * @brief Indicates the success or failure of the funtion that returns it
 * 
 */
enum bbl_result_t {
    BBL_RESULT_Success = 0,
    BBL_RESULT_UnknownError = 1,
    BBL_RESULT_ArgumentOutOfRange,
    BBL_RESULT_ArgumentIsNull,
    BBL_RESULT_UnknownKind,
    BBL_RESULT_WrongKind,
    BBL_RESULT_NotFound,
};

struct bbl_layout_t {
    size_t size_bytes;
    size_t align_bytes;
};

enum bbl_bind_kind_t {
    BBL_BIND_KIND_OpaquePtr = 0,
    BBL_BIND_KIND_OpaqueBytes,
    BBL_BIND_KIND_ValueType,
};

enum bbl_builtin_t {
    BBL_BUILTIN_Void,
    BBL_BUILTIN_Bool,
    BBL_BUILTIN_UChar,
    BBL_BUILTIN_UShort,
    BBL_BUILTIN_ULong,
    BBL_BUILTIN_ULongLong,
    BBL_BUILTIN_UInt,
    BBL_BUILTIN_UInt8,
    BBL_BUILTIN_UInt16,
    BBL_BUILTIN_UInt32,
    BBL_BUILTIN_UInt64,
    BBL_BUILTIN_Size,
    BBL_BUILTIN_Char_S,
    BBL_BUILTIN_Char_U,
    BBL_BUILTIN_Short,
    BBL_BUILTIN_Int,
    BBL_BUILTIN_Long,
    BBL_BUILTIN_LongLong,
    BBL_BUILTIN_Int8,
    BBL_BUILTIN_Int16,
    BBL_BUILTIN_Int32,
    BBL_BUILTIN_Int64,
    BBL_BUILTIN_Float,
    BBL_BUILTIN_Double,
};

enum bbl_type_kind_t {
    BBL_TYPEKIND_Builtin,
    BBL_TYPEKIND_Class,
    BBL_TYPEKIND_ClassTemplateSpecialization,
    BBL_TYPEKIND_Enum,
    BBL_TYPEKIND_StdFunction,
    BBL_TYPEKIND_Pointer,
    BBL_TYPEKIND_LValueReference,
    BBL_TYPEKIND_RValueReference,
    BBL_TYPEKIND_Array,
};


#endif