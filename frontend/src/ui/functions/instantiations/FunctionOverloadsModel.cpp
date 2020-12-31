#include "FunctionOverloadsModel.h"

#include <QSize>

using namespace templex;
using namespace templex::frontend;

FunctionOverloadsModel::FunctionOverloadsModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int FunctionOverloadsModel::rowCount(const QModelIndex&) const
{
    return overloads_.size();
}

QVariant FunctionOverloadsModel::data(const QModelIndex& index, int role) const
{

    auto overloadTemplate = overloads_[index.row()];

    if (role == Qt::DisplayRole) {
        return overloadTemplate->getName().data();
    }

    if (role == Qt::SizeHintRole) {
        return QSize(200, 40);
    }

    return QVariant();
}

void FunctionOverloadsModel::clear()
{
    if (rowCount() == 0)
        return;

    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    overloads_.clear();
    endRemoveRows();
}

void FunctionOverloadsModel::loadData(std::vector<model::TemplatePtr> data)
{
    if (data.empty())
        return;

    clear();

    beginInsertRows(QModelIndex(), rowCount(), data.size() - 1);
    overloads_ = data;
    endInsertRows();
}

model::TemplatePtr FunctionOverloadsModel::getItem(const QModelIndex& index) const
{
    if (index.row() < 0 || index.row() > rowCount()) {
        return nullptr;
    }

    return overloads_[index.row()];
}
