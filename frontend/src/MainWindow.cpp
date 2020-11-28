#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QPushButton>

#include "helpers/MenuRegistry.h"

using namespace templex;
using namespace templex::frontend;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
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

    MenuRegistry::buildMenuRegistry();

    initTopMenu();
    initLeftMenu();
}

void MainWindow::initTopMenu()
{
    auto mainMenuItems = MenuRegistry::getMainMenu();
    for (auto& name : mainMenuItems) {
        QFrame* wrapper     = new QFrame(this);
        QHBoxLayout* layout = new QHBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);

        QPushButton* button = new QPushButton(this);
        button->setText(name);

        layout->addWidget(button);

        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        wrapper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        wrapper->setLayout(layout);
    }
}

void MainWindow::initLeftMenu() {}

MainWindow::~MainWindow()
{
    delete ui;
}
