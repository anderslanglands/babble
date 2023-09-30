#include "matchers.hpp"
#include "astutil.hpp"
#include "bbl.hpp"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Type.h"

#include <spdlog/spdlog.h>

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>
#include <clang/AST/Mangle.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Casting.h>

#include <exception>
#include <memory>
#include <variant>

namespace bbl {

clang::TranslationUnitDecl const*
find_translation_unit(clang::DeclContext const* dc) {
    if (dc == nullptr) {
        return nullptr;
    }

    if (auto const* tud = dyn_cast<clang::TranslationUnitDecl>(dc)) {
        return tud;
    }

    return find_translation_unit(dc->getParent());
}

auto create_mangle_context(clang::ASTContext& ast_context)
    -> std::unique_ptr<clang::MangleContext> {
    // Just get the mangle context from the ASTContext
    /// XXX: would be nice if we could force Itanium ABI here so that Windows
    /// and Linux produced the same IDs
    return std::unique_ptr<clang::MangleContext>(
        ast_context.createMangleContext());
}

void ExtractModules::run(
    clang::ast_matchers::MatchFinder::MatchResult const& result) {
    clang::ASTContext* ast_context = result.Context;
    std::unique_ptr<clang::MangleContext> mangle_context =
        create_mangle_context(*ast_context);

    if (clang::FunctionDecl const* fd =
            result.Nodes.getNodeAs<clang::FunctionDecl>("Module")) {
        std::string name =
            fd->getNameAsString().substr(9); //< 9 is the length of "bbl_bind_"
        clang::SourceManager& sm = ast_context->getSourceManager();
        auto loc_expanded = sm.getExpansionLoc(fd->getSourceRange().getBegin());
        std::string source_filename = sm.getFilename(loc_expanded).str();

        std::string module_id = get_mangled_name(fd, mangle_context.get());

        _bbl_ctx->insert_module(module_id, Module{
                                               source_filename,
                                               name,
                                               {}, // classes
                                               {}, // functions
                                               {}, // stdfunctions
                                               {}, // enums
                                           });

        // Add this module's id to the list of modules in its source file
        /// XXX: source files should probably already be created if we do
        /// includes pass first?
        if (SourceFile* source_file =
                _bbl_ctx->get_source_file(source_filename)) {
            source_file->modules.push_back(module_id);
        } else {
            _bbl_ctx->insert_source_file(
                source_filename, SourceFile{{}, {module_id}, source_filename});
        }

        // fd->dumpColor();
    }
}

/// Find all DeclRefExr descendents of `stmt` and if they refer to a
/// CXXMethodDecl then push them onto `method_decls`
void find_cxxmethoddecl_descendents(
    clang::Stmt const* stmt,
    std::vector<clang::CXXMethodDecl const*>& method_decls) {
    if (auto const* dre = llvm::dyn_cast<clang::DeclRefExpr>(stmt)) {
        if (auto const* cmd =
                llvm::dyn_cast<clang::CXXMethodDecl>(dre->getDecl())) {
            method_decls.push_back(cmd);
            return;
        }
    }

    for (auto const* child : stmt->children()) {
        find_cxxmethoddecl_descendents(child, method_decls);
    }
}

clang::CXXRecordDecl const*
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
    // XXX: Do we really need to do this? It's an abundance of caution that's
    // costing us an extra tool run...
    if (crd->getQualifiedNameAsString() != "bbl::Class") {
        BBL_THROW("got class binding but underlying record {} - {} "
                  "does not match bbl::Class",
                  crd->getQualifiedNameAsString(), crd->getID());
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

clang::EnumDecl const*
get_enum_to_extract_from_construct_expr(clang::CXXConstructExpr const* cce) {
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
                  crd->getQualifiedNameAsString(), crd->getID());
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
        BBL_THROW("tried to bind type \"{}\" as an Enum", qt.getAsString());
    }

    clang::EnumDecl const* enum_decl =
        dyn_cast<clang::EnumDecl>(type->getAsTagDecl());
    if (enum_decl == nullptr) {
        BBL_THROW("got null enum decl ptr from {}", qt.getAsString());
    }

    return enum_decl;
}

// Get the bbl::Class decl from the given CXXConstructExpr, find the size and
// align fields, evaluate their expressions, and return the result as a Layout
// object
Layout get_record_layout(clang::CXXConstructExpr const* cce,
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
BindKind search_for_bind_kind_calls(clang::Stmt const* stmt,
                                    clang::ASTContext& ast_context) {
    if (stmt == nullptr || llvm::isa<clang::CompoundStmt>(stmt)) {
        // if we haven't found one, default to opaque ptr
        return BindKind::OpaquePtr;
    }

    if (clang::MemberExpr const* me = dyn_cast<clang::MemberExpr>(stmt)) {
        if (me->getMemberDecl()->getNameAsString() == "value_type") {
            return BindKind::ValueType;
        } else if (me->getMemberDecl()->getNameAsString() == "opaque_bytes") {
            return BindKind::OpaqueBytes;
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

llvm::APSInt evaluate_field_expression_int(clang::RecordDecl const* rd,
                                           char const* name,
                                           clang::ASTContext& ast_context) {
    clang::FieldDecl const* vd_size =
        find_named_child_of_type<clang::FieldDecl>(rd, "type_size");
    if (vd_size == nullptr) {
        BBL_THROW("could not get {} from {}", name,
                  rd->getQualifiedNameAsString());
    }

    clang::Expr const* expr_size = vd_size->getInClassInitializer();
    if (expr_size == nullptr) {
        BBL_THROW("could not get {} expr from {}", name,
                  rd->getQualifiedNameAsString());
    }

    return expr_size->EvaluateKnownConstInt(ast_context);
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

void ExtractMethodBindings::run(
    clang::ast_matchers::MatchFinder::MatchResult const& result) {
    clang::ASTContext* ast_context = result.Context;
    std::unique_ptr<clang::MangleContext> mangle_context =
        create_mangle_context(*ast_context);
    clang::SourceManager& sm = ast_context->getSourceManager();

    if (clang::DeclRefExpr const* dre =
            result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRefExpr(m)")) {
        if (auto const* cmd =
                llvm::dyn_cast<clang::CXXMethodDecl>(dre->getDecl())) {
            clang::CXXRecordDecl const* crd_parent = cmd->getParent();

            // Handle templated calls. These are calls that require spelling one
            // or more template arguments in the call to be resolved, for
            // instance anything where the template argument appears only in the
            // return type.
            std::string template_call;
            if (cmd->isFunctionTemplateSpecialization()) {
                std::string dre_text =
                    get_source_text(dre->getSourceRange(), sm);
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

                    template_call =
                        dre_text.substr(template_start_pos,
                                        dre_text.size() - template_start_pos);
                }
            }

            clang::CXXMemberCallExpr const* mce =
                find_first_ancestor_of_type<clang::CXXMemberCallExpr>(
                    dre, ast_context);
            if (mce) {
                // If there's a StringLiteral inside an ImplicitCastExpr,
                // that will be our rename string
                std::string rename_str;
                if (auto const* ice =
                        find_first_child_of_type<clang::ImplicitCastExpr>(
                            mce)) {
                    if (auto const* sl =
                            find_first_child_of_type<clang::StringLiteral>(
                                ice)) {
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
                std::string target_class_id =
                    get_mangled_name(crd_target, mangle_context.get());
                std::string parent_id =
                    get_mangled_name(crd_parent, mangle_context.get());
                if (target_class_id != parent_id &&
                    !crd_target->isDerivedFrom(crd_parent)) {
                    BBL_THROW("method {} is bound to class {} but {} is "
                              "not derived from {}",
                              cmd->getQualifiedNameAsString(),
                              crd_target->getQualifiedNameAsString(),
                              crd_target->getQualifiedNameAsString(),
                              crd_parent->getQualifiedNameAsString());
                }

                if (bbl::Class* cls = _bbl_ctx->get_class(target_class_id)) {
                    // extract and add the method to the corresponding class
                    std::string method_id =
                        get_mangled_name(cmd, mangle_context.get());

                    if (_bbl_ctx->has_method(method_id)) {
                        /// XXX: report loction of binding
                        SPDLOG_WARN("method {} is already bound, ignoring "
                                    "second binding",
                                    cmd->getQualifiedNameAsString());
                    } else {
                        try {
                            bbl::Method method =
                                _bbl_ctx->extract_method_binding(
                                    cmd, rename_str, template_call,
                                    mangle_context.get());
                            _bbl_ctx->insert_method_binding(method_id,
                                                            std::move(method));
                            cls->methods.emplace_back(std::move(method_id));
                        } catch (std::exception& e) {
                            BBL_RETHROW(e, "could not bind method {}",
                                        cmd->getQualifiedNameAsString());
                        }
                    }

                } else {
                    BBL_THROW("method {} is targeting class {} but this class "
                              "is not extracted",
                              cmd->getQualifiedNameAsString(),
                              crd_target->getQualifiedNameAsString());
                }
            } else {
                SPDLOG_WARN("Could not get CXXMemberCallExpr from DeclRefExpr "
                            "{}",
                            get_source_text(dre->getSourceRange(),
                                            ast_context->getSourceManager()));
            }
        } else {
            BBL_THROW("got decl ref expr but decl is not CXXMethodDecl");
        }
    }

    if (clang::DeclRefExpr const* dre =
            result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRefExpr(field)")) {
        auto const* fd = dyn_cast_or_null<clang::FieldDecl>(dre->getDecl());
        if (!fd) {
            BBL_THROW("could not get FieldDecl from DeclRefExpr");
        }

        QType qt_field;
        try {
            qt_field =
                _bbl_ctx->extract_qualtype(fd->getType(), mangle_context.get());
        } catch (std::exception& e) {
            BBL_RETHROW(e, "could not extract field \"{}\" from class \"{}\"",
                        fd->getNameAsString(),
                        fd->getParent()->getQualifiedNameAsString());
        }

        clang::CXXMemberCallExpr const* mce =
            find_first_ancestor_of_type<clang::CXXMemberCallExpr>(dre,
                                                                  ast_context);
        if (!mce) {
            BBL_THROW("could not get CXXMemberCallExpr from DeclRefExpr "
                      "extracting field {}",
                      fd->getQualifiedNameAsString());
        }

        clang::CXXRecordDecl const* crd_parent =
            dyn_cast_or_null<clang::CXXRecordDecl>(fd->getParent());
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
        std::string target_class_id =
            get_mangled_name(crd_target, mangle_context.get());
        std::string parent_id =
            get_mangled_name(crd_parent, mangle_context.get());
        if (target_class_id != parent_id &&
            !crd_target->isDerivedFrom(crd_parent)) {
            BBL_THROW("field {} is bound to class {} but {} is "
                      "not derived from {}",
                      fd->getQualifiedNameAsString(),
                      crd_target->getQualifiedNameAsString(),
                      crd_target->getQualifiedNameAsString(),
                      crd_parent->getQualifiedNameAsString());
        }

        bbl::Class* cls = _bbl_ctx->get_class(target_class_id);
        if (cls == nullptr) {
            BBL_THROW("extracting field on class {} but class has not been "
                      "extracted.",
                      target_class_id);
        }

        cls->fields.emplace_back(Field{
            std::move(qt_field),
            fd->getNameAsString(),
        });
    }

    if (clang::DeclRefExpr const* dre_fn =
            result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRefExpr(fn)")) {
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
            if (clang::DeclRefExpr const* dre =
                    dyn_cast<clang::DeclRefExpr>(stmt);
                dre && dre != dre_fn) {
                dre_target = dre;
                return true;
            }
            return false;
        });

        if (dre_target == nullptr) {
            BBL_THROW("Got unexpected set of DeclRefExprs");
        }

        std::string spelling =
            get_source_text(dre_target->getSourceRange(), sm);

        if (auto const* fd =
                llvm::dyn_cast<clang::FunctionDecl>(dre_target->getDecl())) {

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

                    template_call =
                        spelling.substr(template_start_pos,
                                        spelling.size() - template_start_pos);
                }
            }

            // Now extract the function
            Function function = _bbl_ctx->extract_function_binding(
                fd, rename_str, spelling, template_call, mangle_context.get());
            std::string id = get_mangled_name(fd, mangle_context.get());

            std::string mod_id;
            if (!find_containing_module(dre_fn, ast_context,
                                        mangle_context.get(), mod_id)) {
                BBL_THROW("could not find containing module for {}",
                          get_source_text(dre_fn->getSourceRange(),
                                          ast_context->getSourceManager()));
            }

            _bbl_ctx->insert_function_binding(mod_id, id, std::move(function));

        } else {
            BBL_THROW("got decl ref expr but decl is not FunctionDecl");
        }
    }

    if (clang::CXXMemberCallExpr const* mce =
            result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("Ctor")) {

        // auto const* cce =
        //     find_first_child_of_type<clang::CXXConstructExpr>(mce);
        clang::CXXConstructExpr const* cce = nullptr;
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
            auto const* mod_decl =
                find_containing_module_decl(mce, ast_context);
            mod_decl->dumpColor();

            BBL_THROW(
                "could not find CXXConstructExpr from Ctor CXXMemberCallExpr");
        }

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
        _bbl_ctx->extract_template_arguments(ctsd, template_args,
                                             mangle_context.get());

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
                    param_name =
                        param_names[param_name_index]->getString().str();
                } else {
                    // handle missing param names with auto-generated names
                    param_name = fmt::format("param{:02}", param_name_index);
                }

                parameters.push_back(
                    bbl::Param{param_name, std::move(std::get<QType>(arg))});
            }

            ++i;
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

        std::string target_class_id =
            get_mangled_name(crd_target, mangle_context.get());

        if (bbl::Class* cls = _bbl_ctx->get_class(target_class_id)) {
            std::string ctor_id = get_mangled_name(ccd, mangle_context.get());
            _bbl_ctx->insert_constructor_binding(ctor_id,
                                                 Constructor{
                                                     rename_str,
                                                     std::move(parameters),
                                                     is_noexcept,
                                                 });

            // Add the constructor to the class
            cls->constructors.emplace_back(ctor_id);

        } else {
            BBL_THROW("Ctor is targeting class {} but this class "
                      "is not extracted",
                      crd_target->getQualifiedNameAsString());
        }
    }

    if (clang::CXXMemberCallExpr const* mce =
            result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("replace_with")) {
        // type to replace with will be the first (and only) template argument
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
                Field{_bbl_ctx->extract_qualtype(field->getType(),
                                                 mangle_context.get()),
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

        std::string target_class_id =
            get_mangled_name(crd_target, mangle_context.get());

        if (bbl::Class* cls = _bbl_ctx->get_class(target_class_id)) {
            cls->fields = std::move(fields);
            cls->bind_kind = BindKind::ValueType;
        } else {
            BBL_THROW("replace_with is targeting class {} but this class "
                      "is not extracted",
                      crd_target->getQualifiedNameAsString());
        }
    }
}

void ExtractClassBindings::run(
    clang::ast_matchers::MatchFinder::MatchResult const& result) {
    clang::ASTContext* ast_context = result.Context;
    std::unique_ptr<clang::MangleContext> mangle_context =
        create_mangle_context(*ast_context);
    clang::SourceManager const& sm = ast_context->getSourceManager();

    if (clang::CXXConstructExpr const* construct_expr =
            result.Nodes.getNodeAs<clang::CXXConstructExpr>("ctor_expr")) {

        // Find the class decl that we're binding from the bbl::Class<Foo>()
        // constructor
        clang::CXXRecordDecl const* type_record_decl =
            get_record_to_extract_from_construct_expr(construct_expr);

        Layout layout = get_record_layout(construct_expr, *ast_context);
        BindKind bind_kind =
            search_for_bind_kind_calls(construct_expr, *ast_context);

        // Get the rule of seven fields from the bbl::Class decl
        clang::CXXConstructorDecl* bbl_ctor_decl =
            construct_expr->getConstructor();
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
        bool is_abstract = evaluate_field_expression_bool(
            bbl_ctsd, "is_abstract", *ast_context);

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

        clang::SourceRange ctor_type_range =
            clang::SourceRange(loc_begin, loc_end);

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

        // and extract the class
        if (type_record_decl->getNameAsString() == "function" &&
            is_in_std_namespace(type_record_decl) &&
            llvm::isa<clang::ClassTemplateSpecializationDecl>(
                type_record_decl)) {
            // this is a specialization of std::function, special-case this
            try {
                StdFunction fun = _bbl_ctx->extract_stdfunction_binding(
                    dyn_cast<clang::ClassTemplateSpecializationDecl>(
                        type_record_decl),
                    class_spelling, rename_str, mangle_context.get());
                std::string id =
                    get_mangled_name(type_record_decl, mangle_context.get());

                std::string mod_id;
                if (!find_containing_module(construct_expr, ast_context,
                                            mangle_context.get(), mod_id)) {
                    BBL_THROW("could not find containing module for {}",
                              get_source_text(construct_expr->getSourceRange(),
                                              ast_context->getSourceManager()));
                }

                _bbl_ctx->insert_stdfunction_binding(mod_id, id,
                                                     std::move(fun));
            } catch (std::exception& e) {
                BBL_RETHROW(e, "could not extract stdfunction {}",
                            class_spelling);
            }

        } else {
            Class cls = _bbl_ctx->extract_class_binding(
                type_record_decl, class_spelling, rename_str, layout, bind_kind,
                RuleOfSeven{
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
                is_abstract, mangle_context.get());

            std::string id =
                get_mangled_name(type_record_decl, mangle_context.get());

            std::string mod_id;
            if (!find_containing_module(construct_expr, ast_context,
                                        mangle_context.get(), mod_id)) {
                BBL_THROW("could not find containing module for {}",
                          get_source_text(construct_expr->getSourceRange(),
                                          ast_context->getSourceManager()));
            }
            // SPDLOG_INFO("inserting class {} {}", id,
            //             type_record_decl->getQualifiedNameAsString());
            _bbl_ctx->insert_class_binding(mod_id, id, std::move(cls));
        }
    } else if (clang::CXXConstructExpr const* construct_expr =
                   result.Nodes.getNodeAs<clang::CXXConstructExpr>(
                       "enum_ctor_expr")) {
        // Find the class decl that we're binding from the bbl::Class<Foo>()
        // constructor
        clang::EnumDecl const* enum_decl =
            get_enum_to_extract_from_construct_expr(construct_expr);

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

        Enum enm = _bbl_ctx->extract_enum_binding(
            enum_decl, class_spelling, rename_str, mangle_context.get());
        std::string id = get_mangled_name(enum_decl, mangle_context.get());

        std::string mod_id;
        if (!find_containing_module(construct_expr, ast_context,
                                    mangle_context.get(), mod_id)) {
            BBL_THROW("could not find containing module for {}",
                      get_source_text(construct_expr->getSourceRange(),
                                      ast_context->getSourceManager()));
        }

        // SPDLOG_INFO("inserting enum {} {}", id,
        //             enum_decl->getQualifiedNameAsString());
        _bbl_ctx->insert_enum_binding(mod_id, id, std::move(enm));
    } else if (clang::VarDecl const* vd =
                   result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {

        /// XXX: Come back to this later...
        BBL_THROW("bbl::Class decls of type \"bbl::Class foo = "
                  "bbl::Class<Foo>()\" are not currently supported.");

        clang::SourceRange vd_range = vd->getSourceRange();
        std::string vd_range_str = get_source_text(vd_range, sm);
        if (vd_range_str.find("=") != std::string::npos) {
            // this is an assignment declaration, so the spelling captured is
            // already correct
            return;
        }

        // This *should* fire after th corresponding CXXConstructExpr, we're
        // just extracting the right spelling here
        clang::QualType qt = vd->getType();
        clang::ClassTemplateSpecializationDecl const* ctsd =
            dyn_cast<clang::ClassTemplateSpecializationDecl>(
                qt->getAsCXXRecordDecl());
        if (ctsd == nullptr || ctsd->getNameAsString() != "Class") {
            BBL_THROW("Got CXXRecordDecl from VarDecl but is it not Class: {}",
                      ctsd->getQualifiedNameAsString());
        }

        // first template arg will then be the decl we are binding
        auto const& template_args = ctsd->getTemplateArgs();
        if (template_args.size() != 1) {
            BBL_THROW("got CTSD from VarDecl but it has {} template args "
                      "instead of 1: {}",
                      template_args.size(), vd_range_str);
        }

        auto const* crd_target =
            template_args[0].getAsType()->getAsCXXRecordDecl();
        if (crd_target == nullptr) {
            BBL_THROW("could not get CXXRecordDecl from template arg in {}",
                      vd_range_str);
        }

        std::string class_id =
            get_mangled_name(crd_target, mangle_context.get());
        Class* cls = _bbl_ctx->get_class(class_id);
        if (cls == nullptr) {
            BBL_THROW("CRD target {} in VarDecl {} has not been extracted",
                      crd_target->getQualifiedNameAsString(), vd_range_str);
        }

        // split the decl into the type and the init expr
        clang::Expr const* init_expr = vd->getInit();
        if (!init_expr) {
            BBL_THROW("could not get init expr from vardecl {}",
                      get_source_text(vd_range, sm));
        }
        clang::SourceRange bbl_type_range(
            vd_range.getBegin(), init_expr->getSourceRange().getBegin());
        std::string bbl_type_str = get_source_text(bbl_type_range, sm);

        // so now our type string should just be the bit between the first and
        // last <>
        size_t begin = bbl_type_str.find_first_of('<');
        size_t end = bbl_type_str.find_last_of('>');
        if (begin == std::string::npos || end == std::string::npos) {
            BBL_THROW("could not find template type in {}", vd_range_str);
        }

        cls->spelling = bbl_type_str.substr(begin + 1, end - begin - 1);
    } else if (clang::CallExpr const* ce =
                   result.Nodes.getNodeAs<clang::CallExpr>(
                       "rename_namespace")) {

        std::vector<clang::StringLiteral const*> lits;
        find_all_descendents_of_type<clang::StringLiteral>(ce, lits);
        if (lits.size() != 2) {
            BBL_THROW("rename_namespace call does not have two string literal "
                      "arguments");
        }

        std::string rename_from = lits[0]->getString().str();
        std::string rename_to = lits[1]->getString().str();

        std::string mod_id;
        if (!find_containing_module(ce, ast_context, mangle_context.get(),
                                    mod_id)) {
            BBL_THROW("could not find module containing CallExpr {}",
                      get_source_text(ce->getSourceRange(), sm));
        }

        Module* mod = _bbl_ctx->get_module(mod_id);
        mod->namespace_from = rename_from;
        mod->namespace_to = rename_to;
    }
}

} // namespace bbl
