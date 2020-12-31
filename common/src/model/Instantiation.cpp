#include "common/model/Instantiation.h"

#include <iostream>
#include <utility>

using namespace templex;
using namespace templex::model;

Instantiation::Instantiation() {}

Instantiation::Instantiation(const std::string& pointOfInstantiation)
    : pointOfInstantiation_(pointOfInstantiation)
{
}

bool Instantiation::isEqual(std::shared_ptr<Instantiation> other) const
{
    // Two instantiation is the same if the class template is the same, and also
    // every parameter matches.
    if (classTemplate_->getName() != other->getTemplate()->getName()) {
        return false;
    }

    bool equal = true;
    for (std::size_t i = 0; i < actualParameters_.size(); i++) {
        if (actualParameters_[i]->getActualParameter() !=
            other->actualParameters_[i]->getActualParameter()) {
            equal = false;
            break;
        }
    }

    return equal;
}

void Instantiation::setTemplate(TemplatePtr classTemplate)
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

void Instantiation::setActualParameter(int parameterIndex, const std::string& value)
{
    if (parameterIndex < 0 ||
        static_cast<std::size_t>(parameterIndex) > actualParameters_.size() - 1) {
        return;
    }

    actualParameters_[parameterIndex]->setActualParameter(value);
}

std::vector<TemplateParameterPtr> Instantiation::getActualParameters() const
{
    return actualParameters_;
}

TemplatePtr Instantiation::getTemplate() const
{
    return classTemplate_;
}

void Instantiation::setPointOfInstantiation(const std::string& value)
{
    pointOfInstantiation_ = value;
}
std::string Instantiation::getPointOfInstantiation() const
{
    return pointOfInstantiation_;
}
