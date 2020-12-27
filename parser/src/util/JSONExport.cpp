#include "JSONExport.h"

#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "common/cache/TypeCache.h"

using namespace templex;
using namespace templex::parser;

void JSONExport::exportCache()
{
    using namespace rapidjson;

    Document d;
    d.SetObject();

    auto& allocator = d.GetAllocator();
    auto& cache     = model::TypeCache::getInstance();

    cache.cleanCache();
    cache.trimPaths();
    cache.createAggregation();

    // Exporting classes.
    Value classes(kArrayType);
    for (auto entryKey : cache.getClassTemplates()) {

        Value type(kObjectType);
        {
            {
                Value className(kStringType);

                auto value = entryKey->getName();

                className.SetString(value.c_str(), value.size(), allocator);

                type.AddMember("className", className, allocator);
            }

            {
                Value parameters(kArrayType);

                auto parameterList = entryKey->getParameterList();
                for (auto parameter : parameterList) {

                    Value parameterValue(kObjectType);
                    {
                        Value kind(kStringType);

                        auto kindValue = parameter->getType();

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

                for (auto instantiation : cache.getInstantiationsFor(entryKey)) {

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
                                Value actual(kStringType);
                                auto value = actualParameter->getActualParameter();
                                actual.SetString(value.c_str(),
                                                 value.size(),
                                                 allocator);

                                param.AddMember("actual", actual, allocator);
                            }
                            actualParameters.PushBack(param.Move(), allocator);
                        }
                        instantiationValue.AddMember("arguments",
                                                     actualParameters,
                                                     allocator);
                    }
                    instantiations.PushBack(instantiationValue.Move(), allocator);
                }
                type.AddMember("instantiations", instantiations, allocator);
            }

            {
                Value aggregations(kArrayType);
                for (auto& [instantiation, aggregation] :
                     cache.getAggregationsFor(entryKey)) {
                    Value aggregationObject(kObjectType);
                    {
                        Value arguments(kArrayType);
                        for (auto arg : instantiation->getActualParameters()) {
                            Value argumentObject(kObjectType);
                            {
                                Value actualString(kStringType);
                                auto actual = arg->getActualParameter();
                                actualString.SetString(actual.c_str(),
                                                       actual.size(),
                                                       allocator);
                                argumentObject.AddMember("actual",
                                                         actualString,
                                                         allocator);
                            }
                            arguments.PushBack(argumentObject.Move(), allocator);
                        }

                        aggregationObject.AddMember("arguments",
                                                    arguments,
                                                    allocator);
                    }
                    {
                        Value aggregationValue(kNumberType);
                        aggregationValue.SetInt(aggregation);
                        aggregationObject.AddMember("aggregation",
                                                    aggregationValue,
                                                    allocator);
                    }
                    aggregations.PushBack(aggregationObject.Move(), allocator);
                }

                type.AddMember("aggregations", aggregations, allocator);
            }
        }

        classes.PushBack(type.Move(), allocator);
    }

    d.AddMember("classes", classes, allocator);

    std::ofstream f("/templex/result/results.json");
    OStreamWrapper osw(f);

    PrettyWriter<OStreamWrapper> writer(osw);
    d.Accept(writer);

    f.close();
}