#pragma once

#if defined(WIN32)
#pragma warning(push)
#pragma warning(disable : 4624)
#pragma warning(disable : 4291)
#endif

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/ParentMapContext.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Preprocessor.h>


#if defined(WIN32)
#pragma warning(pop)
#endif

// https://stackoverflow.com/questions/11083066/getting-the-source-behind-clangs-ast
/**
 * Gets the portion of the code that corresponds to given SourceRange exactly as
 * the range is given.
 *
 * @warning The end location of the SourceRange returned by some Clang functions
 * (such as clang::Expr::getSourceRange) might actually point to the first
 * character (the "location") of the last token of the expression, rather than
 * the character past-the-end of the expression like clang::Lexer::getSourceText
 * expects. get_source_text_raw() does not take this into account. Use
 * get_source_text() instead if you want to get the source text including the
 * last token.
 *
 * @warning This function does not obtain the source of a macro/preprocessor
 * expansion. Use get_source_text() for that.
 */
std::string get_source_text_raw(clang::SourceRange range,
                                const clang::SourceManager& sm);

// https://stackoverflow.com/questions/11083066/getting-the-source-behind-clangs-ast
/**
 * Gets the portion of the code that corresponds to given SourceRange, including
 * the last token. Returns expanded macros.
 *
 * @see get_source_text_raw()
 */
std::string get_source_text(clang::SourceRange range,
                            const clang::SourceManager& sm);

std::string get_spelling_text(clang::SourceRange range,
                              const clang::SourceManager& sm);

std::string get_source_and_location(clang::Stmt const* stmt, clang::SourceManager const& sm);

namespace clang {
class MangleContext;
}

/// Get the mangled name from the given NamedDecl.
///
/// We use these as unique identifiers throughout the extraction process. These
/// *should* be globally unique for a given build, but will be different on
/// Windows and Linux due to the different ABI, and may (?) be different between
/// different clang versions. This will cause problems if we use the IDs
/// anywhere in our test suite, so probably best to avoid that.
std::string get_mangled_name(clang::NamedDecl const* nd,
                             clang::MangleContext* ctx);

/// Depth-first search of the subtree under (and including) `stmt`
/// @return The first descendent found with type `T`
template <typename T>
auto find_first_descendent_of_type(clang::Stmt const* stmt) -> T const* {
    if (auto const* stmt_cast = llvm::dyn_cast<T>(stmt)) {
        return stmt_cast;
    }

    for (auto const* child : stmt->children()) {
        if (auto const* child_result =
                find_first_descendent_of_type<T>(child)) {
            return child_result;
        }
    }

    return nullptr;
}

/// Find all descendents of Stmt `stmt` of type `T`.
template <typename T>
auto find_all_descendents_of_type(clang::Stmt const* stmt,
                                  std::vector<T const*>& result) -> void {
    if (stmt == nullptr) {
        return;
    }

    if (T const* ptr = llvm::dyn_cast<T>(stmt); ptr != nullptr) {
        result.push_back(ptr);
    }

    for (auto const* child : stmt->children()) {
        find_all_descendents_of_type(child, result);
    }
}

/// Visit the subtree, depth-first, under `stmt`, executing function F for each
/// node.
///
/// F should have signature (Stmt const*) -> bool
/// if F returns true, then the search is stopped.
template <typename F>
auto visit_subtree(clang::Stmt const* stmt, F fun) -> bool {
    if (stmt == nullptr) {
        return false;
    }

    if (fun(stmt)) {
        return true;
    }

    for (auto const* child : stmt->children()) {
        if (visit_subtree(child, fun)) {
            return true;
        }
    }

    return false;
}

/// Search all ancestors (and including) `stmt`
/// @return The first ancestor found with type `T`
template <typename T>
auto find_first_ancestor_of_type(clang::Stmt const* stmt,
                                 clang::ASTContext* ctx) -> T const* {
    if (stmt == nullptr) {
        return nullptr;
    }

    if (auto const* stmt_cast = llvm::dyn_cast_or_null<T>(stmt)) {
        return stmt_cast;
    }

    auto const& parents = ctx->getParents(*stmt);
    for (auto const& parent : parents) {
        if (auto const* stmt_parent = find_first_ancestor_of_type<T>(
                parent.get<clang::Stmt>(), ctx)) {
            return stmt_parent;
        }
    }

    return nullptr;
}

inline auto get_parent(clang::Stmt const* stmt, clang::ASTContext* ctx)
    -> clang::Stmt const* {
    auto const& parents = ctx->getParents(*stmt);
    if (parents.empty()) {
        return nullptr;
    }

    return parents[0].get<clang::Stmt>();
}

template <typename T>
inline auto get_parent(clang::Stmt const* stmt, clang::ASTContext* ctx)
    -> T const* {
    return llvm::dyn_cast_or_null<T>(get_parent(stmt, ctx));
}

/// Walk up the AST node graph and find a bbl module, returning true as soon as
/// one is found
inline auto walk_to_module(clang::DynTypedNode const& node,
                           clang::ASTContext& ctx, clang::MangleContext* mctx,
                           std::string& id) -> bool {
    if (auto const* fd = node.get<clang::FunctionDecl>()) {
        if (fd->getNameAsString().find("bbl_bind_") == 0) {
            id = get_mangled_name(fd, mctx);
            return true;
        }
    }

    for (auto const& parent : ctx.getParents(node)) {
        if (walk_to_module(parent, ctx, mctx, id)) {
            return true;
        }
    }

    return false;
}

inline auto walk_to_module_decl(clang::DynTypedNode const& node,
                                clang::ASTContext& ctx)
    -> clang::FunctionDecl const* {
    if (auto const* fd = node.get<clang::FunctionDecl>()) {
        if (fd->getNameAsString().find("bbl_bind_") == 0) {
            return fd;
        }
    }

    for (auto const& parent : ctx.getParents(node)) {
        if (clang::FunctionDecl const* fd = walk_to_module_decl(parent, ctx)) {
            return fd;
        }
    }

    return nullptr;
}

/// Find the bbl module (i.e. BBL_MODULE(X)) containing Stmt `stmt`.
///
/// @return true if a module is found, false otherwise
inline auto find_containing_module(clang::Stmt const* stmt,
                                   clang::ASTContext* ctx,
                                   clang::MangleContext* mctx, std::string& id)
    -> bool {
    for (auto const& parent : ctx->getParents(*stmt)) {
        if (walk_to_module(parent, *ctx, mctx, id)) {
            return true;
        }
    }

    return false;
}

inline auto find_containing_module_decl(clang::Stmt const* stmt,
                                        clang::ASTContext* ctx)
    -> clang::FunctionDecl const* {
    for (auto const& parent : ctx->getParents(*stmt)) {
        if (clang::FunctionDecl const* fd = walk_to_module_decl(parent, *ctx)) {
            return fd;
        }
    }

    return nullptr;
}

inline auto find_containing_module_decl(clang::Decl const* decl,
                                        clang::ASTContext* ctx)
    -> clang::FunctionDecl const* {
    for (auto const& parent : ctx->getParents(*decl)) {
        if (clang::FunctionDecl const* fd = walk_to_module_decl(parent, *ctx)) {
            return fd;
        }
    }

    return nullptr;
}

inline auto walk_to_vardecl(clang::DynTypedNode const& node,
                            clang::ASTContext& ctx) -> clang::VarDecl const* {
    if (auto const* fd = node.get<clang::FunctionDecl>()) {
        if (fd->getNameAsString().find("bbl_bind_") == 0) {
            return nullptr;
        }
    }

    for (auto const& parent : ctx.getParents(node)) {
        if (clang::VarDecl const* vd = walk_to_vardecl(parent, ctx)) {
            return vd;
        }
    }

    return nullptr;
}

template <typename U>
auto get_parent_as(clang::Decl const* decl_or_stmt, clang::ASTContext& ctx)
    -> U const* {
    for (auto const& node : ctx.getParents(*decl_or_stmt)) {
        if (U const* p = node.get<U>()) {
            return p;
        }
    }

    return nullptr;
}

template <typename U>
auto get_parent_as(clang::Stmt const* decl_or_stmt, clang::ASTContext& ctx)
    -> U const* {
    for (auto const& node : ctx.getParents(*decl_or_stmt)) {
        if (U const* p = node.get<U>()) {
            return p;
        }
    }

    return nullptr;
}

inline auto find_containing_vardecl(clang::Stmt const* stmt,
                                    clang::ASTContext* ctx)

    -> clang::VarDecl const* {
    for (auto const& parent : ctx->getParents(*stmt)) {
        if (clang::VarDecl const* vd = walk_to_vardecl(parent, *ctx)) {
            return vd;
        }
    }

    return nullptr;
}

/// Find the first child of `stmt` with type `T`
/// @return the first child found, or nullptr if none found
template <typename T>
auto find_first_child_of_type(clang::Stmt const* stmt) -> T const* {
    for (auto const* child : stmt->children()) {
        if (auto const* child_result = llvm::dyn_cast<T>(child)) {
            return child_result;
        }
    }

    return nullptr;
}

template <typename T>
auto find_all_children_of_type(clang::DeclContext const* decl)
    -> std::vector<T const*> {
    std::vector<T const*> result;
    for (auto const* child : decl->decls()) {
        if (llvm::isa<T>(child)) {
            result.push_back(llvm::dyn_cast<T>(child));
        }
    }

    return result;
}

template <typename T>
auto find_named_child_of_type(clang::DeclContext const* decl, char const* name)
    -> T const* {
    for (auto const* child : decl->decls()) {
        if (T const* tchild = llvm::dyn_cast<T>(child)) {
            if (tchild->getNameAsString() == name) {
                return tchild;
            }
        }
    }

    return nullptr;
}

auto is_in_std_namespace(clang::DeclContext const* dc) -> bool;
auto is_in_namespace(clang::DeclContext const* dc, std::string const& name)
    -> bool;

inline auto get_comment_from_decl(clang::Decl const* decl,
                                  clang::ASTContext* ast_context)
    -> std::string {
    std::string result;
    clang::RawComment* raw_comment =
        ast_context->getRawCommentForDeclNoCache(decl);
    if (raw_comment) {
        result = raw_comment->getFormattedText(ast_context->getSourceManager(),
                                               ast_context->getDiagnostics());
    }

    return result;
}

