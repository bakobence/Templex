#include "RecursiveASTVisitor.h"

#include "clang/AST/DeclTemplate.h"
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/Basic/LangOptions.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/Casting.h>

#include <algorithm>
#include <cctype>
#include <string>

#include "common/model/Template.h"
#include "common/model/TemplateParameter.h"

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

    auto& cache = model::TypeCache::getInstance();

    if (cache.containsClassName(className)) {
        return true;
    }

    auto classTemplate = std::make_shared<model::Template>();
    classTemplate->setName(className);

    // Template parameters does not have a base class, but they are all
    // NamedDecl.
    auto* parameterList = D->getTemplateParameters();
    for (auto** it = parameterList->begin(); it != parameterList->end(); it++) {
        auto parameter = std::make_shared<model::TemplateParameter>();
        parameter->setParameterName((*it)->getName());

        // Checking the "type" of the template parameter.
        if (auto* type = llvm::dyn_cast<clang::TemplateTypeParmDecl>(*it)) {
            parameter->setType("type");
        } else if (auto* nonType =
                       llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(*it)) {
            parameter->setType("non-type");

            if (parameter->getParameterName().empty()) {
                if (auto valueDecl = llvm::dyn_cast<clang::ValueDecl>(*it)) {
                    parameter->setParameterName(valueDecl->getType().getAsString());
                }
            }
        }

        // Append the class template.
        classTemplate->addParameter(parameter);
    }

    cache.addClassTemplate(classTemplate);

    return true;
}

bool RecursiveASTVisitor::VisitFunctionTemplateDecl(clang::FunctionTemplateDecl* D)
{
    auto functionName = D->getQualifiedNameAsString();

    auto& cache = model::TypeCache::getInstance();

    return true;
}

bool RecursiveASTVisitor::VisitClassTemplateSpecializationDecl(
    clang::ClassTemplateSpecializationDecl* D)
{
    clang::SourceLocation loc;
    if (D->isExplicitSpecialization()) {
        loc = D->getTemplateKeywordLoc();
    } else {
        loc = D->getPointOfInstantiation();
    }

    if (loc.isInvalid()) {
        return true;
    }

    if (sourceManager_.isInSystemHeader(loc)) {
        return true;
    }

    auto& cache        = model::TypeCache::getInstance();
    auto className     = D->getQualifiedNameAsString();
    auto& argumentList = D->getTemplateArgs();
    auto classTemplate = cache.getClassTemplateByClassName(className);
    auto location      = getDeclLocation(loc);
    auto instantiation = std::make_shared<model::Instantiation>(location);

    instantiation->setClassTemplate(classTemplate);
    instantiation->setPointOfInstantiation(location);

    for (int i = 0; i < argumentList.size(); i++) {
        const auto& arg = argumentList.get(i);

        if (arg.getKind() == clang::TemplateArgument::ArgKind::Type) {
            instantiation->setActualParameter(i, sanitizeType(arg.getAsType()));
        } else if (arg.getKind() == clang::TemplateArgument::ArgKind::Integral) {
            auto integral = arg.getAsIntegral();
            instantiation->setActualParameter(i, integral.toString(10));
        } else if (arg.getKind() == clang::TemplateArgument::ArgKind::Declaration) {
            auto decl = arg.getAsDecl();
            if (auto functionDecl = llvm::dyn_cast<clang::FunctionDecl>(decl)) {
                auto returnType   = functionDecl->getReturnType();
                auto functionName = functionDecl->getName();

                std::string signature = returnType.getAsString();
                signature.append("(*");
                signature.append(functionName);
                signature.append(")(");
                for (auto* param : functionDecl->parameters()) {
                    signature.append(param->getOriginalType().getAsString());
                    signature.append(",");
                }
                if (functionDecl->param_size() != 0) {
                    signature.erase(signature.size() - 1, 1);
                }
                signature.append(")");

                instantiation->setActualParameter(i, signature);
            }
        } else if (arg.getKind() == clang::TemplateArgument::ArgKind::NullPtr) {
            instantiation->setActualParameter(i, "nullptr");
        }
    }

    cache.addClassInstantiation(instantiation);

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

std::string RecursiveASTVisitor::sanitizeType(clang::QualType type) const
{
    clang::LangOptions langOptions;
    clang::PrintingPolicy policy(langOptions);
    policy.SuppressTagKeyword = 1;

    auto argument = type.getAsString(policy);
    argument.erase(std::remove_if(argument.begin(), argument.end(), ::isspace),
                   argument.end());

    return argument;
}

bool RecursiveASTVisitor::shouldSkipDecl(const clang::SourceLocation& location)
{
    return sourceManager_.isInSystemHeader(location);
}