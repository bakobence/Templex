#include "TemplateModel.h"

#include <QDebug>
#include <QSize>

using namespace templex;
using namespace templex::frontend;

TemplateModel::TemplateModel(const std::vector<model::TemplatePtr>& data,
                             QObject* parent)
    : QAbstractListModel(parent),
      templates_(data)
{
    sort();
}

int TemplateModel::rowCount(const QModelIndex&) const
{
    return templates_.size();
}

QVariant TemplateModel::data(const QModelIndex& index, int role) const
{

    auto classTemplate = templates_[index.row()];

    if (role == Qt::DisplayRole) {
        return classTemplate->getName().data();
    }

    if (role == Qt::SizeHintRole) {
        return QSize(200, 40);
    }

    return QVariant();
}

QVariant
TemplateModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return "Class templates";
    }
}

model::TemplatePtr TemplateModel::getItem(const QModelIndex& index) const
{
    if (index.row() < 0 || index.row() > rowCount()) {
        return nullptr;
    }

    return templates_[index.row()];
}

void TemplateModel::sort()
{
    emit layoutAboutToBeChanged();
    std::sort(templates_.begin(),
              templates_.end(),
              [](model::TemplatePtr left, model::TemplatePtr right) {
                  return left->getName() < right->getName();
              });

    emit layoutChanged();
}
