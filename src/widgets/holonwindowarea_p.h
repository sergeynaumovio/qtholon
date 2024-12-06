// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWAREA_P_H
#define HOLONWINDOWAREA_P_H

#include "holonnamespace.h"
#include <QMap>

class HolonAbstractWindow;
class HolonDesktop;
class HolonDockWidget;
class HolonDockWidgetSplitState;
class HolonWindowArea;
class QMainWindow;

class HolonWindowAreaPrivate
{
    HolonDockWidgetSplitState *dockWidgetSplitState{};
    QByteArray mainWindowStateCache;

public:
    HolonDesktop *const desktop;
    HolonWindowArea *const q_ptr;
    QMainWindow *const mainWindow;

    Holon::WindowType windowType;
    QList<HolonDockWidget *> dockList;
    QMap<HolonAbstractWindow *, HolonDockWidget *> dockByWindow;
    bool maximized{};

    HolonWindowAreaPrivate(HolonDesktop *desktop,
                           HolonWindowArea *q,
                           Holon::WindowType winType = Holon::TaskWindow);

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
