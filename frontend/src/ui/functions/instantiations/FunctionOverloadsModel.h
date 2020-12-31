#pragma once

#include "common/model/Template.h"

#include <vector>

#include <QAbstractListModel>

namespace templex {
namespace frontend {

class FunctionOverloadsModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit FunctionOverloadsModel(QObject* parent);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    void loadData(std::vector<model::TemplatePtr> data);
    void clear();

    model::TemplatePtr getItem(const QModelIndex& index) const;

private:
    std::vector<model::TemplatePtr> overloads_;
};

} // namespace frontend
} // namespace templex
