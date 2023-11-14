#pragma once

#include "bbl-detail.h"

#include "unordered_dense.h"

#include <optional>
#include <spdlog/spdlog.h>

#if defined(WIN32)
#pragma warning(push)
#pragma warning(disable : 4624)
#pragma warning(disable : 4291)
#endif

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Type.h>
#include <llvm/Support/CommandLine.h>

#if defined(WIN32)
#pragma warning(pop)
#endif

#include <exception>
#include <map>
#include <memory>
#include <stdint.h>
#include <string>
#include <variant>

namespace bbl {

class Context;

#define BBL_THROW(...)                                                         \
    throw std::runtime_error(                                                  \
        fmt::format("{}:{} {}", __FILE__, __LINE__, fmt::format(__VA_ARGS__)))

#define BBL_THROW_MTBE(...)                                                    \
    throw MissingTypeBindingException(                                         \
        fmt::format("{}:{} {}", __FILE__, __LINE__, fmt::format(__VA_ARGS__)))

#define BBL_RETHROW(E, ...)                                                    \
    throw std::runtime_error(fmt::format("{}:{} {}\n  {}",                     \
                                         __FILE__,                             \
                                         __LINE__,                             \
                                         fmt::format(__VA_ARGS__),             \
                                         E.what()))

#define BBL_RETHROW_MTBE(E, ...)                                               \
    throw MissingTypeBindingException(fmt::format("{}:{} {}\n  {}",            \
                                                  __FILE__,                    \
                                                  __LINE__,                    \
                                                  fmt::format(__VA_ARGS__),    \
                                                  E.what()))

#define BBL_UNIMPLEMENTED(...)                                                 \
    throw std::runtime_error(fmt::format("{}:{} UNIMPLEMENTED {}",             \
                                         __FILE__,                             \
                                         __LINE__,                             \
                                         fmt::format(__VA_ARGS__)))

#define BBL_UNREACHABLE(...)                                                   \
    throw std::runtime_error(fmt::format(                                      \
        "{}:{} UNREACHABLE {}", __FILE__, __LINE__, fmt::format(__VA_ARGS__)))

template <typename Key, typename Value>
using MapType = ankerl::unordered_dense::map<Key, Value>;

/// Custom exception type for notifying about missing type bindings when
/// extracting functions etc.
class MissingTypeBindingException : public std::runtime_error {
public:
    MissingTypeBindingException(std::string const& what)
        : std::runtime_error(what) {}
};

struct ClassId {
    std::string id;
};

inline bool operator==(ClassId const& lhs, ClassId const& rhs) {
    return lhs.id == rhs.id;
}

struct ClassTemplateSpecializationId {
    std::string id;
};

inline bool operator==(ClassTemplateSpecializationId const& lhs,
                       ClassTemplateSpecializationId const& rhs) {
    return lhs.id == rhs.id;
}

struct EnumId {
    std::string id;
};

inline bool operator==(EnumId const& lhs, EnumId const& rhs) {
    return lhs.id == rhs.id;
}

struct StdFunctionId {
    std::string id;
};

inline bool operator==(StdFunctionId const& lhs, StdFunctionId const& rhs) {
    return lhs.id == rhs.id;
}

struct Type {
    std::variant<bbl_builtin_t,
                 ClassId,
                 ClassTemplateSpecializationId,
                 EnumId,
                 StdFunctionId>
        kind;
};

bool operator==(Type const& lhs, Type const& rhs);

class QType;
bool operator==(QType const& lhs, QType const& rhs);
inline bool operator!=(QType const& lhs, QType const& rhs) {
    return !(lhs == rhs);
}

struct Pointer {
    std::unique_ptr<QType> pointee;
};

inline bool operator==(Pointer const& lhs, Pointer const& rhs) {
    return *lhs.pointee == *rhs.pointee;
}

struct LValueReference {
    std::unique_ptr<QType> pointee;
};

inline bool operator==(LValueReference const& lhs, LValueReference const& rhs) {
    return *lhs.pointee == *rhs.pointee;
}

struct RValueReference {
    std::unique_ptr<QType> pointee;
};

inline bool operator==(RValueReference const& lhs, RValueReference const& rhs) {
    return *lhs.pointee == *rhs.pointee;
}

struct Array {
    std::unique_ptr<QType> element_type;
    size_t size;
};

inline bool operator==(Array const& lhs, Array const& rhs) {
    return *lhs.element_type == *rhs.element_type && lhs.size == rhs.size;
}

/// A (const) qualified type
struct QType {
    bool is_const;
    std::variant<Type, Pointer, LValueReference, RValueReference, Array> type;
};

QType clone(QType const& qt);
QType wrap_in_pointer(QType const& qt);

/// An integral value, represented as a string for precision's sake. Used to
/// store template arguments
struct Integral {
    std::string value;
};

using TemplateArg = std::variant<QType, Integral>;

/// A function parameter
struct Param {
    std::string name;
    QType type;
};

inline bool is_equivalent(Param const& lhs, Param const& rhs) {
    return lhs.type == rhs.type;
}

/// A function, as bound by `bbl::fn()`
struct Function {
    std::string qualified_name;
    std::string name;
    std::string rename;
    std::string spelling;
    std::string template_call;
    std::string comment;

    QType return_type;
    std::vector<Param> params;

    /// is explicitly declared (or evaluated) not to throw
    bool is_noexcept;
};

inline bool is_equivalent(Function const& lhs, Function const& rhs) {
    if (lhs.name != rhs.name || lhs.template_call != rhs.template_call ||
        lhs.return_type != rhs.return_type ||
        lhs.params.size() != rhs.params.size()) {
        return false;
    }

    for (int i = 0; i < lhs.params.size(); ++i) {
        if (!is_equivalent(lhs.params[i], rhs.params[i])) {
            return false;
        }
    }

    return lhs.is_noexcept == rhs.is_noexcept;
}

/// A method, as bound by the `.m()` call on bbl::Class
struct Method {
    Function function;
    std::string target_class_id;
    bool is_const;
    bool is_static;
    bool is_virtual;
    bool is_pure;
};

inline bool is_equivalent(Method const& lhs, Method const& rhs) {
    return is_equivalent(lhs.function, rhs.function) &&
           lhs.is_const == rhs.is_const && lhs.is_static == rhs.is_static;
}

using MethodMap = MapType<std::string, Method>;

/// A Constructor, basically a more-limited Method
struct Constructor {
    std::string rename;
    std::string comment;
    std::vector<Param> params;
    bool is_noexcept;
};

/// The layout of a given type in terms of its size and alignment
///
/// Note: these values are computed on the platform the binding is generated on.
struct Layout {
    size_t size_bytes;
    size_t align_bytes;
};

/// The desired representation of the Class in the C API
///
/// * OpaquePtr (the default) is the "traditional" binding method of just
/// passing pointers back and forth
/// * ValueType means a matching type is created in C, with all members exposed.
/// Useful for things like 3D vectors etc.
/// * OpaqueBytes is halfway in-between, where the type is represented as an
/// opaque bag of bytes that the C side cannot introspect, but is still passed
/// on the stack
///
/// One of these can be selected by calling `opaque_ptr()`, `opaque_bytes()` or
/// `value_type()` on bbl::Class(). The last one called, wins. If none is
/// called, the default is OpaquePtr.
///
/// Types that are not moveable or copyable cannot be bound as anything other
/// than OpaquePtr and will fail a static_assert in the binding compilation if
/// they are set otherwise
enum class BindKind { OpaquePtr = 0, OpaqueBytes, ValueType };

struct RuleOfSeven {
    bool is_default_constructible;
    bool is_copy_constructible;
    bool is_nothrow_copy_constructible;
    bool is_move_constructible;
    bool is_nothrow_move_constructible;
    bool is_copy_assignable;
    bool is_nothrow_copy_assignable;
    bool is_move_assignable;
    bool is_nothrow_move_assignable;
    bool is_destructible;
    bool has_virtual_destructor;
};

struct Field {
    QType type;
    std::string name;
    std::string comment;
};

/// A struct, class or class template specialization, bound by and detected from
/// a `bbl::Class<X>()`
struct Class {
    Context* ctx;
    std::string qualified_name;
    std::string spelling;
    std::string name;
    std::string rename;
    std::string comment;
    std::vector<TemplateArg> template_args;
    std::vector<std::string> methods;
    std::vector<std::string> constructors;
    std::vector<Field> fields;
    std::optional<std::string> smartptr_to;
    bool smartptr_is_const;
    std::vector<std::string> inherits_from;
    Layout layout;
    BindKind bind_kind;
    RuleOfSeven rule_of_seven;
    bool is_abstract;
    std::string id;
};

/// A std::function specialization, bound by and detected from a
/// `bbl::Class<X>()`
///
/// We have separate bindings for these because they require special handling
struct StdFunction {
    std::string spelling;
    std::string rename;
    std::string comment;
    QType return_type;
    std::vector<QType> params;
};

/// A single enum variant, as a pair of the name and integral value, where the
/// integral value is also stored as a string for precision's sake
using EnumVariant = std::pair<std::string, std::string>;

/// A bound enum type, as bound by `bbl::Enum<X>()`
struct Enum {
    /// How the enum was spelt in the binding, i.e. `X` in `bbl::Enum<X>()`
    std::string spelling;
    /// The name of the enum
    std::string name;
    /// The name to give the enum in the C API
    std::string rename;
    std::string comment;
    /// List of variants in the enum
    std::vector<EnumVariant> variants;
    /// The type that this enum is represented as (e.g. `int` in most cases)
    QType integer_type;
    /// The prefix string for the variants
    /// Is no value, then one is autogenerated from the enum name.
    /// If an empty string, then no prefix is applied.
    std::optional<std::string> prefix;
};

/// A single Module, as declared by the BBL_MODULE(name) macro in the binding
/// source
struct Module {
    /// The binding source files that this module was authored in
    std::vector<std::string> source_files;
    /// The name of the module
    std::string name;
    /// List of Classes bound in this module
    std::vector<std::string> classes;
    /// List of Functions bound in this module
    std::vector<std::string> functions;
    /// List of StdFunctions bound in this module
    std::vector<std::string> stdfunctions;
    /// List of Enums bound in this module
    std::vector<std::string> enums;
    /// Function implementations to be pasted into the module
    std::vector<std::string> function_impls;
    /// Namespace string to replace
    std::string namespace_from;
    /// Namespace string to replace with
    std::string namespace_to;
};

/// Represents a `#include` directive from the binding source file
struct Inclusion {
    /// The full include directive, as written
    std::string directive;
    /// The filename as spelt in the include directive
    std::string filename;
    /// true if the directive used `"` instead of `<`
    bool is_local;
};

struct SourceFile {
    std::vector<Inclusion> inclusions;
    std::vector<std::string> modules;
    std::string filename;
};

using ClassMap = MapType<std::string, Class>;
using FunctionMap = MapType<std::string, Function>;
using ConstructorMap = MapType<std::string, Constructor>;
using StdFunctionMap = MapType<std::string, StdFunction>;
using EnumMap = MapType<std::string, Enum>;
using TypenameMap = MapType<std::string, std::string>;
using ModuleMap = MapType<std::string, Module>;
using TypeToModuleMap = MapType<std::string, std::string>;
using SourceFileMap = MapType<std::string, SourceFile>;

struct DeclMaps {
    ClassMap class_map;
    MethodMap method_map;
    ConstructorMap constructor_map;
    FunctionMap function_map;
    StdFunctionMap stdfunction_map;
    EnumMap enum_map;
    TypenameMap typename_map;
};

class Context {
    DeclMaps _decl_maps;
    ModuleMap _module_map;
    TypeToModuleMap _type_to_module_map;
    SourceFileMap _source_file_map;
    llvm::cl::OptionCategory _option_category;

public:
    Context();

    /// Extract the list of inclusions from the given source file
    auto extract_inclusions(std::string const& source_file)
        -> std::vector<Inclusion>;

    /// Extract the given TemplateArgument, `arg` and push it onto the list of
    /// `template_args`, recursively expanding template parameter packs if
    /// found.
    auto extract_single_template_arg(clang::TemplateArgument const& arg,
                                     std::vector<TemplateArg>& template_args,
                                     clang::MangleContext* mangle_ctx) -> void;

    /// Extract the template arguments from `ctsd` and push them onto
    /// `template_args`.
    auto extract_template_arguments(
        clang::ClassTemplateSpecializationDecl const* ctsd,
        std::vector<TemplateArg>& template_args,
        clang::MangleContext* mangle_ctx) -> void;

    /// Extract the given qualified type, `qt`
    auto extract_qualtype(clang::QualType const& qt,
                          clang::MangleContext* mangle_ctx) -> QType;

    /// Extract the class (or class template specialization) `crd`.
    [[nodiscard]] auto
    extract_class_binding(clang::CXXRecordDecl const* crd,
                          std::string const& spelling,
                          std::string const& rename,
                          std::string const& comment,
                          Layout layout,
                          BindKind bind_kind,
                          RuleOfSeven const& rule_of_seven,
                          bool is_abstract,
                          std::vector<std::string> const& inherits_from,
                          clang::MangleContext* mangle_ctx) -> Class;

    /// Insert the class `cls` with ID `id` into Module `mod_id`
    auto insert_class_binding(std::string const& mod_id,
                              std::string const& id,
                              Class&& cls) -> void;

    [[nodiscard]] auto extract_stdfunction_binding(
        clang::ClassTemplateSpecializationDecl const* ctsd,
        std::string const& spelling,
        std::string const& rename,
        std::string const& comment,
        clang::MangleContext* mangle_ctx) -> StdFunction;
    auto insert_stdfunction_binding(std::string const& mod_id,
                                    std::string const& id,
                                    StdFunction&& fun) -> void;

    /// Get the StdFunction with given `id`. Returns nullptr on failure
    auto has_stdfunction(std::string const& id) const noexcept -> bool;
    auto get_stdfunction(std::string const& id) noexcept -> StdFunction*;
    auto get_stdfunction(std::string const& id) const noexcept
        -> StdFunction const*;

    auto stdfunctions() const noexcept
        -> StdFunctionMap::value_container_type const&;

    [[nodiscard]] auto
    extract_enum_binding(clang::EnumDecl const* ed,
                         std::string const& spelling,
                         std::string const& name,
                         std::string const& rename,
                         std::string const& comment,
                         std::optional<std::string> const& prefix,
                         clang::MangleContext* mangle_ctx) -> Enum;
    auto insert_enum_binding(std::string const& mod_id,
                             std::string const& id,
                             Enum&& fun) -> void;

    /// Get the Enum with given `id`. Returns nullptr on failure
    auto has_enum(std::string const& id) const noexcept -> bool;
    auto get_enum(std::string const& id) noexcept -> Enum*;
    auto get_enum(std::string const& id) const noexcept -> Enum const*;

    /// Extracts type information from CXXMethodDecl `cmd` and converts it to a
    /// bbl::Method
    [[nodiscard]] auto
    extract_method_binding(clang::CXXMethodDecl const* cmd,
                           std::string const& target_class_id,
                           std::string const& rename,
                           std::string const& template_call,
                           std::string const& comment,
                           clang::MangleContext* mangle_ctx) -> Method;

    auto has_method(std::string const& method_id) const -> bool;
    auto insert_method_binding(std::string const& method_id, Method&& method)
        -> void;
    auto get_method(std::string const& method_id) const -> Method const*;

    auto has_constructor(std::string const& constructor_id) const -> bool;
    auto insert_constructor_binding(std::string const& constructor_id,
                                    Constructor&& constructor) -> void;
    auto get_constructor(std::string const& constructor_id) const
        -> Constructor const*;

    /// Extracts type information from FunctionDecl `fd` and converts it to a
    /// bbl::Function
    [[nodiscard]] auto
    extract_function_binding(clang::FunctionDecl const* fd,
                             std::string const& rename,
                             std::string const& spelling,
                             std::string const& template_call,
                             std::string const& comment,
                             clang::MangleContext* mangle_ctx) -> Function;

    /// Insert the Function `fun` with given `id` in the Module with ID `mod_id`
    auto insert_function_binding(std::string const& mod_id,
                                 std::string const& id,
                                 Function&& fun) -> void;

    /// Get the Function with given `id`. Returns nullptr on failure
    auto get_function(std::string const& id) noexcept -> Function*;
    auto get_function(std::string const& id) const noexcept -> Function const*;

    /// Get the Module with the given id. Returns nullptr on failure
    auto get_module(std::string const& id) -> Module*;

    /// Insert the given Module into the context with the given id
    auto insert_module(std::string const& id, Module&& module) -> void;

    /// Get the number of Modules on this Context
    auto num_modules() const noexcept -> size_t;

    /// Get the Module storage directly
    auto modules() const noexcept -> ModuleMap::value_container_type const&;

    /// Return true if this context has a Class with the given id
    auto has_class(std::string const& id) const noexcept -> bool;

    /// Get the Class with given `id`. Returns nullptr on failure
    auto get_class(std::string const& id) noexcept -> Class*;

    /// Get the Class with given `id`. Returns nullptr on failure
    auto get_class(std::string const& id) const noexcept -> Class const*;

    /// Get the SourceFile with given `filename`. Returns nullptr on failure
    auto get_source_file(const std::string& filename) -> SourceFile*;

    /// Get the SourceFile with given `filename`. Returns nullptr on failure
    auto get_source_file(const std::string& filename) const
        -> SourceFile const*;

    /// Insert the given SourceFile into the context with the given `filename`
    /// identifier
    auto insert_source_file(const std::string& filename,
                            SourceFile&& source_file) -> void;

    auto insert_function_impl(std::string const& mod_id,
                              std::string const& impl) -> void;

    /// Compile the sources in the given command line to AST and run the
    /// extraction.
    ///
    /// This stores the necessary information for performing a later
    /// transpilation to a C API on this Context object.
    auto compile_and_extract(int argc, char const** argv) noexcept(false)
        -> void;

    /// Get a string representation of the given `QType` suitable for debugging
    /// and test printing
    std::string get_qtype_as_string(QType const& qt) const;

    /// Get a string representation of the given `Class` suitable for debugging
    /// and test printing
    std::string get_class_as_string(Class const& cls);

    /// Get a string representation of the given `Method` suitable for debugging
    /// and test printing
    std::string get_method_as_string(Method const& method) const;

    /// Get a string representation of the given `Function` suitable for
    /// debugging and test printing
    std::string get_function_as_string(Function const& function) const;

    /// Get a string representation of the given `StdFunction` suitable for
    /// debugging and test printing
    std::string get_stdfunction_as_string(StdFunction const& function) const;

    /// Get a string representation of the given `Enum` suitable for debugging
    /// and test printing
    std::string get_enum_as_string(Enum const& enm);
};

auto evaluate_field_expression_bool(clang::RecordDecl const* rd,
                                    char const* name,
                                    clang::ASTContext& ast_context) -> bool;

} // namespace bbl