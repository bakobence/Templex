#include "FunctionInstantiationsPage.h"
#include "ui_FunctionInstantiationsPage.h"

#include "common/cache/TypeCache.h"
#include "ui/BasicDelegate.h"

using namespace templex;
using namespace templex::frontend;

FunctionInstantiationsPage::FunctionInstantiationsPage()
    : ui_(new Ui::FunctionInstantiationsPage()),
      namesModel_(nullptr)
{
}

void FunctionInstantiationsPage::initialize()
{
    ui_->setupUi(this);

    initModels();
    initView();
    initConnections();

    namesListView_->setCurrentIndex(namesFilterProxy_->index(0, 0));
}

void FunctionInstantiationsPage::initModels()
{
    auto& cache        = model::TypeCache::getInstance();
    auto functionNames = cache.getFunctionNames();

    namesModel_       = new FunctionNamesModel(functionNames, this);
    namesFilterProxy_ = new ClassNameFilterProxy(this);
    overloadsModel_   = new FunctionOverloadsModel(this);
    tableModel_       = new InstantiationsModel(this);
}

void FunctionInstantiationsPage::initView()
{
    ui_->templatesLabel->setProperty("label", "main");
    ui_->overloadsLabel->setProperty("label", "main");
    ui_->instantiationsLabel->setProperty("label", "main");

    namesListView_     = ui_->templateList;
    overloadsListView_ = ui_->overloadsList;
    tableView_         = ui_->tableView;

    namesFilterProxy_->setSourceModel(namesModel_);
    namesListView_->setModel(namesFilterProxy_);
    namesListView_->setItemDelegate(new BasicDelegate(namesListView_));
    namesListView_->setHorizontalScrollBarPolicy(
        Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    namesListView_->setMinimumWidth(180);
    namesListView_->setSelectionMode(
        QAbstractItemView::SelectionMode::SingleSelection);

    overloadsListView_->setModel(overloadsModel_);
    overloadsListView_->setItemDelegate(new BasicDelegate(overloadsListView_));
    overloadsListView_->setHorizontalScrollBarPolicy(
        Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    overloadsListView_->setMinimumWidth(180);
    overloadsListView_->setSelectionMode(
        QAbstractItemView::SelectionMode::SingleSelection);

    tableView_->setModel(tableModel_);
    tableView_->setSelectionMode(QAbstractItemView::NoSelection);
    tableView_->setAlternatingRowColors(true);
    tableView_->setItemDelegate(new BasicDelegate(tableView_));
    tableView_->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableView_->horizontalHeader()->setStretchLastSection(true);
    tableView_->horizontalHeader()->setDefaultSectionSize(210);
    tableView_->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tableView_->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    tableView_->horizontalHeader()->setCascadingSectionResizes(true);
    tableView_->verticalHeader()->setVisible(false);
}

void FunctionInstantiationsPage::initConnections()
{
    connect(
        overloadsListView_->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        this,
        [this] {
            auto selection = overloadsListView_->selectionModel()->selectedIndexes();
            if (selection.size() != 1)
                return;

            auto nameSelection = namesListView_->selectionModel()->selectedIndexes();
            if (nameSelection.size() != 1)
                return;

            auto index = namesFilterProxy_->mapToSource(nameSelection.first());
            auto functionName = namesModel_->getItem(index);

            auto overload = overloadsModel_->getItem(selection.first());

            auto& cache = model::TypeCache::getInstance();
            auto instantiations =
                cache.getFunctionOverloadInstantiationsFor(overload, functionName);

            tableModel_->loadData(overload, instantiations);
        });
    connect(namesListView_->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            [this] {
                auto selection = namesListView_->selectionModel()->selectedIndexes();
                if (selection.size() != 1)
                    return;

                auto index = namesFilterProxy_->mapToSource(selection.first());
                auto functionName = namesModel_->getItem(index);

                auto& cache    = model::TypeCache::getInstance();
                auto overloads = cache.getOverloadTempaltesForFunction(functionName);

                overloadsModel_->loadData(overloads);
                overloadsListView_->setCurrentIndex(overloadsModel_->index(0, 0));
            });

    connect(ui_->filterEdit, &QLineEdit::textChanged, this, [this] {
        namesFilterProxy_->filterName(ui_->filterEdit->text());

        if (namesFilterProxy_->rowCount() > 0) {
            namesListView_->setCurrentIndex(namesFilterProxy_->index(0, 0));
        } else {
            namesListView_->clearSelection();
        }

        namesModel_->sort();
    });
}

void FunctionInstantiationsPage::deinitialize()
{
    deleteLater();
}
QString FunctionInstantiationsPage::pageName()
{
    return "Function Instantiations";
}
