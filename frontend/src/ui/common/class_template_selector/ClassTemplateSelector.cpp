#include "ClassTemplateSelector.h"
#include "ui_ClassTemplateSelector.h"

#include "common/cache/TypeCache.h"
#include "ui/BasicDelegate.h"

#include <QDebug>

using namespace templex;
using namespace templex::frontend;

ClassTemplateSelector::ClassTemplateSelector(
    const std::vector<model::TemplatePtr>& templates, QWidget* parent)
    : QWidget(parent),
      ui_(new Ui::ClassTemplateSelector()),
      filter(true)
{

    initialize(templates);
}

ClassTemplateSelector::ClassTemplateSelector(QWidget* parent)
    : QWidget(parent),
      ui_(new Ui::ClassTemplateSelector()),
      filter(false)
{
    initialize();
}

void ClassTemplateSelector::initialize(
    const std::vector<model::TemplatePtr>& templates)
{
    ui_->setupUi(this);

    auto& cache = model::TypeCache::getInstance();
    model_ = new TemplateModel(filter ? templates : cache.getClassTemplates(), this);
    proxyModel_ = new ClassNameFilterProxy(this);

    list_ = ui_->listView;

    ui_->bigLabel->setProperty("label", "main");

    proxyModel_->setSourceModel(model_);
    list_->setModel(proxyModel_);
    list_->setItemDelegate(new BasicDelegate(list_));
    list_->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    list_->setMinimumWidth(180);
    list_->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    connect(list_->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            [this] {
                auto selection = list_->selectionModel()->selectedIndexes();
                if (selection.size() != 1) {
                    return;
                }

                emit selectionChanged(
                    model_->getItem(proxyModel_->mapToSource(selection.first())));
            });

    connect(ui_->filterEdit, &QLineEdit::textChanged, this, [this] {
        proxyModel_->filterName(ui_->filterEdit->text());

        if (proxyModel_->rowCount() > 0) {
            list_->setCurrentIndex(proxyModel_->index(0, 0));
        } else {
            list_->clearSelection();
        }

        model_->sort();
    });
}

void ClassTemplateSelector::setCurrentIndex(int row)
{
    list_->setCurrentIndex(proxyModel_->index(row, 0));
}
