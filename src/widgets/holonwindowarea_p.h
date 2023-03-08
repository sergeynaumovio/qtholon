// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWAREA_P_H
#define HOLONWINDOWAREA_P_H

#include <QMap>

class HolonAbstractWindow;
class HolonDesktop;
class HolonWindowArea;
class HolonDockWidget;
class QMainWindow;

class HolonWindowAreaPrivate
{
public:
    HolonDesktop *const desktop;
    HolonWindowArea *const q_ptr;
    QMainWindow *const mainWindow;
    HolonDockWidget *defaultDock{};
    QList<HolonDockWidget *> dockList;
    QMap<HolonAbstractWindow *, HolonDockWidget *> dockByWindow;
    bool maximized{};

    HolonWindowAreaPrivate(HolonDesktop *desktop, HolonWindowArea *q);
    virtual ~HolonWindowAreaPrivate();

    void addWindow(HolonAbstractWindow *window);
    void closeWindow(HolonAbstractWindow *window);
    void maximizeWindow(HolonDockWidget *dock);
    void setChecked(bool checked);
    void setDefaultDockWidget();
    void splitWindow(HolonAbstractWindow *first, HolonAbstractWindow *second, Qt::Orientation orientation);
};

#endif // HOLONWINDOWAREA_P_H
