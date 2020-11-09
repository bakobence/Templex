#include "RecursiveASTVisitor.h"

#include <llvm/ADT/Twine.h>
#include <string>

using namespace templex;
using namespace templex::parser;

using super = clang::RecursiveASTVisitor<parser::RecursiveASTVisitor>;

RecursiveASTVisitor::RecursiveASTVisitor(clang::SourceManager& sourceManager)
    : sourceManager_(sourceManager) 
{}

bool RecursiveASTVisitor::VisitClassTemplateDecl(clang::ClassTemplateDecl* D)
{
    /*
    auto classDecl = D->getTemplatedDecl();
    auto className = classDecl->getName();

    llvm::outs() << "Template class: " << className << "\n";

    for (auto* spec : D->specializations()) {
        llvm::outs() << "  Specialization:\n";
        for (auto arg : spec->getTemplateArgs().asArray()) {
            llvm::outs() << "   Kind: " << arg.getKind() << " Value: "
                         << arg.getAsType()->getCanonicalTypeInternal().getAsString()
                         << "\n";
        }
    }
    */

    return true;
}

bool RecursiveASTVisitor::VisitClassTemplateSpecializationDecl(clang::ClassTemplateSpecializationDecl* D) {
    
    auto args = D->getTemplateArgs().asArray();
    
    llvm::outs() << D->getQualifiedNameAsString() << "\n";
    llvm::outs() << getDeclLocation(D->getPointOfInstantiation()) << "\n";
    llvm::outs() << " Types: ";
    for (auto arg : args) {
        llvm::outs() << arg.getAsType()->getCanonicalTypeInternal().getAsString();
    }
    llvm::outs() << "\n";

    return true;
}

std::string RecursiveASTVisitor::getDeclLocation(clang::SourceLocation location) const {

    return llvm::Twine(sourceManager_.getFilename(location)
        + " "
        + llvm::Twine(sourceManager_.getSpellingLineNumber(location))
        + ":"
        + llvm::Twine(sourceManager_.getSpellingColumnNumber(location)))
        .str();
}