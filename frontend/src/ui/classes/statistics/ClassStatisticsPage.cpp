#include "ClassStatisticsPage.h"
#include "ui_ClassStatisticsPage.h"

#include "common/cache/TypeCache.h"

#include <QDebug>
#include <QtCharts>

using namespace templex;
using namespace templex::frontend;

ClassStatisticsPage::ClassStatisticsPage()
    : ui_(new Ui::ClassStatisticsPage()),
      currentClassTemplate_(nullptr),
      set_(nullptr),
      series_(nullptr),
      chart_(nullptr),
      x_(nullptr),
      y_(nullptr)
{
}

void ClassStatisticsPage::initialize()
{
    ui_->setupUi(this);

    classSelector_ = new ClassTemplateSelector(this);
    ui_->classTemplateSelector->layout()->addWidget(classSelector_);

    ui_->chartsLabel->setProperty("label", "main");

    set_    = new QBarSet("Number of instantiations of given argument set");
    series_ = new QBarSeries();
    series_->append(set_);
    chart_ = new QChart();
    chart_->addSeries(series_);
    chart_->setAnimationOptions(QChart::SeriesAnimations);

    x_ = new QBarCategoryAxis();
    chart_->addAxis(x_, Qt::AlignBottom);
    series_->attachAxis(x_);
    y_ = new QValueAxis();
    y_->setTickAnchor(0);
    chart_->addAxis(y_, Qt::AlignLeft);
    series_->attachAxis(y_);

    chart_->legend()->setVisible(true);
    chart_->legend()->setAlignment(Qt::AlignBottom);

    ui_->chartView->setChart(chart_);
    ui_->chartView->setRenderHint(QPainter::Antialiasing);
    ui_->chartView->setRubberBand(QChartView::HorizontalRubberBand);

    connect(x_,
            &QBarCategoryAxis::rangeChanged,
            this,
            [this](const QString& min, const QString& max) {
                int minIndex = 0;
                int maxIndex = 0;
                for (int i = 0; i < x_->count(); i++) {
                    if (x_->at(i) == min)
                        minIndex = i;
                    else if (x_->at(i) == max)
                        maxIndex = i;
                }

                int maxRange = 0;
                for (int i = minIndex; i <= maxIndex; i++) {
                    if (set_->at(i) > maxRange)
                        maxRange = set_->at(i);
                }

                y_->setRange(0, maxRange);
            });

    connect(classSelector_,
            &ClassTemplateSelector::selectionChanged,
            this,
            &ClassStatisticsPage::onClassTemplateSelected);

    connect(series_, &QBarSeries::hovered, this, &ClassStatisticsPage::onBarHovered);

    classSelector_->setCurrentIndex(0);
}

void ClassStatisticsPage::onClassTemplateSelected(
    std::shared_ptr<model::Template> classTemplate)
{
    currentClassTemplate_ = classTemplate;
    currentArguments_.clear();

    auto aggregation =
        model::TypeCache::getInstance().getAggregationsFor(classTemplate);

    chart_->setTitle(classTemplate->getName().data());

    set_->remove(0, set_->count());
    x_->clear();
    int max = 1;
    for (auto& [instantiation, aggregation] : aggregation) {
        if (aggregation > max)
            max = aggregation;

        QString label;
        std::vector<std::string> arguments;
        for (auto argument : instantiation->getActualParameters()) {
            label.append(QString("%1=%2 ")
                             .arg(argument->getParameterName().data())
                             .arg(argument->getActualParameter().data()));
            arguments.push_back(argument->getActualParameter());
        }
        label.replace('<', "&lt;");
        label.replace('>', "&gt;");
        currentArguments_.emplace_back(arguments);
        label.remove(label.size() - 1, 1);
        *set_ << aggregation;
        x_->append(label);
    }

    y_->setRange(0, max);
    y_->setTickAnchor(0);
}

void ClassStatisticsPage::onBarHovered(bool status, int index, QBarSet* set)
{
    Q_UNUSED(set);

    if (!status)
        return;

    if (currentClassTemplate_ == nullptr)
        return;

    auto className = QString(currentClassTemplate_->getName().data());
    auto label     = x_->at(index);
    className.append('<');
    for (auto& arg : currentArguments_[index]) {
        className.append(arg.data());
        className.append(',');
    }
    className.remove(className.size() - 1, 1);
    className.append("> count: ");
    className.append(QString::number(set_->at(index)));
    className.replace('<', "&lt;");
    className.replace('>', "&gt;");
    chart_->setTitle(className);
}

void ClassStatisticsPage::deinitialize()
{
    deleteLater();
}

QString ClassStatisticsPage::pageName()
{
    return "Class Statistics";
}
