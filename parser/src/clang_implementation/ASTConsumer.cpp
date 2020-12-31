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
}