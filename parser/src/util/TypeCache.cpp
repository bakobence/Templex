#include "TypeCache.h"

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <algorithm>
#include <fstream>
#include <utility>

using namespace templex;
using namespace templex::parser;

void TypeCache::dump() const
{
    llvm::outs() << "\n\n_______________Dumping type cache______________\n\n";
    for (auto entry : instantiations_) {
        auto key = entry.first;
        auto val = entry.second;
        llvm::outs() << "Class Template: " << key->getClassName() << "\n";
        for (auto instantiation : val) {
            llvm::outs() << "  ";
            for (auto parameter : instantiation->getActualParameters()) {
                llvm::outs() << parameter->getParameterName() << "="
                             << parameter->getActualParameter() << " ";
            }
            llvm::outs() << "\n";
        }

        llvm::outs() << "\n\n";
    }
}

void TypeCache::exportJSON() const
{
    using namespace rapidjson;

    Document d;
    d.SetObject();

    auto& allocator = d.GetAllocator();

    // Exporting classes.
    Value classes(kArrayType);

    for (auto entry : instantiations_) {

        Value type(kObjectType);
        {
            {
                Value className(kStringType);

                auto value = entry.first->getClassName();

                className.SetString(value.c_str(), value.size(), allocator);

                type.AddMember("className", className, allocator);
            }

            {
                Value parameters(kArrayType);

                auto parameterList = entry.first->getParameterList();
                for (auto parameter : parameterList) {

                    Value parameterValue(kObjectType);
                    {
                        Value kind(kStringType);

                        auto kindValue = parameter->getTypeAsString();

                        kind.SetString(kindValue.c_str(),
                                       kindValue.size(),
                                       allocator);

                        parameterValue.AddMember("kind", kind, allocator);
                    }

                    {
                        Value name(kStringType);

                        auto parameterName = parameter->getParameterName();

                        name.SetString(parameterName.c_str(),
                                       parameterName.size(),
                                       allocator);

                        parameterValue.AddMember("name", name, allocator);
                    }

                    parameters.PushBack(parameterValue.Move(), allocator);
                }

                type.AddMember("parameters", parameters, allocator);
            }

            {
                Value instantiations(kArrayType);

                for (auto instantiation : entry.second) {

                    Value instantiationValue(kObjectType);

                    {
                        Value poi(kStringType);

                        auto value = instantiation->getPointOfInstantiation();

                        poi.SetString(value.c_str(), value.size(), allocator);

                        instantiationValue.AddMember("poi", poi, allocator);
                    }

                    {
                        Value actualParameters(kArrayType);

                        auto params = instantiation->getActualParameters();
                        for (auto actualParameter : params) {

                            Value param(kObjectType);

                            {
                                Value kind(kStringType);

                                auto value = actualParameter->getTypeAsString();

                                kind.SetString(value.c_str(),
                                               value.size(),
                                               allocator);

                                param.AddMember("kind", kind, allocator);
                            }

                            {
                                Value actual(kStringType);

                                auto value = actualParameter->getActualParameter();

                                actual.SetString(value.c_str(),
                                                 value.size(),
                                                 allocator);

                                param.AddMember("actual", actual, allocator);
                            }

                            actualParameters.PushBack(param.Move(), allocator);
                        }

                        instantiationValue.AddMember("params",
                                                     actualParameters,
                                                     allocator);
                    }

                    instantiations.PushBack(instantiationValue.Move(), allocator);
                }

                type.AddMember("instantiation", instantiations, allocator);
            }
        }

        classes.PushBack(type.Move(), allocator);
    }

    d.AddMember("classes", classes, allocator);

    std::ofstream f("result_json.txt");
    OStreamWrapper osw(f);

    PrettyWriter<OStreamWrapper> writer(osw);
    d.Accept(writer);

    f.close();
}

void TypeCache::addClassTemplate(ClassTemplatePtr classTemplate)
{
    instantiations_.try_emplace(classTemplate, ClassInstantiations());
}

void TypeCache::addClassInstantiation(ClassInstantiationPtr classInstantiation)
{
    // The instantiation contains a pointer to the same object that we use as a key
    // in the cache.
    auto key = classInstantiation->getClassTemplate();

    // Key should already be in the cache.
    if (key == nullptr || instantiations_.find(key) == instantiations_.end()) {
        return;
    }

    instantiations_.at(key).emplace_back(classInstantiation);
}

bool TypeCache::containsClassName(const std::string& className) const
{
    return instantiations_.find(makeClassKey(className)) != instantiations_.end();
}

ClassTemplatePtr
TypeCache::getClassTemplateByClassName(const std::string& className) const
{
    // Constructing a key for a faster lookup.
    auto key = makeClassKey(className);

    if (auto it = instantiations_.find(key); it != instantiations_.end()) {
        return it->first;

    } else {
        return nullptr;
    }
}

ClassTemplatePtr TypeCache::makeClassKey(const std::string& className) const
{
    return std::make_shared<ClassTemplate>(className);
}
