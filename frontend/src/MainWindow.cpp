#include "MainWindow.h"

#include <QDebug>
#include <QPushButton>

#include <iostream>

#include "ui_MainWindow.h"

using namespace templex;
using namespace templex::frontend;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    MenuRegistry::buildMenuRegistry();

    initContext();
    initUi();
}

void MainWindow::initContext()
{
    // Basically load the json from the workdir.
}

void MainWindow::initUi()
{
    ui->setupUi(this);

    resize(1280, 768);

    initMenu();
}

void MainWindow::initMenu()
{
    for (auto& mainMenu : MenuRegistry::getMainMenu()) {
        auto* button = new QPushButton(this);
        button->setText(mainMenu.getLabel());
        button->setProperty("menu", "item");
        ui->topPanel->layout()->addWidget(button);

        connect(button, &QPushButton::clicked, this, [mainMenu, button, this] {
            setupSubMenus(mainMenu);

            for (auto* button : MenuRegistry::getMainMenuButtons()) {
                button->setEnabled(true);
            }

            button->setEnabled(false);
        });

        MenuRegistry::registerMainButton(mainMenu, button);
    }

    static_cast<QHBoxLayout*>(ui->topPanel->layout())->addStretch(1);

    auto* exitButton = new QPushButton(this);
    exitButton->setText("Exit");
    exitButton->setProperty("menu", "item");

    ui->topPanel->layout()->addWidget(exitButton);

    ui->logoMain->setText("Templex");
    ui->logoMain->setProperty("logo", "main");
    ui->logoSub->setText("static-analyzer");
    ui->logoSub->setProperty("logo", "sub");

    emit MenuRegistry::getMainMenuButtons().first()->clicked();
}

void MainWindow::setupSubMenus(const MenuRegistry::MenuData& mainMenuItem)
{
    while (QLayoutItem* item = ui->subMenuWrapper->layout()->takeAt(0)) {
        if (QWidget* widget = item->widget())
            widget->deleteLater();

        delete item;
    }

    MenuRegistry::clearSubMenu();

    for (auto& subMenu : MenuRegistry::getSubMenu(mainMenuItem)) {
        auto* button = new QPushButton(this);
        button->setText(subMenu.getLabel());
        button->setProperty("submenu", "item");
        ui->subMenuWrapper->layout()->addWidget(button);

        MenuRegistry::registerButton(subMenu, button);
    }

    static_cast<QBoxLayout*>(ui->subMenuWrapper->layout())->addStretch(1);

    emit MenuRegistry::getSubMenuButtons().first()->click();
}

MainWindow::~MainWindow()
{
    delete ui;
}
