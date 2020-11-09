#include <clang/Frontend/CompilerInstance.h>

#include "ASTFrontendAction.h"
#include "ASTConsumer.h"

using namespace templex;

std::unique_ptr<clang::ASTConsumer>
parser::ASTFrontendAction::CreateASTConsumer(clang::CompilerInstance& compiler,
                                             llvm::StringRef file)
{
    return std::unique_ptr<clang::ASTConsumer>(new parser::ASTConsumer(compiler.getSourceManager()));
}