#pragma once

#include <QList>
#include <QMap>
#include <QObject>
#include <QPushButton>
#include <QString>

#include <functional>
#include <stdexcept>

#include "helpers/GlobalDefines.h"
#include "ui/IPage.h"

namespace templex {
namespace frontend {

class MenuRegistry : public QObject {
    Q_OBJECT

public:
    class MenuData {
    public:
        MenuData(int id, const QString& label) : id_(id), label_(label) {}

        bool operator==(const MenuData& other) const { return id_ == other.id_; }

        friend qhash_result_t qHash(const MenuRegistry::MenuData& key,
                                    uint seed) noexcept
        {
            return QtPrivate::QHashCombine()(key.id_, seed);
        }

        bool operator<(const MenuData& other) const { return id_ < other.id_; }

        inline int getId() const { return id_; }

        inline QString getLabel() const { return label_; }

    private:
        int id_;
        QString label_;
    };

    static void buildMenuRegistry();

    static QList<MenuData> getMainMenu();

    static QList<MenuData> getSubMenus(const MenuData& mainMenuItem);

    static std::function<std::shared_ptr<IPage>()> getCreator(const MenuData& menu);

private:
    template <typename PageT,
              std::enable_if_t<std::is_base_of_v<IPage, PageT>>* = nullptr>
    static void registerPage(int id, QString subPageName, MenuData mainMenu)
    {
        MenuData sub(id, subPageName);
        menuStructure_[mainMenu].push_back(sub);
        pagesCreators_[sub] = []() { return std::make_shared<PageT>(); };
    }

private:
    static QMap<MenuData, QList<MenuData>> menuStructure_;

    static QMap<MenuData, std::function<std::shared_ptr<IPage>()>> pagesCreators_;
};

qhash_result_t qHash(const MenuRegistry::MenuData& key, uint seed) noexcept;

} // namespace frontend
} // namespace templex
