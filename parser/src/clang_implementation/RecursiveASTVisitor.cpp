#include "RecursiveASTVisitor.h"

#include <clang/AST/DeclTemplate.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/Basic/LangOptions.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/Casting.h>

#include <string>

#include "model/ClassTemplate.h"
#include "model/TemplateParameter.h"

using namespace templex;
using namespace templex::parser;

using super = clang::RecursiveASTVisitor<parser::RecursiveASTVisitor>;

RecursiveASTVisitor::RecursiveASTVisitor(clang::SourceManager& sourceManager)
    : sourceManager_(sourceManager)
{
}

bool RecursiveASTVisitor::VisitClassTemplateDecl(clang::ClassTemplateDecl* D)
{
    auto className = D->getQualifiedNameAsString();

    auto& cache = TypeCache::getInstance();

    if (cache.containsClassName(className)) {
        return true;
    }

    auto classTemplate = std::make_shared<ClassTemplate>();
    classTemplate->setClassName(className);

    // Template parameters does not have a base class, but they are all
    // NamedDecl.
    auto* parameterList = D->getTemplateParameters();
    for (auto** it = parameterList->begin(); it != parameterList->end(); it++) {
        auto parameter = std::make_shared<TemplateParameter>();
        parameter->setParameterName((*it)->getName());

        // Checking the "type" of the template parameter.
        if (auto* type = llvm::dyn_cast<clang::TemplateTypeParmDecl>(*it)) {
            parameter->setType(TemplateParameter::Type::TYPE);
        } else if (auto* nonType =
                       llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(*it)) {
            parameter->setType(TemplateParameter::Type::NON_TYPE);
        }

        // Append the class template.
        classTemplate->addParameter(parameter);
    }

    cache.addClassTemplate(classTemplate);

    return true;
}

bool RecursiveASTVisitor::VisitClassTemplateSpecializationDecl(
    clang::ClassTemplateSpecializationDecl* D)
{
    auto& cache        = TypeCache::getInstance();
    auto className     = D->getQualifiedNameAsString();
    auto& argumentList = D->getTemplateArgs();
    auto classTemplate = cache.getClassTemplateByClassName(className);
    auto location      = getDeclLocation(D->getPointOfInstantiation());
    auto instantiation = std::make_shared<ClassInstantiation>(location);

    instantiation->setClassTemplate(classTemplate);
    instantiation->setPointOfInstantiation(location);

    for (int i = 0; i < argumentList.size(); i++) {
        const auto& arg = argumentList.get(i);

        if (arg.getKind() == clang::TemplateArgument::ArgKind::Type) {
            auto qualType = arg.getAsType();
            clang::LangOptions langOptions;
            clang::PrintingPolicy policy(langOptions);
            policy.SuppressTagKeyword = 1;
            instantiation->setActualParameter(i, qualType.getAsString(policy));
        }
    }

    cache.addClassInstantiation(instantiation);

    /*


    llvm::outs() << className << " " << location << "\nTemplate argument list\n";

    for (auto arg : args) {

        llvm::outs() << " [kind] " << arg.getKind();

        if (arg.getKind() == clang::TemplateArgument::ArgKind::Type) {

            llvm::outs() << "  Type: ";
            llvm::outs() <<
    arg.getAsType()->getCanonicalTypeInternal().getAsString(); llvm::outs() <<
    "\n";

        } else if (arg.getKind() == clang::TemplateArgument::ArgKind::Pack) {

            llvm::outs() << "  Variadic pack\n";

        } else if (arg.getKind() == clang::TemplateArgument::ArgKind::Integral) {

            llvm::outs() << "  Integral\n";
        }
    }

    llvm::outs() << "\n";
    */

    return true;
}

std::string
RecursiveASTVisitor::getDeclLocation(clang::SourceLocation location) const
{
    return llvm::Twine(sourceManager_.getFilename(location) + " " +
                       llvm::Twine(sourceManager_.getSpellingLineNumber(location)) +
                       ":" +
                       llvm::Twine(sourceManager_.getSpellingColumnNumber(location)))
        .str();
}