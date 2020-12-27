#include "common/cache/TypeCache.h"

#include <algorithm>
#include <iostream>
#include <utility>

using namespace templex;
using namespace templex::model;

void TypeCache::dump() const
{
    std::cout << "\n\nDumping type cache\n";
    for (auto entry : classes_) {
        auto key = entry.first;
        auto val = entry.second;
        std::cout << "Class Template: " << key->getName() << "\n";
        for (auto instantiation : val) {
            std::cout << "  ";
            for (auto parameter : instantiation->getActualParameters()) {
                std::cout << parameter->getParameterName() << "="
                          << parameter->getActualParameter() << " ";
            }
            std::cout << "\n";
        }

        std::cout << "\n\n";
    }

    for (auto& [key, value] : aggregation_) {
        std::cout << "Class Template " << key->getName() << "\n";
        for (auto& [inst, aggregation] : value) {
            std::cout << "\t[ ";
            for (auto actualParam : inst->getActualParameters()) {
                std::cout << actualParam->getParameterName() << "="
                          << actualParam->getActualParameter() << " ";
            }
            std::cout << "] aggregation: " << aggregation << "\n";
        }
    }

    std::cout << "\n\n";
}

void TypeCache::addClassTemplate(TemplatePtr classTemplate)
{
    classes_.try_emplace(classTemplate, Instantiations());
}

void TypeCache::addClassInstantiation(InstantiationPtr classInstantiation)
{
    // The instantiation contains a pointer to the same object that we use as a key
    // in the cache.
    auto key = classInstantiation->getClassTemplate();

    // Key should already be in the cache.
    if (key == nullptr || classes_.find(key) == classes_.end()) {
        return;
    }

    classes_.at(key).emplace_back(classInstantiation);
}

bool TypeCache::containsClassName(const std::string& className) const
{
    return classes_.find(makeClassKey(className)) != classes_.end();
}

TemplatePtr
TypeCache::getClassTemplateByClassName(const std::string& className) const
{
    // Constructing a key for a faster lookup.
    auto key = makeClassKey(className);

    if (auto it = classes_.find(key); it != classes_.end()) {
        return it->first;

    } else {
        return nullptr;
    }
}

std::vector<TemplatePtr> TypeCache::getClassTemplates() const
{
    std::vector<TemplatePtr> returnValue;
    for (auto& entry : classes_) {
        returnValue.push_back(entry.first);
    }
    return returnValue;
}

TemplatePtr TypeCache::makeClassKey(const std::string& className) const
{
    return std::make_shared<Template>(className);
}

TypeCache::Instantiations
TypeCache::getInstantiationsFor(const std::string& className) const
{
    auto key = makeClassKey(className);
    return getInstantiationsFor(key);
}

TypeCache::Instantiations
TypeCache::getInstantiationsFor(TemplatePtr classTemplate) const
{
    if (classTemplate == nullptr) {
        return TypeCache::Instantiations();
    }

    if (classes_.find(classTemplate) != classes_.end()) {
        return classes_.at(classTemplate);
    }

    return TypeCache::Instantiations();
}

TypeCache::InstantiationAggregations
TypeCache::getAggregationsFor(TemplatePtr classTemplate) const
{
    if (classTemplate == nullptr) {
        return TypeCache::InstantiationAggregations();
    }

    if (aggregation_.find(classTemplate) != aggregation_.end()) {
        return aggregation_.at(classTemplate);
    }

    return TypeCache::InstantiationAggregations();
}

void TypeCache::addClassInstantiationAggregation(InstantiationPtr inst, int value)
{
    // Query the parent.
    auto key = inst->getClassTemplate();
    if (key == nullptr)
        return;

    // If the parent is missing, just default init.
    if (aggregation_.find(key) == aggregation_.end()) {
        aggregation_.emplace(key, InstantiationAggregations());
    }

    // Now add. We assume the initial cache building was valid.
    aggregation_[key].push_back(std::make_pair(inst, value));
}

TypeCache::InstantiationsCache TypeCache::getStlContainersInstantiationCache() const
{
    TypeCache::InstantiationsCache returnValue;
    std::copy_if(classes_.begin(),
                 classes_.end(),
                 std::inserter(returnValue, returnValue.end()),
                 [](auto& entry) -> bool {
                     bool contains = false;
                     for (auto* stlName : STL_CONTAINERS) {
                         if (std::string(stlName) == entry.first->getName()) {
                             return true;
                         }
                     }

                     return contains;
                 });

    return returnValue;
}

TypeCache::InstantiationAggregationsCache
TypeCache::getStlContainersAggregationCache() const
{
    TypeCache::InstantiationAggregationsCache returnValue;
    std::copy_if(aggregation_.begin(),
                 aggregation_.end(),
                 std::inserter(returnValue, returnValue.end()),
                 [](auto& entry) -> bool {
                     bool contains = false;
                     for (auto* stlName : STL_CONTAINERS) {
                         if (std::string(stlName) == entry.first->getName()) {
                             return true;
                         }
                     }

                     return contains;
                 });

    return returnValue;
}

void TypeCache::cleanCache()
{
    for (auto it = classes_.cbegin(); it != classes_.cend();) {
        if (it->second.empty()) {
            classes_.erase(it++);
        } else {
            ++it;
        }
    }
}

void TypeCache::trimPaths()
{
    std::vector<std::string> tempPaths;

    // Every class's
    for (auto& [key, value] : classes_) {
        // Every instantiation
        for (auto instantiation : value) {
            tempPaths.emplace_back(instantiation->getPointOfInstantiation());
        }
    }

    // Calculate the amount of characters matching
    int stripLength = 0;
    auto first      = *tempPaths.begin();
    for (std::size_t i = 0; i < first.size(); i++) {
        char it         = first.data()[i];
        bool allMatched = true;
        for (auto& path : tempPaths) {
            if (path[i] != it) {
                allMatched = false;
                break;
            }
        }

        if (allMatched)
            stripLength++;
        else
            break;
    }

    for (auto& [key, value] : classes_) {
        for (auto instantiation : value) {
            auto poi = instantiation->getPointOfInstantiation();
            poi.erase(0, stripLength);
            instantiation->setPointOfInstantiation(poi);
        }
    }
}

void TypeCache::createAggregation()
{
    for (auto& [key, value] : classes_) {
        auto& instantiations = aggregation_[key];

        for (auto instantiation : value) {

            bool contains = false;
            for (auto& [inst, aggregate] : instantiations) {
                if (inst->isEqual(instantiation)) {
                    contains = true;
                    aggregate++;
                    break;
                }
            }

            if (!contains) {
                instantiations.push_back(std::make_pair(instantiation, 1));
            }
        }
    }
}
