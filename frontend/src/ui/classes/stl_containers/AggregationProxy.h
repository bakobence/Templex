#pragma once

#include <QSortFilterProxyModel>

namespace templex {
namespace frontend {

class AggregationProxy : public QSortFilterProxyModel {
    Q_OBJECT
public:
    AggregationProxy(QObject* parent = 0);

    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex& sourceParent) const override;
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
};

} // namespace frontend
} // namespace templex
