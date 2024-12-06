// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDESKTOP_P_H
#define HOLONDESKTOP_P_H

#include <QMetaType>
#include <QScopedStorage>

class HolonAbstractTask;
class HolonTheme;
class HolonAbstractWindow;
class HolonDesktop;
class HolonDesktopPrivateData;
class HolonSidebar;
class HolonSidebarDockWidget;
class HolonSidebarMainWindow;
class HolonTaskbar;
class HolonWindowArea;
class HolonWorkflow;
template<typename> class QList;
class QResizeEvent;
class QStackedWidget;
class QString;

class HolonDesktopPrivate
{
    Q_GADGET

    friend class HolonDesktop;
    friend class HolonDesktopPrivateData;
    const QScopedStorage<HolonDesktopPrivateData, 368> d_ptr;

    HolonDesktopPrivate(HolonDesktop *q);

    void addSidebar(HolonSidebar *sidebar);
    void addTask(HolonAbstractTask *task);
    void addWindow(HolonAbstractWindow *window);
    void addWindowArea(HolonWindowArea *window);
    void addWorkflow(HolonWorkflow *workflow);
    void closeTask(HolonAbstractTask *task);
    void closeWindow(HolonAbstractWindow *window);
    void closeWindowArea(HolonWindowArea *windowArea);
    void emitWarning(const QString &warning) const;
    void setLayout();
    void setTask(HolonAbstractTask *task);
    void setWindow(HolonAbstractWindow *window);
    void setWindowArea(HolonWindowArea *windowArea);
    void setTheme(HolonTheme *theme);
    HolonAbstractTask *task() const;
    HolonTheme *theme() const;
    HolonWorkflow *workflow() const;

public:
    HolonDesktop *const q_ptr;

    enum TaskbarArea
    {
        Left,
        Right,
        Top,
        Bottom
    };
    Q_ENUM(TaskbarArea)

    ~HolonDesktopPrivate();

    void cacheMainWindowState(HolonWindowArea *windowArea);
    void removeSidebar(HolonSidebar *sidebar);
    void restoreSidebar(HolonSidebar *sidebar);
    void restoreMainWindowStateFromCache(HolonWindowArea *windowArea);
    void saveSidebarMainWindowState(HolonSidebarMainWindow *sidebarMainWindow);
    HolonSidebarDockWidget *sidebarDockWidget(HolonSidebar *sidebar) const;
    HolonTaskbar *taskbar() const;
    HolonDesktopPrivate::TaskbarArea taskbarArea() const;
};

#endif // HOLONDESKTOP_P_H
