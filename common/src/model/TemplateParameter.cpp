#include "common/model/TemplateParameter.h"

#include <iostream>

using namespace templex;
using namespace templex::model;

TemplateParameter::TemplateParameter()
    : parameterName_(std::string()),
      type_(std::string()),
      actualParameter_(std::string())
{
}

TemplateParameter::TemplateParameter(const TemplateParameter& other)
{
    type_          = other.type_;
    parameterName_ = other.parameterName_;
}

std::string TemplateParameter::getParameterName() const
{
    return parameterName_;
}

void TemplateParameter::setParameterName(const std::string& value)
{
    parameterName_ = value;
}

std::string TemplateParameter::getType() const
{
    return type_;
}

void TemplateParameter::setType(const std::string& value)
{
    type_ = value;
}

std::string TemplateParameter::getActualParameter() const
{
    return actualParameter_;
}

void TemplateParameter::setActualParameter(const std::string& value)
{
    actualParameter_ = value;
}
