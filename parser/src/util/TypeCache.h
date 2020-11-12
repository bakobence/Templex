
namespace templex {
namespace parser {

class TypeCache {

public:
    static TypeCache& getInstance()
    {
        // C++ standard 6.7.1
        // Will get constructed on the first call, and deleted on the program exists.
        static TypeCache instance;
        return instance;
    }
};

} // namespace parser
} // namespace templex