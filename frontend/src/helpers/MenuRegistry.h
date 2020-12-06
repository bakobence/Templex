#pragma once

#include <QHash>
#include <QList>
#include <QObject>
#include <QPushButton>
#include <QString>

#include <stdexcept>

#include "helpers/GlobalDefines.h"

namespace templex {
namespace frontend {

class MenuRegistry : public QObject {
    Q_OBJECT

public:
    class MenuData {
    public:
        MenuData(int id, const QString& label)
            : id_(id),
              label_(label),
              button_(nullptr)
        {
        }

        bool operator==(const MenuData& other) const { return id_ == other.id_; }

        friend qhash_result_t qHash(const MenuRegistry::MenuData& key,
                                    uint seed) noexcept
        {
            return QtPrivate::QHashCombine()(key.id_, seed);
        }

        inline int getId() const { return id_; }

        inline QString getLabel() const { return label_; }

        inline QPushButton* getButton() const { return button_; }

        inline void setButton(QPushButton* button) { button_ = button; }

    private:
        int id_;
        QString label_;
        QPushButton* button_;
    };

    static void buildMenuRegistry();

    static QList<MenuData> getMainMenu();

    static QList<MenuData> getSubMenu(const MenuData& mainMenuItem);

    static void registerMainButton(const MenuData& menuItem, QPushButton* button);

    static QList<QPushButton*> getMainMenuButtons();

    static void clearSubMenu();

    static QList<QPushButton*> getSubMenuButtons();

private:
    static QList<MenuData> mainMenus_;
    static QHash<MenuData, QList<MenuData>> subMenus_;
};

uint qHash(const MenuRegistry::MenuData& key, uint seed) noexcept;

} // namespace frontend
} // namespace templex
