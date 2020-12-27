#pragma once

#include <QListView>

#include "ClassNameFilterProxy.h"
#include "TemplateModel.h"
#include "common/model/Template.h"

namespace templex {
namespace frontend {
namespace Ui {
class ClassTemplateSelector;
}

class ClassTemplateSelector : public QWidget {
    Q_OBJECT
public:
    ClassTemplateSelector(const std::vector<model::TemplatePtr>& templates,
                          QWidget* parent = nullptr);

    ClassTemplateSelector(QWidget* parent = nullptr);

    void setCurrentIndex(int row);

signals:
    void selectionChanged(model::TemplatePtr classTemplate);

private:
    void initialize(const std::vector<model::TemplatePtr>& templates = {});

private:
    Ui::ClassTemplateSelector* ui_;

    QListView* list_;
    TemplateModel* model_;
    ClassNameFilterProxy* proxyModel_;

    bool filter;
};

} // namespace frontend
} // namespace templex
