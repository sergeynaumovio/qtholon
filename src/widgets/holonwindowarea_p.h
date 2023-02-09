// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWAREA_P_H
#define HOLONWINDOWAREA_P_H

#include <QList>
#include <QMap>

class HolonDesktop;
class HolonWindow;
class HolonWindowArea;
class QDockWidget;
class QMainWindow;

class HolonWindowAreaPrivate
{
public:
    HolonDesktop *const desktop;
    HolonWindowArea *const q_ptr;
    QMainWindow *const mainWindow;
    QDockWidget *const defaultDock;
    QList<QDockWidget *> dockList;
    QMap<HolonWindow *, QDockWidget *> dockByWindow;
    bool maximized{};

    HolonWindowAreaPrivate(HolonDesktop *desktop, HolonWindowArea *q);
    virtual ~HolonWindowAreaPrivate();

    void addWindow(HolonWindow *window);
    void closeWindow(HolonWindow *window);
    void maximizeWindow(QDockWidget *dock);
};

#endif // HOLONWINDOWAREA_P_H
