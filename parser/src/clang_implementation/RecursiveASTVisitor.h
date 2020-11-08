#include <clang/AST/RecursiveASTVisitor.h>

namespace templex {
namespace parser {

class RecursiveASTVisitor : public clang::RecursiveASTVisitor<RecursiveASTVisitor> {
public:
    RecursiveASTVisitor();

    bool shouldVisitTemplateInstantiations() const { return true; }

    bool VisitClassTemplateDecl(clang::ClassTemplateDecl* D);
};

} // namespace parser
} // namespace templex
