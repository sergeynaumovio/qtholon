// Copyright (C) 2022 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOW_H
#define HOLONWINDOW_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QLoaderIconInterface>

class HolonDesktop;
class HolonSidebar;

class Q_HOLON_EXPORT HolonWindow : public QObject, public QLoaderSettings,
                                                   public QLoaderIconInterface

{
    Q_OBJECT
    Q_INTERFACES(QLoaderIconInterface)

protected:
    HolonWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonWindow(QLoaderSettings *settings, HolonSidebar *parent);

public:
    enum Area
    {
        Central = 0x1,
        Sidebar = 0x2
    };
    Q_DECLARE_FLAGS(Areas, HolonWindow::Area)

    virtual HolonWindow::Areas areas() const;
    QIcon icon() const override;
    virtual QString title() const;
    virtual QWidget *toolbar() const;
    virtual QWidget *widget() const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(HolonWindow::Areas)

#endif // HOLONWINDOW_H
