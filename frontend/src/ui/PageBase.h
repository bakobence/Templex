#pragma once

#include "ui/IPage.h"

namespace templex {
namespace frontend {

class PageBase
        : public QWidget
        , public IPage
{
    Q_OBJECT

public:
    PageBase();
    ~PageBase();

protected:
    void initialize() override;
    void deinitialize() override;
    QString pageName() override;
    QWidget* widget() override { return this; }

};

} // namespace frontend
} // namespace templex
