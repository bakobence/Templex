#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QtGlobal>

#include "ui/MainWindow.h"
#include "util/JSONImport.h"

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
    app.setApplicationName("Templex frontend");
    app.setApplicationDisplayName("Templex frontend");
    app.setApplicationVersion("0.1.0");
    app.setStyleSheet(style);

    bool success = JSONImport::importCache();

    if (!success) {
        QMessageBox::critical(
            nullptr,
            "Error",
            "Unable to parse the input JSON file, the program will now exit.");

        return 1;
    }

    qDebug() << "Starting: Templex frontend 0.1.0";

    MainWindow window;

    qDebug() << "Creating MainWindow";

    window.show();

    return app.exec();
}
