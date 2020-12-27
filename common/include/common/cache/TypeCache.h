#pragma once

#include <array>
#include <map>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common/model/Instantiation.h"
#include "common/model/Template.h"
#include "common/util/Comparators.h"

#include "templex_common_export.h"

namespace templex {
namespace model {

class TEMPLEX_COMMON_EXPORT TypeCache {
public:
    using Instantiations = std::vector<InstantiationPtr>;
    using InstantiationsCache =
        std::map<TemplatePtr, Instantiations, ::nostd::less_pointee<Template>>;

    using InstantiationAggregations = std::vector<std::pair<InstantiationPtr, int>>;
    using InstantiationAggregationsCache = std::
        map<TemplatePtr, InstantiationAggregations, ::nostd::less_pointee<Template>>;

public:
    static TypeCache& getInstance()
    {
        // C++ standard 6.7.1
        static TypeCache instance;
        return instance;
    }

    TypeCache(const TypeCache&) = delete;
    void operator=(const TypeCache&) = delete;

private:
    TypeCache() {}

public:
    void dump() const;

    void addClassTemplate(TemplatePtr classTemplate);
    void addClassInstantiation(InstantiationPtr instantiation);
    bool containsClassName(const std::string& className) const;
    TemplatePtr getClassTemplateByClassName(const std::string& className) const;

    std::vector<TemplatePtr> getClassTemplates() const;
    Instantiations getInstantiationsFor(const std::string& className) const;
    Instantiations getInstantiationsFor(TemplatePtr classTemplate) const;

    InstantiationAggregations getAggregationsFor(TemplatePtr classTemplate) const;
    void addClassInstantiationAggregation(InstantiationPtr inst, int value);

    InstantiationsCache getStlContainersInstantiationCache() const;
    InstantiationAggregationsCache getStlContainersAggregationCache() const;

    void cleanCache();
    void trimPaths();
    void createAggregation();

private:
    TemplatePtr makeClassKey(const std::string& className) const;

private:
    InstantiationsCache classes_;
    InstantiationAggregationsCache aggregation_;

    constexpr static std::array<const char*, 11> STL_CONTAINERS = {
        "std::array",
        "std::deque",
        "std::forward_list",
        "std::list",
        "std::map",
        "std::queue",
        "std::set",
        "std::stack",
        "std::unordered_map",
        "std::unordered_set",
        "std::vector"};
};

} // namespace model
} // namespace templex
