#include "MainWindow.h"

#include <QDebug>
#include <QPushButton>

#include <iostream>

#include "ui_MainWindow.h"

using namespace templex;
using namespace templex::frontend;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      currentPage_(nullptr)
{
    qDebug() << "Initializing MainWindow";
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

    resize(1366, 768);

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

            for (auto* mainButtons : mainMenuButtons_.values()) {
                mainButtons->setEnabled(true);
            }

            button->setEnabled(false);
        });

        mainMenuButtons_[mainMenu] = button;
    }

    static_cast<QHBoxLayout*>(ui->topPanel->layout())->addStretch(1);

    auto* exitButton = new QPushButton(this);
    exitButton->setText("Exit");
    exitButton->setProperty("menu", "item");

    connect(exitButton, &QPushButton::clicked, this, [] {
        QCoreApplication::quit();
    });

    ui->topPanel->layout()->addWidget(exitButton);

    ui->logoMain->setText("Templex");
    ui->logoMain->setProperty("logo", "main");
    ui->logoSub->setText("static-analyzer");
    ui->logoSub->setProperty("logo", "sub");

    emit mainMenuButtons_[MenuRegistry::getMainMenu().first()]->click();
}

void MainWindow::setupSubMenus(const MenuRegistry::MenuData& mainMenuItem)
{

    while (QLayoutItem* item = ui->subMenuWrapper->layout()->takeAt(0)) {
        if (QWidget* widget = item->widget())
            widget->deleteLater();

        delete item;
    }

    subMenuButtons_.clear();

    for (auto& subMenu : MenuRegistry::getSubMenus(mainMenuItem)) {
        auto* button = new QPushButton(this);
        button->setText(subMenu.getLabel());
        button->setProperty("submenu", "item");
        ui->subMenuWrapper->layout()->addWidget(button);

        connect(button, &QPushButton::clicked, this, [this, button] {
            for (auto subMenu : subMenuButtons_.keys()) {
                if (auto mapButton = subMenuButtons_[subMenu]; mapButton == button) {
                    mapButton->setEnabled(false);
                    createPage(subMenu);
                } else {
                    mapButton->setEnabled(true);
                }
            }
        });

        subMenuButtons_[subMenu] = button;
    }

    static_cast<QBoxLayout*>(ui->subMenuWrapper->layout())->addStretch(1);

    emit subMenuButtons_[MenuRegistry::getSubMenus(mainMenuItem).first()]->click();
}

void MainWindow::createPage(const MenuRegistry::MenuData& subMenu)
{
    if (currentPage_ != nullptr) {
        currentPage_->deinitialize();
    }

    auto page = MenuRegistry::getCreator(subMenu)();

    page->initialize();

    currentPage_ = page;

    qDebug() << "Navigating to page: " << currentPage_->pageName();

    ui->contentWrapper->layout()->addWidget(currentPage_->widget());
}

MainWindow::~MainWindow()
{
    delete ui;
}
