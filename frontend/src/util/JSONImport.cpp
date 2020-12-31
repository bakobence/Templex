#include "JSONImport.h"

#include <stdexcept>

#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>

#include "common/cache/TypeCache.h"
#include "common/model/Instantiation.h"
#include "common/model/Template.h"
#include "common/model/TemplateParameter.h"

using namespace templex;
using namespace templex::frontend;

bool JSONImport::importCache()
{
    auto arguments = QCoreApplication::arguments();
    if (!arguments.contains("-p"))
        throw new std::runtime_error("Input JSON not specified");

    int index = arguments.indexOf("-p") + 1;

    if (index > arguments.size() - 1)
        throw new std::runtime_error("Input JSON not specified");

    QString path = arguments[index];
    QFile file(path);

    if (!file.exists()) {
        throw new std::runtime_error("Specified JSON file not found.");
    }

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    auto d = file.readAll();
    file.close();

    auto document = QJsonDocument::fromJson(d);

    bool classResult    = importClasses(document);
    bool functionResult = importFunctions(document);

    model::TypeCache::getInstance().dump();

    return classResult && functionResult;
}

bool JSONImport::importClasses(const QJsonDocument& doc)
{
    auto& cache = model::TypeCache::getInstance();

    qDebug() << "Importing class templates from JSON";
    auto object = doc.object();

    if (!object.contains(CLASSES) || !object[CLASSES].isArray()) {
        return false;
    }

    auto classes = object[CLASSES].toArray();

    for (auto classObject : classes) {
        if (!classObject.isObject())
            continue;

        auto templateEntity = std::make_shared<model::Template>();
        auto classTemplate  = classObject.toObject();

        if (!classTemplate.contains(CLASS_NAME) ||
            !classTemplate[CLASS_NAME].isString()) {
            return false;
        }

        auto className = classTemplate[CLASS_NAME].toString();
        templateEntity->setName(className.toStdString());

        if (!classTemplate.contains(PARAMETERS) ||
            !classTemplate[PARAMETERS].isArray()) {
            return false;
        }

        cache.addClassTemplate(templateEntity);

        // Read the template parameter list.
        auto parameters = classTemplate[PARAMETERS].toArray();
        for (auto parameter : parameters) {
            if (!parameter.isObject())
                continue;

            auto parameterEntity = std::make_shared<model::TemplateParameter>();
            auto object          = parameter.toObject();

            if (!object.contains(KIND) || !object[KIND].isString()) {
                return false;
            }

            auto kind = object[KIND].toString();
            parameterEntity->setType(kind.toStdString());

            if (!object.contains(NAME) || !object[NAME].isString()) {
                return false;
            }

            auto name = object[NAME].toString();
            parameterEntity->setParameterName(name.toStdString());

            templateEntity->addParameter(parameterEntity);
        }

        if (!classTemplate.contains(INSTANTIATIONS) ||
            !classTemplate[INSTANTIATIONS].isArray()) {
            return false;
        }

        // Read the actual arguments of the template parameter list.
        auto instantiations = classTemplate[INSTANTIATIONS].toArray();
        for (auto instantiation : instantiations) {
            if (!instantiation.isObject())
                continue;

            auto instantiationEntity = std::make_shared<model::Instantiation>();
            auto object              = instantiation.toObject();

            instantiationEntity->setTemplate(templateEntity);

            if (!object.contains(POI) || !object[POI].isString()) {
                return false;
            }

            auto poi = object[POI].toString();
            instantiationEntity->setPointOfInstantiation(poi.toStdString());

            if (!object.contains(ARGUMENTS) || !object[ARGUMENTS].isArray()) {
                return false;
            }

            auto args = object[ARGUMENTS].toArray();
            for (int i = 0; i < args.size(); i++) {
                if (!args[i].isObject())
                    continue;

                auto argumentObject = args[i].toObject();

                if (!argumentObject.contains(ACTUAL) ||
                    !argumentObject[ACTUAL].isString()) {
                    return false;
                }

                auto actual = argumentObject[ACTUAL].toString();

                instantiationEntity->setActualParameter(i, actual.toStdString());
            }

            cache.addClassInstantiation(instantiationEntity);
        }

        if (!classTemplate.contains(AGGREGATIONS) ||
            !classTemplate[AGGREGATIONS].isArray()) {
            return false;
        }

        auto aggregations = classTemplate[AGGREGATIONS].toArray();
        for (auto aggregation : aggregations) {
            if (!aggregation.isObject())
                continue;

            auto instantiationEntity = std::make_shared<model::Instantiation>();
            auto object              = aggregation.toObject();

            instantiationEntity->setTemplate(templateEntity);

            if (!object.contains(ARGUMENTS) || !object[ARGUMENTS].isArray()) {
                return false;
            }

            auto arguments = object[ARGUMENTS].toArray();
            for (int i = 0; i < arguments.size(); i++) {
                if (!arguments[i].isObject())
                    continue;

                auto argumentObject = arguments[i].toObject();

                if (!argumentObject.contains(ACTUAL) ||
                    !argumentObject[ACTUAL].isString()) {
                    return false;
                }

                auto actual = argumentObject[ACTUAL].toString();
                instantiationEntity->setActualParameter(i, actual.toStdString());
            }

            if (!object.contains(AGGREGATION) ||
                !object[AGGREGATION].toVariant().canConvert<int>()) {
                return false;
            }

            int aggregationValue = object[AGGREGATION].toVariant().toInt();

            cache.addClassInstantiationAggregation(instantiationEntity,
                                                   aggregationValue);
        }
    }

    return true;
}

bool JSONImport::importFunctions(const QJsonDocument& doc)
{
    auto& cache = model::TypeCache::getInstance();

    qDebug() << "Importing function templates from JSON";

    auto object = doc.object();

    if (!object.contains(FUNCTIONS) || !object[FUNCTIONS].isArray()) {
        return false;
    }

    auto functions = object[FUNCTIONS].toArray();

    for (auto functionObject : functions) {
        if (!functionObject.isObject())
            continue;

        auto function = functionObject.toObject();

        if (!function.contains(FUNCTION_NAME) ||
            !function[FUNCTION_NAME].isString()) {
            return false;
        }

        auto functionName = function[FUNCTION_NAME].toString().toStdString();

        if (!function.contains(OVERLOADS) || !function[OVERLOADS].isArray()) {
            return false;
        }

        auto overloads = function[OVERLOADS].toArray();

        for (auto overloadObject : overloads) {
            if (!overloadObject.isObject())
                continue;

            auto templateEntity = std::make_shared<model::Template>();
            auto overload       = overloadObject.toObject();

            if (!overload.contains(COMPLETE_FUNCTION_SIGNATURE) ||
                !overload[COMPLETE_FUNCTION_SIGNATURE].isString()) {
                return false;
            }

            if (!overload.contains(PARAMETERS) || !overload[PARAMETERS].isArray()) {
                return false;
            }

            if (!overload.contains(INSTANTIATIONS) ||
                !overload[INSTANTIATIONS].isArray()) {
                return false;
            }

            auto functionSignature =
                overload[COMPLETE_FUNCTION_SIGNATURE].toString();
            auto parameters     = overload[PARAMETERS].toArray();
            auto instantiations = overload[INSTANTIATIONS].toArray();

            templateEntity->setName(functionSignature.toStdString());
            cache.addFunctionTemplate(templateEntity, functionName);

            for (auto parameterObject : parameters) {
                if (!parameterObject.isObject())
                    continue;

                auto parameterEntity = std::make_shared<model::TemplateParameter>();
                auto parameter       = parameterObject.toObject();

                if (!parameter.contains(KIND) || !parameter[KIND].isString()) {
                    return false;
                }

                if (!parameter.contains(NAME) || !parameter[NAME].isString()) {
                    return false;
                }

                auto kind = parameter[KIND].toString().toStdString();
                auto name = parameter[NAME].toString().toStdString();

                parameterEntity->setType(kind);
                parameterEntity->setParameterName(name);
                templateEntity->addParameter(parameterEntity);
            }

            for (auto instantiationObject : instantiations) {
                if (!instantiationObject.isObject())
                    continue;

                auto instantiationEntity = std::make_shared<model::Instantiation>();
                auto instantiation       = instantiationObject.toObject();

                if (!instantiation.contains(POI) || !instantiation[POI].isString()) {
                    return false;
                }

                if (!instantiation.contains(ARGUMENTS) ||
                    !instantiation[ARGUMENTS].isArray()) {
                    return false;
                }

                instantiationEntity->setTemplate(templateEntity);

                auto poi       = instantiation[POI].toString().toStdString();
                auto arguments = instantiation[ARGUMENTS].toArray();

                instantiationEntity->setPointOfInstantiation(poi);

                for (int i = 0; i < arguments.size(); i++) {
                    auto argumentObject = arguments[i];
                    if (!argumentObject.isObject())
                        continue;

                    auto argument = argumentObject.toObject();

                    if (!argument.contains(ACTUAL) || !argument[ACTUAL].isString()) {
                        return false;
                    }

                    auto actual = argument[ACTUAL].toString().toStdString();

                    instantiationEntity->setActualParameter(i, actual);
                }

                cache.addFunctionInstantiation(instantiationEntity, functionName);
            }
        }
    }

    return true;
}
