#pragma once

#include <memory>
#include <string>

#include "templex_common_export.h"

namespace templex {
namespace model {

class TemplateParameter;
using TemplateParameterPtr = std::shared_ptr<TemplateParameter>;

class TEMPLEX_COMMON_EXPORT TemplateParameter {
public:
    TemplateParameter();
    TemplateParameter(const TemplateParameter& other);

    std::string getParameterName() const;
    void setParameterName(const std::string& value);

    std::string getType() const;
    void setType(const std::string& value);

    std::string getActualParameter() const;
    void setActualParameter(const std::string& value);

private:
    // The parameter name, eg: template<typename T> .. T is the parameter name.
    std::string parameterName_;
    std::string type_;

    // The actual parameter, when instantiated, eg. std::vector<int> .. 'int' is the
    // actual parameter
    std::string actualParameter_;
};
} // namespace model
} // namespace templex
