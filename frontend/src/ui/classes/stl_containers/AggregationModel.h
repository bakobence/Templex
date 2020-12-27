#pragma once

#include "common/model/Instantiation.h"

#include <QAbstractTableModel>

#include <vector>

namespace templex {
namespace frontend {

class AggregationModel : public QAbstractTableModel {
    Q_OBJECT
public:
    AggregationModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QVariant
    headerData(int section, Qt::Orientation orientation, int role) const override;

    void loadData(model::TemplatePtr classTemplate,
                  const std::vector<std::pair<model::InstantiationPtr, int>>& data);
    void clear();

    model::InstantiationPtr getInstantiation(const QModelIndex& index) const;

private:
    model::TemplatePtr classTemplate_;
    std::vector<std::pair<model::InstantiationPtr, int>> data_;
};

} // namespace frontend
} // namespace templex
