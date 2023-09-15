#pragma once

#include "bbl.hpp"

#include "bbl_detail.h"

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

#if 0
template <> struct fmt::formatter<bbl::QType> {
    fmt::formatter<bbl::Builtin> fmt_builtin;

    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template <typename FormatContext>
    auto format(const bbl::QType& qt, FormatContext& ctx)
        -> decltype(ctx.out()) {

        char const* s_const = qt.is_const ? " const" : "";
        return format_to(ctx.out(), "{}{}", to_string(qt), s_const);
    }
};

template <> struct fmt::formatter<bbl::TemplateArg> {
    fmt::formatter<bbl::QType> fmt_qtype;

    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template <typename FormatContext>
    auto format(const bbl::TemplateArg& arg, FormatContext& ctx)
        -> decltype(ctx.out()) {

        if (std::holds_alternative<bbl::QType>(arg)) {
            auto const& qt = std::get<bbl::QType>(arg);
            fmt_qtype.format(qt, ctx);
            return ctx.out();
        } else {
            auto const& integral = std::get<bbl::Integral>(arg);
            return format_to(ctx.out(), "{}", integral.value);
        }
    }
};

template <> struct fmt::formatter<bbl::Method> {
    fmt::formatter<bbl::QType> fmt_qtype;

    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template <typename FormatContext>
    auto format(const bbl::Method& method, FormatContext& ctx)
        -> decltype(ctx.out()) {
        auto out = format_to(ctx.out(), "{}(", method.function.qualified_name);
        bool first = true;

        for (auto const& param : method.function.params) {
            if (first) {
                first = false;
            } else {
                out = format_to(out, ", ");
            }

            fmt_qtype.format(param.type, ctx);
            out = format_to(out, " {}", param.name);
        }
        out = format_to(out, ") -> ");
        fmt_qtype.format(method.function.return_type, ctx);

        if (!method.function.rename.empty()) {
            out = format_to(out, " \"{}\"", method.function.rename);
        }

        return out;
    }
};

template <> struct fmt::formatter<bbl::Class> {
    fmt::formatter<bbl::TemplateArg> fmt_template_arg;
    fmt::formatter<bbl::Method> fmt_method;

    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template <typename FormatContext>
    auto format(const bbl::Class& cls, FormatContext& ctx)
        -> decltype(ctx.out()) {

        if (!cls.template_args.empty()) {
            auto out =
                format_to(ctx.out(), "ClassTemplateSpecialization {} {}<",
                          cls.id, cls.qualified_name);

            bool first = true;
            for (auto const& arg : cls.template_args) {
                if (first) {
                    first = false;
                } else {
                    out = format_to(out, ", ");
                }
                fmt_template_arg.format(arg, ctx);
            }
            out = format_to(out, ">");

            if (!cls.rename.empty()) {
                out = format_to(out, " \"{}\"\n", cls.rename);
            }

            for (auto const& method : cls.methods) {
                out = format_to(out, "  ");
                out = fmt_method.format(method, ctx);
                out = format_to(out, "\n");
            }

            return out;
        } else {
            auto out = format_to(ctx.out(), "Class {} {} \"{}\"\n", cls.id,
                                 cls.qualified_name, cls.rename);
            for (auto const& method : cls.methods) {
                out = format_to(out, "  ");
                out = fmt_method.format(method, ctx);
                out = format_to(out, "\n");
            }

            return out;
        }
    }
};
#endif
