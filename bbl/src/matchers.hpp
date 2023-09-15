#pragma once

#if defined(WIN32)
#pragma warning(push)
#pragma warning(disable : 4624)
#pragma warning(disable : 4291)
#endif

#include <clang/AST/AST.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>

#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/FrontendActions.h>

#if defined(WIN32)
#pragma warning(pop)
#endif

namespace bbl {

class Context;

struct BabbleIDs {
    int64_t bbl_class = -1;
    int64_t bbl_class_m_ctor = -1;
    int64_t bbl_class_m_copy_ctor = -1;
    int64_t bbl_class_m_move_ctor = -1;
    int64_t bbl_class_m_dtor = -1;
    int64_t bbl_class_m = -1;
    int64_t bbl_enum = -1;
};

/// Responsible for finding the bbl types in the AST so we can double-check that
/// when we're traversing the AST looking for calls to them that we're
/// definitely on the right expressions. To be honest, this is probably
/// over-cautious and not worth the extra work
class ExtractBabble : public clang::ast_matchers::MatchFinder::MatchCallback {
    BabbleIDs _ids;

public:
    virtual void
    run(clang::ast_matchers::MatchFinder::MatchResult const& result) override;
    BabbleIDs get_ids() const { return _ids; }
};

/// Responsible for finding the BBL_MODULE() declarations (which are just
/// specially named functions) in the AST, and extracting them in a first pass
/// so that decls can be added to them later
class ExtractModules : public clang::ast_matchers::MatchFinder::MatchCallback {
    bbl::Context* _bbl_ctx;

public:
    ExtractModules(bbl::Context* bbl_ctx) : _bbl_ctx(bbl_ctx) {}
    virtual void
    run(clang::ast_matchers::MatchFinder::MatchResult const& result) override;
};

/// Responsible for extracting the method bindings by looking for the `.m()`
/// calls attached to a `bbl::Class`
class ExtractMethodBindings
    : public clang::ast_matchers::MatchFinder::MatchCallback {
    bbl::Context* _bbl_ctx;
    int64_t _bbl_class_id;

public:
    ExtractMethodBindings(bbl::Context* bbl_ctx, int64_t bbl_class_id)
        : _bbl_ctx(bbl_ctx), _bbl_class_id(bbl_class_id) {}
    virtual void
    run(clang::ast_matchers::MatchFinder::MatchResult const& result) override;
};

/// Responsible for extracting classes (and structs, specializations etc) by
/// looking for the `bbl:Class<X>()` constructor in the AST
class ExtractClassBindings
    : public clang::ast_matchers::MatchFinder::MatchCallback {
    bbl::Context* _bbl_ctx;
    int64_t _bbl_class_id;
    int64_t _bbl_enum_id;

public:
    ExtractClassBindings(bbl::Context* bbl_ctx, int64_t bbl_class_id,
                         int64_t bbl_enum_id)
        : _bbl_ctx(bbl_ctx), _bbl_class_id(bbl_class_id),
          _bbl_enum_id(bbl_enum_id) {}
    virtual void
    run(clang::ast_matchers::MatchFinder::MatchResult const& result) override;
};

} // namespace bbl