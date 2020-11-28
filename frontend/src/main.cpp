#include <QApplication>

#include "MainWindow.h"

using namespace templex;
using namespace templex::frontend;

auto main(int argc, char* argv[]) -> int
{
    QApplication app(argc, argv);

    MainWindow window;

    window.show();

    return app.exec();
}
