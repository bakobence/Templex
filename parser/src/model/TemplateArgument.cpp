#include "TemplateArgument.h"

using namespace templex;
using namespace templex::parser;

std::string TemplateArgument::getArgumentName() const { return argumentName_; }

void TemplateArgument::setArgumentName(const std::string& value)
{
    this->argumentName_ = value;
}

TemplateArgument::Type TemplateArgument::getType() const { return type_; }

void TemplateArgument::setType(Type value) { this->type_ = value; }