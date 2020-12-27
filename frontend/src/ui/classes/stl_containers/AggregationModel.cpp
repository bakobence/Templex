#include "AggregationModel.h"
#include <QDebug>

using namespace templex;
using namespace templex::frontend;

AggregationModel::AggregationModel(QObject* parent)
    : QAbstractTableModel(parent),
      classTemplate_(nullptr),
      data_({})
{
}

void AggregationModel::clear()
{
    if (rowCount() == 0 && columnCount() == 0)
        return;

    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    data_.clear();
    endRemoveRows();

    beginRemoveColumns(QModelIndex(), 0, columnCount() - 1);
    classTemplate_ = nullptr;
    endRemoveColumns();
}

void AggregationModel::loadData(
    model::TemplatePtr classTemplate,
    const std::vector<std::pair<model::InstantiationPtr, int>>& data)
{
    if (data.empty() || classTemplate == nullptr)
        return;

    clear();

    beginInsertColumns(QModelIndex(),
                       columnCount(),
                       classTemplate->getParameterCount());
    classTemplate_ = classTemplate;
    endInsertColumns();

    beginInsertRows(QModelIndex(), rowCount(), data.size() - 1);
    data_ = data;
    endInsertRows();
}

int AggregationModel::rowCount(const QModelIndex&) const
{
    return data_.size();
}

int AggregationModel::columnCount(const QModelIndex&) const
{
    if (classTemplate_ == nullptr) {
        return 0;
    } else {
        return classTemplate_->getParameterCount() + 1;
    }
}

QVariant AggregationModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (index.row() < 0 || index.row() > rowCount() - 1) {
            return QVariant();
        }

        auto [instantiation, aggregation] = data_[index.row()];

        if (index.column() == columnCount() - 1) {
            return aggregation;
        } else {
            auto actualizedParameters = instantiation->getActualParameters();
            auto argumentIndex        = static_cast<std::size_t>(index.column());

            return QString(
                actualizedParameters[argumentIndex]->getActualParameter().data());
        }
    }

    return QVariant();
}

QVariant AggregationModel::headerData(int section,
                                      Qt::Orientation orientation,
                                      int role) const
{
    if (orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole) {
        if (section == columnCount() - 1) {
            return "Count";
        } else {
            auto parameterList = classTemplate_->getParameterList();
            auto parameter     = parameterList[section];

            return QString(parameter->getParameterName().data());
        }
    }

    return QVariant();
}

model::InstantiationPtr
AggregationModel::getInstantiation(const QModelIndex& index) const
{
    if (index.row() < 0 || index.row() > rowCount() - 1) {
        return nullptr;
    }

    return data_[index.row()].first;
}
