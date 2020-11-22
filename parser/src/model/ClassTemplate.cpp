#include "ClassTemplate.h"

#include <llvm/Support/raw_ostream.h>

using namespace templex;
using namespace templex::parser;

ClassTemplate::ClassTemplate() {}

const bool ClassTemplate::operator<(const ClassTemplate& other) const
{
    return this->className_ < other.className_;
}

std::string ClassTemplate::getClassName() const { return className_; }

void ClassTemplate::setClassName(const std::string& value)
{
    this->className_ = value;
}

std::vector<TemplateArgumentPtr> ClassTemplate::getArgumentList() const
{
    return argumentList_;
}

void ClassTemplate::addArgument(TemplateArgumentPtr value)
{
    this->argumentList_.push_back(value);
}
