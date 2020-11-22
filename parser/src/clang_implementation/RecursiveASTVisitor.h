#pragma once

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <llvm/ADT/StringRef.h>

#include "util/TypeCache.h"

namespace templex {
namespace parser {

class RecursiveASTVisitor : public clang::RecursiveASTVisitor<RecursiveASTVisitor> {
public:
    RecursiveASTVisitor(clang::SourceManager& sourceManager);

    bool shouldVisitTemplateInstantiations() const { return true; }

    bool VisitClassTemplateDecl(clang::ClassTemplateDecl* D);

    bool VisitClassTemplateSpecializationDecl(clang::ClassTemplateSpecializationDecl* D);

private:
    std::string getDeclLocation(clang::SourceLocation location) const;

private:
    clang::SourceManager& sourceManager_;
};

} // namespace parser
} // namespace templex
