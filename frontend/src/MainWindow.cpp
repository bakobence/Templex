#include "MainWindow.h"

#include <QPushButton>

#include "helpers/MenuRegistry.h"
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

    showMaximized();

    initMenu();
}

void MainWindow::initMenu()
{
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    for (auto& mainMenu : MenuRegistry::getMainMenu()) {
        auto* wrapper      = new QFrame(this);
        auto* buttonLayout = new QHBoxLayout();
        auto* button       = new QPushButton(this);

        buttonLayout->setContentsMargins(0, 0, 0, 0);
        button->setText(mainMenu.getLabel());
        button->setProperty("menu", true);

        buttonLayout->addWidget(button);
        wrapper->setLayout(buttonLayout);
        layout->addWidget(wrapper);
    }

    layout->addStretch(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
