#include "bbl-capi.h"
#include "bbl-context.h"
#include "bbl-detail.h"
#include "plugin.hpp"


#include <algorithm>
#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>

#include <exception>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <string.h>
#include <string>
#include <variant>

std::string builtin_to_string(bbl_builtin_t builtin) {
#define FMT_BUILTIN(VAR, STR)                                                  \
    case bbl_builtin_t::BBL_BUILTIN_##VAR:                                     \
        return STR;

    switch (builtin) {
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
        throw std::runtime_error("unhandled builtin format");
    }

#undef FMT_BUILTIN
}

std::string qtype_to_string(bbl_context_t ctx, bbl_qtype_t qt) {
    bbl_type_kind_t type_kind;
    bbl_qtype_get_type_kind(qt, &type_kind);
    bool is_const;
    bbl_qtype_is_const(qt, &is_const);
    char const* s_const = is_const ? " const" : "";

    bbl_builtin_t builtin;
    bbl_qtype_t pointee_type;
    bbl_classid_t classid;
    bbl_functionid_t functionid;
    bbl_stdfunctionid_t stdfunctionid;
    bbl_enumid_t enumid;

    bbl_class_t cls;
    bbl_stdfunction_t fun;
    bbl_enum_t enm;
    char const* spelling;
    size_t len;

    switch (type_kind) {
    case BBL_TYPEKIND_Builtin:
        bbl_qtype_get_as_builtin(qt, &builtin);
        return fmt::format("{}{}", builtin_to_string(builtin), s_const);
        break;
    case BBL_TYPEKIND_Pointer:
        bbl_qtype_get_pointee_type(qt, &pointee_type);
        return fmt::format("{}*{}", qtype_to_string(ctx, pointee_type),
                           s_const);
        break;
    case BBL_TYPEKIND_LValueReference:
        bbl_qtype_get_pointee_type(qt, &pointee_type);
        return fmt::format("{}&{}", qtype_to_string(ctx, pointee_type),
                           s_const);
        break;
    case BBL_TYPEKIND_RValueReference:
        bbl_qtype_get_pointee_type(qt, &pointee_type);
        return fmt::format("{}&&{}", qtype_to_string(ctx, pointee_type),
                           s_const);
        break;
    case BBL_TYPEKIND_Array:
        bbl_qtype_get_array_element_type(qt, &pointee_type);
        bbl_qtype_get_array_size(qt, &len);
        return fmt::format("{}[{}]{}", qtype_to_string(ctx, pointee_type), len,
                           s_const);
        break;
    case BBL_TYPEKIND_Class:
    case BBL_TYPEKIND_ClassTemplateSpecialization:
        bbl_qtype_get_as_classid(qt, &classid);
        bbl_context_get_class(ctx, classid, &cls);
        bbl_class_get_spelling(cls, &spelling, &len);
        return fmt::format("{}{}", std::string_view(spelling, len), s_const);
        break;
    case BBL_TYPEKIND_StdFunction:
        bbl_qtype_get_as_stdfunctionid(qt, &stdfunctionid);
        bbl_context_get_stdfunction(ctx, stdfunctionid, &fun);
        bbl_stdfunction_get_spelling(fun, &spelling, &len);
        return fmt::format("{}{}", std::string_view(spelling, len), s_const);
        break;
    case BBL_TYPEKIND_Enum:
        bbl_qtype_get_as_enumid(qt, &enumid);
        bbl_context_get_enum(ctx, enumid, &enm);
        bbl_enum_get_spelling(enm, &spelling, &len);
        return fmt::format("{}{}", std::string_view(spelling, len), s_const);
        break;
    }

    throw std::runtime_error("unhandled type");
}

std::string function_to_string(bbl_context_t ctx, bbl_function_t fun) {
    std::string result;

    char const* name;
    char const* rename;
    size_t len;
    bbl_function_get_name(fun, &name, &len);

    result = fmt::format("{}{}(", result, std::string_view(name, len));

    bool first = true;

    size_t num_params = 0;
    bbl_function_num_params(fun, &num_params);

    for (size_t i = 0; i < num_params; ++i) {
        if (first) {
            first = false;
        } else {
            result = fmt::format("{}, ", result);
        }

        bbl_param_t param;
        bbl_function_get_param(fun, i, &param);

        bbl_qtype_t qt;
        char const* name;
        bbl_param_get_name(param, &name, &len);
        bbl_param_get_type(param, &qt);

        result = fmt::format("{}{} {}", result, qtype_to_string(ctx, qt),
                             std::string_view(name, len));
    }

    bbl_qtype_t return_type;
    bbl_function_get_return_type(fun, &return_type);

    result =
        fmt::format("{}) -> {}", result, qtype_to_string(ctx, return_type));

    bbl_function_get_rename(fun, &rename, &len);
    if (len > 0) {
        result =
            fmt::format("{} \"{}\"", result, std::string_view(rename, len));
    }

    bool is_noexcept;
    bbl_function_get_is_noexcept(fun, &is_noexcept);

    if (is_noexcept) {
        result = fmt::format("{} noexcept", result);
    }

    return result;
}

std::string constructor_to_string(bbl_context_t ctx, bbl_constructor_t ctor) {
    bool first = true;

    size_t num_params = 0;
    bbl_constructor_num_params(ctor, &num_params);

    std::string result;

    size_t len = 0;
    char const* rename = nullptr;
    bbl_constructor_get_rename(ctor, &rename, &len);
    if (len > 0) {
        result =
            fmt::format("{}\"{}\" ", result, std::string_view(rename, len));
    }

    result = fmt::format("{}(", result);
    for (size_t i = 0; i < num_params; ++i) {
        if (first) {
            first = false;
        } else {
            result = fmt::format("{}, ", result);
        }

        bbl_param_t param;
        bbl_constructor_get_param(ctor, i, &param);

        bbl_qtype_t qt;
        char const* name;
        bbl_param_get_name(param, &name, &len);
        bbl_param_get_type(param, &qt);

        result = fmt::format("{}{} {}", result, qtype_to_string(ctx, qt),
                             std::string_view(name, len));
    }
    result = fmt::format("{})", result);

    bool is_noexcept = false;
    bbl_constructor_get_is_noexcept(ctor, &is_noexcept);
    if (is_noexcept) {
        result = fmt::format("{} noexcept", result);
    }

    return result;
}

std::string method_to_string(bbl_context_t ctx, bbl_method_t method) {
    bbl_function_t function = nullptr;
    bbl_method_get_function(method, &function);

    std::string result = function_to_string(ctx, function);

    bool is_const = false;
    bool is_static = false;
    bool is_virtual = false;
    bool is_pure = false;
    bbl_method_get_is_const(method, &is_const);
    bbl_method_get_is_static(method, &is_static);
    bbl_method_get_is_virtual(method, &is_virtual);
    bbl_method_get_is_pure(method, &is_pure);

    if (is_const) {
        result = fmt::format("{} const", result);
    }

    if (is_static) {
        result = fmt::format("{} static", result);
    }

    if (is_virtual) {
        result = fmt::format("{} virtual", result);
    }

    if (is_pure) {
        result = fmt::format("{} pure", result);
    }

    return result;
}

std::string class_to_string(bbl_context_t ctx, bbl_class_t cls) {
    size_t len = 0;
    char const* spelling;
    char const* rename;
    bbl_layout_t layout;
    bbl_bind_kind_t bind_kind;

    bbl_class_get_spelling(cls, &spelling, &len);
    bbl_class_get_layout(cls, &layout);
    bbl_class_get_bind_kind(cls, &bind_kind);

    std::string result =
        fmt::format("{} [{}:{}]", std::string_view(spelling, len),
                    layout.size_bytes, layout.align_bytes);

    bool is_abstract = false;
    bbl_class_is_abstract(cls, &is_abstract);
    if (is_abstract) {
        result = fmt::format("{} abstract", result);
    }

    bool is_copy_constructible = false;
    bbl_class_is_copy_constructible(cls, &is_copy_constructible);
    if (is_copy_constructible) {
        result = fmt::format("{} copy", result);

        bool is_nothrow_copy_constructible = false;
        bbl_class_is_nothrow_copy_constructible(cls,
                                                &is_nothrow_copy_constructible);
        if (is_nothrow_copy_constructible) {
            result = fmt::format("{}(nothrow)", result);
        }
    }

    bool is_move_constructible = false;
    bbl_class_is_move_constructible(cls, &is_move_constructible);
    if (is_move_constructible) {
        result = fmt::format("{} move", result);

        bool is_nothrow_move_constructible = false;
        bbl_class_is_nothrow_move_constructible(cls,
                                                &is_nothrow_move_constructible);
        if (is_nothrow_move_constructible) {
            result = fmt::format("{}(nothrow)", result);
        }
    }

    bool is_copy_assignable = false;
    bbl_class_is_copy_assignable(cls, &is_copy_assignable);
    if (is_copy_assignable) {
        result = fmt::format("{} copyass", result);

        bool is_nothrow_copy_assignable = false;
        bbl_class_is_nothrow_copy_assignable(cls, &is_nothrow_copy_assignable);
        if (is_nothrow_copy_assignable) {
            result = fmt::format("{}(nothrow)", result);
        }
    }

    bool is_move_assignable = false;
    bbl_class_is_move_assignable(cls, &is_move_assignable);
    if (is_move_assignable) {
        result = fmt::format("{} moveass", result);

        bool is_nothrow_move_assignable = false;
        bbl_class_is_nothrow_move_assignable(cls, &is_nothrow_move_assignable);
        if (is_nothrow_move_assignable) {
            result = fmt::format("{}(nothrow)", result);
        }
    }

    switch (bind_kind) {
    case BBL_BIND_KIND_OpaquePtr:
        result = fmt::format("{} OP", result);
        break;
    case BBL_BIND_KIND_OpaqueBytes:
        result = fmt::format("{} OB", result);
        break;
    case BBL_BIND_KIND_ValueType:
        result = fmt::format("{} VT", result);
        break;
    }

    bbl_class_get_rename(cls, &rename, &len);
    if (len > 0) {
        result =
            fmt::format("{} \"{}\"", result, std::string_view(rename, len));
    }

    return result;
}

std::string stdfunction_to_string(bbl_context_t ctx, bbl_stdfunction_t fun) {
    std::string result;
    size_t len = 0;
    char const* spelling;
    bbl_qtype_t return_type;

    bbl_stdfunction_get_spelling(fun, &spelling, &len);
    bbl_stdfunction_get_return_type(fun, &return_type);

    result =
        fmt::format("{} std::function<{} (", std::string_view(spelling, len),
                    qtype_to_string(ctx, return_type));

    bool first = true;
    size_t num_params = 0;
    bbl_stdfunction_num_params(fun, &num_params);

    for (size_t i = 0; i < num_params; ++i) {
        if (first) {
            first = false;
        } else {
            result = fmt::format("{}, ", result);
        }

        bbl_qtype_t qt;
        bbl_stdfunction_get_param(fun, i, &qt);

        result = fmt::format("{}{}", result, qtype_to_string(ctx, qt));
    }

    result = fmt::format("{})>", result);

    return result;
}

std::string enum_to_string(bbl_context_t ctx, bbl_enum_t enm) {

    char const* spelling;
    size_t len;
    bbl_enum_get_spelling(enm, &spelling, &len);
    bbl_qtype_t type;
    bbl_enum_get_type(enm, &type);

    std::string result =
        fmt::format("{}: {} {{", std::string_view(spelling, len),
                    qtype_to_string(ctx, type));

    bool first = true;

    size_t num_variants = 0;
    bbl_enum_num_variants(enm, &num_variants);

    for (size_t i = 0; i < num_variants; ++i) {
        if (first) {
            first = false;
        } else {
            result = fmt::format("{}, ", result);
        }

        char const* name;
        size_t len_name = 0;
        char const* value;
        size_t len_value = 0;
        bbl_enum_get_variant(enm, i, &name, &len_name, &value, &len_value);

        result =
            fmt::format("{}{} = {}", result, std::string_view(name, len_name),
                        std::string_view(value, len_value));
    }

    result = fmt::format("{}}}", result);

    return result;
}

int main(int argc, char const** argv) {
    // parse args
    // clang expects arguments like:
    //     source.cpp -- -Iinclude1 -Iinclude2 -Wno-thing
    // so we'll take options like the output path behind a second `--`.

    std::vector<std::string> bbl_opts;
    int clang_arg_count = argc;
    int double_dash_count = 0;
    for (int i = 1; i < argc; ++i) {
        if (std::string("--") == argv[i]) {
            double_dash_count++;
            if (double_dash_count == 2) {
                clang_arg_count = i;
            }
        } else if (double_dash_count == 2) {
            bbl_opts.push_back(argv[i]);
        }
    }

    // first, create the context object
    bbl_context_t ctx;
    bbl_context_create(&ctx);

    // next, compile and run the extraction on the source files passed in as
    // arguments.
    // XXX: Should replace this with a dedicated options struct
    if (bbl_context_compile_and_extract(ctx, clang_arg_count, argv) !=
        BBL_RESULT_Success) {
        SPDLOG_ERROR("compilation failed");
        return 1;
    }

    size_t len;
    std::string path_out_cpp;
    std::string path_out_h;
    if (bbl_opts.size() == 2) {
        path_out_cpp = bbl_opts[0];
        path_out_h = bbl_opts[1];
    }

    // or just translate to C using the built-in functionality
    bbl_capi_t capi = nullptr;
    if (bbl_capi_create(ctx, &capi) != BBL_RESULT_Success) {
        SPDLOG_ERROR("could not create C API");
        return 1;
    }
    char const* header;
    len = 0;
    if (bbl_capi_get_header(capi, &header, &len) != BBL_RESULT_Success) {
        SPDLOG_ERROR("could not get header");
        return 2;
    }

    char const* source;
    if (bbl_capi_get_source(capi, &source, &len) != BBL_RESULT_Success) {
        SPDLOG_ERROR("could not get source");
        return 3;
    }

    // generate header guard
    std::filesystem::path fs_out_h(path_out_h);
    std::string header_guard = fs_out_h.filename().string();
    std::transform(header_guard.begin(), header_guard.end(),
                   header_guard.begin(), [](unsigned char c) {
                       if (c == '.' || c == '-') {
                           return '_';
                       } else {
                           return (char)toupper(c);
                       }
                   });
    header_guard = fmt::format("__{}__", header_guard);
    std::string header_str = fmt::format(
        "#ifndef {0}\n#define {0}\n{1}\n\n#endif\n", header_guard, header);

    if (path_out_cpp.empty() || path_out_h.empty()) {
        fmt::print("--- HEADER ---\n{}--- END HEADER ---\n\n", header_str);
        fmt::print("--- SOURCE ---\n{}--- END SOURCE ---\n", source);
    } else {
        std::ofstream os_cpp;
        os_cpp.open(path_out_cpp);
        if (!os_cpp.is_open()) {
            SPDLOG_ERROR("could not open output cpp file {}", path_out_cpp);
            return 4;
        }

        std::ofstream os_h;
        os_h.open(path_out_h);
        if (!os_h.is_open()) {
            SPDLOG_ERROR("could not open output h file {}", path_out_h);
            return 5;
        }

        os_cpp << source;
        os_h << header_str;
    }

    // Create the plugin manager to load other language plugins
    bbl::PluginManager plugin_manager;
    plugin_manager.exec(ctx, capi, ".");

    // finally, be a good little boy and clean up
    bbl_capi_destroy(capi);
    bbl_context_destroy(ctx);

    return 0;
}