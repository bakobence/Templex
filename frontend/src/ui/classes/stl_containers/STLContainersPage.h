#pragma once

#include "AggregationModel.h"
#include "AggregationProxy.h"
#include "InstantiationLocationModel.h"
#include "common/cache/TypeCache.h"
#include "ui/PageBase.h"
#include "ui/common/class_template_selector/ClassTemplateSelector.h"

#include <QListView>
#include <QTableView>

namespace templex {
namespace frontend {
namespace Ui {
class STLContainersPage;
}

class STLContainersPage : public PageBase {
    Q_OBJECT

public:
    STLContainersPage();

    void initialize() override;
    void deinitialize() override;
    QString pageName() override;

private:
    void onTemplateSelected(model::TemplatePtr templatePtr);
    void onInstantiationSelected();

private:
    Ui::STLContainersPage* ui_;

    AggregationModel* aggregationModel_;
    AggregationProxy* aggregationProxy_;
    QTableView* aggregationTable_;

    InstantiationLocationModel* locationModel_;
    QListView* sourceLocationList_;

    ClassTemplateSelector* templateSelector_;
    model::TemplatePtr selectedTemplate_;

    model::TypeCache::InstantiationsCache instantiationCache_;
    model::TypeCache::InstantiationAggregationsCache aggregationCache_;
};

} // namespace frontend
} // namespace templex
