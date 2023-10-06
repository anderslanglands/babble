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

using llvm::dyn_cast;

std::string get_source_text_raw(clang::SourceRange range,
                                const clang::SourceManager& sm) {
    return clang::Lexer::getSourceText(
               clang::CharSourceRange::getCharRange(range), sm,
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

auto is_in_namespace(clang::DeclContext const* dc, std::string const& name) -> bool {
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

std::string get_mangled_name(clang::NamedDecl const* nd, clang::MangleContext* ctx) {
    if (ctx->shouldMangleDeclName(nd)) {
        std::string mangled_name;
        llvm::raw_string_ostream ostr(mangled_name);
        ctx->mangleName(nd, ostr);
        ostr.flush();
        return mangled_name;
    } else {
        return nd->getQualifiedNameAsString();
    }
}
