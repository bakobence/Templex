#include "MenuRegistry.h"

#include "ui/classes/instantiations/ClassInstantiationsPage.h"
#include "ui/classes/statistics/ClassStatisticsPage.h"
#include "ui/classes/stl_containers/STLContainersPage.h"
#include "ui/functions/instantiations/FunctionInstantiationsPage.h"

using namespace templex;
using namespace templex::frontend;

QMap<MenuRegistry::MenuData, QList<MenuRegistry::MenuData>>
    MenuRegistry::menuStructure_;

QMap<MenuRegistry::MenuData, std::function<std::shared_ptr<IPage>()>>
    MenuRegistry::pagesCreators_;

void MenuRegistry::buildMenuRegistry()
{
    qDebug() << "Building up menu registry";

    int mainIdPool = 0;
    MenuRegistry::MenuData classes(mainIdPool++, "Classes");
    MenuRegistry::MenuData functions(mainIdPool++, "Functions");

    int subIdPool = 0;
    registerPage<ClassInstantiationsPage>(subIdPool++, "Instantiations", classes);
    registerPage<ClassStatisticsPage>(subIdPool++, "Statistics", classes);
    registerPage<STLContainersPage>(subIdPool++, "STL Containers", classes);
    registerPage<FunctionInstantiationsPage>(subIdPool++,
                                             "Instantiations",
                                             functions);
}

QList<MenuRegistry::MenuData> MenuRegistry::getMainMenu()
{
    return menuStructure_.keys();
}

QList<MenuRegistry::MenuData>
MenuRegistry::getSubMenus(const MenuRegistry::MenuData& mainMenuItem)
{
    if (!menuStructure_.contains(mainMenuItem)) {
        throw std::runtime_error("Invalid main menu index");
    }

    return menuStructure_.value(mainMenuItem);
}

std::function<std::shared_ptr<IPage>()>
MenuRegistry::getCreator(const MenuRegistry::MenuData& menu)
{
    if (!pagesCreators_.contains(menu)) {
        return nullptr;
    }

    return pagesCreators_[menu];
}
