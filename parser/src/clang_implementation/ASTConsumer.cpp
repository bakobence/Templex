#include "ASTConsumer.h"

using namespace templex;

void parser::ASTConsumer::HandleTranslationUnit(clang::ASTContext& context)
{
    for (const auto& decl : context.getTranslationUnitDecl()->decls()) {
        if (!context.getSourceManager().isInSystemHeader(decl->getLocation())) {
            visitor_.TraverseDecl(decl);
        }
    }
}