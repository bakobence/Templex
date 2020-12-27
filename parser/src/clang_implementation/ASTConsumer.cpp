#include "ASTConsumer.h"

using namespace templex;
using namespace templex::parser;

ASTConsumer::ASTConsumer(clang::SourceManager& sourceManager)
    : visitor_(sourceManager)
{
}

void ASTConsumer::HandleTranslationUnit(clang::ASTContext& context)
{
    visitor_.TraverseDecl(context.getTranslationUnitDecl());
    /*
    for (const auto& decl : context.getTranslationUnitDecl()->decls()) {
        if (!context.getSourceManager().isInSystemHeader(decl->getLocation())) {
            visitor_.TraverseDecl(decl);
        }
    }
    */
}