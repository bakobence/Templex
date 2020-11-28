#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "clang_implementation/ASTFrontendAction.h"
#include "util/TypeCache.h"

namespace cl = llvm::cl;

using namespace templex;
using namespace templex::parser;

cl::OptionCategory Category("parser category");

cl::opt<std::string>
    BuildPath("b",
              cl::value_desc("compile_commands.json"),
              cl::desc("Path to the compilation database (compile_commands.json)"),
              cl::Optional);

static cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);

auto main(int argc, const char** argv) -> int
{
    std::string error;
    std::unique_ptr<clang::tooling::CompilationDatabase> cd =
        clang::tooling::CompilationDatabase::autoDetectFromDirectory(argv[2], error);

    clang::tooling::ClangTool tool(*cd, cd->getAllFiles());

    llvm::outs() << "Starting the parser...\n\n";

    int result = tool.run(clang::tooling::newFrontendActionFactory<
                              templex::parser::ASTFrontendAction>()
                              .get());

    TypeCache::getInstance().dump();
    TypeCache::getInstance().exportJSON();

    return result;
}