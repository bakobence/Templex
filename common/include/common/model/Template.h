#pragma once

#include <string>
#include <vector>

#include "common/model/TemplateParameter.h"

#include "templex_common_export.h"

namespace templex {
namespace model {

class Template;
using TemplatePtr = std::shared_ptr<Template>;

class TEMPLEX_COMMON_EXPORT Template {
public:
    Template();
    Template(const std::string& className);

    // Using the types name as unique identifier for efficient mapping
    bool operator<(const Template& other) const;

public:
    std::string getName() const;
    void setName(const std::string& value);

    std::vector<TemplateParameterPtr> getParameterList() const;
    void addParameter(TemplateParameterPtr value);

    int getParameterCount() const;

private:
    std::string name_;
    std::vector<TemplateParameterPtr> parameterList_;
};

} // namespace model
} // namespace templex
