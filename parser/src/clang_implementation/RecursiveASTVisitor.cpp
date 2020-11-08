#include "RecursiveASTVisitor.h"

using namespace templex;

using super = clang::RecursiveASTVisitor<parser::RecursiveASTVisitor>;

parser::RecursiveASTVisitor::RecursiveASTVisitor() {}

bool parser::RecursiveASTVisitor::VisitClassTemplateDecl(clang::ClassTemplateDecl* D)
{
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

    return true;
}