#pragma once

#include "common/model/Instantiation.h"
#include <QAbstractListModel>

namespace templex {
namespace frontend {

class InstantiationLocationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    InstantiationLocationModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    void loadData(const std::vector<model::InstantiationPtr>& data);
    void clear();

private:
    std::vector<model::InstantiationPtr> data_;
};

}
}

