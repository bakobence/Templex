#include "InstantiationLocationModel.h"

using namespace templex;
using namespace templex::frontend;

InstantiationLocationModel::InstantiationLocationModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int InstantiationLocationModel::rowCount(const QModelIndex&) const
{
    return data_.size();
}

QVariant InstantiationLocationModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (index.row() < 0 || index.row() > rowCount() - 1) {
            return QVariant();
        }

        auto instantiation = data_[index.row()];

        return QString(instantiation->getPointOfInstantiation().data());
    }

    return QVariant();
}

void InstantiationLocationModel::loadData(
    const std::vector<model::InstantiationPtr>& data)
{
    if (data.empty())
        return;

    clear();

    beginInsertRows(QModelIndex(), rowCount(), data.size() - 1);
    data_ = data;
    endInsertRows();
}
void InstantiationLocationModel::clear()
{
    if (rowCount() == 0)
        return;

    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    data_.clear();
    endRemoveRows();
}
