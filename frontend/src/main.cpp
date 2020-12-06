#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QtGlobal>

#include "MainWindow.h"

using namespace templex;
using namespace templex::frontend;

auto main(int argc, char* argv[]) -> int
{
    QString style;

    for (auto& name : {"style"}) {
        QFile sheet{QString(":/qss/%1.qss").arg(name)};
        sheet.open(QFile::ReadOnly);
        style += sheet.readAll();
    }

    QApplication app(argc, argv);
    app.setApplicationName("Templex client");
    app.setApplicationDisplayName("Templex client");
    app.setApplicationVersion("0.1.0");
    app.setStyleSheet(style);

    MainWindow window;

    window.show();

    return app.exec();
}
