#include "MenuRegistry.h"

using namespace templex;
using namespace templex::frontend;

QList<MenuRegistry::MenuData> MenuRegistry::mainMenus_;

QHash<MenuRegistry::MenuData, QList<MenuRegistry::MenuData>> MenuRegistry::subMenus_;

void MenuRegistry::buildMenuRegistry()
{
    MenuRegistry::MenuData classes(0, "Classes");
    MenuRegistry::MenuData functions(1, "Functions");

    mainMenus_ = {{0, "Classes"}, {1, "Functions"}};

    subMenus_ = {
        {mainMenus_[0], {{0, "Instantiations"}, {1, "STL containers"}}},
        {mainMenus_[1],
         {{0, "Instantiations"}, {1, "STL algorithm"}, {2, "Comparators"}}}};
}

QList<MenuRegistry::MenuData> MenuRegistry::getMainMenu()
{
    return mainMenus_;
}

QList<MenuRegistry::MenuData>
MenuRegistry::getSubMenu(const MenuRegistry::MenuData& mainMenuItem)
{
    if (!subMenus_.contains(mainMenuItem)) {
        throw std::runtime_error("Invalid main menu index");
    }

    return subMenus_[mainMenuItem];
}
