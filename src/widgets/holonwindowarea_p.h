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
class QObject;

class HolonWindowAreaPrivate
{
    HolonDockWidgetSplit *rootSplit{};
    QByteArray mainWindowStateBeforeMaximized;

    void addSplit(HolonDockWidget *firstDock, HolonDockWidget *secondDock, Qt::Orientation orientation);
    void parseSplitPath(const QStringList &path);
    void removeSplit(HolonDockWidget *dock);
    void restoreSplitPath(const QStringList &path, QObject *parent);
    void restoreSplitState();
    void saveSplitState();
    void saveSplitStateRecursive(QStringList &splitState, const QString &section, QObject *parent);
    void setSplitItemDock(HolonDockWidget *dock);

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
    QByteArray mainWindowState() const;
    void maximizeWindow(HolonDockWidget *dock);
    void saveMainWindowState();
    void setChecked(bool checked);
    void setDefaultDockWidget();
    void splitWindow(HolonAbstractWindow *first, HolonAbstractWindow *second, Qt::Orientation orientation);
};

#endif // HOLONWINDOWAREA_P_H
