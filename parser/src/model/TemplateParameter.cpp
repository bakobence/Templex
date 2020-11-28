#include "TemplateParameter.h"

using namespace templex;
using namespace templex::parser;

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

TemplateParameter::Type TemplateParameter::getType() const
{
    return type_;
}

void TemplateParameter::setType(Type value)
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

std::string TemplateParameter::getTypeAsString() const
{
    switch (type_) {
    case Type::TYPE:
        return "Type";
    case Type::NON_TYPE:
        return "Non type";
    case Type::TEMPLATE:
        return "Template";
    }

    return "";
}