// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDESKTOP_P_H
#define HOLONDESKTOP_P_H

#include <QMetaObject>
#include <QScopedPointer>
#include <QScopedStorage>

class HolonAbstractTask;
class HolonAbstractTaskWindow;
class HolonAbstractWindow;
class HolonDesktop;
class HolonDesktopPrivateData;
class HolonSidebar;
class HolonSidebarDockWidget;
class HolonSidebarMainWindow;
class HolonTaskbar;
class HolonTaskThread;
class HolonTheme;
class HolonWindowArea;
class HolonWorkflow;
class QString;

class HolonDesktopPrivate
{
    Q_GADGET

    friend class HolonDesktop;
    friend class HolonDesktopPrivateData;

#if Q_HOLON_DEBUG

    const QScopedPointer<HolonDesktopPrivateData> d_ptr;

#else

    const QScopedStorage<HolonDesktopPrivateData, 432> d_ptr;

#endif


    HolonDesktopPrivate(HolonDesktop *q);

    void addSidebar(HolonSidebar *sidebar);
    void addTask(HolonAbstractTask *task);
    void addWindow(HolonAbstractWindow *window);
    void addWindowArea(HolonWindowArea *window);
    void addWorkflow(HolonWorkflow *workflow);
    void closeTask(HolonAbstractTask *task);
    void closeWindow(HolonAbstractWindow *window);
    void closeWindowArea(HolonWindowArea *windowArea);
    HolonAbstractTask *currentTask() const;
    HolonAbstractTaskWindow *currentTaskWindow() const;
    HolonWorkflow *currentWorkflow() const;
    void emitWarning(const QString &warning) const;
    void setLayout();
    void setCurrentTask(HolonAbstractTask *task);
    void setCurrentWindow(HolonAbstractWindow *window);
    void setCurrentWindowArea(HolonWindowArea *windowArea);
    void setTheme(HolonTheme *theme);
    HolonTaskThread *taskThread() const;
    HolonTheme *theme() const;

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
