// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWAREA_P_H
#define HOLONWINDOWAREA_P_H

#include <QList>

class HolonDesktop;
class HolonWindow;
class HolonWindowArea;
class QDockWidget;
class QMainWindow;

class HolonWindowAreaPrivate
{
public:
    HolonDesktop *const desktop;
    HolonWindowArea *q_ptr;
    QMainWindow *const mainWindow;
    QDockWidget *const defaultDock;
    QList<QDockWidget *> dockList;
    bool maximized{};

    HolonWindowAreaPrivate(HolonDesktop *desktop, HolonWindowArea *q);
    virtual ~HolonWindowAreaPrivate();

    void addWindow(HolonWindow *window);
    void maximizeWindow(QDockWidget *dock);
    void closeWindow(QDockWidget *dock, HolonWindow *window);
};

#endif // HOLONWINDOWAREA_P_H
