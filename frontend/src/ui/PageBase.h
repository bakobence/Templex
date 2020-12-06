#pragma once

#include <QWidget>

#include "ui/IPage.h"

namespace templex {
namespace frontend {

class PageBase
        : public QWidget
        , public IPage
{
public:
    PageBase(QObject* parent);

protected:
    void initialize() override;
    void deinitialize() override;
    QString pageName() override;
    QWidget* widget() override;

};

} // namespace frontend
} // namespace templex
