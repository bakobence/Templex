#include "InstantiationSortingProxy.h"

#include <QDebug>
using namespace templex;
using namespace templex::frontend;

InstantiationSortingProxy::InstantiationSortingProxy(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

bool InstantiationSortingProxy::filterAcceptsRow(int, const QModelIndex&) const
{
    return true;
}
bool InstantiationSortingProxy::lessThan(const QModelIndex& left,
                                         const QModelIndex& right) const
{
    auto leftData  = sourceModel()->data(left);
    auto rightData = sourceModel()->data(right);

    if (leftData.userType() == QMetaType::QString) {
        return leftData.toString() < rightData.toString();
    }

    return QSortFilterProxyModel::lessThan(left, right);
}
