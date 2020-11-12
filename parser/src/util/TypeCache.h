
#include <memory>

namespace templex {
namespace parser {

class TypeCache {

public:
    static TypeCache* getInstance()
    {
        // C++ standard 6.7.1
        static auto const instance = std::make_unique<TypeCache>();
        return instance.get();
    }

private:
    TypeCache() {}

public:
    TypeCache(const TypeCache&) = delete;
    void operator=(const TypeCache&) = delete;

private:
};

} // namespace parser
} // namespace templex