#pragma once

#include "ui/PageBase.h"
#include "ui/common/class_template_selector/ClassTemplateSelector.h"

#include "InstantiationSortingProxy.h"
#include "InstantiationsModel.h"

#include <QListView>
#include <QTableView>

namespace templex {
namespace frontend {
namespace Ui {
class ClassInstantiationsPage;
}

class ClassInstantiationsPage : public PageBase {
    Q_OBJECT

public:
    ClassInstantiationsPage();

    void initialize() override;
    void deinitialize() override;
    QString pageName() override;

private:
    void initModels();
    void initView();
    void initConnections();

    void onClassTemplateSelected(model::TemplatePtr classTemplate);
    void onClassNameFilterChanged();

private:
    Ui::ClassInstantiationsPage* ui_;

    ClassTemplateSelector* classSelector_;

    QTableView* rightView_;
    InstantiationsModel* instantiationsModel_;
    InstantiationSortingProxy* instantiationProxyModel_;
};

} // namespace frontend
} // namespace templex
