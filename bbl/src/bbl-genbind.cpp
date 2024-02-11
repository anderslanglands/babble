#include "argparse.hpp"
#include "astutil.hpp"
#include "unordered_dense.h"

#include <spdlog/spdlog.h>

#include <clang/AST/Mangle.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <filesystem>
#include <fstream>
#include <regex>
#include <set>
#include <stdio.h>
#include <string>
#include <vector>

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;
using llvm::dyn_cast;

namespace fs = std::filesystem;

enum Result {
    SUCCESS = 0,
    ERROR_FAILED_TO_PARSE_ARGUMENTS,
    ERROR_AST_GENERATION_FAILED,
    ERROR_COMPILATION_FAILED,
    ERROR_NO_COMMON_PREFIX,
    ERROR_FILE_OPEN,
};

static auto get_operator_rename(std::string const& name) -> std::string;

auto get_function_cast_string(FunctionDecl const* fd,
                              std::string const& scope,
                              bool is_const,
                              PrintingPolicy const& ppolicy) -> std::string;

llvm::cl::OptionCategory option_category("bbl::Context options");

using CRDMap = ankerl::unordered_dense::map<std::string, CXXRecordDecl const*>;
using CTDMap =
    ankerl::unordered_dense::map<std::string, ClassTemplateDecl const*>;
using FDMap = ankerl::unordered_dense::map<std::string, FunctionDecl const*>;
using FTDMap =
    ankerl::unordered_dense::map<std::string, FunctionTemplateDecl const*>;

auto find_all_class_decls(clang::DeclContext const* dc,
                          CRDMap& classes,
                          CTDMap& class_templates,
                          MangleContext* mangle_context) -> void {
    if (dc == nullptr) {
        return;
    }

    for (auto const* child : dc->decls()) {
        if (auto const* ctpsd =
                dyn_cast<ClassTemplatePartialSpecializationDecl>(child)) {
            /// XXX: Some partial specializations break the mangler. 
            /// we could switch to pointers as keys, but we also need to think
            /// about how we represent them too, or if we do...
            continue;
        } else if (auto const* ctd = dyn_cast<ClassTemplateDecl>(child);
                   ctd != nullptr && ctd->isThisDeclarationADefinition()) {
            std::string mangled_name = get_mangled_name(ctd, mangle_context);
            if (class_templates.find(mangled_name) == class_templates.end()) {
                class_templates.insert(std::pair(mangled_name, ctd));
            }

            find_all_class_decls(ctd->getTemplatedDecl(),
                                 classes,
                                 class_templates,
                                 mangle_context);
        } else if (auto const* crd = dyn_cast<CXXRecordDecl>(child);
                   crd != nullptr && crd->isThisDeclarationADefinition()) {
            std::string mangled_name = get_mangled_name(crd, mangle_context);
            if (classes.find(mangled_name) == classes.end()) {
                classes.insert(std::pair(mangled_name, crd));
            }

            find_all_class_decls(crd, classes, class_templates, mangle_context);
        } else if (auto const* nd = dyn_cast<NamespaceDecl>(child)) {
            find_all_class_decls(nd, classes, class_templates, mangle_context);
        }
    }
}

auto find_all_function_decls(clang::DeclContext const* dc,
                             FDMap& functions,
                             FTDMap& function_templates,
                             MangleContext* mangle_context) -> void {
    if (dc == nullptr) {
        return;
    }

    for (auto const* child : dc->decls()) {
        if (dyn_cast<CXXMethodDecl>(child) ||
            dyn_cast<CXXConstructorDecl>(child) ||
            dyn_cast<CXXDestructorDecl>(child)) {
            continue;
        } else if (auto const* fd = dyn_cast<FunctionDecl>(child)) {
            std::string mangled_name = get_mangled_name(fd, mangle_context);
            if (functions.find(mangled_name) == functions.end()) {
                functions.insert(std::pair(mangled_name, fd));
            }
        } else if (auto const* ftd = dyn_cast<FunctionTemplateDecl>(child)) {
            std::string mangled_name = get_mangled_name(ftd, mangle_context);
            if (function_templates.find(mangled_name) ==
                function_templates.end()) {
                function_templates.insert(std::pair(mangled_name, ftd));
            }
        } else if (auto const* nd = dyn_cast<NamespaceDecl>(child)) {
            find_all_function_decls(
                nd, functions, function_templates, mangle_context);
        }
    }
}

using EDMap = ankerl::unordered_dense::map<std::string, EnumDecl const*>;

auto find_all_enum_decls(clang::DeclContext const* dc,
                         EDMap& enums,
                         MangleContext* mangle_context) -> void {
    if (dc == nullptr) {
        return;
    }

    for (auto const* child : dc->decls()) {
        if (auto const* ed = dyn_cast<EnumDecl>(child)) {
            std::string mangled_name = get_mangled_name(ed, mangle_context);
            if (enums.find(mangled_name) == enums.end()) {
                enums.insert(std::pair(mangled_name, ed));
            }
        } else if (auto const* nd = dyn_cast<NamespaceDecl>(child)) {
            find_all_enum_decls(nd, enums, mangle_context);
        } else if (auto const* crd = dyn_cast<CXXRecordDecl>(child)) {
            find_all_enum_decls(crd, enums, mangle_context);
        }
    }
}

class NamespaceVisitor : public clang::RecursiveASTVisitor<NamespaceVisitor> {
    ASTContext* _ast_context;
    SourceManager& _sm;
    std::unique_ptr<clang::MangleContext> _mangle_context;
    std::string _ns;

    CRDMap& _class_decls;
    CTDMap& _class_template_decls;
    FDMap& _function_decls;
    FTDMap& _function_template_decls;
    EDMap& _enum_decls;

public:
    explicit NamespaceVisitor(clang::ASTContext* ast_context,
                              std::string const& ns,
                              CRDMap& class_decls,
                              CTDMap& class_template_decls,
                              FDMap& function_decls,
                              FTDMap& function_template_decls,
                              EDMap& enum_decls)
        : _ast_context(ast_context), _sm(ast_context->getSourceManager()),
          _mangle_context(create_mangle_context(*ast_context)), _ns(ns),
          _class_decls(class_decls),
          _class_template_decls(class_template_decls),
          _function_decls(function_decls),
          _function_template_decls(function_template_decls),
          _enum_decls(enum_decls) {}

    auto VisitNamespaceDecl(NamespaceDecl* nd) -> bool {

        if (nd->getQualifiedNameAsString() == _ns) {
            // SPDLOG_INFO("Got namespace {}", _ns);
        } else {
            return true;
        }

        find_all_class_decls(
            nd, _class_decls, _class_template_decls, _mangle_context.get());

        find_all_function_decls(nd,
                                _function_decls,
                                _function_template_decls,
                                _mangle_context.get());

        find_all_enum_decls(nd, _enum_decls, _mangle_context.get());

        return true;
    }
};

struct Function {
    std::string filename;
    std::string name;
    std::string qualified_name;
    std::string cast;
    std::string rename;
    bool is_template;
};

using FunctionMap =
    ankerl::unordered_dense::map<std::string, std::vector<Function>>;

struct Method : public Function {
    bool is_const;
    bool is_static;
};

using MethodMap =
    ankerl::unordered_dense::map<std::string, std::vector<Method>>;

struct Constructor {
    std::string types;
    std::string names;
    bool is_default;
};

struct Class {
    std::string filename;
    std::string name;
    std::string qualified_name;
    MethodMap methods;
    std::vector<Constructor> ctors;
    std::vector<std::string> fields;
    bool is_template;
};

struct Enum {
    std::string filename;
    std::string qualified_name;
};

auto create_method_from_cmd(CXXMethodDecl const* cmd,
                            bool is_template,
                            CXXRecordDecl const* crd,
                            PrintingPolicy const& ppolicy) -> Method {
    Method method;
    method.is_template = is_template;
    method.name = cmd->getNameAsString();
    method.qualified_name = cmd->getQualifiedNameAsString();
    method.rename = get_operator_rename(method.qualified_name);
    method.is_const = cmd->isConst();
    method.is_static = cmd->isStatic();

    std::string scope =
        method.is_static ? ""
                         : fmt::format("{}::", crd->getQualifiedNameAsString());

    method.cast =
        get_function_cast_string(cmd, scope, method.is_const, ppolicy);

    return method;
}

auto create_constructor_from_ccd(CXXConstructorDecl const* ccd,
                                 CXXRecordDecl const* crd,
                                 PrintingPolicy const& ppolicy) -> Constructor {

    std::string types;
    std::string names;
    bool first = true;
    int param_index = 0;
    for (auto const& param : ccd->parameters()) {
        if (first) {
            first = false;
        } else {
            names = fmt::format("{}, ", names);
            types = fmt::format("{}, ", types);
        }
        types = fmt::format("{}{}", types, param->getType().getAsString());

        std::string param_name = param->getNameAsString();
        names = param_name.empty() ? fmt::format("{}\"param_{:02}\"", names, param_index)
                                   : fmt::format("{}\"{}\"", names, param_name);

        param_index++;
    }

    return {types, names, param_index == 0};
}

// Grab methods, fields etc from this CXXRecordDecl and store them in a new
// Class object
auto create_class_from_crd(CXXRecordDecl const* crd,
                           PrintingPolicy const& ppolicy) -> Class {
    Class cls;
    cls.filename = get_filename(crd, crd->getASTContext().getSourceManager());
    cls.name = crd->getNameAsString();
    cls.qualified_name = crd->getQualifiedNameAsString();
    cls.is_template = false;

    // iterate over all children of the decl context to pluck out methods
    for (Decl const* child : crd->decls()) {
        if (CXXConstructorDecl const* ccd =
                dyn_cast<CXXConstructorDecl>(child)) {
            if (ccd->isDeleted() || ccd->getAccess() != clang::AS_public ||
                ccd->isCopyOrMoveConstructor()) {
                continue;
            }

            Constructor ctor = create_constructor_from_ccd(ccd, crd, ppolicy);
            cls.ctors.emplace_back(std::move(ctor));
        } else if (auto const* cdd = dyn_cast<CXXDestructorDecl>(child)) {
            continue;
        } else if (auto const* cmd = dyn_cast<CXXMethodDecl>(child)) {
            if (cmd->isDeleted() || cmd->getAccess() != clang::AS_public) {
                continue;
            }

            Method method = create_method_from_cmd(cmd, false, crd, ppolicy);
            cls.methods[method.qualified_name].emplace_back(std::move(method));
        } else if (auto const* ftd = dyn_cast<FunctionTemplateDecl>(child)) {
            if (auto const* cmd =
                    dyn_cast_or_null<CXXMethodDecl>(ftd->getTemplatedDecl())) {
                Method method = create_method_from_cmd(cmd, true, crd, ppolicy);
                cls.methods[method.qualified_name].emplace_back(
                    std::move(method));
            } else {
                SPDLOG_ERROR("could not cast ftd to cmd {}",
                             ftd->getQualifiedNameAsString());
            }
        } else if (auto const* fd = dyn_cast<FieldDecl>(child);
                   fd != nullptr && crd->isTrivial()) {
            cls.fields.emplace_back(fd->getQualifiedNameAsString());
        }
    }
    return cls;
}

auto longest_common_prefix(std::set<std::string> const& set) -> std::string {
    // set is already sorted, so we can do it this way
    std::string const& first = *set.begin();
    std::string const& last = *set.rbegin();

    size_t min_length = std::min(first.size(), last.size());

    size_t i = 0;
    while (i < min_length && first[i] == last[i]) {
        i++;
    }

    std::string lcp = first.substr(0, i);
    size_t last_fslash = lcp.rfind("/");
    size_t last_bslash = lcp.rfind("\\");

    if (last_fslash != std::string::npos) {
        if (last_bslash != std::string::npos) {
            return lcp.substr(0, std::max(last_fslash, last_bslash) + 1);
        } else {
            return lcp.substr(0, last_fslash + 1);
        }
    } else if (last_bslash != std::string::npos) {
        return lcp.substr(0, last_bslash + 1);
    } else {
        return "";
    }
}

struct BindFile {
    fs::path path;
    std::string inclusions;
    std::string content;
};

auto write_class_to_string(Class const& cls) -> std::string {
    std::string result;
    if (cls.is_template) {
        result =
            fmt::format("{}#if 0\n    /// TODO: instantiate this template\n", result);
    }

    result =
        fmt::format("{}    bbl::Class<{}>()\n", result, cls.qualified_name);

    if (!cls.fields.empty()) {
        result = fmt::format("{}        .value_type()\n", result);
    }
    for (std::string const& field : cls.fields) {
        result = fmt::format("{}        .f(&{})\n", result, field);
    }

    int ctor_index = 0;
    for (Constructor const& ctor : cls.ctors) {
        std::string rename_str =
            ctor.is_default ? "default" : fmt::format("ctor_{:02}", ctor_index);

        result = fmt::format("{}        .ctor(bbl::Class<{}>::Ctor<{}>({}), \"{}\")\n",
                             result,
                             cls.qualified_name,
                             ctor.types,
                             ctor.names,
                             rename_str);

        ctor_index++;
    }

    for (auto const& [qname, overloads] : cls.methods) {
        int overload_index = 0;
        for (Method const& method : overloads) {

            if (method.is_template) {
                result = fmt::format(
                    "{}        /** TODO: instantiate this template\n", result);
            }
            if (overloads.size() > 1) {
                std::string rename_str =
                    method.rename.empty()
                        ? fmt::format(
                              ", \"{}_{:02}\"", method.name, overload_index)
                        : fmt::format(
                              ", \"{}_{:02}\"", method.rename, overload_index);

                // overloaded functions must be cast
                result = fmt::format("{}        .m({}\n            &{}{})\n",
                                     result,
                                     method.cast,
                                     method.qualified_name,
                                     rename_str);

            } else {
                std::string rename_str =
                    method.rename.empty()
                        ? std::string()
                        : fmt::format(", \"{}\"", method.rename);

                result = fmt::format("{}        .m(&{}{})\n",
                                     result,
                                     method.qualified_name,
                                     rename_str);
            }

            if (method.is_template) {
                result = fmt::format("{}        */\n", result);
            }

            overload_index++;
        }
    }
    result = fmt::format("{}    ;\n", result);

    if (cls.is_template) {
        result = fmt::format("{}#endif\n", result);
    }

    return result;
}

auto write_function_to_string(Function const& function,
                              size_t num_overloads,
                              int overload_index) -> std::string {
    std::string result;
    if (function.is_template) {
        result =
            fmt::format("{}    /** TODO: instantiate this template\n", result);
    }
    if (num_overloads > 1) {
        std::string rename_str =
            function.rename.empty()
                ? fmt::format(", \"{}_{:02}\"", function.name, overload_index)
                : fmt::format(
                      ", \"{}_{:02}\"", function.rename, overload_index);

        // overloaded functions must be cast
        result = fmt::format("{}    bbl::fn({}\n            &{}{});\n",
                             result,
                             function.cast,
                             function.qualified_name,
                             rename_str);

    } else {
        std::string rename_str = function.rename.empty()
                                     ? std::string()
                                     : fmt::format(", \"{}\"", function.rename);

        result = fmt::format("{}    bbl::fn(&{}{});\n",
                             result,
                             function.qualified_name,
                             rename_str);
    }
    if (function.is_template) {
        result = fmt::format("{}    */\n", result);
    }

    return result;
}

auto write_enum_to_string(Enum const& enm) -> std::string {
    return fmt::format("    bbl::Enum<{}>();\n", enm.qualified_name);
}

static auto replace_all(std::string subject, std::string find, std::string replace)
    -> std::string {
    size_t pos = 0;
    while ((pos = subject.find(find, pos)) != std::string::npos) {
        subject.replace(pos, find.length(), replace);
        pos += replace.length();
    }
    return subject;
}

int main(int argc, char const** argv) {
    // grab the babble-specific args from after the last `--` and mark where the
    // regular clang args end
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

    // parse the babble args
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

    // pass the stripped args to clang
    llvm::Expected<clang::tooling::CommonOptionsParser> opt =
        clang::tooling::CommonOptionsParser::create(
            clang_arg_count, argv, option_category);
    if (!opt) {
        SPDLOG_ERROR("clang could not parse options\n");
        return ERROR_FAILED_TO_PARSE_ARGUMENTS;
    }

    // create the tool
    ClangTool tool(opt->getCompilations(), opt->getSourcePathList());

    // Extract the include directives from each source given
    std::regex re_inclusion(R"(^\s*#include\s*([<"])([\w\-\./]+)[>"])");
    std::string inclusions = "#include <babble>\n\n";
    for (auto const& source_path : tool.getSourcePaths()) {
        std::string absolute_path =
            std::filesystem::absolute(source_path).string();

        /// XXX: There's definitely a way to do this with the
        /// preprocessor, but I can't figure out how to invoke it
        /// correctly in our context. This will do for now, but note
        /// that it will fail if the user #if's out any includes (it
        /// will still include them)
        std::ifstream infile(absolute_path);

        std::string line;
        while (std::getline(infile, line)) {
            std::smatch matches;
            if (std::regex_search(line, matches, re_inclusion)) {
                assert(matches.size() == 3);
                // XXX: This is probably brittle...
                if (matches[2].str().find("babble") == std::string::npos) {
                    inclusions =
                        fmt::format("{}{}\n", inclusions, matches[0].str());
                }
            }
        }
    }

    // build the ASTUnits we'll use to grab the decls
    std::vector<std::unique_ptr<clang::ASTUnit>> ast_units;
    tool.buildASTs(ast_units);

    if (ast_units.empty()) {
        // oops something went very badly wrong here
        SPDLOG_ERROR("no AST units generated");
        return ERROR_AST_GENERATION_FAILED;
    }

    int num_errors = 0;
    for (auto const& ast : ast_units) {
        num_errors += ast->getDiagnostics().getNumErrors();
    }

    if (num_errors > 0) {
        SPDLOG_ERROR("compilation generated {} errors. Cannot continue",
                     num_errors);
        return ERROR_COMPILATION_FAILED;
    }

    // Visit each instance of the given namespace in the AST units and pluck out
    // all relevant decls
    CRDMap class_decls;
    CTDMap class_template_decls;
    FDMap function_decls;
    FTDMap function_template_decls;
    EDMap enum_decls;
    for (auto const& unit : ast_units) {
        clang::ASTContext& ast_ctx = unit->getASTContext();
        NamespaceVisitor visitor(&ast_ctx,
                                 ns,
                                 class_decls,
                                 class_template_decls,
                                 function_decls,
                                 function_template_decls,
                                 enum_decls);
        visitor.TraverseAST(ast_ctx);
    }

    std::string result;
    std::set<std::string> decl_filenames;

    // we create this to get better string representations for QTypes, but it
    // doesn't seem to have any effect, unfortunately...
    /// TODO: figure out why
    PrintingPolicy ppolicy(ast_units[0]->getASTContext().getLangOpts());

    // Create a new Class from each CXXRecordDecl we found
    std::vector<Class> classes;
    for (auto const& [mangled_name, crd] : class_decls.values()) {
        Class cls = create_class_from_crd(crd, ppolicy);
        if (!cls.filename.empty()) {
            decl_filenames.insert(cls.filename);
        }
        classes.emplace_back(std::move(cls));
    }

    // Create a new Class from each ClassTemplateDecl we found
    for (auto const& [mangled_name, ctd] : class_template_decls.values()) {
        Class cls = create_class_from_crd(ctd->getTemplatedDecl(), ppolicy);
        cls.is_template = true;
        if (!cls.filename.empty()) {
            decl_filenames.insert(cls.filename);
        }
        classes.emplace_back(std::move(cls));
    }

    // For each created class, write its binding to the result string
    for (Class const& cls : classes) {
        result = fmt::format("{}{}\n", result, write_class_to_string(cls));
    }

    // Grab all relevant info from the FunctionDecls to allow us to create the
    // binding.
    // Each created Function is then pushed on a vector, which is itself in a
    // map keyed by the function's qualified name, thus allowing us to identify
    // overloads
    FunctionMap functions;
    for (auto const& [mangled_name, fd] : function_decls.values()) {
        Function function;
        function.filename =
            get_filename(fd, fd->getASTContext().getSourceManager());
        if (!function.filename.empty()) {
            decl_filenames.insert(function.filename);
        }
        function.name = fd->getNameAsString();
        function.qualified_name = fd->getQualifiedNameAsString();
        function.cast = get_function_cast_string(fd, "", false, ppolicy);
        function.rename = get_operator_rename(function.qualified_name);
        function.is_template = false;

        functions[function.qualified_name].emplace_back(std::move(function));
    }

    for (auto const& [mangled_name, ftd] : function_template_decls.values()) {
        Function function;
        function.filename =
            get_filename(ftd, ftd->getASTContext().getSourceManager());
        if (!function.filename.empty()) {
            decl_filenames.insert(function.filename);
        }
        function.name = ftd->getNameAsString();
        function.qualified_name = ftd->getQualifiedNameAsString();
        function.cast = get_function_cast_string(
            ftd->getTemplatedDecl(), "", false, ppolicy);
        function.rename = get_operator_rename(function.qualified_name);
        function.is_template = true;

        functions[function.qualified_name].emplace_back(std::move(function));
    }

    // For each overload of each function, write out its binding
    for (auto const& [qname, overloads] : functions) {
        int overload_index = 0;
        for (Function const& function : overloads) {
            result =
                fmt::format("{}{}\n",
                            result,
                            write_function_to_string(
                                function, overloads.size(), overload_index));
            overload_index++;
        }
    }

    result = fmt::format("{}\n", result);

    std::vector<Enum> enums;
    for (auto const& [mangled_name, ed] : enum_decls) {
        Enum enm;
        enm.filename = get_filename(ed, ed->getASTContext().getSourceManager());
        if (!enm.filename.empty()) {
            decl_filenames.insert(enm.filename);
        }
        enm.qualified_name = ed->getQualifiedNameAsString();
        enums.emplace_back(std::move(enm));
    }

    for (Enum const& enm : enums) {
        result =
            fmt::format("{}    bbl::Enum<{}>();\n", result, enm.qualified_name);
    }

    // wrap everything together to create the final bindfile
    result = fmt::format(
        "{}\nBBL_MODULE({}) {{\n\n{}\n}}\n", inclusions, project_name, result);

    // just write it to stdout for now
    // SPDLOG_INFO("result:\n{}", result);

    // Figure out the common prefix and relative paths of all the header files
    // that contain the decls we've extracted
    std::string prefix = longest_common_prefix(decl_filenames);

    // This seems brittle. It might be possible to have decls not in a common
    // include tree (although unlikely?)
    // we already hit issues where some functions just don't have a filename 
    // associated in the source manager
    if (prefix.empty()) {
        SPDLOG_ERROR("no common prefix on:");
        for (std::string const& filename : decl_filenames) {
            SPDLOG_ERROR("{}", filename);
        }
        return ERROR_NO_COMMON_PREFIX;
    }

    // Convert the paths to .cpp paths relative to the output directory to
    // create out bindfile paths
    ankerl::unordered_dense::map<std::string, BindFile> bindfiles;
    fs::path bindfile_root = fs::path(output_directory) / "bind";
    // insert a "misc" bindfile for the empty decl filename case - for some reason
    // some decls don't give a filename in clang
    /// XXX: figure out why
    bindfiles.insert(std::pair("", BindFile{bindfile_root/"bbl-misc.cpp", inclusions, {}}));
    for (std::string const& absolute_path : decl_filenames) {
        if (absolute_path.find(prefix) != 0) {
            SPDLOG_ERROR(
                "path {} does not contain prefix {}", absolute_path, prefix);
            return ERROR_NO_COMMON_PREFIX;
        }

        fs::path relative_path =
            absolute_path.substr(prefix.size(), std::string::npos);

        fs::path bindfile_path =
            bindfile_root / relative_path.replace_extension(".cpp");

        // insert an empty bindfile for this source file path
        /// XXX: get specific inclusions for each bindfile
        bindfiles.insert(
            std::pair(absolute_path, BindFile{bindfile_path, inclusions, {}}));
    }

    // now iterate over all the extracted objects and add their bindings to the
    // correspoding bindfile based on their source filename
    for (Class const& cls : classes) {
        for (auto& [source_path, bindfile] : bindfiles) {
            if (cls.filename == source_path) {
                bindfile.content = fmt::format(
                    "{}{}\n", bindfile.content, write_class_to_string(cls));
            }
        }
    }

    for (auto const& [qname, overloads] : functions) {
        int overload_index = 0;
        for (Function const& function : overloads) {
            for (auto& [source_path, bindfile] : bindfiles) {
                if (function.filename == source_path) {
                    bindfile.content = fmt::format(
                        "{}{}\n",
                        bindfile.content,
                        write_function_to_string(
                            function, overloads.size(), overload_index));
                }
            }

            overload_index++;
        }
    }

    for (Enum const& enm : enums) {
        for (auto& [source_path, bindfile] : bindfiles) {
            if (enm.filename == source_path) {
                bindfile.content = fmt::format(
                    "{}{}\n", bindfile.content, write_enum_to_string(enm));
            }
        }
    }

    // for each BindFile, finish its content with the module wrapper and write
    // it out, and add it to the CMakeLists.txt
    std::string cmakelists;
    for (auto const& [_, bindfile] : bindfiles) {
        fs::path parent_dir = bindfile.path.parent_path();
        fs::create_directories(parent_dir);

        std::ofstream out_file(bindfile.path);
        if (!out_file.is_open()) {
            SPDLOG_ERROR("could not open {} for writing",
                         bindfile.path.string());
            return ERROR_FILE_OPEN;
        }

        std::string content = fmt::format("{}\nBBL_MODULE({}) {{\n\n{}\n}}\n",
                                          inclusions,
                                          project_name,
                                          bindfile.content);

        out_file << content;
        std::string bindfile_relpath = replace_all(
            fs::relative(bindfile.path, output_directory).string(), "\\", "/");
        cmakelists = fmt::format("{}  {}\n", cmakelists, bindfile_relpath);
    }

    char const* babble_version = "0.5";
    cmakelists = fmt::format(R"(cmake_minimum_required(VERSION 3.15)
project({0} VERSION 0.1 LANGUAGES C CXX)

find_package(babble {1} CONFIG REQUIRED)

include(GNUInstallDirs)

# The bindfile contains all our binding definitions
set(bindfiles 
{2})

bbl_translate_binding(
    {0}
    BINDFILES 
        ${{bindfiles}}
    COMPILE_ARGS 
        -DNOMINMAX 
        -D_MT 
)

# target_link_libraries({0}-c PUBLIC)
# target_compile_definitions({0}-c PRIVATE NOMINMAX)

# This is just here to trigger generation of compile commands for the bind file so we get LSP functionality in the bind file
add_library(bind-dummy ${{bindfiles}})
target_link_libraries(bind-dummy babble::bind)

install(
  TARGETS 
    {0}-c
  LIBRARY 
    DESTINATION ${{CMAKE_INSTALL_LIBDIR}}
  ARCHIVE 
    DESTINATION ${{CMAKE_INSTALL_LIBDIR}}
  RUNTIME 
    DESTINATION ${{CMAKE_INSTALL_BINDIR}}
  INCLUDES 
    DESTINATION ${{CMAKE_INSTALL_INCLUDEDIR}}
)
)",
                             project_name,
                             babble_version,
                             cmakelists);

    fs::path cmakelists_path = fs::path(output_directory) / "CMakeLists.txt";
    std::ofstream out_cmakelists(cmakelists_path);
    if (!out_cmakelists.is_open()) {
        SPDLOG_ERROR("could not open {}", cmakelists_path.string());
        return ERROR_FILE_OPEN;
    }
    out_cmakelists << cmakelists;

    fs::path dummy_path = fs::path(output_directory) / "bbl-dummy.cpp";
    std::ofstream out_dummy(dummy_path);
    out_dummy << "// dummy file to create cmake dependency. ignore\n";


    return SUCCESS;
}

static auto get_operator_rename(std::string const& name) -> std::string {
    if (name.rfind("operator==") != std::string::npos) {
        return "op_eq";
    } else if (name.rfind("operator[]") != std::string::npos) {
        return "op_index";
    } else if (name.rfind("operator=") != std::string::npos) {
        return "op_assign";
    } else if (name.rfind("operator<<=") != std::string::npos) {
        return "op_lshift_assign";
    } else if (name.rfind("operator<<") != std::string::npos) {
        return "op_lshift";
    } else if (name.rfind("operator<=") != std::string::npos) {
        return "op_lte";
    } else if (name.rfind("operator<") != std::string::npos) {
        return "op_lt";
    } else if (name.rfind("operator>>=") != std::string::npos) {
        return "op_rshift_assign";
    } else if (name.rfind("operator>>") != std::string::npos) {
        return "op_rshift";
    } else if (name.rfind("operator>=") != std::string::npos) {
        return "op_gte";
    } else if (name.rfind("operator>") != std::string::npos) {
        return "op_gt";
    } else if (name.rfind("operator++") != std::string::npos) {
        return "op_inc";
    } else if (name.rfind("operator+=") != std::string::npos) {
        return "op_add_assign";
    } else if (name.rfind("operator+") != std::string::npos) {
        return "op_add";
    } else if (name.rfind("operator-=") != std::string::npos) {
        return "op_sub_assign";
    } else if (name.rfind("operator-") != std::string::npos) {
        return "op_sub";
    } else if (name.rfind("operator*=") != std::string::npos) {
        return "op_mul_assign";
    } else if (name.rfind("operator*") != std::string::npos) {
        return "op_mul";
    } else if (name.rfind("operator/=") != std::string::npos) {
        return "op_div_assign";
    } else if (name.rfind("operator/") != std::string::npos) {
        return "op_div";
    } else if (name.rfind("operator^=") != std::string::npos) {
        return "op_xor_assign";
    } else if (name.rfind("operator^") != std::string::npos) {
        return "op_xor";
    } else if (name.rfind("operator!=") != std::string::npos) {
        return "op_neq";
    } else if (name.rfind("operator!") != std::string::npos) {
        return "op_not";
    } else if (name.rfind("operator|=") != std::string::npos) {
        return "op_bitor_assign";
    } else if (name.rfind("operator|") != std::string::npos) {
        return "op_bitor";
    } else if (name.rfind("operator&&=") != std::string::npos) {
        return "op_and_assign";
    } else if (name.rfind("operator&&") != std::string::npos) {
        return "op_and";
    } else if (name.rfind("operator&=") != std::string::npos) {
        return "op_bit_assign";
    } else if (name.rfind("operator&") != std::string::npos) {
        return "op_bitand";
    } else if (name.rfind("operator||=") != std::string::npos) {
        return "op_or_assign";
    } else if (name.rfind("operator||") != std::string::npos) {
        return "op_or";
    } else if (name.rfind("operator bool") != std::string::npos) {
        return "op_bool";
    }

    return "";
}

std::string get_function_cast_string(FunctionDecl const* fd,
                                     std::string const& scope,
                                     bool is_const,
                                     PrintingPolicy const& ppolicy) {
    std::string result =
        fmt::format("({} ", fd->getReturnType().getAsString(ppolicy));
    // static methods are declared like a regular function
    // pointer, regular methods are member function pointers
    result = fmt::format("{}({}*)(", result, scope);

    bool first = true;
    for (auto const& param : fd->parameters()) {
        if (first) {
            first = false;
        } else {
            result = fmt::format("{}, ", result);
        }

        result =
            fmt::format("{}{}", result, param->getType().getAsString(ppolicy));
    }

    char const* s_const = is_const ? " const" : "";

    result = fmt::format("{}){})", result, s_const);

    return result;
}
