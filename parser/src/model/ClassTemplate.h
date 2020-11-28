#pragma once

#include <string>

#include <llvm/Support/raw_ostream.h>

#include "model/TemplateParameter.h"

namespace templex {
namespace parser {

class ClassTemplate;
using ClassTemplatePtr = std::shared_ptr<ClassTemplate>;

class ClassTemplate {
public:
    ClassTemplate();
    ClassTemplate(const std::string& className);

    // Using the types name as unique identifier for efficient mapping
    const bool operator<(const ClassTemplate& other) const;

public:
    std::string getClassName() const;
    void setClassName(const std::string& value);

    std::vector<TemplateParameterPtr> getParameterList() const;
    void addParameter(TemplateParameterPtr value);

private:
    std::string className_;
    std::vector<TemplateParameterPtr> parameterList_;
};

} // namespace parser
} // namespace templex