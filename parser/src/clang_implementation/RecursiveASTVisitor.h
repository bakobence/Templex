#pragma once

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <llvm/ADT/StringRef.h>

#include "common/cache/TypeCache.h"

namespace templex {
namespace parser {

class RecursiveASTVisitor : public clang::RecursiveASTVisitor<RecursiveASTVisitor> {
public:
    RecursiveASTVisitor(clang::SourceManager& sourceManager);

    bool shouldVisitTemplateInstantiations() const { return true; }

    bool VisitClassTemplateDecl(clang::ClassTemplateDecl* D);
    bool VisitFunctionTemplateDecl(clang::FunctionTemplateDecl* D);

    bool
    VisitClassTemplateSpecializationDecl(clang::ClassTemplateSpecializationDecl* D);

private:
    std::string getDeclLocation(clang::SourceLocation location) const;
    std::string sanitizeType(clang::QualType type) const;
    std::string makeFunctionTemplateId(model::TemplatePtr functionTemplate,
                                       clang::FunctionDecl* D) const;

    void populateTemplateWithParams(model::TemplatePtr templateModel,
                                    clang::TemplateDecl* D);

private:
    clang::SourceManager& sourceManager_;
};

} // namespace parser
} // namespace templex
