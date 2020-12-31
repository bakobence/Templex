#include "ClassInstantiationsPage.h"
#include "ui_ClassInstantiationsPage.h"

#include "common/cache/TypeCache.h"
#include "ui/BasicDelegate.h"

#include <QDebug>

using namespace templex;
using namespace templex::frontend;

ClassInstantiationsPage::ClassInstantiationsPage()
    : ui_(new Ui::ClassInstantiationsPage())
{
}

void ClassInstantiationsPage::initialize()
{
    ui_->setupUi(this);

    initModels();
    initView();
    initConnections();

    classSelector_->setCurrentIndex(0);
}

void ClassInstantiationsPage::initModels()
{
    instantiationsModel_     = new InstantiationsModel(this);
    instantiationProxyModel_ = new InstantiationSortingProxy(this);
}

void ClassInstantiationsPage::initView()
{
    classSelector_ = new ClassTemplateSelector(this);
    ui_->classTemplateSelectorWrapper->layout()->addWidget(classSelector_);

    rightView_ = ui_->rightPane;

    ui_->rightLabel->setProperty("label", "main");

    instantiationProxyModel_->setSourceModel(instantiationsModel_);
    rightView_->setModel(instantiationProxyModel_);
    rightView_->setSelectionMode(QAbstractItemView::NoSelection);
    rightView_->setAlternatingRowColors(true);
    rightView_->setItemDelegate(new BasicDelegate(rightView_));
    rightView_->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    rightView_->horizontalHeader()->setStretchLastSection(true);
    rightView_->horizontalHeader()->setDefaultSectionSize(210);
    rightView_->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    rightView_->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    rightView_->horizontalHeader()->setCascadingSectionResizes(true);
    rightView_->verticalHeader()->setVisible(false);
    rightView_->setSortingEnabled(true);
}

void ClassInstantiationsPage::initConnections()
{
    connect(classSelector_,
            &ClassTemplateSelector::selectionChanged,
            this,
            &ClassInstantiationsPage::onClassTemplateSelected);
}

void ClassInstantiationsPage::onClassTemplateSelected(
    model::TemplatePtr classTemplate)
{
    auto instantiations =
        model::TypeCache::getInstance().getInstantiationsFor(classTemplate);

    instantiationsModel_->loadData(classTemplate, instantiations);
}

void ClassInstantiationsPage::deinitialize()
{
    deleteLater();
}

QString ClassInstantiationsPage::pageName()
{
    return "Class Instantiations";
}
