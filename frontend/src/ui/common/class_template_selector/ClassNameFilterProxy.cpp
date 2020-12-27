#include "ClassNameFilterProxy.h"
#include <QDebug>
using namespace templex;
using namespace templex::frontend;

ClassNameFilterProxy::ClassNameFilterProxy(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

void ClassNameFilterProxy::filterName(const QString& name)
{
    name_ = name;
    invalidateFilter();
}

bool ClassNameFilterProxy::lessThan(const QModelIndex& left,
                                    const QModelIndex& right) const
{
    auto leftData  = sourceModel()->data(left);
    auto rightData = sourceModel()->data(right);

    if (leftData.userType() == QMetaType::QString) {
        return leftData.toString() < rightData.toString();
    }

    return QSortFilterProxyModel::lessThan(left, right);
}

bool ClassNameFilterProxy::filterAcceptsRow(int sourceRow, const QModelIndex&) const
{
    auto index = sourceModel()->index(sourceRow, 0);
    auto data  = sourceModel()->data(index);

    return data.toString().contains(name_, Qt::CaseSensitivity::CaseInsensitive);
}
