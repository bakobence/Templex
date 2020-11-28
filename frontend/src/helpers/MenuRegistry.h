#pragma once

#include <QList>
#include <QMap>
#include <QObject>
#include <QString>

#include <stdexcept>

namespace templex {
namespace frontend {

class MenuRegistry : public QObject {
    Q_OBJECT

public:
    static void buildMenuRegistry()
    {
        mainMenus_ = {"Classes", "Functions"};

        QList<QString> classesSubMenu{"Instantiations", "STL containers"};

        QList<QString> functionsSubMenu{"Instantiations",
                                        "Comparators",
                                        "STL algorithm"};

        subMenus_[0] = classesSubMenu;
        subMenus_[1] = functionsSubMenu;
    }

    static QList<QString> getMainMenu()
    {
        return mainMenus_;
    }

    static QList<QString> getSubMenuFor(int mainMenuIndex)
    {
        if (mainMenuIndex < 0 || mainMenuIndex > mainMenus_.count() - 1) {
            throw std::runtime_error("Invalid main menu index");
        }

        if (!subMenus_.contains(mainMenuIndex)) {
            throw std::runtime_error("Invalid main menu index");
        }

        return subMenus_[mainMenuIndex];
    }

private:
    static QList<QString> mainMenus_;
    static QMap<int, QList<QString>> subMenus_;
};

} // namespace frontend
} // namespace templex
