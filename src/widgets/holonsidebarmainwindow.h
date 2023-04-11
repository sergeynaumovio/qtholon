// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSIDEBARMAINWINDOW_H
#define HOLONSIDEBARMAINWINDOW_H

#include <QMainWindow>
#include <QScopedStorage>

class HolonDesktopPrivate;
class HolonSidebar;
class HolonSidebarDockWidget;
class HolonSidebarMainWindowPrivate;

class HolonSidebarMainWindow : public QMainWindow
{
    Q_OBJECT

    const QScopedStorage<HolonSidebarMainWindowPrivate, 72> d_ptr;

public:
    HolonSidebarMainWindow(HolonDesktopPrivate &desktop_d);
    ~HolonSidebarMainWindow();

    HolonSidebarDockWidget *addSidebar(HolonSidebar *sidebar);
    QList<HolonSidebar *> sidebars() const;
};

#endif // HOLONSIDEBARMAINWINDOW_H
