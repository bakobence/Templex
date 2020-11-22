#include "Exceptions.h"

using namespace templex;
using namespace templex::parser;

Exception::Exception() {}

Exception::Exception(const std::string& detail,
                     const std::string& type,
                     const std::string& what)
    : detail_(detail),
      type_(type),
      what_(what)
{
}

std::string Exception::detail() const { return detail_; }

std::string Exception::type() const { return type_; }

ClassTemplateRevisitedException::ClassTemplateRevisitedException() {}

ClassTemplateRevisitedException::ClassTemplateRevisitedException(
    const std::string& detail)
    : Exception(detail_,
                "ClassTemplateRevisitedException",
                "Multiple visitation of a ClassTemplateDecl occured")
{
}