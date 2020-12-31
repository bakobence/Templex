#pragma once

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace templex {
namespace frontend {

class JSONImport {
public:
    static bool importCache();
    static bool importClasses(const QJsonDocument& doc);
    static bool importFunctions(const QJsonDocument& doc);

private:
    static constexpr const char* const CLASSES        = "classes";
    static constexpr const char* const CLASS_NAME     = "className";
    static constexpr const char* const ARGUMENTS      = "arguments";
    static constexpr const char* const PARAMETERS     = "parameters";
    static constexpr const char* const INSTANTIATIONS = "instantiations";
    static constexpr const char* const KIND           = "kind";
    static constexpr const char* const NAME           = "name";
    static constexpr const char* const POI            = "poi";
    static constexpr const char* const ACTUAL         = "actual";
    static constexpr const char* const AGGREGATIONS   = "aggregations";
    static constexpr const char* const AGGREGATION    = "aggregation";
    static constexpr const char* const FUNCTIONS      = "functions";
    static constexpr const char* const FUNCTION_NAME  = "functionName";
    static constexpr const char* const OVERLOADS      = "overloads";
    static constexpr const char* const COMPLETE_FUNCTION_SIGNATURE =
        "completeFunctionSignature";
};

} // namespace frontend
} // namespace templex
