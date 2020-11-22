#pragma once

#include <clang/Frontend/FrontendAction.h>

namespace templex {
namespace parser {

class ASTFrontendAction : public clang::ASTFrontendAction {
protected:
    std::unique_ptr<clang::ASTConsumer>
    CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef file) override;
};

} // namespace parser
} // namespace templex