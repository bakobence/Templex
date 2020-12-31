#pragma once

#include <QAbstractListModel>
#include <vector>

namespace templex {
namespace frontend {

class FunctionNamesModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit FunctionNamesModel(const std::vector<std::string>& names,
                                QObject* parent);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    void sort();

    std::string getItem(const QModelIndex& index) const;

private:
    std::vector<std::string> names_;
};

} // namespace frontend
} // namespace templex
