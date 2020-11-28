#pragma once

#include <memory>
#include <string>
#include <vector>

#include "model/ClassTemplate.h"
#include "model/TemplateParameter.h"

namespace templex {
namespace parser {

class ClassInstantiation;
using ClassInstantiationPtr = std::shared_ptr<ClassInstantiation>;

class ClassInstantiation {
public:
    ClassInstantiation();
    ClassInstantiation(const std::string& pointOfInstantiation);

public:
    void setClassTemplate(ClassTemplatePtr classTemplate);

    void setActualParameter(int parameterIndex, const std::string& value);
    std::vector<TemplateParameterPtr> getActualParameters() const;

    ClassTemplatePtr getClassTemplate() const;

    void setPointOfInstantiation(const std::string& value);
    std::string getPointOfInstantiation() const;

private:
    // The template the descirbes the class
    ClassTemplatePtr classTemplate_;

    // The point where the class template was instantiated for the first time.
    std::string pointOfInstantiation_;

    // The actual parameters of the class template parameters.
    std::vector<TemplateParameterPtr> actualParameters_;
};
} // namespace parser
} // namespace templex