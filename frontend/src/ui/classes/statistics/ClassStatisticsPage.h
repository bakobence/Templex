#pragma once

#include "ui/PageBase.h"
#include "ui/common/class_template_selector/ClassTemplateSelector.h"

#include <QtCharts>

namespace templex {
namespace frontend {
namespace Ui {
class ClassStatisticsPage;
}

class ClassStatisticsPage : public PageBase {
    Q_OBJECT
public:
    ClassStatisticsPage();

    void initialize() override;
    void deinitialize() override;
    QString pageName() override;

    void
    onClassTemplateSelected(std::shared_ptr<model::Template> classTemplate);

    void onBarHovered(bool status, int index, QBarSet* set);

private:
    Ui::ClassStatisticsPage* ui_;

    ClassTemplateSelector* classSelector_;
    std::shared_ptr<model::Template> currentClassTemplate_;

    QBarSet* set_;
    QBarSeries* series_;
    QChart* chart_;
    QBarCategoryAxis* x_;
    QValueAxis* y_;
};

} // namespace frontend
} // namespace templex
