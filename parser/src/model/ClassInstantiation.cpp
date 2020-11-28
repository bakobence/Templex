#include "ClassInstantiation.h"

#include <utility>

using namespace templex;
using namespace templex::parser;

ClassInstantiation::ClassInstantiation() {}

ClassInstantiation::ClassInstantiation(const std::string& pointOfInstantiation)
    : pointOfInstantiation_(pointOfInstantiation)
{
}

void ClassInstantiation::setClassTemplate(ClassTemplatePtr classTemplate)
{
    // Copying the pointer to the same object that the cache points to.
    classTemplate_ = classTemplate;

    auto parameterList = classTemplate_->getParameterList();

    actualParameters_.reserve(parameterList.size());
    for (auto parameter : parameterList) {
        // Copy the not acutalized parameter object.
        auto actualParameter = std::make_shared<TemplateParameter>(*parameter);
        actualParameters_.emplace_back(actualParameter);
    }
}

void ClassInstantiation::setActualParameter(int parameterIndex,
                                            const std::string& value)
{
    if (parameterIndex < 0 || parameterIndex > actualParameters_.size() - 1) {
        return;
    }

    actualParameters_[parameterIndex]->setActualParameter(value);
}

std::vector<TemplateParameterPtr> ClassInstantiation::getActualParameters() const
{
    return actualParameters_;
}

ClassTemplatePtr ClassInstantiation::getClassTemplate() const
{
    return classTemplate_;
}

void ClassInstantiation::setPointOfInstantiation(const std::string& value)
{
    pointOfInstantiation_ = value;
}
std::string ClassInstantiation::getPointOfInstantiation() const
{
    return pointOfInstantiation_;
}