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

void JSONImport::importCache()
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

    bool result = importClasses(document);

    qDebug() << "Result JSON process" << (result ? "succeeded" : "failed");

    model::TypeCache::getInstance().dump();
}

bool JSONImport::importClasses(const QJsonDocument& doc)
{
    auto& cache = model::TypeCache::getInstance();

    qDebug() << "Importing template classes from JSON";
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

            instantiationEntity->setClassTemplate(templateEntity);

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

            instantiationEntity->setClassTemplate(templateEntity);

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
