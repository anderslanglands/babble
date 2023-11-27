#include "argparse.hpp"
#include "astutil.hpp"

#include <spdlog/spdlog.h>

#include <clang/AST/Mangle.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <stdio.h>
#include <string>
#include <vector>

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;
using llvm::dyn_cast;

enum Result {
    SUCCESS = 0,
    ERROR_FAILED_TO_PARSE_ARGUMENTS = 1,
};

auto get_operator_rename(std::string const& name) -> std::string {
    if (name.rfind("operator=") != std::string::npos) {
        return "\"op_assign\"";
    } else if (name.rfind("operator==") != std::string::npos) {
        return "\"op_eq\"";
    } else if (name.rfind("operator<") != std::string::npos) {
        return "\"op_lt\"";
    } else if (name.rfind("operator<=") != std::string::npos) {
        return "\"op_lte\"";
    } else if (name.rfind("operator>") != std::string::npos) {
        return "\"op_gt\"";
    } else if (name.rfind("operator>=") != std::string::npos) {
        return "\"op_gte\"";
    } else if (name.rfind("operator+") != std::string::npos) {
        return "\"op_add\"";
    } else if (name.rfind("operator+=") != std::string::npos) {
        return "\"op_add_assign\"";
    } else if (name.rfind("operator-") != std::string::npos) {
        return "\"op_sub\"";
    } else if (name.rfind("operator-=") != std::string::npos) {
        return "\"op_sub_assign\"";
    } else if (name.rfind("operator*") != std::string::npos) {
        return "\"op_mul\"";
    } else if (name.rfind("operator*=") != std::string::npos) {
        return "\"op_mul_assign\"";
    } else if (name.rfind("operator/") != std::string::npos) {
        return "\"op_div\"";
    } else if (name.rfind("operator/=") != std::string::npos) {
        return "\"op_div_assign\"";
    } else if (name.rfind("operator^") != std::string::npos) {
        return "\"op_xor\"";
    } else if (name.rfind("operator^=") != std::string::npos) {
        return "\"op_xor_assign\"";
    } else if (name.rfind("operator!") != std::string::npos) {
        return "\"op_not\"";
    } else if (name.rfind("operator!=") != std::string::npos) {
        return "\"op_not_assign\"";
    } else if (name.rfind("operator|") != std::string::npos) {
        return "\"op_bitor\"";
    } else if (name.rfind("operator|=") != std::string::npos) {
        return "\"op_bitor_assign\"";
    } else if (name.rfind("operator&") != std::string::npos) {
        return "\"op_bitand\"";
    } else if (name.rfind("operator&=") != std::string::npos) {
        return "\"op_bit_assign\"";
    } else if (name.rfind("operator||") != std::string::npos) {
        return "\"op_or\"";
    } else if (name.rfind("operator||=") != std::string::npos) {
        return "\"op_or_assign\"";
    } else if (name.rfind("operator&&") != std::string::npos) {
        return "\"op_and\"";
    } else if (name.rfind("operator&&=") != std::string::npos) {
        return "\"op_and_assign\"";
    } else if (name.rfind("operator<<") != std::string::npos) {
        return "\"op_lshift\"";
    } else if (name.rfind("operator<<=") != std::string::npos) {
        return "\"op_lshift_assign\"";
    } else if (name.rfind("operator>>") != std::string::npos) {
        return "\"op_rshift\"";
    } else if (name.rfind("operator>>=") != std::string::npos) {
        return "\"op_rshift_assign\"";
    }

    return "";
}

class GenBind : public MatchFinder::MatchCallback {
    std::string _result;

public:
    auto result() const -> std::string const& { return _result; }

    virtual void run(MatchFinder::MatchResult const& match_result) {
        std::unique_ptr<MangleContext> mangle_context =
            create_mangle_context(*match_result.Context);
        if (CXXRecordDecl const* crd =
                match_result.Nodes.getNodeAs<CXXRecordDecl>("crd")) {

            // we only want to get the definition of the class, not fwd decls
            // etc
            if (!crd->isThisDeclarationADefinition()) {
                return;
            }

            _result = fmt::format("{}    bbl::Class<{}>()\n",
                                  _result,
                                  crd->getQualifiedNameAsString());

            size_t method_count = 0;
            size_t num_ctors = 0;
            for (auto const& method : crd->methods()) {
                if (auto const* dtor = dyn_cast<CXXDestructorDecl>(method)) {
                    continue;
                }

                if (auto const* ctor_decl =
                        dyn_cast<CXXConstructorDecl>(method)) {
                    if (ctor_decl->isCopyOrMoveConstructor()) {
                        continue;
                    }

                    _result = fmt::format("{}        .ctor(bbl::Ctor<{}",
                                          _result,
                                          crd->getQualifiedNameAsString());

                    for (auto const& param : ctor_decl->parameters()) {
                        _result = fmt::format(
                            "{}, {}", _result, param->getType().getAsString());
                    }
                    _result = fmt::format("{}>(", _result);

                    bool first = true;
                    size_t num_params = 0;
                    for (auto const& param : ctor_decl->parameters()) {
                        if (first) {
                            first = false;
                        } else {
                            _result = fmt::format("{}, ", _result);
                        }
                        _result =
                            fmt::format("{}\"{}\"", _result, param->getName());
                        num_params++;
                    }
                    _result = fmt::format("{})", _result);

                    if (num_params == 0) {
                        _result = fmt::format("{}, \"default\")\n", _result);
                    } else {
                        _result =
                            fmt::format("{}, \"{}\")\n",
                                        _result,
                                        fmt::format("ctor{:02}", num_ctors));
                    }
                    num_ctors++;
                } else {
                    _result = fmt::format("{}        .m(&{}",
                                          _result,
                                          method->getQualifiedNameAsString());

                    std::string rename = get_operator_rename(method->getQualifiedNameAsString());
                    if (!rename.empty()) {
                        _result = fmt::format("{}, {}", _result, rename);
                    }

                    _result = fmt::format("{})\n", _result);
                }
            }

            _result = fmt::format("{}    ;\n\n", _result);
        } else if (EnumDecl const* ed =
                       match_result.Nodes.getNodeAs<EnumDecl>("ed")) {
            _result = fmt::format("{}    bbl::Enum<{}>();\n\n", _result, ed->getQualifiedNameAsString());
        } else if (FunctionDecl const* fd =
                       match_result.Nodes.getNodeAs<FunctionDecl>("fd")) {
            _result = fmt::format("{}    bbl::fn(&{}", _result, fd->getQualifiedNameAsString());

            std::string rename = get_operator_rename(fd->getQualifiedNameAsString());
            if (!rename.empty()) {
                _result = fmt::format("{}, \"{}\"", _result, rename);
            }
            _result = fmt::format("{});\n\n", _result);
        }
    }
};

llvm::cl::OptionCategory option_category("bbl::Context options");

int main(int argc, char const** argv) {
    std::vector<char const*> bbl_opts;
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

    bbl_opts.insert(bbl_opts.begin(), argv[0]);
    int bbl_argc = bbl_opts.size();
    argparse::ArgumentParser bblargs("bbl-translate");
    bblargs.add_argument("project-name")
        .help("Name of the project. Will determine the naming of generated "
              "source files");

    bblargs.add_argument("output-directory")
        .help("Directory where the generated project will be written");

    bblargs.add_argument("namespace").help("Namespace to scrape bindings from");

    try {
        bblargs.parse_args(bbl_argc, bbl_opts.data());
    } catch (std::exception& e) {
        SPDLOG_ERROR("failed to parse babble command-line arguments");
        SPDLOG_ERROR(e.what());
        return ERROR_FAILED_TO_PARSE_ARGUMENTS;
    }

    auto project_name = bblargs.get<std::string>("project-name");
    auto output_directory = bblargs.get<std::string>("output-directory");
    auto ns = bblargs.get<std::string>("namespace");

    llvm::Expected<clang::tooling::CommonOptionsParser> opt =
        clang::tooling::CommonOptionsParser::create(
            clang_arg_count, argv, option_category);
    if (!opt) {
        SPDLOG_ERROR("clang could not parse options\n");
        return ERROR_FAILED_TO_PARSE_ARGUMENTS;
    }

    ClangTool tool(opt->getCompilations(), opt->getSourcePathList());

    GenBind gen_bind;
    MatchFinder finder;

    finder.addMatcher(
        cxxRecordDecl(hasAncestor(namespaceDecl(hasName(ns)))).bind("crd"),
        &gen_bind);

    finder.addMatcher(
        enumDecl(hasAncestor(namespaceDecl(hasName(ns)))).bind("ed"),
        &gen_bind);

    finder.addMatcher(
        functionDecl(allOf(hasAncestor(namespaceDecl(hasName(ns))),
                           unless(hasAncestor(recordDecl()))))
            .bind("fd"),
        &gen_bind);

    tool.run(newFrontendActionFactory(&finder).get());

    SPDLOG_INFO("binding:\n{}", gen_bind.result());

    return SUCCESS;
}