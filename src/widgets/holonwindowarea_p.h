// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWAREA_P_H
#define HOLONWINDOWAREA_P_H

#include <QMap>

class HolonAbstractWindow;
class HolonDesktop;
class HolonDockWidget;
class HolonDockWidgetSplit;
class HolonWindowArea;
class QMainWindow;

class HolonWindowAreaPrivate
{
    HolonDockWidgetSplit *rootSplit{};
    QByteArray mainWindowState;

    void addSplit(HolonDockWidget *firstDock, HolonDockWidget *secondDock, Qt::Orientation orientation);
    void removeSplit(HolonDockWidget *dock);

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
    void saveState();
    void setChecked(bool checked);
    void setDefaultDockWidget();
    void splitWindow(HolonAbstractWindow *first, HolonAbstractWindow *second, Qt::Orientation orientation);
    QByteArray state() const;
};

#endif // HOLONWINDOWAREA_P_H
