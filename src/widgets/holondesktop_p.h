// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
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
class HolonTaskModel;
class HolonWindowArea;
class HolonWorkflowModel;
template<typename> class QList;
class QResizeEvent;
class QStackedWidget;
class QString;

class HolonDesktopPrivate
{
    Q_GADGET

    friend class HolonDesktop;
    friend class HolonDesktopPrivateData;
    const QScopedStorage<HolonDesktopPrivateData, 424> d_ptr;

    HolonDesktopPrivate(HolonDesktop *q);

    void addSidebar(HolonSidebar *sidebar);
    void addTask(HolonAbstractTask *task);
    void addTaskModel(HolonTaskModel *taskModel);
    void addTheme(HolonTheme *theme);
    void addWindow(HolonAbstractWindow *window);
    void addWindowArea(HolonWindowArea *window);
    void addWorkflowModel(HolonWorkflowModel *workflowModel);
    void closeTask(HolonAbstractTask *task);
    void closeWindow(HolonAbstractWindow *window);
    void closeWindowArea(HolonWindowArea *windowArea);
    HolonAbstractTask *currentTask() const;
    HolonTaskModel *currentTaskModel() const;
    HolonTheme *currentTheme() const;
    HolonWorkflowModel *currentWorkflowModel() const;
    void emitWarning(const QString &warning) const;
    void setCurrentTask(HolonAbstractTask *task);
    void setCurrentTaskModel(HolonTaskModel *model);
    void setCurrentTheme(HolonTheme *theme);
    void setCurrentWindow(HolonAbstractWindow *window);
    void setCurrentWindowArea(HolonWindowArea *windowArea);
    void setLayout();
    QList<HolonAbstractWindow *> windows() const;

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
    void saveSidebarState(HolonAbstractWindow *firstSidebarWindow);
    HolonSidebarDockWidget *sidebarDockWidget(HolonSidebar *sidebar) const;
    HolonTaskbar *taskbar() const;
    HolonDesktopPrivate::TaskbarArea taskbarArea() const;
};

#endif // HOLONDESKTOP_P_H
