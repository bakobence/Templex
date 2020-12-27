#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "common/model/Template.h"
#include "common/model/TemplateParameter.h"

#include "templex_common_export.h"

namespace templex {
namespace model {

class Instantiation;
using InstantiationPtr = std::shared_ptr<Instantiation>;

class TEMPLEX_COMMON_EXPORT Instantiation {
public:
    Instantiation();
    Instantiation(const std::string& pointOfInstantiation);

public:
    TemplatePtr getClassTemplate() const;
    void setClassTemplate(TemplatePtr classTemplate);

    void setActualParameter(int parameterIndex, const std::string& value);
    std::vector<TemplateParameterPtr> getActualParameters() const;

    void setPointOfInstantiation(const std::string& value);
    std::string getPointOfInstantiation() const;

    bool isEqual(std::shared_ptr<Instantiation> other) const;

private:
    // The template the descirbes the class
    TemplatePtr classTemplate_;

    // The point where the class template was instantiated for the first time.
    std::string pointOfInstantiation_;

    // The actual parameters of the class template parameters.
    std::vector<TemplateParameterPtr> actualParameters_;
};
} // namespace model
} // namespace templex
