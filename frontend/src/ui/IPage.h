#pragma once

#include <QWidget>

class IPage {
public:

    virtual ~IPage() = default;

    virtual void initialize()   = 0;
    virtual void deinitialize() = 0;
    virtual QString pageName()  = 0;
    virtual QWidget* widget()   = 0;
};
