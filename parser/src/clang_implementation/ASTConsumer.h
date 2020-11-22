#pragma once

#include <clang/AST/ASTConsumer.h>

#include <memory>

#include "RecursiveASTVisitor.h"

namespace templex {
namespace parser {

class ASTConsumer : public clang::ASTConsumer {
public:
    ASTConsumer(clang::SourceManager& sourceManager);

    void HandleTranslationUnit(clang::ASTContext& context) override;

private:
    RecursiveASTVisitor visitor_;
};

} // namespace parser
} // namespace templex