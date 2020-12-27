#include "AggregationProxy.h"

using namespace templex;
using namespace templex::frontend;

AggregationProxy::AggregationProxy(QObject* parent) : QSortFilterProxyModel(parent)
{
}

bool AggregationProxy::filterAcceptsRow(int, const QModelIndex&) const
{
    return true;
}

bool AggregationProxy::lessThan(const QModelIndex& left,
                                const QModelIndex& right) const
{
    auto leftData  = sourceModel()->data(left);
    auto rightData = sourceModel()->data(right);

    if (leftData.userType() == QMetaType::QString) {
        return leftData.toString() < rightData.toString();
    }

    return QSortFilterProxyModel::lessThan(left, right);
}
