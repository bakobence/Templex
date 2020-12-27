#pragma once

#include <QHash>
#include <QMainWindow>
#include <QPushButton>

#include "helpers/MenuRegistry.h"

namespace templex {
namespace frontend {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void initContext();
    void initUi();

    void initMenu();
    void setupSubMenus(const MenuRegistry::MenuData& mainMenuItem);

    void createPage(const MenuRegistry::MenuData& subMenuItem);

private:
    Ui::MainWindow* ui;

    QHash<MenuRegistry::MenuData, QPushButton*> mainMenuButtons_;
    QHash<MenuRegistry::MenuData, QPushButton*> subMenuButtons_;

    std::shared_ptr<IPage> currentPage_;
};

} // namespace frontend
} // namespace templex
