#include "InstantiationsModel.h"

#include <QDebug>
#include <QSize>

using namespace templex;
using namespace templex::frontend;

InstantiationsModel::InstantiationsModel(QObject* parent)
    : QAbstractTableModel(parent),
      classTemplate_(nullptr)
{
}

void InstantiationsModel::clear()
{
    if (rowCount() == 0 && columnCount() == 0)
        return;

    // This functions removes every row and column from the model.
    // Starting with the rows first for performance reasons.

    // Removing rows.
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    instantiations_.clear();
    endRemoveRows();

    // Column number is determined by the class template.
    beginRemoveColumns(QModelIndex(), 0, columnCount() - 1);
    classTemplate_ = nullptr;
    endRemoveColumns();
}

void InstantiationsModel::loadData(model::TemplatePtr classTemplate,
                                   const std::vector<model::InstantiationPtr>& data)
{
    if (data.empty() || classTemplate == nullptr)
        return;

    // Before populating, we clear the entire model.
    clear();

    // Adding columns first.
    beginInsertColumns(QModelIndex(),
                       columnCount(),
                       classTemplate->getParameterCount());
    classTemplate_ = classTemplate;
    endInsertColumns();

    // Now the rows.
    beginInsertRows(QModelIndex(), rowCount(), data.size() - 1);
    instantiations_ = data;
    endInsertRows();
}

int InstantiationsModel::rowCount(const QModelIndex&) const
{
    return instantiations_.size();
}

int InstantiationsModel::columnCount(const QModelIndex&) const
{
    if (classTemplate_ == nullptr) {
        return 0;
    } else {
        return classTemplate_->getParameterCount() + 1;
    }
}

QVariant InstantiationsModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
        if (index.row() < 0 || index.row() > (int)instantiations_.size() - 1) {
            return QVariant();
        }

        auto instantiation = instantiations_[index.row()];

        if (index.column() == 0) {
            return QString(instantiation->getPointOfInstantiation().data());
        } else {
            auto actualParameters = instantiation->getActualParameters();
            auto argumentIndex    = static_cast<std::size_t>(index.column());

            return QString(
                actualParameters[argumentIndex - 1]->getActualParameter().data());
        }
    }

    return QVariant();
}

QVariant InstantiationsModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const
{
    if (orientation == Qt::Orientation::Horizontal) {
        if (role == Qt::DisplayRole) {
            auto parameters = classTemplate_->getParameterList();

            if (section == 0) {
                return "Point of instantiation";
            }

            auto parameter = parameters[section - 1];

            return QString(parameter->getParameterName().data());
        }
    }

    return QVariant();
}
