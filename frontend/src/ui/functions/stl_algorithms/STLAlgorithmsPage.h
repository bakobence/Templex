#pragma once

#include "ui/PageBase.h"

namespace templex {
namespace frontend {
namespace Ui {
class STLAlgorithmsPage;
}

class STLAlgorithmsPage : public PageBase {
    Q_OBJECT

public:
    STLAlgorithmsPage();

    void initialize() override;
    void deinitialize() override;
    QString pageName() override;

private:
    Ui::STLAlgorithmsPage* ui_;
};

} // namespace frontend
} // namespace templex
