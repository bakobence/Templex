#include "FunctionNamesModel.h"
#include <QSize>

using namespace templex;
using namespace templex::frontend;

FunctionNamesModel::FunctionNamesModel(const std::vector<std::string>& names,
                                       QObject* parent)
    : QAbstractListModel(parent),
      names_(names)
{
}

int FunctionNamesModel::rowCount(const QModelIndex&) const
{
    return names_.size();
}

QVariant FunctionNamesModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        return QString(names_[index.row()].data());
    }

    if (role == Qt::SizeHintRole) {
        return QSize(200, 40);
    }

    return QVariant();
}

void FunctionNamesModel::sort()
{
    emit layoutAboutToBeChanged();
    std::sort(names_.begin(), names_.end(), [](std::string left, std::string right) {
        return left < right;
    });
    emit layoutChanged();
}

std::string FunctionNamesModel::getItem(const QModelIndex& index) const
{
    if (index.row() < 0 || index.row() > rowCount()) {
        return nullptr;
    }

    return names_[index.row()];
}
