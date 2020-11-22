#include <exception>
#include <string>

namespace templex {
namespace parser {

class Exception : public std::exception {
protected:
    std::string detail_;
    std::string type_;
    std::string what_;

public:
    Exception();
    Exception(const std::string& detail,
              const std::string& type,
              const std::string& what);

    std::string detail() const;
    std::string type() const;

    virtual const char* what() const noexcept override { return what_.c_str(); }
};

class ClassTemplateRevisitedException : Exception {
public:
    ClassTemplateRevisitedException();
    ClassTemplateRevisitedException(const std::string& detail);
};

} // namespace parser
} // namespace templex