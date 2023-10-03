#include "bbl.hpp"
#include "astutil.hpp"
#include "bbl_detail.h"
#include "bblfmt.hpp"

#include "bbl-c.hpp"

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTTypeTraits.h"
#include "clang/AST/Decl.h"
#include "clang/AST/TemplateBase.h"
#include "clang/AST/Type.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/ExceptionSpecificationType.h"
#include "clang/Frontend/ASTUnit.h"
#include "llvm/Support/raw_ostream.h"

#include <filesystem>
#include <memory>
#include <regex>
#include <spdlog/spdlog.h>

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <exception>
#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

namespace bbl {

std::string to_string(bbl::Type const& ty, char const* s_const,
                      DeclMaps const& decl_maps) {
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
                           classid.id, name, s_const);
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
        return fmt::format("{}&{}", to_string(*lvr.pointee, decl_maps),
                           s_const);
    } else if (std::holds_alternative<bbl::RValueReference>(qt.type)) {
        bbl::RValueReference const& rvr =
            std::get<bbl::RValueReference>(qt.type);
        return fmt::format("{}&&{}", to_string(*rvr.pointee, decl_maps),
                           s_const);
    } else if (std::holds_alternative<bbl::Pointer>(qt.type)) {
        bbl::Pointer const& ptr = std::get<bbl::Pointer>(qt.type);
        return fmt::format("{}*{}", to_string(*ptr.pointee, decl_maps),
                           s_const);
    } else if (std::holds_alternative<bbl::Array>(qt.type)) {
        bbl::Array const& arr = std::get<bbl::Array>(qt.type);
        return fmt::format("{}[{}]{}", to_string(*arr.element_type, decl_maps),
                           arr.size, s_const);
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
    clang::TemplateArgument const& arg, std::vector<TemplateArg>& template_args,
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
    std::vector<TemplateArg>& template_args, clang::MangleContext* mangle_ctx)
    -> void {

    clang::TemplateArgumentList const& args = ctsd->getTemplateArgs();
    try {
        for (unsigned i = 0; i < args.size(); ++i) {
            clang::TemplateArgument const& arg = args.get(i);
            extract_single_template_arg(arg, template_args, mangle_ctx);
        }
    } catch (std::exception& e) {
        BBL_RETHROW(e, "could not extract template args from {}",
                    ctsd->getQualifiedNameAsString());
    }
}

auto evaluate_field_expression_bool(clang::RecordDecl const* rd,
                                    char const* name,
                                    clang::ASTContext& ast_context) -> bool {
    clang::FieldDecl const* vd_size =
        find_named_child_of_type<clang::FieldDecl>(rd, name);
    if (vd_size == nullptr) {
        BBL_THROW("could not get field {} from {}", name,
                  rd->getQualifiedNameAsString());
    }

    clang::Expr const* expr_size = vd_size->getInClassInitializer();
    if (expr_size == nullptr) {
        BBL_THROW("could not get {} expr from {}", name,
                  rd->getQualifiedNameAsString());
    }

    bool result = false;
    if (!expr_size->EvaluateAsBooleanCondition(result, ast_context)) {
        BBL_THROW("Could not evaluate {} expression as bool", name);
    }

    return result;
}

auto Context::extract_class_binding(
    clang::CXXRecordDecl const* crd, std::string const& spelling,
    std::string const& rename, std::string const& comment, Layout layout,
    BindKind bind_kind, RuleOfSeven const& rule_of_seven, bool is_abstract,
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
                 layout,
                 bind_kind,
                 rule_of_seven,
                 is_abstract,
                 id};
}

auto Context::insert_class_binding(std::string const& mod_id,
                                   std::string const& id, Class&& cls) -> void {
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
                  cls.spelling, id, mod.name,
                  get_class_as_string(_decl_maps.class_map.at(id)));
    } else {
        _type_to_module_map.emplace(id, mod_id);
    }

    // If we have a module and no existing bindings, insert
    mod.classes.push_back(id);
    _decl_maps.class_map.emplace(id, std::move(cls));
}

auto Context::extract_method_binding(clang::CXXMethodDecl const* cmd,
                                     std::string const& rename,
                                     std::string const& template_call,
                                     std::string const& comment,
                                     clang::MangleContext* mangle_ctx)
    -> Method {
    return Method{
        extract_function_binding(cmd, rename, cmd->getNameAsString(),
                                 template_call, comment, mangle_ctx),
        cmd->isConst(),
        cmd->isStatic(),
        cmd->isVirtual(),
        cmd->isPure(),
    };
}

auto Context::extract_function_binding(
    clang::FunctionDecl const* fd, std::string const& rename,
    std::string const& spelling, std::string const& template_call,
    std::string const& comment, clang::MangleContext* mangle_ctx) -> Function {
    std::string qualified_name = fd->getQualifiedNameAsString();
    std::string name = fd->getNameAsString();

    QType return_type;
    try {
        return_type = extract_qualtype(fd->getReturnType(), mangle_ctx);
    } catch (MissingTypeBindingException& e) {
        BBL_RETHROW(e, "{} return type has missing binding", qualified_name);
    } catch (std::exception& e) {
        BBL_RETHROW(e, "failed to extract return type {} for function {}",
                    fd->getReturnType().getAsString(), qualified_name);
    }

    std::vector<Param> params;
    for (auto const& param : fd->parameters()) {
        std::string param_name = param->getNameAsString();
        QType type;
        try {
            type = extract_qualtype(param->getType(), mangle_ctx);
        } catch (MissingTypeBindingException& e) {
            BBL_RETHROW(e, "{} param \"{}\" has missing binding",
                        qualified_name, param_name);
        } catch (std::exception& e) {
            BBL_RETHROW(e,
                        "could not extract type of param {} on "
                        "function {}",
                        param_name, qualified_name);
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
                                      std::string const& id, Function&& fun)
    -> void {
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
                  fun.qualified_name, id, mod.name);
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
    std::string const& spelling, std::string const& rename,
    std::string const& comment, clang::MangleContext* mangle_ctx)
    -> StdFunction {

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
        return StdFunction{spelling, comment, std::move(return_type),
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
                  fun.spelling, id, mod.name);
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

    return Enum{spelling, rename, comment, std::move(variants),
                extract_qualtype(ed->getIntegerType(), mangle_ctx)};
}

auto Context::insert_enum_binding(std::string const& mod_id,
                                  std::string const& id, Enum&& enm) -> void {
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
        BBL_THROW("Enum {} ({}) is already bound in module {}", enm.spelling,
                  id, mod.name);
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

    result = fmt::format("{}{} [{}:{}]", result, cls.spelling,
                         cls.layout.size_bytes, cls.layout.align_bytes);

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

        result = fmt::format("{}{} {}", result,
                             to_string(param.type, _decl_maps), param.name);
    }
    result = fmt::format("{}) -> {}", result,
                         to_string(function.return_type, _decl_maps));

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

    result = fmt::format("{} std::function<{} (", function.spelling,
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
    std::string result = fmt::format("{}: {} {{", enm.spelling,
                                     to_string(enm.integer_type, _decl_maps));

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
            if (matches[2] != "babble" && matches[2] != "babble-std") {
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

auto Context::compile_and_extract(int argc, char const** argv) noexcept(false)
    -> void {
    using namespace clang::tooling;
    using namespace clang::ast_matchers;

    llvm::Expected<clang::tooling::CommonOptionsParser> opt =
        clang::tooling::CommonOptionsParser::create(argc, argv,
                                                    _option_category);
    if (!opt) {
        BBL_THROW("clang could not parse options");
    }

    SPDLOG_INFO("creating tool");
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

    // SPDLOG_INFO("building asts");
    // std::vector<std::unique_ptr<clang::ASTUnit>> ast_units;
    // tool.buildASTs(ast_units);

    // extract the modules itself
    SPDLOG_INFO("running module finder");
    //  find_module(this);
    // result =
    // tool.run(newFrontendActionFactory(&find_module).get());
    ExtractModules module_extractor(this);
    MatchFinder module_finder;

    DeclarationMatcher module_matcher =
        traverse(clang::TK_IgnoreUnlessSpelledInSource,
                 functionDecl(matchesName("bbl_bind_")).bind("Module"));
    module_finder.addMatcher(module_matcher, &module_extractor);
    int result = tool.run(newFrontendActionFactory(&module_finder).get());

    // ignore diagnostics after the first run - we don't need the
    // same info printed out again
    tool.setDiagnosticConsumer(new clang::IgnoringDiagConsumer());

    // Extract class bindings
    MatchFinder class_finder;
    ExtractClassBindings class_binding_extractor(this);

    StatementMatcher m_construct_expr_matcher = traverse(
        clang::TK_IgnoreUnlessSpelledInSource,
        cxxConstructExpr(
            hasDeclaration(cxxConstructorDecl(
                hasParent(recordDecl(hasName("bbl::Class"))),
                unless(anyOf(isCopyConstructor(), isMoveConstructor())))),
            isExpansionInMainFile(),
            hasAncestor(functionDecl(matchesName("bbl_bind_"))))
            .bind("ctor_expr"));

    DeclarationMatcher m_var_decl_matcher = traverse(
        clang::TK_IgnoreUnlessSpelledInSource,
        varDecl(
            hasInitializer(cxxConstructExpr(hasDeclaration(cxxConstructorDecl(
                hasParent(recordDecl(hasName("bbl::Class"))))))))
            .bind("VarDecl"));

    StatementMatcher m_enum_construct_expr_matcher = traverse(
        clang::TK_IgnoreUnlessSpelledInSource,
        cxxConstructExpr(
            hasDeclaration(cxxConstructorDecl(
                hasParent(recordDecl(hasName("bbl::Enum"))),
                unless(anyOf(isCopyConstructor(), isMoveConstructor())))),
            isExpansionInMainFile(),
            hasAncestor(functionDecl(matchesName("bbl_bind_"))))
            .bind("enum_ctor_expr"));

    StatementMatcher m_rename_namespace_matcher = traverse(
        clang::TK_AsIs,
        callExpr(hasDeclaration(functionDecl(hasName("bbl::rename_namespace"))),
                 hasAncestor(functionDecl(matchesName("bbl_bind_"))))
            .bind("rename_namespace"));

    class_finder.addMatcher(m_construct_expr_matcher, &class_binding_extractor);
    class_finder.addMatcher(m_enum_construct_expr_matcher,
                            &class_binding_extractor);
    class_finder.addMatcher(m_rename_namespace_matcher,
                            &class_binding_extractor);
    class_finder.addMatcher(m_var_decl_matcher, &class_binding_extractor);
    SPDLOG_INFO("running class finder");
    result = tool.run(newFrontendActionFactory(&class_finder).get());

    // Extract method bindings
    MatchFinder method_finder;
    ExtractMethodBindings method_binding_extractor(this);

    StatementMatcher m_dre_matcher =
        traverse(clang::TK_IgnoreUnlessSpelledInSource,
                 declRefExpr(allOf(hasDeclaration(cxxMethodDecl()),
                                   hasAncestor(cxxMemberCallExpr(
                                       callee(cxxMethodDecl(hasName("m")))))))
                     .bind("DeclRefExpr(m)"));

    StatementMatcher m_dre_fn_matcher = traverse(
        clang::TK_IgnoreUnlessSpelledInSource,
        declRefExpr(hasAncestor(functionDecl(matchesName("bbl_bind_"))),
                    to(functionDecl(hasName("fn"))))
            .bind("DeclRefExpr(fn)"));

    StatementMatcher m_dre_field_matcher =
        traverse(clang::TK_IgnoreUnlessSpelledInSource,
                 declRefExpr(allOf(hasDeclaration(fieldDecl()),
                                   hasAncestor(cxxMemberCallExpr(
                                       callee(cxxMethodDecl(hasName("f")))))))
                     .bind("DeclRefExpr(field)"));

    StatementMatcher m_ctor_matcher = traverse(
        clang::TK_AsIs,
        cxxMemberCallExpr(hasAncestor(functionDecl(matchesName("bbl_bind_"))),
                          hasDeclaration(cxxMethodDecl(hasName("ctor"))))
            .bind("Ctor"));

    StatementMatcher m_replace_with_matcher =
        traverse(clang::TK_AsIs,
                 cxxMemberCallExpr(
                     hasAncestor(functionDecl(matchesName("bbl_bind_"))),
                     hasDeclaration(cxxMethodDecl(hasName("replace_with"))))
                     .bind("replace_with"));

    method_finder.addMatcher(m_dre_matcher, &method_binding_extractor);
    method_finder.addMatcher(m_dre_fn_matcher, &method_binding_extractor);
    method_finder.addMatcher(m_ctor_matcher, &method_binding_extractor);
    method_finder.addMatcher(m_dre_field_matcher, &method_binding_extractor);
    method_finder.addMatcher(m_replace_with_matcher, &method_binding_extractor);

    SPDLOG_INFO("running method finder");
    result = tool.run(newFrontendActionFactory(&method_finder).get());

    C_API c_api(*this);
}

} // namespace bbl