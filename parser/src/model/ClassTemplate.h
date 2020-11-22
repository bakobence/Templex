#pragma once

#include <string>

#include <llvm/Support/raw_ostream.h>

#include "model/TemplateArgument.h"

namespace templex {
namespace parser {

class ClassTemplate;
using ClassTemplatePtr = std::shared_ptr<ClassTemplate>;

class ClassTemplate {
public:
    ClassTemplate();

    // Using the types name as unique identifier for efficient mapping
    const bool operator<(const ClassTemplate& other) const;

public:
    std::string getClassName() const;
    void setClassName(const std::string& value);

    std::vector<TemplateArgumentPtr> getArgumentList() const;
    void addArgument(TemplateArgumentPtr value);

private:
    std::string className_;
    std::vector<TemplateArgumentPtr> argumentList_;
};

} // namespace parser
} // namespace templex