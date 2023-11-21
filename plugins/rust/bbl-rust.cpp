#include "bbl-capi.hpp"
#include <bbl-plugin.h>

#include <bbl-context.hpp>

#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <set>
#include <stdio.h>


using namespace babble;

static auto builtin_to_string(bbl_builtin_t builtin,
                              std::set<std::string>& imports) -> std::string {
    switch (builtin) {
    case BBL_BUILTIN_Void:
        imports.insert("std::ffi::c_void");
        return "c_void";
    case BBL_BUILTIN_Bool:
        return "bool";
    case BBL_BUILTIN_UChar:
        imports.insert("std::ffi::c_uchar");
        return "c_uchar";
    case BBL_BUILTIN_UShort:
        imports.insert("std::ffi::c_ushort");
        return "c_ushort";
    case BBL_BUILTIN_ULong:
        imports.insert("std::ffi::c_ulong");
        return "c_ulong";
    case BBL_BUILTIN_ULongLong:
        imports.insert("std::ffi::c_ulonglong");
        return "c_ulonglong";
    case BBL_BUILTIN_UInt:
        imports.insert("std::ffi::c_uint");
        return "c_uint";
    case BBL_BUILTIN_UInt8:
        return "u8";
    case BBL_BUILTIN_UInt16:
        return "u16";
    case BBL_BUILTIN_UInt32:
        return "u32";
    case BBL_BUILTIN_UInt64:
        return "u64";
    case BBL_BUILTIN_Size:
        return "usize";
    case BBL_BUILTIN_Char_S:
        imports.insert("std::ffi::c_char");
        return "c_char";
    case BBL_BUILTIN_Char_U:
        imports.insert("std::ffi::c_uchar");
        return "c_uchar";
    case BBL_BUILTIN_Short:
        imports.insert("std::ffi::c_short");
        return "c_short";
    case BBL_BUILTIN_Int:
        imports.insert("std::ffi::c_int");
        return "c_int";
    case BBL_BUILTIN_Long:
        imports.insert("std::ffi::c_long");
        return "c_long";
    case BBL_BUILTIN_LongLong:
        imports.insert("std::ffi::c_longlong");
        return "c_longlong";
    case BBL_BUILTIN_Int8:
        return "i8";
    case BBL_BUILTIN_Int16:
        return "i16";
    case BBL_BUILTIN_Int32:
        return "i32";
    case BBL_BUILTIN_Int64:
        return "i64";
    case BBL_BUILTIN_Float:
        imports.insert("std::ffi::c_float");
        return "c_float";
    case BBL_BUILTIN_Double:
        imports.insert("std::ffi::c_double");
        return "c_double";
    default:
        throw std::runtime_error(
            fmt::format("unknown builtin type {}", builtin));
    }
}

static auto sanitize_word(std::string_view word) -> std::string {
    static std::set<std::string> reserved = {
        "as",      "break",  "const",    "continue",    "crate",  "else",
        "enum",    "extern", "false",    "fn",          "for",    "if",
        "impl",    "in",     "let",      "loop",        "match",  "mod",
        "move",    "mut",    "pub",      "ref",         "return", "self",
        "Self",    "static", "struct",   "super",       "trait",  "true",
        "type",    "unsafe", "use",      "where",       "while",  "async",
        "await",   "dyn",    "abstract", "become",      "box",    "do",
        "final",   "macro",  "override", "priv",        "typeof", "unsized",
        "virtual", "yield",  "try",      "macro_rules", "union"};

    if (reserved.find(std::string(word)) != reserved.end()) {
        return fmt::format("{}_", word);
    } else {
        return std::string(word);
    }
}

static auto structid_to_string(C_API capi, C_StructId id) -> std::string_view {
    C_Struct strct = capi.get_struct(id);
    return strct.get_name();
}

static auto enumid_to_string(C_API capi, C_EnumId id) -> std::string_view {
    C_Enum enm = capi.get_enum(id);
    return enm.get_name();
}

static auto qtype_to_string(C_API capi,
                            C_QType qt,
                            std::set<std::string>& imports) -> std::string;

template <class ParamRangeT>
static auto parameter_list_to_string(C_API capi,
                                     ParamRangeT params,
                                     std::optional<C_Param> receiver,
                                     std::optional<C_Param> result_param,
                                     std::set<std::string>& imports)
    -> std::string {
    std::string result = "(";

    bool first = true;

    if (receiver.has_value()) {
        C_Param param = receiver.value();
        result = fmt::format("{}{}: {}",
                             result,
                             sanitize_word(param.get_name()),
                             qtype_to_string(capi, param.get_type(), imports));
        first = false;
    }

    for (auto param : params) {
        if (first) {
            first = false;
        } else {
            result = fmt::format("{}, ", result);
        }

        result = fmt::format("{}{}: {}",
                             result,
                             sanitize_word(param.get_name()),
                             qtype_to_string(capi, param.get_type(), imports));
    }

    if (result_param.has_value()) {
        result = fmt::format(
            "{}, {}: {}",
            result,
            sanitize_word(result_param->get_name()),
            qtype_to_string(capi, result_param->get_type(), imports));
    }

    return fmt::format("{})", result);
}

static auto stdfunction_to_string(C_API capi,
                                  C_StdFunctionId id,
                                  std::set<std::string>& imports)
    -> std::string {
    C_StdFunction fun = capi.get_stdfunction(id);

    return fmt::format(
        "fn{}",
        parameter_list_to_string(
            capi, fun.params(), {}, fun.get_result_param(), imports));
}

static auto qtype_to_string(C_API capi,
                            C_QType qt,
                            std::set<std::string>& imports) -> std::string {
    bbl_capi_type_kind_t kind = qt.get_type_kind();

    char const* s_mut = qt.is_const() ? "const " : "mut ";

    switch (kind) {
    case bbl_capi_typekind_Builtin:
        return builtin_to_string(qt.get_as_builtin().value(), imports);
    case bbl_capi_typekind_Struct:
        return std::string(
            structid_to_string(capi, qt.get_as_structid().value()));
    case bbl_capi_typekind_Enum:
        return std::string(enumid_to_string(capi, qt.get_as_enumid().value()));
    case bbl_capi_typekind_Pointer:
        return fmt::format(
            "*{}{}",
            s_mut,
            qtype_to_string(capi, qt.get_pointee_type().value(), imports));
    case bbl_capi_typekind_Array:
        return fmt::format(
            "[{}; {}]",
            qtype_to_string(capi, qt.get_array_element_type().value(), imports),
            qt.get_array_element_size().value());
    case bbl_capi_typekind_StdFunction:
        return stdfunction_to_string(
            capi, qt.get_as_stdfunctionid().value(), imports);
    default:
        throw std::runtime_error(fmt::format("unknown qtype kind {}", kind));
    }
}

extern "C" {

BBL_PLUGIN_API void bbl_plugin_init(char const** name, int* version) {
    *name = "rust";
    *version = BBL_PLUGIN_API_VERSION;
}

BBL_PLUGIN_API int bbl_plugin_exec(bbl_context_t cpp_ctx,
                                   bbl_capi_t capi_,
                                   char const* project_name,
                                   char const* output_path) {
    printf("running bbl rust\n");

    C_API capi(capi_);

    std::string source = "";
    std::set<std::string> imports;
    for (auto mod : capi.modules()) {

        for (auto enum_id : mod.enums()) {
            try {
                C_Enum enm = capi.get_enum(enum_id);
                source = fmt::format(
                    "{}#[repr(C)]\npub enum {} {{\n", source, enm.get_name());

                for (auto var : enm.variants()) {
                    source = fmt::format(
                        "{}    {} = {},\n", source, var.name, var.value);
                }

                source = fmt::format("{}}}\n\n", source);
            } catch (std::exception& e) {
                SPDLOG_ERROR("could not translate enum: {}", e.what());
                continue;
            }
        }

        for (auto struct_id : mod.structs()) {
            try {
                C_Struct strct = capi.get_struct(struct_id);
                if (strct.get_bind_kind() == BBL_BIND_KIND_OpaquePtr) {
                    source = fmt::format(
                        "{}pub struct {} {{\n", source, strct.get_name());
                    source = fmt::format("{}    _unused: [u8; 0],\n", source);
                    source = fmt::format("{}}}\n\n", source);
                } else if (strct.get_bind_kind() == BBL_BIND_KIND_ValueType) {
                    source = fmt::format("{}#[repr(C)]\n", source);
                    source = fmt::format(
                        "{}pub struct {} {{\n", source, strct.get_name());

                    for (auto field : strct.fields()) {
                        source = fmt::format(
                            "{}    {}: {},\n",
                            source,
                            field.get_name(),
                            qtype_to_string(capi, field.get_type(), imports));
                    }

                    source = fmt::format("{}}}\n\n", source);
                } else {
                    SPDLOG_ERROR("opaque bytes is unsupported on struct {}",
                                 strct.get_name());
                    continue;
                }
            } catch (std::exception& e) {
                SPDLOG_ERROR("could not translate struct: {}", e.what());
                continue;
            }
        }

        for (auto fun_id : mod.functions()) {
            try {
                C_Function fun = capi.get_function(fun_id);

                imports.insert("std::ffi::c_int");
                source = fmt::format(
                    "{}pub fn {}{} -> c_int;\n\n",
                    source,
                    fun.get_name(),
                    parameter_list_to_string(capi,
                                             fun.params(),
                                             fun.get_receiver_param(),
                                             fun.get_result_param(),
                                             imports));
            } catch (std::exception& e) {
                SPDLOG_ERROR("could not translate function: {}", e.what());
                continue;
            }
        }
    }

    std::string import_str;
    for (auto const& import : imports) {
        import_str = fmt::format("{}use {};\n", import_str, import);
    }

    if (!import_str.empty()) {
        source = fmt::format("{}\n{}", import_str, source);
    }

    printf("%s", source.c_str());

    if (!std::string(output_path).empty()) {
        std::string filename = fmt::format("{}/{}.rs", project_name);
        std::ofstream file;
        file.open(filename);
        file << source;
        file.close();
    }

    return 0;
}
}