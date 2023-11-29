#include "astutil.hpp"

#if defined(WIN32)
#pragma warning(push)
#pragma warning(disable : 4624)
#pragma warning(disable : 4291)
#endif

#include <clang/AST/Mangle.h>

#if defined(WIN32)
#pragma warning(pop)
#endif

#include <spdlog/fmt/fmt.h>

using llvm::dyn_cast;

std::string get_source_text_raw(clang::SourceRange range,
                                const clang::SourceManager& sm) {
    return clang::Lexer::getSourceText(
               clang::CharSourceRange::getCharRange(range),
               sm,
               clang::LangOptions())
        .str();
}

std::string get_source_text(clang::SourceRange range,
                            const clang::SourceManager& sm) {
    clang::LangOptions lo;

    // NOTE: sm.getSpellingLoc() used in case the range corresponds to a
    // macro/preprocessed source.
    auto start_loc = sm.getExpansionLoc(range.getBegin());
    auto last_token_loc = sm.getExpansionLoc(range.getEnd());
    auto end_loc = clang::Lexer::getLocForEndOfToken(last_token_loc, 0, sm, lo);
    auto printable_range = clang::SourceRange{start_loc, end_loc};
    return get_source_text_raw(printable_range, sm);
}

std::string get_source_and_location(clang::Stmt const* stmt,
                                    clang::SourceManager const& sm) {
    std::string filename =
        sm.getFilename(stmt->getSourceRange().getBegin()).str();
    std::string source_text = get_spelling_text(stmt->getSourceRange(), sm);
    int line = sm.getExpansionLineNumber(stmt->getSourceRange().getBegin());
    int col = sm.getExpansionColumnNumber(stmt->getSourceRange().getBegin());

    return fmt::format("{} - {}:{}\n{}", filename, line, col, source_text);
}

std::string get_source_and_location(clang::Decl const* decl,
                                    clang::SourceManager const& sm) {
    std::string filename =
        sm.getFilename(decl->getSourceRange().getBegin()).str();
    std::string source_text = get_spelling_text(decl->getSourceRange(), sm);
    int line = sm.getExpansionLineNumber(decl->getSourceRange().getBegin());
    int col = sm.getExpansionColumnNumber(decl->getSourceRange().getBegin());

    return fmt::format("{} - {}:{}\n{}", filename, line, col, source_text);
}

std::string location_to_string(clang::Decl const* decl,
                               clang::SourceManager const& sm) {
    std::string filename =
        sm.getFilename(decl->getSourceRange().getBegin()).str();
    int line = sm.getExpansionLineNumber(decl->getSourceRange().getBegin());
    int col = sm.getExpansionColumnNumber(decl->getSourceRange().getBegin());

    return fmt::format("{} - {}:{}", filename, line, col);
}

std::string get_filename(clang::NamedDecl const* decl,
                         clang::SourceManager const& sm) {
    
    std::string filename = sm.getFilename(decl->getSourceRange().getBegin()).str();
    return filename;
}

std::string get_spelling_text(clang::SourceRange range,
                              const clang::SourceManager& sm) {
    clang::LangOptions lo;

    // NOTE: sm.getSpellingLoc() used in case the range corresponds to a
    // macro/preprocessed source.
    auto start_loc = sm.getExpansionLoc(range.getBegin());
    auto last_token_loc = sm.getExpansionLoc(range.getEnd());
    auto end_loc = clang::Lexer::getLocForEndOfToken(last_token_loc, 0, sm, lo);
    auto printable_range = clang::SourceRange{start_loc, end_loc};
    return get_source_text_raw(printable_range, sm);
}

auto is_in_std_namespace(clang::DeclContext const* dc) -> bool {
    if (dc == nullptr) {
        return false;
    }

    if (auto const* nsd = dyn_cast<clang::NamespaceDecl>(dc)) {
        if (nsd->getNameAsString() == "std") {
            return true;
        }
    }

    return is_in_std_namespace(dc->getParent());
}

auto is_in_namespace(clang::DeclContext const* dc, std::string const& name)
    -> bool {
    if (dc == nullptr) {
        return false;
    }

    if (auto const* nsd = dyn_cast<clang::NamespaceDecl>(dc)) {
        if (nsd->getNameAsString() == name) {
            return true;
        }
    }

    return is_in_namespace(dc->getParent(), name);
}

std::string get_mangled_name(clang::NamedDecl const* nd,
                             clang::MangleContext* ctx) {
    if (ctx->shouldMangleDeclName(nd)) {
        std::string source = get_source_and_location(nd, nd->getASTContext().getSourceManager());
        std::string kind = nd->getDeclKindName();
        std::string qname = nd->getQualifiedNameAsString();
        std::string mangled_name;
        llvm::raw_string_ostream ostr(mangled_name);
        ctx->mangleName(nd, ostr);
        ostr.flush();
        if (mangled_name.empty()) {
            return nd->getQualifiedNameAsString().c_str();
        } else {
            return mangled_name;
        }
    } else {
        return nd->getQualifiedNameAsString();
    }
}

auto create_mangle_context(clang::ASTContext& ast_context)
    -> std::unique_ptr<clang::MangleContext> {
    // Use the microsoft mangling always for consistency
    return std::unique_ptr<clang::MangleContext>(
        clang::MicrosoftMangleContext::create(ast_context,
                                              ast_context.getDiagnostics()));
}

// Use clang's pretty printing to get a usable "spelling" for a given
// expression. We use this instead of grabbing the actual source text because
// navigating the complexities of the preprocessor for macro expansion is too
// hard. The resulting name will not necessarily be exactly what the user wrote,
// but should work fine to feed back to compile, which is what we need
auto expr_to_string(clang::Expr const* expr, clang::ASTContext* ctx)
    -> std::string {
    static clang::PrintingPolicy print_policy(ctx->getLangOpts());
    // print_policy.FullyQualifiedName = 1;
    // print_policy.SuppressScope = 0;
    // print_policy.SuppressSpecifiers = 0;
    // print_policy.SuppressElaboration = 0;
    // print_policy.SuppressInitializers = 0;
    // print_policy.PrintCanonicalTypes = 1;
    // print_policy.SuppressTemplateArgsInCXXConstructors = 0;
    // print_policy.SuppressDefaultTemplateArgs = 0;

    std::string expr_string;
    llvm::raw_string_ostream stream(expr_string);
    expr->printPretty(stream, nullptr, print_policy);
    stream.flush();
    return expr_string;
}

auto decl_to_string(clang::Decl const* decl, clang::ASTContext* ctx)
    -> std::string {
    static clang::PrintingPolicy print_policy(ctx->getLangOpts());
    print_policy.FullyQualifiedName = 1;
    // print_policy.SuppressScope = 0;
    // print_policy.SuppressSpecifiers = 0;
    // print_policy.SuppressElaboration = 0;
    // print_policy.SuppressInitializers = 0;
    // print_policy.PrintCanonicalTypes = 1;
    // print_policy.SuppressTemplateArgsInCXXConstructors = 0;
    // print_policy.SuppressDefaultTemplateArgs = 0;

    std::string expr_string;
    llvm::raw_string_ostream stream(expr_string);
    decl->print(stream, print_policy);
    stream.flush();
    return expr_string;
}
