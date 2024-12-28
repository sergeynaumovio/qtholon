// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWAREA_P_H
#define HOLONWINDOWAREA_P_H

#include <QMap>
#include <QScopedStorage>

class HolonAbstractWindow;
class HolonDesktop;
class HolonDockWidget;
class HolonDockWidgetSplitState;
class HolonWindowArea;
class HolonWindowAreaState;
class QMainWindow;

class HolonWindowAreaPrivate
{
    HolonDockWidgetSplitState *dockWidgetSplitState{};
    QByteArray mainWindowStateCache;
    const QScopedStorage<HolonWindowAreaState, 8> windowAreaState;

public:
    HolonWindowArea *const q_ptr;
    HolonDesktop *const desktop;
    QMainWindow *const mainWindow;

    QList<HolonDockWidget *> dockList;
    QMap<HolonAbstractWindow *, HolonDockWidget *> dockByWindow;
    bool maximized{};

    HolonWindowAreaPrivate(HolonWindowArea *q, HolonDesktop *desktop);

    virtual ~HolonWindowAreaPrivate();

    void addWindow(HolonAbstractWindow *window);
    Qt::DockWidgetArea area() const;
    void cacheMainWindowState();
    void closeWindow(HolonAbstractWindow *window);
    void maximizeWindow(HolonDockWidget *dock);
    void restoreMainWindowStateFromCache();
    void saveWindowAreaState();
    void setChecked(bool checked);
    void setValue(const QString &key, const QVariant &value);
    void splitWindow(HolonAbstractWindow *window, Qt::Orientation orientation);

    void splitWindow(HolonAbstractWindow *first,
                     HolonAbstractWindow *second,
                     Qt::Orientation orientation,
                     int secondWindowComboboxIndex = 0);
};

#endif // HOLONWINDOWAREA_P_H
