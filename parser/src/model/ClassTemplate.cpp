#include "ClassTemplate.h"

using namespace templex;
using namespace templex::parser;

ClassTemplate::ClassTemplate() {}

ClassTemplate::ClassTemplate(const std::string& className) : className_{className} {}

const bool ClassTemplate::operator<(const ClassTemplate& other) const
{
    return this->className_ < other.className_;
}

std::string ClassTemplate::getClassName() const
{
    return className_;
}

void ClassTemplate::setClassName(const std::string& value)
{
    this->className_ = value;
}

std::vector<TemplateParameterPtr> ClassTemplate::getParameterList() const
{
    return parameterList_;
}

void ClassTemplate::addParameter(TemplateParameterPtr value)
{
    this->parameterList_.push_back(value);
}
