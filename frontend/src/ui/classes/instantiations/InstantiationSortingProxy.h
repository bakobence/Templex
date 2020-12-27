#pragma once

#include <QSortFilterProxyModel>

namespace templex {
namespace frontend {

class InstantiationSortingProxy : public QSortFilterProxyModel {
    Q_OBJECT
public:
    InstantiationSortingProxy(QObject* parent = 0);

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex& sourceParent) const override;
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
};

} // namespace frontend
} // namespace templex
