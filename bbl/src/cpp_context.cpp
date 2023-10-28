#include "cpp_context.hpp"
#include "astutil.hpp"
#include "bbl_detail.h"
#include "bblfmt.hpp"
#include "process.hpp"

#include <clang/AST/DeclCXX.h>
#include <clang/AST/ExprCXX.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTTypeTraits.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Mangle.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/TemplateBase.h>
#include <clang/AST/Type.h>
#include <clang/Basic/ExceptionSpecificationType.h>
#include <clang/Frontend/ASTUnit.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/raw_ostream.h>

#include <filesystem>
#include <memory>
#include <optional>
#include <ratio>
#include <regex>
#include <spdlog/spdlog.h>

#include <chrono>
#include <exception>
#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

using llvm::dyn_cast;

namespace bbl {

QType clone(QType const& qt) {
    if (std::holds_alternative<Type>(qt.type)) {
        const auto& type = std::get<Type>(qt.type);
        return QType{qt.is_const, type};
    } else if (std::holds_alternative<Pointer>(qt.type)) {
        const auto& pointer = std::get<Pointer>(qt.type);
        return QType{qt.is_const,
                     Pointer{std::make_unique<QType>(clone(*pointer.pointee))}};
    } else if (std::holds_alternative<LValueReference>(qt.type)) {
        const auto& lvref = std::get<LValueReference>(qt.type);
        return QType{
            qt.is_const,
            LValueReference{std::make_unique<QType>(clone(*lvref.pointee))}};
    } else if (std::holds_alternative<RValueReference>(qt.type)) {
        const auto& rvref = std::get<RValueReference>(qt.type);
        return QType{
            qt.is_const,
            RValueReference{std::make_unique<QType>(clone(*rvref.pointee))}};
    } else if (std::holds_alternative<Array>(qt.type)) {
        const auto& array = std::get<Array>(qt.type);
        return QType{
            qt.is_const,
            Array{std::make_unique<QType>(clone(*array.element_type)),
                  array.size}
        };
    } else {
        BBL_THROW("unreachable QType variant");
    }
}

QType wrap_in_pointer(QType const& qt) {
    return QType {
        false,
        Pointer {
            std::make_unique<QType>(clone(qt))
        }
    };
}

class Timer {
    using time_point = std::chrono::time_point<std::chrono::steady_clock>;

    time_point _start;

public:
    Timer() : _start(std::chrono::steady_clock::now()) {}

    double elapsed_seconds() {
        time_point end = std::chrono::steady_clock::now();
        auto dur = end - _start;
        return std::chrono::duration<double, std::milli>(dur).count() / 1000.0;
    }

    double elapsed_milliseconds() {
        time_point end = std::chrono::steady_clock::now();
        auto dur = end - _start;
        return std::chrono::duration<double, std::milli>(dur).count();
    }
};

std::string
to_string(bbl::Type const& ty, char const* s_const, DeclMaps const& decl_maps) {
    if (std::holds_alternative<bbl_builtin_t>(ty.kind)) {
        bbl_builtin_t const& builtin = std::get<bbl_builtin_t>(ty.kind);
        return fmt::format("{}{}", builtin, s_const);
    } else if (std::holds_alternative<bbl::ClassId>(ty.kind)) {
        bbl::ClassId const& classid = std::get<bbl::ClassId>(ty.kind);
        std::string name;
        if (auto it = decl_maps.class_map.find(classid.id);
            it != decl_maps.class_map.end()) {
            return fmt::format("{}{}", it->second.spelling, s_const);
        } else if (auto it = decl_maps.typename_map.find(classid.id);
                   it != decl_maps.typename_map.end()) {
            name = it->second;
        }
        return fmt::format("Class({} \"{}\"){}", classid.id, name, s_const);
    } else if (std::holds_alternative<bbl::ClassTemplateSpecializationId>(
                   ty.kind)) {
        bbl::ClassTemplateSpecializationId const& classid =
            std::get<bbl::ClassTemplateSpecializationId>(ty.kind);
        std::string name;
        if (auto it = decl_maps.class_map.find(classid.id);
            it != decl_maps.class_map.end()) {
            return fmt::format("{}{}", it->second.spelling, s_const);
        } else if (auto it = decl_maps.typename_map.find(classid.id);
                   it != decl_maps.typename_map.end()) {
            name = it->second;
        }
        return fmt::format("ClassTemplateSpecialization({} \"{}\"){}",
                           classid.id,
                           name,
                           s_const);
    } else if (std::holds_alternative<bbl::EnumId>(ty.kind)) {
        bbl::EnumId const& enumid = std::get<bbl::EnumId>(ty.kind);
        std::string name;
        if (auto it = decl_maps.enum_map.find(enumid.id);
            it != decl_maps.enum_map.end()) {
            return fmt::format("{}{}", it->second.spelling, s_const);
        } else if (auto it = decl_maps.typename_map.find(enumid.id);
                   it != decl_maps.typename_map.end()) {
            name = it->second;
        }
        return fmt::format("Enum({} \"{}\"){}", enumid.id, name, s_const);
    } else if (std::holds_alternative<bbl::StdFunctionId>(ty.kind)) {
        bbl::StdFunctionId const& id = std::get<bbl::StdFunctionId>(ty.kind);
        std::string name;
        if (auto it = decl_maps.stdfunction_map.find(id.id);
            it != decl_maps.stdfunction_map.end()) {
            return fmt::format("{}{}", it->second.spelling, s_const);
        } else if (auto it = decl_maps.typename_map.find(id.id);
                   it != decl_maps.typename_map.end()) {
            name = it->second;
        }
        return fmt::format("StdFunction({} \"{}\"){}", id.id, name, s_const);
    } else {
        BBL_THROW("unhandled Type kind in format");
    }
}

std::string to_string(QType const& qt, DeclMaps const& decl_maps) {
    char const* s_const = qt.is_const ? " const" : "";

    if (std::holds_alternative<bbl::Type>(qt.type)) {
        bbl::Type const& ty = std::get<bbl::Type>(qt.type);
        return to_string(ty, s_const, decl_maps);
    } else if (std::holds_alternative<bbl::LValueReference>(qt.type)) {
        bbl::LValueReference const& lvr =
            std::get<bbl::LValueReference>(qt.type);
        return fmt::format(
            "{}&{}", to_string(*lvr.pointee, decl_maps), s_const);
    } else if (std::holds_alternative<bbl::RValueReference>(qt.type)) {
        bbl::RValueReference const& rvr =
            std::get<bbl::RValueReference>(qt.type);
        return fmt::format(
            "{}&&{}", to_string(*rvr.pointee, decl_maps), s_const);
    } else if (std::holds_alternative<bbl::Pointer>(qt.type)) {
        bbl::Pointer const& ptr = std::get<bbl::Pointer>(qt.type);
        return fmt::format(
            "{}*{}", to_string(*ptr.pointee, decl_maps), s_const);
    } else if (std::holds_alternative<bbl::Array>(qt.type)) {
        bbl::Array const& arr = std::get<bbl::Array>(qt.type);
        return fmt::format("{}[{}]{}",
                           to_string(*arr.element_type, decl_maps),
                           arr.size,
                           s_const);
    } else {
        BBL_THROW("unhandled QType kind in format");
    }
}

std::string to_string(TemplateArg const& arg, DeclMaps const& decl_maps) {
    if (std::holds_alternative<QType>(arg)) {
        return to_string(std::get<QType>(arg), decl_maps);
    } else {
        return std::get<Integral>(arg).value;
    }
}

auto extract_builtin_type(clang::BuiltinType const* btype) -> bbl_builtin_t {
#define CASE(VAR)                                                              \
    case clang::BuiltinType::VAR:                                              \
        return bbl_builtin_t::BBL_BUILTIN_##VAR;

    switch (btype->getKind()) {
        CASE(Void)
        CASE(Char_S)
        CASE(Short)
        CASE(Int)
        CASE(Long)
        CASE(LongLong)
        CASE(UChar)
        CASE(UShort)
        CASE(UInt)
        CASE(ULong)
        CASE(ULongLong)
        CASE(Float)
        CASE(Double)
    default:
        BBL_THROW("unhandled builtin {}", int(btype->getKind()));
        break;
    }
}

auto Context::extract_qualtype(clang::QualType const& qt,
                               clang::MangleContext* mangle_ctx) -> QType {
    bool is_const = qt.isConstQualified();

    if (qt->isLValueReferenceType()) {
        try {
            auto pointee = std::make_unique<QType>(
                extract_qualtype(qt->getPointeeType(), mangle_ctx));
            return QType{is_const, LValueReference{std::move(pointee)}};
        } catch (std::exception& e) {
            BBL_RETHROW(
                e,
                "failed to extract pointee type from lvalue reference type {}",
                qt.getAsString());
        }
    } else if (qt->isRValueReferenceType()) {
        try {
            auto pointee = std::make_unique<QType>(
                extract_qualtype(qt->getPointeeType(), mangle_ctx));
            return QType{is_const, RValueReference{std::move(pointee)}};
        } catch (std::exception& e) {
            BBL_RETHROW(
                e,
                "failed to extract pointee type from rvalue reference type {}",
                qt.getAsString());
        }
    } else if (qt->isPointerType()) {
        try {
            auto pointee = std::make_unique<QType>(
                extract_qualtype(qt->getPointeeType(), mangle_ctx));
            return QType{is_const, Pointer{std::move(pointee)}};
        } catch (std::exception& e) {
            BBL_RETHROW(e,
                        "failed to extract pointee type from pointer type {}",
                        qt.getAsString());
        }
    } else if (auto const* sttpt = dyn_cast<clang::SubstTemplateTypeParmType>(
                   qt.getTypePtr())) {
        // type is a substituted template type param, get the underlying type
        // the constness of this type should be set by *this* type, not the
        // underlying one
        try {
            auto qtype =
                extract_qualtype(sttpt->getReplacementType(), mangle_ctx);
            qtype.is_const = is_const;
            return qtype;
        } catch (std::exception& e) {
            BBL_RETHROW(e,
                        "failed to extract named replacement type from subst "
                        "template type {}",
                        qt.getAsString());
        }
    } else if (qt.getAsString() == "int8_t") {
        return QType{is_const, Type{BBL_BUILTIN_Int8}};
    } else if (qt.getAsString() == "int16_t") {
        return QType{is_const, Type{BBL_BUILTIN_Int16}};
    } else if (qt.getAsString() == "int32_t") {
        return QType{is_const, Type{BBL_BUILTIN_Int32}};
    } else if (qt.getAsString() == "int64_t") {
        return QType{is_const, Type{BBL_BUILTIN_Int64}};
    } else if (qt.getAsString() == "uint8_t") {
        return QType{is_const, Type{BBL_BUILTIN_UInt8}};
    } else if (qt.getAsString() == "uint16_t") {
        return QType{is_const, Type{BBL_BUILTIN_UInt16}};
    } else if (qt.getAsString() == "uint32_t") {
        return QType{is_const, Type{BBL_BUILTIN_UInt32}};
    } else if (qt.getAsString() == "uint64_t") {
        return QType{is_const, Type{BBL_BUILTIN_UInt64}};
    } else if (qt.getAsString() == "bool" || qt.getAsString() == "_Bool") {
        return QType{is_const, Type{BBL_BUILTIN_Bool}};
    } else if (qt.getAsString() == "size_t") {
        return QType{is_const, Type{BBL_BUILTIN_Size}};
    } else if (auto const* btype =
                   dyn_cast<clang::BuiltinType>(qt.getTypePtr())) {
        return QType{is_const, Type{extract_builtin_type(btype)}};
    } else if (qt->isRecordType()) {
        clang::RecordDecl const* rd = qt->getAsRecordDecl();
        std::string id = get_mangled_name(rd, mangle_ctx);

        if (rd->getNameAsString() == "function" && is_in_std_namespace(rd) &&
            llvm::isa<clang::ClassTemplateSpecializationDecl>(rd)) {

            _decl_maps.typename_map[id] = rd->getQualifiedNameAsString();
            return QType{is_const, Type{StdFunctionId{id}}};
        } else {
            _decl_maps.typename_map[id] = rd->getNameAsString();
            return QType{is_const, Type{ClassId{id}}};
        }

    } else if (qt.getAsString() == "size_t") {
        return QType{is_const, Type{BBL_BUILTIN_Size}};
    } else if (clang::ElaboratedType const* et =
                   llvm::dyn_cast<clang::ElaboratedType>(qt.getTypePtr())) {
        try {
            QType inner = extract_qualtype(et->getNamedType(), mangle_ctx);
            inner.is_const = is_const;
            return inner;
        } catch (std::exception& e) {
            BBL_RETHROW(e,
                        "failed to extract named type from elaborated type {}",
                        qt.getAsString());
        }
    } else if (clang::TypedefType const* tdt =
                   dyn_cast<clang::TypedefType>(qt.getTypePtr())) {
        try {
            QType inner = extract_qualtype(tdt->desugar(), mangle_ctx);
            inner.is_const = is_const;
            return inner;
        } catch (std::exception& e) {
            BBL_RETHROW(e,
                        "failed to extract desugared type from TypedefType {}",
                        qt.getAsString());
        }
    } else if (clang::TemplateSpecializationType const* tst =
                   dyn_cast<clang::TemplateSpecializationType>(
                       qt.getTypePtr())) {
        if (!tst->isTypeAlias()) {
            BBL_THROW("got a TemplateSpecializationType that is not an alias. "
                      "Don't know how to handle");
        }
        try {
            QType inner = extract_qualtype(tst->getAliasedType(), mangle_ctx);
            inner.is_const = is_const;
            return inner;
        } catch (std::exception& e) {
            BBL_RETHROW(e,
                        "failed to extract aliased type from "
                        "TemplateSpecializationType {}",
                        qt.getAsString());
        }
    } else if (clang::EnumType const* et =
                   dyn_cast<clang::EnumType>(qt.getTypePtr())) {
        std::string id = get_mangled_name(et->getDecl(), mangle_ctx);
        _decl_maps.typename_map[id] = et->getDecl()->getNameAsString();

        // check that the user has bound the corresponding enum
        if (!_decl_maps.enum_map.contains(id)) {
            BBL_THROW("enum {} has not been bound",
                      et->getDecl()->getQualifiedNameAsString());
        }

        return QType{is_const, Type{EnumId{id}}};
    } else if (clang::ConstantArrayType const* cat =
                   dyn_cast<clang::ConstantArrayType>(qt.getTypePtr())) {
        QType element_type =
            extract_qualtype(cat->getElementType(), mangle_ctx);
        size_t size = cat->getSize().getLimitedValue();
        return QType{
            is_const,
            Array{std::unique_ptr<QType>(new QType(std::move(element_type))),
                  size}
        };
    } else {
        qt.dump();
        BBL_THROW("unhandled qtype {}", qt.getAsString());
    }
}

auto Context::extract_single_template_arg(
    clang::TemplateArgument const& arg,
    std::vector<TemplateArg>& template_args,
    clang::MangleContext* mangle_ctx) -> void {
    if (arg.getKind() == clang::TemplateArgument::Type) {
        clang::QualType qt = arg.getAsType();
        template_args.push_back(extract_qualtype(qt, mangle_ctx));
    } else if (arg.getKind() == clang::TemplateArgument::Integral) {
        llvm::APSInt integral = arg.getAsIntegral();
        BBL_THROW("cannot handle integral template argument kind");
    } else if (arg.getKind() == clang::TemplateArgument::Pack) {
        // iterate over pack members and recursively extract
        for (auto const& pack_arg : arg.pack_elements()) {
            extract_single_template_arg(pack_arg, template_args, mangle_ctx);
        }
    } else {
        BBL_THROW("cannot handle template argument kind of {}",
                  int(arg.getKind()));
    }
}

auto Context::extract_template_arguments(
    clang::ClassTemplateSpecializationDecl const* ctsd,
    std::vector<TemplateArg>& template_args,
    clang::MangleContext* mangle_ctx) -> void {

    clang::TemplateArgumentList const& args = ctsd->getTemplateArgs();
    try {
        for (unsigned i = 0; i < args.size(); ++i) {
            clang::TemplateArgument const& arg = args.get(i);
            extract_single_template_arg(arg, template_args, mangle_ctx);
        }
    } catch (std::exception& e) {
        BBL_RETHROW(e,
                    "could not extract template args from {}",
                    ctsd->getQualifiedNameAsString());
    }
}

auto evaluate_field_expression_bool(clang::RecordDecl const* rd,
                                    char const* name,
                                    clang::ASTContext& ast_context) -> bool {
    clang::FieldDecl const* vd_size =
        find_named_child_of_type<clang::FieldDecl>(rd, name);
    if (vd_size == nullptr) {
        BBL_THROW("could not get field {} from {}",
                  name,
                  rd->getQualifiedNameAsString());
    }

    clang::Expr const* expr_size = vd_size->getInClassInitializer();
    if (expr_size == nullptr) {
        BBL_THROW("could not get {} expr from {}",
                  name,
                  rd->getQualifiedNameAsString());
    }

    bool result = false;
    if (!expr_size->EvaluateAsBooleanCondition(result, ast_context)) {
        BBL_THROW("Could not evaluate {} expression as bool", name);
    }

    return result;
}

auto Context::extract_class_binding(clang::CXXRecordDecl const* crd,
                                    std::string const& spelling,
                                    std::string const& rename,
                                    std::string const& comment,
                                    Layout layout,
                                    BindKind bind_kind,
                                    RuleOfSeven const& rule_of_seven,
                                    bool is_abstract,
                                    clang::MangleContext* mangle_ctx) -> Class {
    std::string id = get_mangled_name(crd, mangle_ctx);
    if (_decl_maps.class_map.contains(id)) {
        BBL_THROW("class binding for id {} already exists", id);
    }

    std::string qualified_name = crd->getQualifiedNameAsString();
    std::string name = crd->getNameAsString();
    std::vector<TemplateArg> template_args;

    if (clang::ClassTemplateSpecializationDecl const* ctsd =
            dyn_cast<clang::ClassTemplateSpecializationDecl>(crd)) {
        // extract_template_arguments(ctsd, template_args,
        // mangle_ctx);
    }

    return Class{this,
                 qualified_name,
                 spelling,
                 name,
                 rename,
                 comment,
                 std::move(template_args),
                 std::vector<std::string>(),
                 std::vector<std::string>(),
                 std::vector<Field>(),
                 {}, // smartptr_to
                 layout,
                 bind_kind,
                 rule_of_seven,
                 is_abstract,
                 id};
}

auto Context::insert_class_binding(std::string const& mod_id,
                                   std::string const& id,
                                   Class&& cls) -> void {
    // First, find the module we're going to insert into
    // this should already have been extracted
    auto it_mod = _module_map.find(mod_id);
    if (it_mod == _module_map.end()) {
        BBL_THROW("module id {} is not in module map", mod_id);
    }
    Module& mod = it_mod->second;

    // Now check that we don't already have another binding
    // somewhere
    if (auto it = _type_to_module_map.find(id);
        it != _type_to_module_map.end()) {
        BBL_THROW("Class {} ({}) is already bound in module {}"
                  "\n{}",
                  cls.spelling,
                  id,
                  mod.name,
                  get_class_as_string(_decl_maps.class_map.at(id)));
    } else {
        _type_to_module_map.emplace(id, mod_id);
    }

    // If we have a module and no existing bindings, insert
    mod.classes.push_back(id);
    _decl_maps.class_map.emplace(id, std::move(cls));
}

auto Context::extract_method_binding(clang::CXXMethodDecl const* cmd,
                                     std::string const& target_class_id,
                                     std::string const& rename,
                                     std::string const& template_call,
                                     std::string const& comment,
                                     clang::MangleContext* mangle_ctx)
    -> Method {
    return Method{
        extract_function_binding(cmd,
                                 rename,
                                 cmd->getNameAsString(),
                                 template_call,
                                 comment,
                                 mangle_ctx),
        target_class_id,
        cmd->isConst(),
        cmd->isStatic(),
        cmd->isVirtual(),
        cmd->isPure(),
    };
}

auto Context::extract_function_binding(clang::FunctionDecl const* fd,
                                       std::string const& rename,
                                       std::string const& spelling,
                                       std::string const& template_call,
                                       std::string const& comment,
                                       clang::MangleContext* mangle_ctx)
    -> Function {
    std::string qualified_name = fd->getQualifiedNameAsString();
    std::string name = fd->getNameAsString();

    QType return_type;
    try {
        return_type = extract_qualtype(fd->getReturnType(), mangle_ctx);
    } catch (MissingTypeBindingException& e) {
        BBL_RETHROW(e, "{} return type has missing binding", qualified_name);
    } catch (std::exception& e) {
        BBL_RETHROW(e,
                    "failed to extract return type {} for function {}",
                    fd->getReturnType().getAsString(),
                    qualified_name);
    }

    std::vector<Param> params;
    for (auto const& param : fd->parameters()) {
        std::string param_name = param->getNameAsString();
        QType type;
        try {
            type = extract_qualtype(param->getType(), mangle_ctx);
        } catch (MissingTypeBindingException& e) {
            BBL_RETHROW(e,
                        "{} param \"{}\" has missing binding",
                        qualified_name,
                        param_name);
        } catch (std::exception& e) {
            BBL_RETHROW(e,
                        "could not extract type of param {} on "
                        "function {}",
                        param_name,
                        qualified_name);
        }

        params.push_back({param_name, std::move(type)});
    }

    clang::ExceptionSpecificationType est = fd->getExceptionSpecType();
    bool no_except =
        (est == clang::EST_NoThrow || est == clang::EST_NoexceptTrue ||
         est == clang::EST_BasicNoexcept);

    return Function{qualified_name,
                    name,
                    rename,
                    spelling,
                    std::move(template_call),
                    comment,
                    std::move(return_type),
                    std::move(params),
                    no_except};
}

auto Context::insert_function_binding(std::string const& mod_id,
                                      std::string const& id,
                                      Function&& fun) -> void {
    // First, find the module we're going to insert into
    // this should already have been extracted
    auto it_mod = _module_map.find(mod_id);
    if (it_mod == _module_map.end()) {
        BBL_THROW("module id {} is not in module map", mod_id);
    }
    Module& mod = it_mod->second;

    // Now check that we don't already have another binding
    // somewhere
    if (auto it = _type_to_module_map.find(id);
        it != _type_to_module_map.end()) {
        BBL_THROW("Function {} ({}) is already bound in module "
                  "{}",
                  fun.qualified_name,
                  id,
                  mod.name);
    } else {
        _type_to_module_map.emplace(id, mod_id);
    }

    // If we have a module and no existing bindings, insert
    mod.functions.push_back(id);
    _decl_maps.function_map.emplace(id, std::move(fun));
}

auto Context::get_function(std::string const& id) noexcept -> Function* {
    if (auto it = _decl_maps.function_map.find(id);
        it != _decl_maps.function_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

auto Context::get_function(std::string const& id) const noexcept
    -> Function const* {
    if (auto it = _decl_maps.function_map.find(id);
        it != _decl_maps.function_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

auto Context::has_stdfunction(std::string const& id) const noexcept -> bool {
    return _decl_maps.stdfunction_map.contains(id);
}

auto Context::get_stdfunction(std::string const& id) noexcept -> StdFunction* {
    if (auto it = _decl_maps.stdfunction_map.find(id);
        it != _decl_maps.stdfunction_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

auto Context::get_stdfunction(std::string const& id) const noexcept
    -> StdFunction const* {
    if (auto it = _decl_maps.stdfunction_map.find(id);
        it != _decl_maps.stdfunction_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

auto Context::stdfunctions() const noexcept
    -> StdFunctionMap::value_container_type const& {
    return _decl_maps.stdfunction_map.values();
}

auto Context::has_enum(std::string const& id) const noexcept -> bool {
    return _decl_maps.enum_map.contains(id);
}

auto Context::get_enum(std::string const& id) noexcept -> Enum* {
    if (auto it = _decl_maps.enum_map.find(id);
        it != _decl_maps.enum_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

auto Context::get_enum(std::string const& id) const noexcept -> Enum const* {
    if (auto const it = _decl_maps.enum_map.find(id);
        it != _decl_maps.enum_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

auto Context::extract_stdfunction_binding(
    clang::ClassTemplateSpecializationDecl const* ctsd,
    std::string const& spelling,
    std::string const& rename,
    std::string const& comment,
    clang::MangleContext* mangle_ctx) -> StdFunction {

    clang::TemplateArgumentList const& args = ctsd->getTemplateArgs();
    assert(args.size());

    clang::QualType qt = args[0].getAsType();
    if (auto const* fpt = dyn_cast<clang::FunctionProtoType>(qt.getTypePtr())) {
        QType return_type = extract_qualtype(fpt->getReturnType(), mangle_ctx);
        std::vector<QType> parameters;
        for (const auto& parm : fpt->param_types()) {
            parameters.emplace_back(extract_qualtype(parm, mangle_ctx));
        }

        std::string id = get_mangled_name(ctsd, mangle_ctx);
        _decl_maps.typename_map[id] = spelling;
        return StdFunction{spelling,
                           rename,
                           comment,
                           std::move(return_type),
                           std::move(parameters)};
    } else {
        BBL_THROW("got std::function CTSD {} but template "
                  "argument is not a "
                  "function prototype",
                  ctsd->getQualifiedNameAsString());
    }
}

auto Context::insert_stdfunction_binding(std::string const& mod_id,
                                         std::string const& id,
                                         StdFunction&& fun) -> void {
    // First, find the module we're going to insert into
    // this should already have been extracted
    auto it_mod = _module_map.find(mod_id);
    if (it_mod == _module_map.end()) {
        BBL_THROW("module id {} is not in module map", mod_id);
    }
    Module& mod = it_mod->second;

    // Now check that we don't already have another binding
    // somewhere
    if (auto it = _type_to_module_map.find(id);
        it != _type_to_module_map.end()) {
        BBL_THROW("StdFunction {} ({}) is already bound in "
                  "module {}",
                  fun.spelling,
                  id,
                  mod.name);
    } else {
        _type_to_module_map.emplace(id, mod_id);
    }

    // If we have a module and no existing bindings, insert
    mod.stdfunctions.push_back(id);
    _decl_maps.stdfunction_map.emplace(id, std::move(fun));
}

auto Context::extract_enum_binding(clang::EnumDecl const* ed,
                                   std::string const& spelling,
                                   std::string const& rename,
                                   std::string const& comment,
                                   clang::MangleContext* mangle_ctx) -> Enum {
    std::vector<clang::EnumConstantDecl const*> constants =
        find_all_children_of_type<clang::EnumConstantDecl>(ed);

    std::vector<EnumVariant> variants;
    for (auto const* c : constants) {
        std::string name = c->getNameAsString();
        llvm::SmallString<32> ss;
        c->getInitVal().toString(ss);

        variants.emplace_back(std::make_pair(name, ss.str()));
    }

    return Enum{spelling,
                rename,
                comment,
                std::move(variants),
                extract_qualtype(ed->getIntegerType(), mangle_ctx)};
}

auto Context::insert_enum_binding(std::string const& mod_id,
                                  std::string const& id,
                                  Enum&& enm) -> void {
    // First, find the module we're going to insert into
    // this should already have been extracted
    auto it_mod = _module_map.find(mod_id);
    if (it_mod == _module_map.end()) {
        BBL_THROW("module id {} is not in module map", mod_id);
    }
    Module& mod = it_mod->second;

    // Now check that we don't already have another binding
    // somewhere
    if (auto it = _type_to_module_map.find(id);
        it != _type_to_module_map.end()) {
        BBL_THROW("Enum {} ({}) is already bound in module {}",
                  enm.spelling,
                  id,
                  mod.name);
    } else {
        _type_to_module_map.emplace(id, mod_id);
    }

    // If we have a module and no existing bindings, insert
    mod.enums.push_back(id);
    _decl_maps.enum_map.emplace(id, std::move(enm));
}

auto Context::has_class(std::string const& id) const noexcept -> bool {
    return _decl_maps.class_map.find(id) != _decl_maps.class_map.end();
}

auto Context::get_class(std::string const& id) noexcept -> Class* {
    if (auto it = _decl_maps.class_map.find(id);
        it != _decl_maps.class_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

auto Context::get_class(std::string const& id) const noexcept -> Class const* {
    if (auto it = _decl_maps.class_map.find(id);
        it != _decl_maps.class_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

Context::Context() : _option_category("bbl::Context options") {}

std::string Context::get_class_as_string(Class const& cls) {
    std::string result;

    result = fmt::format("{}{} [{}:{}]",
                         result,
                         cls.spelling,
                         cls.layout.size_bytes,
                         cls.layout.align_bytes);

    switch (cls.bind_kind) {
    case BindKind::OpaquePtr:
        result = fmt::format("{} OP", result);
        break;
    case BindKind::OpaqueBytes:
        result = fmt::format("{} OB", result);
        break;
    case BindKind::ValueType:
        result = fmt::format("{} VT", result);
        break;
    }

    /*
    if (!cls.template_args.empty()) {
        result = fmt::format("{}<", result);

        bool first = true;
        for (auto const& arg : cls.template_args) {
            if (first) {
                first = false;
            } else {
                result = fmt::format("{}, ", result);
            }

            result = fmt::format("{}{}", result, to_string(arg,
    _class_map, _typename_map));
        }
        result = fmt::format("{}>", result);
    }
    */

    if (!cls.rename.empty()) {
        result = fmt::format("{} \"{}\"", result, cls.rename);
    }

    // result = fmt::format("{} {}", result, cls.id);

    return result;
}

std::string Context::get_method_as_string(Method const& method) {
    std::string result = get_function_as_string(method.function);

    if (method.is_const) {
        result = fmt::format("{} const", result);
    }

    if (method.is_static) {
        result = fmt::format("{} static", result);
    }

    return result;
}

auto Context::has_method(std::string const& method_id) const -> bool {
    return _decl_maps.method_map.contains(method_id);
}

auto Context::insert_method_binding(std::string const& method_id,
                                    Method&& method) -> void {
    _decl_maps.method_map.emplace(method_id, std::move(method));
}

auto Context::get_method(std::string const& method_id) const -> Method const* {
    if (auto const it = _decl_maps.method_map.find(method_id);
        it != _decl_maps.method_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

auto Context::has_constructor(std::string const& constructor_id) const -> bool {
    return _decl_maps.constructor_map.contains(constructor_id);
}

auto Context::insert_constructor_binding(std::string const& constructor_id,
                                         Constructor&& constructor) -> void {
    _decl_maps.constructor_map.emplace(constructor_id, std::move(constructor));
}

auto Context::get_constructor(std::string const& constructor_id) const
    -> Constructor const* {
    if (auto const it = _decl_maps.constructor_map.find(constructor_id);
        it != _decl_maps.constructor_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

std::string Context::get_function_as_string(Function const& function) {
    std::string result;

    result = fmt::format("{}{}(", result, function.name);

    bool first = true;

    for (auto const& param : function.params) {
        if (first) {
            first = false;
        } else {
            result = fmt::format("{}, ", result);
        }

        result = fmt::format(
            "{}{} {}", result, to_string(param.type, _decl_maps), param.name);
    }
    result = fmt::format(
        "{}) -> {}", result, to_string(function.return_type, _decl_maps));

    if (!function.rename.empty()) {
        result = fmt::format("{} \"{}\"", result, function.rename);
    }

    if (function.is_noexcept) {
        result = fmt::format("{} noexcept", result);
    }

    return result;
}

std::string Context::get_stdfunction_as_string(StdFunction const& function) {
    std::string result;

    result = fmt::format("{} std::function<{} (",
                         function.spelling,
                         to_string(function.return_type, _decl_maps));

    bool first = true;
    for (auto const& param : function.params) {
        if (first) {
            first = false;
        } else {
            result = fmt::format("{}, ", result);
        }

        result = fmt::format("{}{}", result, to_string(param, _decl_maps));
    }

    result = fmt::format("{})>", result);

    return result;
}

std::string Context::get_enum_as_string(Enum const& enm) {
    std::string result = fmt::format(
        "{}: {} {{", enm.spelling, to_string(enm.integer_type, _decl_maps));

    bool first = true;
    for (auto const& var : enm.variants) {
        if (first) {
            first = false;
        } else {
            result = fmt::format("{}, ", result);
        }

        result = fmt::format("{}{} = {}", result, var.first, var.second);
    }

    result = fmt::format("{}}}", result);

    return result;
}

std::string Context::get_qtype_as_string(QType const& qt) const {
    return to_string(qt, _decl_maps);
}

auto Context::get_source_file(const std::string& filename) -> SourceFile* {
    if (auto it = _source_file_map.find(filename);
        it != _source_file_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

auto Context::get_source_file(const std::string& filename) const
    -> SourceFile const* {
    if (auto const it = _source_file_map.find(filename);
        it != _source_file_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

auto Context::insert_module(std::string const& id, Module&& module) -> void {
    _module_map.emplace(id, std::move(module));
}

auto Context::num_modules() const noexcept -> size_t {
    return _module_map.values().size();
}

auto Context::modules() const noexcept
    -> ModuleMap::value_container_type const& {
    return _module_map.values();
}

auto Context::get_module(std::string const& id) -> Module* {
    if (auto it = _module_map.find(id); it != _module_map.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

auto Context::insert_source_file(const std::string& filename,
                                 SourceFile&& source_file) -> void {
    _source_file_map.emplace(filename, std::move(source_file));
}

// I know, I know, now I have two problems
std::regex re_inclusion(R"(^\s*#include\s*([<"])([\w\-\./]+)[>"])");

auto Context::extract_inclusions(std::string const& source_file)
    -> std::vector<Inclusion> {
    std::vector<Inclusion> result;

    /// XXX: There's definitely a way to do this with the
    /// preprocessor, but I can't figure out how to invoke it
    /// correctly in our context This will do for now, but note
    /// that it will fail if the user #if's out any includes (it
    /// will still include them)
    std::ifstream infile(source_file);

    std::string line;
    while (std::getline(infile, line)) {
        std::smatch matches;
        if (std::regex_search(line, matches, re_inclusion)) {
            assert(matches.size() == 3);
            // XXX: This is probably brittle...
            if (matches[2].str().find("babble") == std::string::npos) {
                result.emplace_back(Inclusion{
                    matches[0],
                    matches[2],
                    matches[1] == "\"",
                });
            }
        }
    }

    return result;
}

auto Context::insert_function_impl(std::string const& mod_id,
                                   std::string const& impl) -> void {
    if (Module* mod = get_module(mod_id)) {
        mod->function_impls.push_back(impl);
    } else {
        BBL_THROW("tried to insert function impl to non-existent module {}",
                  mod_id);
    }
}

static auto create_mangle_context(clang::ASTContext& ast_context)
    -> std::unique_ptr<clang::MangleContext> {
    // Use the microsoft mangling always for consistency
    return std::unique_ptr<clang::MangleContext>(
        clang::MicrosoftMangleContext::create(ast_context,
                                              ast_context.getDiagnostics()));
}

static auto
extract_module_from_function_decl(clang::FunctionDecl const* fd,
                                  bbl::Context* bbl_ctx,
                                  clang::ASTContext* ast_context,
                                  clang::SourceManager const& sm,
                                  clang::MangleContext* mangle_context)
    -> void {
    std::string name =
        fd->getNameAsString().substr(9); //< 9 is the length of "bbl_bind_"
    auto loc_expanded = sm.getExpansionLoc(fd->getSourceRange().getBegin());
    std::string source_filename = sm.getFilename(loc_expanded).str();

    std::string module_id = get_mangled_name(fd, mangle_context);

    if (Module* mod = bbl_ctx->get_module(module_id)) {
        // module has already been created, add this source file to it
        mod->source_files.push_back(source_filename);
    } else {
        bbl_ctx->insert_module(module_id,
                               Module{
                                   {source_filename},
                                   name,
                                   {}, // classes
                                   {}, // functions
                                   {}, // stdfunctions
                                   {}, // enums
                               });
    }

    // Add this module's id to the list of modules in its source file
    /// XXX: source files should probably already be created if we do
    /// includes pass first?
    if (SourceFile* source_file = bbl_ctx->get_source_file(source_filename)) {
        source_file->modules.push_back(module_id);
    } else {
        bbl_ctx->insert_source_file(
            source_filename, SourceFile{{}, {module_id}, source_filename});
    }

    // fd->dumpColor();
}

static clang::CXXRecordDecl const*
get_record_to_extract_from_construct_expr(clang::CXXConstructExpr const* cce) {
    clang::CXXConstructorDecl* ctor_decl = cce->getConstructor();
    if (!ctor_decl) {
        BBL_THROW("could not get constructor decl from construct expr");
    }

    // The next few layers are getting from bbl::Class<Foo>() to the
    // Decl for Foo First we get the specialization decl...
    clang::ClassTemplateSpecializationDecl const* ctsd =
        dyn_cast<clang::ClassTemplateSpecializationDecl>(
            ctor_decl->getParent());
    if (!ctsd) {
        BBL_THROW("could not cast ctor decl {} parent to "
                  "ClassTemplateSpecializationDecl",
                  ctor_decl->getNameAsString());
    }

    // then the class template decl...
    clang::ClassTemplateDecl const* ctd = ctsd->getSpecializedTemplate();
    if (!ctd) {
        BBL_THROW("could not get specialized template from {}",
                  ctsd->getNameAsString());
    }

    // which then gives us the decl for the class that's been templated,
    // i.e. bbl::Class<T>
    clang::CXXRecordDecl const* crd = ctd->getTemplatedDecl();
    if (!crd) {
        BBL_THROW("could not get templated decl from {}",
                  ctd->getNameAsString());
    }

    // then check that it's definitely bbl::Class before we go any
    // further in case user has written something very weird...
    if (crd->getQualifiedNameAsString() != "bbl::Class") {
        BBL_THROW("got class binding but underlying record {} - {} "
                  "does not match bbl::Class",
                  crd->getQualifiedNameAsString(),
                  crd->getID());
    }

    // Now we get the type that we're binding from the template
    // argument list of the CTSD
    if (ctsd->getTemplateArgs().size() != 1) {
        BBL_THROW("expected 1 template argument on {}, found {}",
                  ctsd->getQualifiedNameAsString(),
                  ctsd->getTemplateArgs().size());
    }

    // Get the type of the first template argument of the
    // specialization...
    clang::QualType qt = ctsd->getTemplateArgs()[0].getAsType();
    clang::Type const* type = qt.getTypePtr();
    if (type == nullptr) {
        BBL_THROW("got null type ptr from {}", qt.getAsString());
    }

    // which will give us the Decl for the record, i.e. the thing we
    // actually want to extract
    clang::CXXRecordDecl* type_record_decl = type->getAsCXXRecordDecl();
    if (type_record_decl == nullptr) {
        BBL_THROW("type \"{}\" cannot be bound as a Class", qt.getAsString());
    }

    return type_record_decl;
}

// Get the bbl::Class decl from the given CXXConstructExpr, find the size and
// align fields, evaluate their expressions, and return the result as a Layout
// object
static Layout get_record_layout(clang::CXXConstructExpr const* cce,
                                clang::ASTContext& ast_context) {
    clang::CXXConstructorDecl* ctor_decl = cce->getConstructor();
    if (!ctor_decl) {
        BBL_THROW("could not get constructor decl from construct expr");
    }

    clang::ClassTemplateSpecializationDecl const* ctsd =
        dyn_cast<clang::ClassTemplateSpecializationDecl>(
            ctor_decl->getParent());
    if (!ctsd) {
        BBL_THROW("could not cast ctor decl {} parent to "
                  "ClassTemplateSpecializationDecl",
                  ctor_decl->getNameAsString());
    }

    clang::FieldDecl const* vd_size =
        find_named_child_of_type<clang::FieldDecl>(ctsd, "type_size");
    if (vd_size == nullptr) {
        BBL_THROW("could not get type_size from {}",
                  ctsd->getQualifiedNameAsString());
    }

    clang::Expr const* expr_size = vd_size->getInClassInitializer();
    if (expr_size == nullptr) {
        BBL_THROW("could not get type_size expr from {}",
                  ctsd->getQualifiedNameAsString());
    }

    llvm::APSInt apsi_size = expr_size->EvaluateKnownConstInt(ast_context);

    clang::FieldDecl const* vd_align =
        find_named_child_of_type<clang::FieldDecl>(ctsd, "type_align");
    if (vd_align == nullptr) {
        BBL_THROW("could not get type_align from {}",
                  ctsd->getQualifiedNameAsString());
    }

    clang::Expr const* expr_align = vd_align->getInClassInitializer();
    if (expr_align == nullptr) {
        BBL_THROW("could not get type_size expr from {}",
                  ctsd->getQualifiedNameAsString());
    }

    llvm::APSInt apsi_align = expr_align->EvaluateKnownConstInt(ast_context);

    return Layout{
        apsi_size.getLimitedValue(),
        apsi_align.getLimitedValue(),
    };
}

/// XXX: This smells a bit. Need an optional here, or return a bool with an out
/// parameter
static BindKind search_for_bind_kind_calls(clang::Stmt const* stmt,
                                           clang::ASTContext& ast_context) {
    if (stmt == nullptr || llvm::isa<clang::CompoundStmt>(stmt)) {
        // if we haven't found one, default to opaque ptr
        return BindKind::OpaquePtr;
    }

    if (clang::MemberExpr const* me = dyn_cast<clang::MemberExpr>(stmt)) {
        if (me->getMemberDecl()->getNameAsString() == "value_type") {
            return BindKind::ValueType;
        } else if (me->getMemberDecl()->getNameAsString() == "opaque_bytes") {
            // XXX: investigate this more. Getting stack corruption on linux
            // (WSL) even though size and alignment is correct.
            return BindKind::OpaquePtr;
        } else if (me->getMemberDecl()->getNameAsString() == "opaque_ptr") {
            return BindKind::OpaquePtr;
        }
    }

    for (const auto& parent : ast_context.getParents(*stmt)) {
        if (auto bind_kind = search_for_bind_kind_calls(
                parent.get<clang::Stmt>(), ast_context);
            bind_kind != BindKind::OpaquePtr) {
            return bind_kind;
        }
    }

    return BindKind::OpaquePtr;
}

static auto
extract_class_from_construct_expr(clang::CXXConstructExpr const* construct_expr,
                                  bbl::Context* bbl_ctx,
                                  clang::ASTContext* ast_context,
                                  clang::SourceManager const& sm,
                                  clang::MangleContext* mangle_context)
    -> void {
    // Find the class decl that we're binding from the bbl::Class<Foo>()
    // constructor
    clang::CXXRecordDecl const* type_record_decl =
        get_record_to_extract_from_construct_expr(construct_expr);

    Layout layout = get_record_layout(construct_expr, *ast_context);
    BindKind bind_kind =
        search_for_bind_kind_calls(construct_expr, *ast_context);

    // Get the rule of seven fields from the bbl::Class decl
    clang::CXXConstructorDecl* bbl_ctor_decl = construct_expr->getConstructor();
    if (!bbl_ctor_decl) {
        BBL_THROW("could not get constructor decl from construct expr");
    }

    clang::ClassTemplateSpecializationDecl const* bbl_ctsd =
        dyn_cast<clang::ClassTemplateSpecializationDecl>(
            bbl_ctor_decl->getParent());
    if (!bbl_ctsd) {
        BBL_THROW("could not cast ctor decl {} parent to "
                  "ClassTemplateSpecializationDecl",
                  bbl_ctor_decl->getNameAsString());
    }

    bool is_default_constructible = evaluate_field_expression_bool(
        bbl_ctsd, "is_default_constructible", *ast_context);
    bool is_copy_constructible = evaluate_field_expression_bool(
        bbl_ctsd, "is_copy_constructible", *ast_context);
    bool is_nothrow_copy_constructible = evaluate_field_expression_bool(
        bbl_ctsd, "is_nothrow_copy_constructible", *ast_context);
    bool is_move_constructible = evaluate_field_expression_bool(
        bbl_ctsd, "is_move_constructible", *ast_context);
    bool is_nothrow_move_constructible = evaluate_field_expression_bool(
        bbl_ctsd, "is_nothrow_move_constructible", *ast_context);
    bool is_copy_assignable = evaluate_field_expression_bool(
        bbl_ctsd, "is_copy_assignable", *ast_context);
    bool is_nothrow_copy_assignable = evaluate_field_expression_bool(
        bbl_ctsd, "is_nothrow_copy_assignable", *ast_context);
    bool is_move_assignable = evaluate_field_expression_bool(
        bbl_ctsd, "is_move_assignable", *ast_context);
    bool is_nothrow_move_assignable = evaluate_field_expression_bool(
        bbl_ctsd, "is_nothrow_move_assignable", *ast_context);
    bool is_destructible = evaluate_field_expression_bool(
        bbl_ctsd, "is_destructible", *ast_context);
    bool has_virtual_destructor = evaluate_field_expression_bool(
        bbl_ctsd, "has_virtual_destructor", *ast_context);
    bool is_abstract =
        evaluate_field_expression_bool(bbl_ctsd, "is_abstract", *ast_context);

    // Get the source range of the type part of the ctor expr
    // for bbl::Class<Foo<float>>("FooFloat") this will be
    //     bbl::Class<Foo<float>>(
    // from there we can do regular string manipulation to get the
    // spelling of the type we're binding
    clang::SourceRange ctor_source_range = construct_expr->getSourceRange();
    clang::SourceRange ctor_paren_range =
        construct_expr->getParenOrBraceRange();

    clang::SourceLocation loc_begin = ctor_source_range.getBegin();
    clang::SourceLocation loc_end = ctor_paren_range.getBegin();

    /// XXX: got to figure out how to do the macro expansion properly
    /// here
    // loc_begin = sm.getSpellingLoc(loc_begin);
    // loc_end = sm.getSpellingLoc(loc_end);

    clang::SourceRange ctor_type_range = clang::SourceRange(loc_begin, loc_end);

    std::string class_spelling = get_source_text(ctor_type_range, sm);
    class_spelling =
        class_spelling.substr(class_spelling.find_first_of("<") + 1);
    class_spelling = class_spelling.substr(0, class_spelling.length() - 2);

    // finally grab the rename string the user's provided (if any)
    std::string rename_str;
    if (clang::StringLiteral const* rename_literal =
            find_first_descendent_of_type<clang::StringLiteral>(
                construct_expr)) {
        rename_str = rename_literal->getString().str();
    }

    // Get any doc comment on the class
    std::string comment = get_comment_from_decl(type_record_decl, ast_context);

    // and extract the class
    if (type_record_decl->getNameAsString() == "function" &&
        is_in_std_namespace(type_record_decl) &&
        llvm::isa<clang::ClassTemplateSpecializationDecl>(type_record_decl)) {
        // this is a specialization of std::function, special-case this
        try {
            StdFunction fun = bbl_ctx->extract_stdfunction_binding(
                dyn_cast<clang::ClassTemplateSpecializationDecl>(
                    type_record_decl),
                class_spelling,
                rename_str,
                comment,
                mangle_context);
            std::string id = get_mangled_name(type_record_decl, mangle_context);

            std::string mod_id;
            if (!find_containing_module(
                    construct_expr, ast_context, mangle_context, mod_id)) {
                BBL_THROW("could not find containing module for {}",
                          get_source_text(construct_expr->getSourceRange(),
                                          ast_context->getSourceManager()));
            }

            bbl_ctx->insert_stdfunction_binding(mod_id, id, std::move(fun));
        } catch (std::exception& e) {
            BBL_RETHROW(e, "could not extract stdfunction {}", class_spelling);
        }

    } else {
        try {
            Class cls = bbl_ctx->extract_class_binding(
                type_record_decl,
                class_spelling,
                rename_str,
                comment,
                layout,
                bind_kind,
                RuleOfSeven{
                    is_default_constructible,
                    is_copy_constructible,
                    is_nothrow_copy_constructible,
                    is_move_constructible,
                    is_nothrow_move_constructible,
                    is_copy_assignable,
                    is_nothrow_copy_assignable,
                    is_move_assignable,
                    is_nothrow_move_assignable,
                    is_destructible,
                    has_virtual_destructor,
                },
                is_abstract,
                mangle_context);

            std::string id = get_mangled_name(type_record_decl, mangle_context);

            std::string mod_id;
            if (!find_containing_module(
                    construct_expr, ast_context, mangle_context, mod_id)) {
                BBL_THROW("could not find containing module for {}",
                          get_source_text(construct_expr->getSourceRange(),
                                          ast_context->getSourceManager()));
            }
            bbl_ctx->insert_class_binding(mod_id, id, std::move(cls));
        } catch (std::runtime_error& e) {
            BBL_RETHROW(e,
                        "failed to extract class binding from\n{}",
                        get_source_and_location(construct_expr, sm));
        }
    }
}

static clang::EnumDecl const*
get_enum_to_extract_from_construct_expr(clang::CXXConstructExpr const* cce,
                                        clang::SourceManager const& sm) {
    clang::CXXConstructorDecl* ctor_decl = cce->getConstructor();
    if (!ctor_decl) {
        BBL_THROW("could not get constructor decl from construct expr");
    }

    // The next few layers are getting from bbl::Class<Foo>() to the
    // Decl for Foo First we get the specialization decl...
    clang::ClassTemplateSpecializationDecl const* ctsd =
        dyn_cast<clang::ClassTemplateSpecializationDecl>(
            ctor_decl->getParent());
    if (!ctsd) {
        BBL_THROW("could not cast ctor decl {} parent to "
                  "ClassTemplateSpecializationDecl",
                  ctor_decl->getNameAsString());
    }

    // then the class template decl...
    clang::ClassTemplateDecl const* ctd = ctsd->getSpecializedTemplate();
    if (!ctd) {
        BBL_THROW("could not get specialized template from {}",
                  ctsd->getNameAsString());
    }

    // which then gives us the decl for the class that's been templated,
    // i.e. bbl::Class<T>
    clang::CXXRecordDecl const* crd = ctd->getTemplatedDecl();
    if (!crd) {
        BBL_THROW("could not get templated decl from {}",
                  ctd->getNameAsString());
    }

    // then check that it's definitely bbl::Class before we go any
    // further in case user has written something very weird...
    if (crd->getQualifiedNameAsString() != "bbl::Enum") {
        BBL_THROW("got enum binding but underlying record {} - {} "
                  "does not match bbl::Enum",
                  crd->getQualifiedNameAsString(),
                  crd->getID());
    }

    // Now we get the type that we're binding from the template
    // argument list of the CTSD
    if (ctsd->getTemplateArgs().size() != 1) {
        BBL_THROW("expected 1 template argument on {}, found {}",
                  ctsd->getQualifiedNameAsString(),
                  ctsd->getTemplateArgs().size());
    }

    // Get the type of the first template argument of the
    // specialization...
    clang::QualType qt = ctsd->getTemplateArgs()[0].getAsType();
    clang::Type const* type = qt.getTypePtr();
    if (type == nullptr) {
        BBL_THROW("got null type ptr from {}", qt.getAsString());
    }

    // which will give us the Decl for the record, i.e. the thing we
    // actually want to extract
    if (!type->isEnumeralType()) {
        BBL_THROW("tried to bind type \"{}\" as an Enum at\n{}",
                  qt.getAsString(),
                  get_source_and_location(cce, sm));
    }

    clang::EnumDecl const* enum_decl =
        dyn_cast<clang::EnumDecl>(type->getAsTagDecl());
    if (enum_decl == nullptr) {
        BBL_THROW("got null enum decl ptr from {}", qt.getAsString());
    }

    return enum_decl;
}

static auto
extract_enum_from_construct_expr(clang::CXXConstructExpr const* construct_expr,
                                 bbl::Context* bbl_ctx,
                                 clang::ASTContext* ast_context,
                                 clang::SourceManager const& sm,
                                 clang::MangleContext* mangle_context) -> void {
    // Find the class decl that we're binding from the bbl::Class<Foo>()
    // constructor
    clang::EnumDecl const* enum_decl =
        get_enum_to_extract_from_construct_expr(construct_expr, sm);

    // Get the source range of the type part of the ctor expr
    // for bbl::Class<Foo<float>>("FooFloat") this will be
    //     bbl::Class<Foo<float>>(
    // from there we can do regular string manipulation to get the
    // spelling of the type we're binding
    clang::SourceRange ctor_source_range = construct_expr->getSourceRange();
    clang::SourceRange ctor_paren_range =
        construct_expr->getParenOrBraceRange();
    clang::SourceRange ctor_type_range = clang::SourceRange(
        ctor_source_range.getBegin(), ctor_paren_range.getBegin());
    std::string class_spelling = get_source_text(ctor_type_range, sm);
    class_spelling =
        class_spelling.substr(class_spelling.find_first_of("<") + 1);
    class_spelling = class_spelling.substr(0, class_spelling.length() - 2);

    // finally grab the rename string the user's provided (if any)
    std::string rename_str;
    if (clang::StringLiteral const* rename_literal =
            find_first_descendent_of_type<clang::StringLiteral>(
                construct_expr)) {
        rename_str = rename_literal->getString().str();
    }

    std::string comment = get_comment_from_decl(enum_decl, ast_context);

    Enum enm = bbl_ctx->extract_enum_binding(
        enum_decl, class_spelling, rename_str, comment, mangle_context);
    std::string id = get_mangled_name(enum_decl, mangle_context);

    std::string mod_id;
    if (!find_containing_module(
            construct_expr, ast_context, mangle_context, mod_id)) {
        BBL_THROW("could not find containing module for {}",
                  get_source_text(construct_expr->getSourceRange(),
                                  ast_context->getSourceManager()));
    }

    // SPDLOG_INFO("inserting enum {} {}", id,
    //             enum_decl->getQualifiedNameAsString());
    bbl_ctx->insert_enum_binding(mod_id, id, std::move(enm));
}

static auto get_target_record_decl_from_member_call_expr(
    clang::CXXMemberCallExpr const* mce) -> clang::CXXRecordDecl const* {

    // find the CXXConstructExpr to get the class we're
    // attaching to
    // XXX: handle object instance not ctor
    auto const* cce =
        find_first_descendent_of_type<clang::CXXConstructExpr>(mce);
    clang::CXXRecordDecl const* crd_target = nullptr;

    if (cce == nullptr) {
        // This can happen if the user has created a variable of
        // type type Class<Foo> and then called m on it, e.g.
        //
        // auto c = bbl::Class<Foo>();
        // c.m(&Foo::bar)

        clang::CXXRecordDecl* bbl_rd = mce->getRecordDecl();
        if (bbl_rd->getNameAsString() != "Class") {
            BBL_THROW("Got presumed detached method binding, but "
                      "implicit object is not a "
                      "bbl::Class");
        }

        auto const* bbl_ctsd =
            dyn_cast<clang::ClassTemplateSpecializationDecl>(bbl_rd);
        if (bbl_ctsd == nullptr) {
            BBL_THROW("Got presumed detached method binding, but "
                      "implicit object is not a "
                      "ClassTemplateSpecializationDecl");
        }

        // If we get here, it's almost certainly a detached method
        // binding, grab the first template argument which should be
        // the target class
        auto& template_args = bbl_ctsd->getTemplateArgs();
        if (template_args.size() != 1) {
            BBL_THROW("Got presumed detached method binding, but "
                      "size of template args is {}",
                      template_args.size());
        }

        if (template_args[0].getKind() != clang::TemplateArgument::Type) {
            BBL_THROW("Got presumed detached method binding, but "
                      "template argument is no a type");
        }

        clang::QualType qt = template_args[0].getAsType();
        crd_target = qt->getAsCXXRecordDecl();
        if (crd_target == nullptr) {
            BBL_THROW("Got presumed detached method binding, but "
                      "template argument is not a CXXRecodDecl");
        }

        return crd_target;
    } else {
        return get_record_to_extract_from_construct_expr(cce);
    }
}

static auto
extract_method_from_decl_ref_expr(clang::DeclRefExpr const* dre,
                                  bbl::Context* bbl_ctx,
                                  clang::ASTContext* ast_context,
                                  clang::SourceManager& sm,
                                  clang::MangleContext* mangle_context)
    -> void {
    auto const* cmd = llvm::dyn_cast<clang::CXXMethodDecl>(dre->getDecl());
    if (!cmd) {
        BBL_THROW("got decl ref expr but decl is not CXXMethodDecl");
    }
    clang::CXXRecordDecl const* crd_parent = cmd->getParent();

    // Handle templated calls. These are calls that require spelling one
    // or more template arguments in the call to be resolved, for
    // instance anything where the template argument appears only in the
    // return type.
    std::string template_call;
    if (cmd->isFunctionTemplateSpecialization()) {
        std::string dre_text = get_source_text(dre->getSourceRange(), sm);
        // XXX: need to figure this out
        // std::string spell_text = get_spelling_text(dre->getSourceRange(),
        // sm); SPDLOG_INFO("source: {}", dre_text); SPDLOG_INFO("spell: {}",
        // spell_text);
        if (dre_text.back() == '>') {
            // this is a templated call, loop back through the string
            // and find the opening angle bracket
            int bracket_count = 1;
            int template_start_pos = 0;
            for (int i = dre_text.size() - 2; i >= 0; --i) {
                if (dre_text[i] == '>') {
                    bracket_count++;
                } else if (dre_text[i] == '<') {
                    bracket_count--;
                }

                if (bracket_count == 0) {
                    template_start_pos = i;
                    break;
                }
            }

            template_call = dre_text.substr(
                template_start_pos, dre_text.size() - template_start_pos);
        }
    }

    clang::CXXMemberCallExpr const* mce =
        find_first_ancestor_of_type<clang::CXXMemberCallExpr>(dre, ast_context);
    if (!mce) {
        SPDLOG_WARN("Could not get CXXMemberCallExpr from DeclRefExpr "
                    "{}",
                    get_source_text(dre->getSourceRange(),
                                    ast_context->getSourceManager()));
    }
    // If there's a StringLiteral inside an ImplicitCastExpr,
    // that will be our rename string
    std::string rename_str;
    if (auto const* ice =
            find_first_child_of_type<clang::ImplicitCastExpr>(mce)) {
        if (auto const* sl =
                find_first_child_of_type<clang::StringLiteral>(ice)) {
            rename_str = sl->getString().str();
        }
    }

    clang::CXXRecordDecl const* crd_target =
        get_target_record_decl_from_member_call_expr(mce);

    // Check that if there's a mismatch between the parent class of
    // this method and the target class we are binding to, that the
    // target class is derived from the binding class this is
    // because in the AST methods always have their parent as the
    // class on which they were declared, not the derived class
    std::string target_class_id = get_mangled_name(crd_target, mangle_context);
    std::string parent_id = get_mangled_name(crd_parent, mangle_context);
    if (target_class_id != parent_id &&
        !crd_target->isDerivedFrom(crd_parent)) {

        std::string source_text = get_spelling_text(dre->getSourceRange(), sm);
        int line = sm.getExpansionLineNumber(dre->getSourceRange().getBegin());
        int col = sm.getExpansionColumnNumber(dre->getSourceRange().getBegin());
        std::string filename =
            sm.getFilename(dre->getSourceRange().getBegin()).str();

        BBL_THROW("method {} is bound to class {} but {} is "
                  "not derived from {} while extracting:\n{}\n at {}:{}:{}",
                  cmd->getQualifiedNameAsString(),
                  crd_target->getQualifiedNameAsString(),
                  crd_target->getQualifiedNameAsString(),
                  crd_parent->getQualifiedNameAsString(),
                  source_text,
                  filename,
                  line,
                  col);
    }

    bbl::Class* cls = bbl_ctx->get_class(target_class_id);
    if (!cls) {
        BBL_THROW("method {} is targeting class {} but this class "
                  "is not extracted",
                  cmd->getQualifiedNameAsString(),
                  crd_target->getQualifiedNameAsString());
    }

    // extract and add the method to the corresponding class
    // methods mangled names are based on the class in which
    // they're first declared, which means we'll get
    // "duplicate" entries for inherited methods.
    // append the name of the target class to differentiate them
    std::string method_id = get_mangled_name(cmd, mangle_context);
    method_id =
        fmt::format("{}/{}", method_id, crd_target->getQualifiedNameAsString());

    if (bbl_ctx->has_method(method_id)) {
        if (std::find(cls->methods.begin(), cls->methods.end(), method_id) !=
            cls->methods.end()) {
            // class already has method insert, warn
            /// XXX: report loction of binding
            SPDLOG_WARN("method {} is already bound on class "
                        "{}, ignoring "
                        "second binding",
                        cmd->getQualifiedNameAsString(),
                        cls->qualified_name);
        } else {
            cls->methods.push_back(method_id);
        }
    } else {
        std::string comment = get_comment_from_decl(cmd, ast_context);
        try {
            bbl::Method method =
                bbl_ctx->extract_method_binding(cmd,
                                                target_class_id,
                                                rename_str,
                                                template_call,
                                                comment,
                                                mangle_context);
            bbl_ctx->insert_method_binding(method_id, std::move(method));
            cls->methods.emplace_back(std::move(method_id));
        } catch (std::exception& e) {
            BBL_RETHROW(
                e, "could not bind method {}", cmd->getQualifiedNameAsString());
        }
    }
}

static auto
extract_function_from_decl_ref_expr(clang::DeclRefExpr const* dre_fn,
                                    bbl::Context* bbl_ctx,
                                    clang::ASTContext* ast_context,
                                    clang::SourceManager& sm,
                                    clang::MangleContext* mangle_context)
    -> void {
    // The DeclRefExpr that we get here will refer to bbl::fn. The
    // surrounding AST when binding a function `foo` is structured like:
    //
    // clang-format off
        // 
        // CallExpr 0x201a9cde350 <line:20:5, col:26> 'void'
        //     |-ImplicitCastExpr 0x201a9cde338 <col:5, col:10> 'void  (*)(unsigned long long (*)(qux::Bar &&), const char *)' <FunctionToPointerDecay> 
        //     | `-DeclRefExpr 0x201a9cde2a0 <col:5, col:10> 'void (unsigned long long (*)(qux::Bar &&), const char*)' lvalue Function 0x201a9cde178 'fn' 'void (unsigned long long(*)(qux::Bar &&), const char *)' (FunctionTemplate 0x201a90dd550 'fn')
        //     |-UnaryOperator 0x201a9cddc68 <col:13, col:14> 'auto (*)(Bar &&) -> size_t' prefix '&' cannot overflow 
        //     | `-DeclRefExpr  0x201a9cddbe8 <col:14> 'auto (Bar &&) -> size_t' lvalue Function 0x201a9cda1f8 'foo' 'auto (Bar &&) -> size_t'
        //     `-ImplicitCastExpr 0x201a9cde380 <col:19> 'const char *' <ArrayToPointerDecay>
        //       `-StringLiteral 0x201a9cddd58 <col:19> 'const char[6]' lvalue "myfoo"
        //
    // clang-format on
    // So we need to go up to the CallExpr, then back down again to the
    // other DeclRefExpr that's not "fn" but "foo". We can then pluck the
    // rename string from the standard ImplicitCastExpr>StringLiteral chain

    auto const* ce =
        find_first_ancestor_of_type<clang::CallExpr>(dre_fn, ast_context);
    if (ce == nullptr) {
        BBL_THROW("could not find CallExpr ancestor of DeclRefExpr fn");
    }

    // grab the rename string the user's provided (if any)
    std::string rename_str;
    if (clang::StringLiteral const* rename_literal =
            find_first_descendent_of_type<clang::StringLiteral>(ce)) {
        rename_str = rename_literal->getString().str();
    }

    clang::DeclRefExpr const* dre_target = nullptr;
    visit_subtree(ce, [&](clang::Stmt const* stmt) -> bool {
        // Both the DeclRefExpr we want to bind as well as the `bbl::fn` one
        // will appear in this search so ignore the fn one.
        if (clang::DeclRefExpr const* dre = dyn_cast<clang::DeclRefExpr>(stmt);
            dre && dre != dre_fn) {
            dre_target = dre;
            return true;
        }
        return false;
    });

    if (dre_target == nullptr) {
        dre_fn->dumpColor();
        BBL_THROW("Got unexpected set of DeclRefExprs");
    }

    std::string spelling = get_source_text(dre_target->getSourceRange(), sm);

    auto const* fd = llvm::dyn_cast<clang::FunctionDecl>(dre_target->getDecl());

    if (fd == nullptr) {
        BBL_THROW("got decl ref expr but decl is not FunctionDecl");
    }

    // Handle templated calls. These are calls that require spelling one
    // or more template arguments in the call to be resolved, for
    // instance anything where the template argument appears only in the
    // return type.
    std::string template_call;
    if (fd->isFunctionTemplateSpecialization()) {
        if (spelling.back() == '>') {
            // this is a templated call, loop back through the string
            // and find the opening angle bracket
            int bracket_count = 1;
            int template_start_pos = 0;
            for (int i = spelling.size() - 2; i >= 0; --i) {
                if (spelling[i] == '>') {
                    bracket_count++;
                } else if (spelling[i] == '<') {
                    bracket_count--;
                }

                if (bracket_count == 0) {
                    template_start_pos = i;
                    break;
                }
            }

            template_call = spelling.substr(
                template_start_pos, spelling.size() - template_start_pos);
        }
    }

    std::string comment = get_comment_from_decl(fd, ast_context);

    // Now extract the function
    Function function = bbl_ctx->extract_function_binding(
        fd, rename_str, spelling, template_call, comment, mangle_context);
    std::string id = get_mangled_name(fd, mangle_context);

    std::string mod_id;
    if (!find_containing_module(dre_fn, ast_context, mangle_context, mod_id)) {
        BBL_THROW("could not find containing module for {}",
                  get_source_text(dre_fn->getSourceRange(),
                                  ast_context->getSourceManager()));
    }

    bbl_ctx->insert_function_binding(mod_id, id, std::move(function));

    // If the function decl is in the bblext namespace, extract the function
    // implementation as well
    if (is_in_namespace(fd, "bblext")) {
        std::string function_source = get_source_text(fd->getSourceRange(), sm);
        bbl_ctx->insert_function_impl(mod_id, function_source);
    }
}

static auto
extract_field_from_decl_ref_expr(clang::DeclRefExpr const* dre,
                                 bbl::Context* bbl_ctx,
                                 clang::ASTContext* ast_context,
                                 clang::SourceManager& sm,
                                 clang::MangleContext* mangle_context) -> void {
    auto const* fd = llvm::dyn_cast_or_null<clang::FieldDecl>(dre->getDecl());
    if (!fd) {
        BBL_THROW("could not get FieldDecl from DeclRefExpr");
    }

    QType qt_field;
    try {
        qt_field = bbl_ctx->extract_qualtype(fd->getType(), mangle_context);
    } catch (std::exception& e) {
        BBL_RETHROW(e,
                    "could not extract field \"{}\" from class \"{}\"",
                    fd->getNameAsString(),
                    fd->getParent()->getQualifiedNameAsString());
    }

    clang::CXXMemberCallExpr const* mce =
        find_first_ancestor_of_type<clang::CXXMemberCallExpr>(dre, ast_context);
    if (!mce) {
        BBL_THROW("could not get CXXMemberCallExpr from DeclRefExpr "
                  "extracting field {}",
                  fd->getQualifiedNameAsString());
    }

    clang::CXXRecordDecl const* crd_parent =
        llvm::dyn_cast_or_null<clang::CXXRecordDecl>(fd->getParent());
    if (crd_parent == nullptr) {
        BBL_THROW("could not get parent as CXXRecordDecl from field {}",
                  fd->getQualifiedNameAsString());
    }

    clang::CXXRecordDecl const* crd_target =
        get_target_record_decl_from_member_call_expr(mce);

    // Check that if there's a mismatch between the parent class of
    // this field and the target class we are binding to, that the
    // target class is derived from the binding class this is
    // because in the AST fields always have their parent as the
    // class on which they were declared, not the derived class
    std::string target_class_id = get_mangled_name(crd_target, mangle_context);
    std::string parent_id = get_mangled_name(crd_parent, mangle_context);
    if (target_class_id != parent_id &&
        !crd_target->isDerivedFrom(crd_parent)) {
        BBL_THROW("field {} is bound to class {} but {} is "
                  "not derived from {}",
                  fd->getQualifiedNameAsString(),
                  crd_target->getQualifiedNameAsString(),
                  crd_target->getQualifiedNameAsString(),
                  crd_parent->getQualifiedNameAsString());
    }

    bbl::Class* cls = bbl_ctx->get_class(target_class_id);
    if (cls == nullptr) {
        BBL_THROW("extracting field on class {} but class has not been "
                  "extracted.",
                  target_class_id);
    }

    std::string comment = get_comment_from_decl(fd, ast_context);

    cls->fields.emplace_back(Field{
        std::move(qt_field),
        fd->getNameAsString(),
        comment,
    });
}

static auto get_as_classid(QType const& qt) -> std::optional<std::string> {
    if (std::holds_alternative<Type>(qt.type)) {
        Type const& type = std::get<Type>(qt.type);
        if (std::holds_alternative<ClassId>(type.kind)) {
            return std::get<ClassId>(type.kind).id;
        } else if (std::holds_alternative<ClassTemplateSpecializationId>(type.kind)) {
            return std::get<ClassTemplateSpecializationId>(type.kind).id;
        }
    }

    return {};
}

static auto
extract_ctor_from_construct_expr(clang::CXXConstructExpr const* cce,
                                 bbl::Context* bbl_ctx,
                                 clang::ASTContext* ast_context,
                                 clang::SourceManager& sm,
                                 clang::MangleContext* mangle_context) -> void {
#if 0
    clang::CXXConstructExpr const* cce = nullptr;
    mce->dumpColor();
    visit_subtree(mce, [&](clang::Stmt const* stmt) {
        if (auto const* _cce = dyn_cast<clang::CXXConstructExpr>(stmt)) {
            auto const* _ccd = _cce->getConstructor();
            if (_ccd->getNameAsString() == "Ctor") {
                cce = _cce;
                return true;
            }
        }
        return false;
    });

    if (cce == nullptr) {
        auto const* mod_decl = find_containing_module_decl(mce, ast_context);
        mod_decl->dumpColor();

        BBL_THROW(
            "could not find CXXConstructExpr from Ctor CXXMemberCallExpr");
    }
#endif

    clang::CXXConstructorDecl const* ccd = cce->getConstructor();
    clang::CXXRecordDecl const* crd = ccd->getParent();
    clang::ClassTemplateSpecializationDecl const* ctsd =
        dyn_cast<clang::ClassTemplateSpecializationDecl>(crd);
    if (!ctsd) {
        BBL_THROW("could not get ClassTemplateSpecializatinDecl from {}",
                  crd->getQualifiedNameAsString());
    }

    // Read the noexcept from the Ctor struct definition
    bool is_noexcept =
        evaluate_field_expression_bool(crd, "is_noexcept", *ast_context);

    std::vector<Param> parameters;
    // now that we have the CTSD, pull the template arguments off it, which
    // are the constructor arguments
    std::vector<TemplateArg> template_args;
    bbl_ctx->extract_template_arguments(ctsd, template_args, mangle_context);

    // and the constructor parameter names are here. The number of types and
    // names is guaranteed to match by static_assert in the Ctor definition
    std::vector<clang::StringLiteral const*> param_names;
    find_all_descendents_of_type<clang::StringLiteral>(cce, param_names);

    // now stitch parameters names and types together
    // first template argument is the class we are binding to so skip that
    int i = 0;
    for (auto& arg : template_args) {
        if (!std::holds_alternative<QType>(arg)) {
            // XXX: better error here
            BBL_THROW("arg {} of constructor is not a type", i);
        }

        if (i > 0) {
            std::string param_name;
            int param_name_index = i - 1;
            if (param_name_index < param_names.size()) {
                param_name = param_names[param_name_index]->getString().str();
            } else {
                // handle missing param names with auto-generated names
                param_name = fmt::format("param{:02}", param_name_index);
            }

            parameters.push_back(
                bbl::Param{param_name, std::move(std::get<QType>(arg))});
        }

        ++i;
    }

    auto const& mce =
        find_first_ancestor_of_type<clang::CXXMemberCallExpr>(cce, ast_context);
    if (mce == nullptr) {
        BBL_THROW("could not find CXXMemberCallExpr from CXXConstructExpr {}",
                  get_source_text(cce->getSourceRange(), sm));
    }

    // If there's a StringLiteral inside an ImplicitCastExpr,
    // that will be our rename string
    std::string rename_str;
    if (auto const* ice =
            find_first_child_of_type<clang::ImplicitCastExpr>(mce)) {
        if (auto const* sl =
                find_first_child_of_type<clang::StringLiteral>(ice)) {
            rename_str = sl->getString().str();
        }
    }

    // // find the CXXConstructExpr to get the class we're
    // // attaching to
    // // XXX: handle object instance not ctor
    // auto const* cce_target =
    //     find_first_descendent_of_type<clang::CXXConstructExpr>(mce);
    // if (cce_target == nullptr) {
    //     BBL_THROW("could not get CXXConstructExpr from "
    //               "CXXMemberCallExpr {}",
    //               get_source_text(mce->getSourceRange(), sm));
    // }

    // // And finally get the target class that we're going to add the
    // // constructor to
    // clang::CXXRecordDecl const* crd_target =
    //     get_record_to_extract_from_construct_expr(cce_target);
    // std::string target_class_id = get_mangled_name(crd_target, mangle_context);

    // the target class to attach to will be the first template argument to the Ctor constructor
    if (!std::holds_alternative<QType>(template_args[0])) {
        BBL_THROW("first template argument to Ctor is not a qtype in {}", get_source_text(cce->getSourceRange(), sm));
    }
    QType const& qt_target = std::get<QType>(template_args[0]);
    std::optional<std::string> opt_target_class_id = get_as_classid(qt_target);
    if (!opt_target_class_id.has_value()) {
        BBL_THROW("first template argument to Ctor is not a class in {}", get_source_text(cce->getSourceRange(), sm));
    }

    std::string target_class_id = opt_target_class_id.value();

    std::string comment = get_comment_from_decl(ccd, ast_context);

    bbl::Class* cls = bbl_ctx->get_class(target_class_id);
    if (cls == nullptr) {
        BBL_THROW("Ctor is targeting class {} but this class "
                  "is not extracted",
                  target_class_id);
    }

    // We're not actually pointing to the constructor here, so we need
    // to create a made-up id
    // XXX: figure a way to get to the actual constructor decl. Perhaps
    // type-matching the parameters?
    std::string ctor_id =
        fmt::format("{}/ctor/{}", rename_str, target_class_id);

    bbl_ctx->insert_constructor_binding(ctor_id,
                                        Constructor{
                                            rename_str,
                                            comment,
                                            std::move(parameters),
                                            is_noexcept,
                                        });

    // Add the constructor to the class
    cls->constructors.emplace_back(ctor_id);
}

static auto extract_replacement_type_from_member_call_expr(
    clang::CXXMemberCallExpr const* mce,
    bbl::Context* bbl_ctx,
    clang::ASTContext* ast_context,
    clang::SourceManager& sm,
    clang::MangleContext* mangle_context) -> void {
    // type to replace with will be the first (and only) template
    // argument
    clang::CXXMethodDecl const* cmd = mce->getMethodDecl();
    clang::TemplateArgumentList const* template_args =
        cmd->getTemplateSpecializationArgs();
    if (template_args == nullptr || template_args->size() != 1) {
        BBL_THROW("got bad template argument list for replace_with. "
                  "Expected single template argument");
    }

    clang::RecordDecl const* rd_replace =
        template_args->data()->getAsType()->getAsRecordDecl();
    if (!rd_replace) {
        BBL_THROW("could not get RecordDecl for replace_with type");
    }

    // Get all the fields from the replacement record
    std::vector<Field> fields;
    for (auto const& field : rd_replace->fields()) {
        fields.emplace_back(
            Field{bbl_ctx->extract_qualtype(field->getType(), mangle_context),
                  field->getNameAsString()});
    }

    // find the CXXConstructExpr to get the class we're
    // attaching to
    // XXX: handle object instance not ctor
    auto const* cce_target =
        find_first_descendent_of_type<clang::CXXConstructExpr>(mce);
    if (cce_target == nullptr) {
        BBL_THROW("could not get CXXConstructExpr from "
                  "CXXMemberCallExpr {}",
                  get_source_text(mce->getSourceRange(), sm));
    }

    // And finally get the target class that we're going to add the
    // constructor to
    clang::CXXRecordDecl const* crd_target =
        get_record_to_extract_from_construct_expr(cce_target);

    std::string target_class_id = get_mangled_name(crd_target, mangle_context);

    if (bbl::Class* cls = bbl_ctx->get_class(target_class_id)) {
        cls->fields = std::move(fields);
        cls->bind_kind = BindKind::ValueType;
    } else {
        BBL_THROW("replace_with is targeting class {} but this class "
                  "is not extracted",
                  crd_target->getQualifiedNameAsString());
    }
}

static auto
extract_smartptr_to_from_member_call_expr(clang::CXXMemberCallExpr const* mce,
                                          bbl::Context* bbl_ctx,
                                          clang::ASTContext* ast_context,
                                          clang::SourceManager& sm,
                                          clang::MangleContext* mangle_context)
    -> void {
    // type to delegate to will be the first (and only) template
    // argument
    clang::CXXMethodDecl const* cmd = mce->getMethodDecl();
    clang::TemplateArgumentList const* template_args =
        cmd->getTemplateSpecializationArgs();
    if (template_args == nullptr || template_args->size() != 1) {
        BBL_THROW("got bad template argument list for smartptr_to. "
                  "Expected single template argument");
    }

    clang::RecordDecl const* rd_smartptr_to =
        template_args->data()->getAsType()->getAsRecordDecl();
    if (!rd_smartptr_to) {
        BBL_THROW("could not get RecordDecl for smartptr_to type");
    }

    // find the CXXConstructExpr to get the class we're
    // attaching to
    // XXX: handle object instance not ctor
    auto const* cce_target =
        find_first_descendent_of_type<clang::CXXConstructExpr>(mce);
    if (cce_target == nullptr) {
        BBL_THROW("could not get CXXConstructExpr from "
                  "CXXMemberCallExpr {}",
                  get_source_text(mce->getSourceRange(), sm));
    }

    // And finally get the target class
    clang::CXXRecordDecl const* crd_target =
        get_record_to_extract_from_construct_expr(cce_target);

    std::string target_class_id = get_mangled_name(crd_target, mangle_context);
    std::string smartptr_to_id =
        get_mangled_name(rd_smartptr_to, mangle_context);

    if (bbl::Class* cls = bbl_ctx->get_class(target_class_id)) {
        cls->smartptr_to = smartptr_to_id;
    } else {
        BBL_THROW("smartptr_to is targeting class {} but this class "
                  "is not extracted",
                  crd_target->getQualifiedNameAsString());
    }
}

class ModuleVisitor : public clang::RecursiveASTVisitor<ModuleVisitor> {
    clang::ASTContext* _ast_context;
    bbl::Context* _bbl_ctx;
    clang::SourceManager& _sm;
    std::unique_ptr<clang::MangleContext> _mangle_context;

public:
    explicit ModuleVisitor(bbl::Context* bbl_ctx,
                           clang::ASTContext* ast_context)
        : _bbl_ctx(bbl_ctx), _ast_context(ast_context),
          _sm(ast_context->getSourceManager()),
          _mangle_context(create_mangle_context(*ast_context)) {}

    bool VisitFunctionDecl(clang::FunctionDecl* fd) {
        if (fd->getNameAsString().find("bbl_bind_") == 0) {
            extract_module_from_function_decl(
                fd, _bbl_ctx, _ast_context, _sm, _mangle_context.get());
        }
        return true;
    }
};

class ClassVisitor : public clang::RecursiveASTVisitor<ClassVisitor> {
    clang::ASTContext* _ast_context;
    bbl::Context* _bbl_ctx;
    clang::SourceManager& _sm;
    std::unique_ptr<clang::MangleContext> _mangle_context;

public:
    explicit ClassVisitor(bbl::Context* bbl_ctx, clang::ASTContext* ast_context)
        : _bbl_ctx(bbl_ctx), _ast_context(ast_context),
          _sm(ast_context->getSourceManager()),
          _mangle_context(create_mangle_context(*ast_context)) {}

    bool VisitCXXConstructExpr(clang::CXXConstructExpr* cce) {
        auto const* ctor_decl = cce->getConstructor();
        if (ctor_decl == nullptr) {
            return true;
        }

        // we shouldn't have one of these anyway but just in case
        if (ctor_decl->isCopyOrMoveConstructor()) {
            return true;
        }

        // check that this bbl::Class is in a bbl module
        if (find_containing_module_decl(cce, _ast_context) == nullptr) {
            return true;
        }

        // check that this is a constructor for bbl::Class
        auto const* crd = llvm::dyn_cast_or_null<clang::CXXRecordDecl>(
            ctor_decl->getParent());
        if (crd == nullptr) {
            return true;
        }

        if (crd->getQualifiedNameAsString() == "bbl::Class") {
            extract_class_from_construct_expr(
                cce, _bbl_ctx, _ast_context, _sm, _mangle_context.get());
        } else if (crd->getQualifiedNameAsString() == "bbl::Enum") {
            extract_enum_from_construct_expr(
                cce, _bbl_ctx, _ast_context, _sm, _mangle_context.get());
        }

        return true;
    }

    // bool VisitCallExpr(clang::CallExpr* ce) {
    //     clang::FunctionDecl const* fd =
    //     llvm::dyn_cast_or_null<clang::FunctionDecl>(ce->getCalleeDecl()); if
    //     (!fd) {
    //         return true;
    //     }

    //     return true;
    // }
};

class FunctionVisitor : public clang::RecursiveASTVisitor<FunctionVisitor> {
    clang::ASTContext* _ast_context;
    bbl::Context* _bbl_ctx;
    clang::SourceManager& _sm;
    std::unique_ptr<clang::MangleContext> _mangle_context;

public:
    explicit FunctionVisitor(bbl::Context* bbl_ctx,
                             clang::ASTContext* ast_context)
        : _bbl_ctx(bbl_ctx), _ast_context(ast_context),
          _sm(ast_context->getSourceManager()),
          _mangle_context(create_mangle_context(*ast_context)) {}

    bool VisitDeclRefExpr(clang::DeclRefExpr* dre) {
        // check that this DeclRefExpr is in a bbl module
        if (find_containing_module_decl(dre, _ast_context) == nullptr) {
            return true;
        }

        if (auto const* cmd =
                llvm::dyn_cast_or_null<clang::CXXMethodDecl>(dre->getDecl())) {
            // this is a reference to a method that we possibly want to bind, if
            // we're inside a .m() call
            if (auto const* cmce =
                    find_first_ancestor_of_type<clang::CXXMemberCallExpr>(
                        dre, _ast_context)) {

                if (cmce->getMethodDecl()->getName() == "m") {
                    extract_method_from_decl_ref_expr(dre,
                                                      _bbl_ctx,
                                                      _ast_context,
                                                      _sm,
                                                      _mangle_context.get());
                } else {
                    SPDLOG_WARN(
                        "got CMCE but its name is {}",
                        cmce->getMethodDecl()->getQualifiedNameAsString());
                }
            } else {
                SPDLOG_WARN("got method but no CMCE ancestor");
            }
        } else if (auto const* fd = llvm::dyn_cast_or_null<clang::FunctionDecl>(
                       dre->getDecl())) {
            if (fd->getQualifiedNameAsString() == "bbl::fn") {
                extract_function_from_decl_ref_expr(
                    dre, _bbl_ctx, _ast_context, _sm, _mangle_context.get());
            }
        } else if (auto const* fd = llvm::dyn_cast_or_null<clang::FieldDecl>(
                       dre->getDecl())) {

            // this is a reference to a field that we possibly want to bind, if
            // we're inside a .f() call
            if (auto const& cmce =
                    find_first_ancestor_of_type<clang::CXXMemberCallExpr>(
                        dre, _ast_context)) {
                if (cmce->getMethodDecl()->getName() == "f") {
                    extract_field_from_decl_ref_expr(dre,
                                                     _bbl_ctx,
                                                     _ast_context,
                                                     _sm,
                                                     _mangle_context.get());
                }
            }
        }
        return true;
    }

    bool VisitCXXMemberCallExpr(clang::CXXMemberCallExpr* cmce) {
        // check that this Expr is in a bbl module
        if (find_containing_module_decl(cmce, _ast_context) == nullptr) {
            return true;
        }

        if (auto const* cmd = cmce->getMethodDecl()) {
            /*if (cmd->getName() == "ctor") {
                extract_ctor_from_member_call_expr(
                    cmce, _bbl_ctx, _ast_context, _sm, _mangle_context.get());
            } else*/
            if (cmd->getName() == "replace_with") {
                extract_replacement_type_from_member_call_expr(
                    cmce, _bbl_ctx, _ast_context, _sm, _mangle_context.get());
            } else if (cmd->getName() == "smartptr_to") {
                extract_smartptr_to_from_member_call_expr(
                    cmce, _bbl_ctx, _ast_context, _sm, _mangle_context.get());
            }
        }

        return true;
    }

    bool VisitCXXConstructExpr(clang::CXXConstructExpr* cce) {
        clang::CXXConstructorDecl const* ccd = cce->getConstructor();
        if (ccd->getNameAsString() == "Ctor") {
            extract_ctor_from_construct_expr(
                cce, _bbl_ctx, _ast_context, _sm, _mangle_context.get());
        }

        return true;
    }
};

auto Context::compile_and_extract(int argc, char const** argv) noexcept(false)
    -> void {
    using namespace clang::tooling;

    Timer timer_all;

    // Get the clang resource dir
    namespace tpl = TinyProcessLib;
    std::string clang_resource_include;
    std::string str_stderr;
    tpl::Process process(
        "clang -print-resource-dir",
        "",
        [&](char const* bytes, size_t n) {
            clang_resource_include =
                fmt::format("-I{}/include", std::string(bytes, n - 1));
        },
        [&](char const* bytes, size_t n) {
        str_stderr = std::string(bytes, n);
    });
    if (process.get_exit_status() != 0) {
        BBL_THROW("could not run clang: {}", str_stderr);
    }

    SPDLOG_INFO("clang resource include: {}", clang_resource_include);

    std::vector<char const*> args(argv, argv + argc);
    args.push_back(clang_resource_include.c_str());
    int arg_count = args.size();

    llvm::Expected<clang::tooling::CommonOptionsParser> opt =
        clang::tooling::CommonOptionsParser::create(
            arg_count, args.data(), _option_category);
    if (!opt) {
        BBL_THROW("clang could not parse options");
    }

    ClangTool tool(opt->getCompilations(), opt->getSourcePathList());

    // Extract the include directives from each source given
    for (auto const& source_path : tool.getSourcePaths()) {
        std::string absolute_path =
            std::filesystem::absolute(source_path).string();
        auto inclusions = extract_inclusions(absolute_path);

        // create a new SourceFile object for this file and stick
        // the found inclusions in it these can then be queried
        // from each module to decide what headers it needs to
        // include
        insert_source_file(absolute_path,
                           SourceFile{
                               inclusions,
                               {}, // modules, will be filled in later
                               absolute_path,
                           });
    }
    std::vector<std::unique_ptr<clang::ASTUnit>> ast_units;
    // SPDLOG_INFO("building asts");
    Timer timer_ast;
    tool.buildASTs(ast_units);

    // SPDLOG_INFO("finding modules");
    Timer timer_mf;
    for (auto const& unit : ast_units) {
        clang::ASTContext& ast_ctx = unit->getASTContext();
        ModuleVisitor visitor(this, &ast_ctx);
        visitor.TraverseAST(ast_ctx);
    }
    // SPDLOG_INFO("module finder took {}s", timer_mf.elapsed_seconds());

    // SPDLOG_INFO("finding classes");
    Timer timer_class;
    for (auto const& unit : ast_units) {
        clang::ASTContext& ast_ctx = unit->getASTContext();
        ClassVisitor visitor(this, &ast_ctx);
        visitor.TraverseAST(ast_ctx);
    }
    // SPDLOG_INFO("class finder took {}s", timer_class.elapsed_seconds());

    // SPDLOG_INFO("finding functions");
    Timer timer_function;
    for (auto const& unit : ast_units) {
        clang::ASTContext& ast_ctx = unit->getASTContext();
        FunctionVisitor visitor(this, &ast_ctx);
        visitor.TraverseAST(ast_ctx);
    }
    // SPDLOG_INFO("function finder took {}s",
    // timer_function.elapsed_seconds());

    // SPDLOG_INFO("building {} ASTs took {}s", ast_units.size(),
    // timer_ast.elapsed_seconds());
}

} // namespace bbl