#pragma once

#include <memory>
#include <string>

namespace templex {
namespace parser {

class TemplateArgument;
using TemplateArgumentPtr = std::shared_ptr<TemplateArgument>;

class TemplateArgument {
public:
    enum class Type { TYPE = 0, NON_TYPE = 1, TEMPLATE = 2 };

    TemplateArgument() = default;

    std::string getArgumentName() const;
    void setArgumentName(const std::string& value);

    Type getType() const;
    void setType(Type value);

private:
    std::string argumentName_;
    Type type_;
};
} // namespace parser
} // namespace templex