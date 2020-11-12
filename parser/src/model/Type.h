#include "llvm/ADT/StringRef.h"

namespace templex {
namespace parser {

class Type {
public:
    Type();

private:
    llvm::StringRef typeName_;
};

} // namespace parser
} // namespace templex