#include <QApplication>
#include <QFile>

#include "MainWindow.h"

using namespace templex;
using namespace templex::frontend;

auto main(int argc, char* argv[]) -> int
{
    QApplication app(argc, argv);
    QFile styleSheet(":/qss/style.qss");
    styleSheet.open(QFile::ReadOnly);
    QString style(styleSheet.readAll());
    app.setStyleSheet(style);

    MainWindow window;

    window.show();

    return app.exec();
}
