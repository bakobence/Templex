#include "RecursiveASTVisitor.h"

#include <llvm/ADT/Twine.h>
#include <string>

using namespace templex;
using namespace templex::parser;

using super = clang::RecursiveASTVisitor<parser::RecursiveASTVisitor>;

RecursiveASTVisitor::RecursiveASTVisitor(clang::SourceManager& sourceManager)
    : sourceManager_(sourceManager)
{
}

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

bool RecursiveASTVisitor::VisitClassTemplateSpecializationDecl(
    clang::ClassTemplateSpecializationDecl* D)
{
    auto args      = D->getTemplateArgs().asArray();
    auto className = D->getQualifiedNameAsString();
    auto location  = getDeclLocation(D->getPointOfInstantiation());

    llvm::outs() << className << " " << location << "\nTemplate argument list\n";
    for (auto arg : args) {
        llvm::outs() << " [kind] " << arg.getKind();
        if (arg.getKind() == clang::TemplateArgument::ArgKind::Type) {
            llvm::outs() << "  Type: ";
            llvm::outs() << arg.getAsType()->getCanonicalTypeInternal().getAsString();
            llvm::outs() << "\n";
        } else if (arg.getKind() == clang::TemplateArgument::ArgKind::Pack) {
            llvm::outs() << "  Variadic pack\n";
        } else if (arg.getKind() == clang::TemplateArgument::ArgKind::Integral) {
            llvm::outs() << "  Integral\n";
        }
    }
    llvm::outs() << "\n";

    return true;
}

std::string RecursiveASTVisitor::getDeclLocation(clang::SourceLocation location) const
{
    return llvm::Twine(sourceManager_.getFilename(location) + " " +
                       llvm::Twine(sourceManager_.getSpellingLineNumber(location)) + ":" +
                       llvm::Twine(sourceManager_.getSpellingColumnNumber(location)))
        .str();
}