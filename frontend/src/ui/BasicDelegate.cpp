#include "BasicDelegate.h"

#include <QApplication>
#include <QPainter>

using namespace templex;
using namespace templex::frontend;

BasicDelegate::BasicDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

void BasicDelegate::paint(QPainter* painter,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const
{
    // QStyledItemDelegate::paint(painter, option, index);
    auto style = QApplication::style();
    auto rect  = option.rect;
    auto text  = index.data(Qt::DisplayRole).toString();

    auto bgColor = index.data(Qt::BackgroundColorRole);
    if (bgColor.isValid() && !(option.state & QStyle::State_Selected))
        painter->fillRect(option.rect,
                          QBrush{bgColor.value<QColor>(), Qt::SolidPattern});
    else
        style->drawPrimitive(QStyle::PE_PanelItemViewItem,
                             &option,
                             painter,
                             option.widget);

    rect.setLeft(rect.left() + MARGIN);
    rect.setRight(rect.right() - MARGIN);

    if (painter->fontMetrics().width(text) > rect.width()) {
        text = painter->fontMetrics().elidedText(text,
                                                 Qt::TextElideMode::ElideRight,
                                                 rect.width());
    }

    auto color = index.data(Qt::ForegroundRole);
    auto font  = index.data(Qt::FontRole);

    painter->save();

    if (color.isValid())
        painter->setPen(color.value<QColor>());

    if (font.isValid())
        painter->setFont(font.value<QFont>());

    style->drawItemText(painter,
                        rect,
                        Qt::AlignLeft | Qt::AlignVCenter,
                        style->standardPalette(),
                        true,
                        text);
    painter->restore();
}

QSize BasicDelegate::sizeHint(const QStyleOptionViewItem& option,
                              const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
