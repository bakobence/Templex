#pragma once

#include "common/model/Template.h"

#include <vector>

#include <QAbstractListModel>

namespace templex {
namespace frontend {

class TemplateModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit TemplateModel(const std::vector<model::TemplatePtr>& data,
                           QObject* parent);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QVariant
    headerData(int section, Qt::Orientation orientation, int role) const override;

    model::TemplatePtr getItem(const QModelIndex& index) const;

    void sort();

private:
    std::vector<model::TemplatePtr> templates_;
};

} // namespace frontend
} // namespace templex
