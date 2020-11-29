#pragma once

#include <QMainWindow>

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

private:
    Ui::MainWindow* ui;
};

} // namespace frontend
} // namespace templex
