#pragma once

#include <QHash>
#include <QList>
#include <QObject>
#include <QString>

#include <stdexcept>

namespace templex {
namespace frontend {

class MenuRegistry : public QObject {
    Q_OBJECT

public:
    class MenuData {
    public:
        MenuData(int id, const QString& label) : id_(id), label_(label) {}

        bool operator==(const MenuData& other) const { return id_ == other.id_; }

        friend uint qHash(const MenuRegistry::MenuData& key, uint seed) noexcept
        {
            return qHash(key.id_, seed);
        }

        inline int getId() const { return id_; }

        inline QString getLabel() const { return label_; }

    private:
        int id_;
        QString label_;
    };

    static void buildMenuRegistry();

    static QList<MenuData> getMainMenu();

    static QList<MenuData> getSubMenu(const MenuData& mainMenuItem);

private:
    static QList<MenuData> mainMenus_;
    static QHash<MenuData, QList<MenuData>> subMenus_;
};

uint qHash(const MenuRegistry::MenuData& key, uint seed) noexcept;

} // namespace frontend
} // namespace templex
