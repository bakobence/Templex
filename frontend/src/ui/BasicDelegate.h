#pragma once

#include <QStyledItemDelegate>

namespace templex {
namespace frontend {

class BasicDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    BasicDelegate(QObject* parent = nullptr);

    virtual ~BasicDelegate() override = default;

    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;

private:
    static constexpr int MARGIN = 8;
};

} // namespace frontend
} // namespace templex
