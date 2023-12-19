#pragma once

#include "cpp_context.hpp"

#include "bbl-detail.h"

#include <spdlog/fmt/fmt.h>

template <> struct fmt::formatter<bbl_builtin_t> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template <typename FormatContext>
    auto format(const bbl_builtin_t& b, FormatContext& ctx)
        -> decltype(ctx.out()) {
#define FMT_BUILTIN(VAR, STR)                                                  \
    case bbl_builtin_t::BBL_BUILTIN_ ## VAR:                                                    \
        return format_to(ctx.out(), STR);

        switch (b) {
            FMT_BUILTIN(Void, "void")
            FMT_BUILTIN(Char_S, "char")
            FMT_BUILTIN(Short, "short")
            FMT_BUILTIN(Int, "int")
            FMT_BUILTIN(Long, "long")
            FMT_BUILTIN(LongLong, "long long")
            FMT_BUILTIN(UChar, "unsigned char")
            FMT_BUILTIN(UShort, "unsigned short")
            FMT_BUILTIN(UInt, "unsigned int")
            FMT_BUILTIN(ULong, "unsigned long")
            FMT_BUILTIN(ULongLong, "unsigned long long")
            FMT_BUILTIN(Float, "float")
            FMT_BUILTIN(Double, "double")
            FMT_BUILTIN(Int8, "int8_t")
            FMT_BUILTIN(Int16, "int16_t")
            FMT_BUILTIN(Int32, "int32_t")
            FMT_BUILTIN(Int64, "int64_t")
            FMT_BUILTIN(UInt8, "uint8_t")
            FMT_BUILTIN(UInt16, "uint16_t")
            FMT_BUILTIN(UInt32, "uint32_t")
            FMT_BUILTIN(UInt64, "uint64_t")
            FMT_BUILTIN(Size, "size_t")
            FMT_BUILTIN(Bool, "bool")
        default:
            BBL_THROW("unhandled builtin format: {}", int(b));
        }

#undef FMT_BUILTIN
    }
};
