#include "common/cache/TypeCache.h"

#include <algorithm>
#include <iostream>
#include <utility>

using namespace templex;
using namespace templex::model;

void TypeCache::dump() const
{
    std::cout << "\n\nDumping type cache\n";
    for (auto& [key, value] : classAggregations) {
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

    for (auto& [functionName, cache] : functions_) {
        std::cout << "Function Template: " << functionName << "\n";
        for (auto& [templatePtr, instantiations] : cache) {
            std::cout << "\tFunction ID: " << templatePtr->getName() << "\n";
            for (auto instantiation : instantiations) {
                std::cout << "\t\t[";
                for (auto actualParam : instantiation->getActualParameters()) {
                    std::cout << actualParam->getParameterName() << "="
                              << actualParam->getActualParameter() << " ";
                }
                std::cout << "]\n";
            }
        }
    }

    std::cout << "\n\n";
}

void TypeCache::addClassTemplate(TemplatePtr classTemplate)
{
    classes_.try_emplace(classTemplate, Instantiations());
}

void TypeCache::addFunctionTemplate(TemplatePtr functionTemplate,
                                    const std::string& functionName)
{
    functions_.try_emplace(functionName);
    functions_[functionName].try_emplace(functionTemplate, Instantiations());
}

void TypeCache::addClassInstantiation(InstantiationPtr classInstantiation)
{
    // The instantiation contains a pointer to the same object that we use as a key
    // in the cache.
    auto key = classInstantiation->getTemplate();

    // Key should already be in the cache.
    if (key == nullptr || classes_.find(key) == classes_.end()) {
        return;
    }

    classes_.at(key).emplace_back(classInstantiation);
}

void TypeCache::addFunctionInstantiation(InstantiationPtr functionInstantiation,
                                         const std::string& functionName)
{
    if (functions_.find(functionName) == functions_.end()) {
        return;
    }

    auto functionTemplate = functionInstantiation->getTemplate();
    auto cache            = functions_[functionName];

    if (cache.find(functionTemplate) == cache.end()) {
        return;
    }

    functions_.at(functionName)
        .at(functionTemplate)
        .push_back(functionInstantiation);
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

std::vector<std::string> TypeCache::getFunctionNames() const
{
    std::vector<std::string> returnValue;
    for (auto& entry : functions_) {
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

    if (classAggregations.find(classTemplate) != classAggregations.end()) {
        return classAggregations.at(classTemplate);
    }

    return TypeCache::InstantiationAggregations();
}

void TypeCache::addClassInstantiationAggregation(InstantiationPtr inst, int value)
{
    // Query the parent.
    auto key = inst->getTemplate();
    if (key == nullptr)
        return;

    // If the parent is missing, just default init.
    if (classAggregations.find(key) == classAggregations.end()) {
        classAggregations.emplace(key, InstantiationAggregations());
    }

    // Now add. We assume the initial cache building was valid.
    classAggregations[key].push_back(std::make_pair(inst, value));
}

std::vector<TemplatePtr>
TypeCache::getOverloadTempaltesForFunction(const std::string& functionName) const
{
    if (functions_.find(functionName) == functions_.end())
        return {};

    std::vector<TemplatePtr> returnValue;
    for (auto& [functionTemplate, instantiations] : functions_.at(functionName)) {
        returnValue.push_back(functionTemplate);
    }
    return returnValue;
}

TypeCache::Instantiations TypeCache::getFunctionOverloadInstantiationsFor(
    TemplatePtr functionTemplate, const std::string& functionName) const
{
    if (functions_.find(functionName) == functions_.end())
        return {};

    if (functions_.at(functionName).find(functionTemplate) ==
        functions_.at(functionName).end())
        return {};

    return functions_.at(functionName).at(functionTemplate);
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
    std::copy_if(classAggregations.begin(),
                 classAggregations.end(),
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

    for (auto it = functions_.cbegin(); it != functions_.cend();) {
        auto cache = it->second;
        for (auto cacheIt = cache.cbegin(); cacheIt != cache.cend();) {
            if (cacheIt->second.empty()) {
                functions_[it->first].erase(cacheIt++);
            } else {
                ++cacheIt;
            }
        }

        if (it->second.empty()) {
            functions_.erase(it++);
        } else {
            it++;
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
    // Every function's
    for (auto& [name, cache] : functions_) {
        for (auto& [templatePtr, instantions] : cache) {
            for (auto& instantiation : instantions) {
                tempPaths.emplace_back(instantiation->getPointOfInstantiation());
            }
        }
    }

    if (tempPaths.empty())
        return;

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

    // Now do the cut-cut
    for (auto& [key, value] : classes_) {
        for (auto& instantiation : value) {
            auto poi = instantiation->getPointOfInstantiation();
            poi.erase(0, stripLength);
            instantiation->setPointOfInstantiation(poi);
        }
    }

    for (auto& [name, cache] : functions_) {
        for (auto& [templatePtr, instantions] : cache) {
            for (auto& instantiation : instantions) {
                auto poi = instantiation->getPointOfInstantiation();
                poi.erase(0, stripLength);
                instantiation->setPointOfInstantiation(poi);
            }
        }
    }
}

void TypeCache::createAggregation()
{
    for (auto& [key, value] : classes_) {
        auto& instantiations = classAggregations[key];

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
