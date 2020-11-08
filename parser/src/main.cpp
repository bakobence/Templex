#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/Support/CommandLine.h>

#include "clang_implementation/ASTFrontendAction.h"

namespace cl = llvm::cl;

cl::OptionCategory Category("parser category");

cl::opt<std::string>
    BuildPath("b",
              cl::value_desc("compile_commands.json"),
              cl::desc("Path to the compilation database (compile_commands.json)"),
              cl::Optional);

static cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);

auto main(int argc, const char** argv) -> int
{

    // clang::tooling::CommonOptionsParser optionsParser(argc, argv, Category);

    std::string error;
    std::unique_ptr<clang::tooling::CompilationDatabase> cd =
        clang::tooling::CompilationDatabase::autoDetectFromDirectory(argv[2], error);

    clang::tooling::ClangTool tool(*cd, cd->getAllFiles());

    llvm::outs() << "Starting the parser...\n";

    return tool.run(
        clang::tooling::newFrontendActionFactory<templex::parser::ASTFrontendAction>()
            .get());
}