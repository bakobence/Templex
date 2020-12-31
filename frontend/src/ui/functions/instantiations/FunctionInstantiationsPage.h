#pragma once

#include "FunctionNamesModel.h"
#include "FunctionOverloadsModel.h"
#include "ui/PageBase.h"
#include "ui/classes/instantiations/InstantiationsModel.h"
#include "ui/common/class_template_selector/ClassNameFilterProxy.h"

#include <QListView>
#include <QTableView>

namespace templex {
namespace frontend {
namespace Ui {
class FunctionInstantiationsPage;
}

class FunctionInstantiationsPage : public PageBase {
    Q_OBJECT
public:
    FunctionInstantiationsPage();

    void initialize() override;
    void deinitialize() override;
    QString pageName() override;

private:
    void initModels();
    void initView();
    void initConnections();

private:
    Ui::FunctionInstantiationsPage* ui_;

    ClassNameFilterProxy* namesFilterProxy_;
    FunctionNamesModel* namesModel_;
    QListView* namesListView_;

    FunctionOverloadsModel* overloadsModel_;
    QListView* overloadsListView_;

    InstantiationsModel* tableModel_;
    QTableView* tableView_;
};

} // namespace frontend
} // namespace templex
