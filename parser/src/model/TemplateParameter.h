#pragma once

#include <memory>
#include <string>

namespace templex {
namespace parser {

class TemplateParameter;
using TemplateParameterPtr = std::shared_ptr<TemplateParameter>;

class TemplateParameter {
public:
    enum class Type { TYPE = 0, NON_TYPE = 1, TEMPLATE = 2 };

    TemplateParameter() = default;
    TemplateParameter(const TemplateParameter& other);

    std::string getParameterName() const;
    void setParameterName(const std::string& value);

    Type getType() const;
    void setType(Type value);

    std::string getActualParameter() const;
    void setActualParameter(const std::string& value);

    std::string getTypeAsString() const;

private:
    // The parameter name, eg: template<typename T> .. T is the parameter name.
    std::string parameterName_;

    Type type_;

    // The actual parameter, when instantiated, eg. std::vector<int> .. 'int' is the
    // actual parameter
    std::string actualParameter_;
};
} // namespace parser
} // namespace templex