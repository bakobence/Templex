#pragma once

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace templex {
namespace frontend {

class JSONImport {
public:
    static void importCache();
    static bool importClasses(const QJsonDocument& doc);

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
};

} // namespace frontend
} // namespace templex
