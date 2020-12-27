#pragma once

#include <QSet>
#include <QSortFilterProxyModel>

namespace templex {
namespace frontend {

class ClassNameFilterProxy : public QSortFilterProxyModel {
    Q_OBJECT
public:
    ClassNameFilterProxy(QObject* parent = 0);

    void filterName(const QString& name);

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex& sourceParent) const override;
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;

private:
    QString name_;

    QSet<int> filtered_;
};

} // namespace frontend
} // namespace templex
