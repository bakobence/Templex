#include "common/model/Template.h"

using namespace templex;
using namespace templex::model;

Template::Template() {}

Template::Template(const std::string& className) : name_{className} {}

bool Template::operator<(const Template& other) const
{
    return name_ < other.name_;
}

std::string Template::getName() const
{
    return name_;
}

void Template::setName(const std::string& value)
{
    name_ = value;
}

std::vector<TemplateParameterPtr> Template::getParameterList() const
{
    return parameterList_;
}

void Template::addParameter(TemplateParameterPtr value)
{
    parameterList_.push_back(value);
}

int Template::getParameterCount() const
{
    return parameterList_.size();
}
