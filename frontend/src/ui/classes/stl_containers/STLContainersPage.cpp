#include "STLContainersPage.h"
#include "ui_STLContainersPage.h"

#include "ui/BasicDelegate.h"

#include <QDebug>

using namespace templex;
using namespace templex::frontend;

STLContainersPage::STLContainersPage()
    : ui_(new Ui::STLContainersPage()),
      aggregationModel_(nullptr),
      aggregationProxy_(nullptr),
      aggregationTable_(nullptr)
{
}

void STLContainersPage::initialize()
{
    ui_->setupUi(this);

    instantiationCache_ =
        model::TypeCache::getInstance().getStlContainersInstantiationCache();
    aggregationCache_ =
        model::TypeCache::getInstance().getStlContainersAggregationCache();

    if (aggregationCache_.empty() && instantiationCache_.empty()) {
        ui_->stackedWidget->setCurrentIndex(1);
    } else {
        ui_->stackedWidget->setCurrentIndex(0);
    }

    aggregationModel_ = new AggregationModel(this);
    aggregationProxy_ = new AggregationProxy(this);
    locationModel_    = new InstantiationLocationModel(this);

    std::vector<model::TemplatePtr> templates;
    for (auto& [templatePtr, inst] : instantiationCache_) {
        templates.emplace_back(templatePtr);
    }

    aggregationTable_   = ui_->aggregationTable;
    sourceLocationList_ = ui_->sourceLocationList;

    templateSelector_ = new ClassTemplateSelector(templates, this);
    ui_->classTemplateSelectorWrapper->layout()->addWidget(templateSelector_);

    ui_->aggregationLabel->setProperty("label", "main");
    ui_->instantiationLabel->setProperty("label", "main");

    aggregationProxy_->setSourceModel(aggregationModel_);
    aggregationTable_->setModel(aggregationProxy_);
    aggregationTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    aggregationTable_->setSelectionMode(
        QAbstractItemView::SelectionMode::SingleSelection);
    aggregationTable_->setItemDelegate(new BasicDelegate(aggregationTable_));
    aggregationTable_->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    aggregationTable_->horizontalHeader()->setStretchLastSection(true);
    aggregationTable_->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Interactive);
    aggregationTable_->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    aggregationTable_->horizontalHeader()->setCascadingSectionResizes(true);
    aggregationTable_->horizontalHeader()->setDefaultSectionSize(210);
    aggregationTable_->verticalHeader()->setVisible(false);
    aggregationTable_->setSortingEnabled(true);

    sourceLocationList_->setModel(locationModel_);
    sourceLocationList_->setSelectionMode(
        QAbstractItemView::SelectionMode::SingleSelection);
    sourceLocationList_->setHorizontalScrollBarPolicy(
        Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    sourceLocationList_->setItemDelegate(new BasicDelegate(sourceLocationList_));

    connect(templateSelector_,
            &ClassTemplateSelector::selectionChanged,
            this,
            &STLContainersPage::onTemplateSelected);

    connect(aggregationTable_->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &STLContainersPage::onInstantiationSelected);

    templateSelector_->setCurrentIndex(0);
}

void STLContainersPage::onTemplateSelected(model::TemplatePtr templatePtr)
{
    selectedTemplate_ = templatePtr;

    auto instantiations = aggregationCache_[templatePtr];

    aggregationModel_->loadData(templatePtr, instantiations);

    aggregationTable_->selectRow(0);
}

void STLContainersPage::onInstantiationSelected()
{
    auto selection = aggregationTable_->selectionModel()->selectedRows();
    if (selection.size() != 1) {
        return;
    }

    auto allInstantiation = instantiationCache_[selectedTemplate_];
    auto selectedInstantiationArgs =
        aggregationModel_
            ->getInstantiation(aggregationProxy_->mapToSource(selection.first()))
            ->getActualParameters();

    std::vector<model::InstantiationPtr> data;
    for (auto instantiation : allInstantiation) {
        auto arguments = instantiation->getActualParameters();
        bool matches   = true;
        for (std::size_t i = 0; i < instantiation->getActualParameters().size();
             i++) {
            auto selectedActual = selectedInstantiationArgs[i]->getActualParameter();
            auto currentActual  = arguments[i]->getActualParameter();

            if (selectedActual != currentActual) {
                matches = false;
                break;
            }
        }

        if (matches)
            data.push_back(instantiation);
    }

    locationModel_->loadData(data);
}

void STLContainersPage::deinitialize()
{
    deleteLater();
}

QString STLContainersPage::pageName()
{
    return "STL Containers";
}
