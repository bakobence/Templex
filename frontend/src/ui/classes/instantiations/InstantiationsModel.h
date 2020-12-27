#pragma once

#include <vector>

#include <QAbstractTableModel>

#include "common/model/Instantiation.h"

namespace templex {
namespace frontend {

class InstantiationsModel : public QAbstractTableModel {
    Q_OBJECT

public:
    InstantiationsModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QVariant
    headerData(int section, Qt::Orientation orientation, int role) const override;

    void loadData(model::TemplatePtr classTemplate,
                  const std::vector<model::InstantiationPtr>& data);
    void clear();

private:
    model::TemplatePtr classTemplate_;
    std::vector<model::InstantiationPtr> instantiations_;
};

} // namespace frontend
} // namespace templex
