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
class HolonSidebarDock;
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
    const QScopedStorage<HolonDesktopPrivateData, 608> d_ptr;

    HolonDesktopPrivate(HolonDesktop *q);

    void addModel(HolonTaskModel *model);
    void addModel(HolonWorkflowModel *model);
    void addSidebar(HolonSidebar *sidebar);
    void addTask(HolonAbstractTask *task);
    void addTheme(HolonTheme *theme);
    void addWindow(HolonAbstractWindow *window);
    void addWindowArea(HolonWindowArea *window);
    void closeTask(HolonAbstractTask *task);
    void closeWindow(HolonAbstractWindow *window);
    HolonAbstractTask *currentTask() const;
    HolonTaskModel *currentTaskModel() const;
    HolonTheme *currentTheme() const;
    HolonWorkflowModel *currentWorkflowModel() const;
    void emitWarning(const QString &warning) const;
    void resizeEvent(QResizeEvent *e);
    void saveMainWindowState();
    void setCurrentTask(HolonAbstractTask *task);
    void setCurrentTaskModel(HolonTaskModel *model);
    void setCurrentTheme(HolonTheme *theme);
    void setCurrentWindow(HolonAbstractWindow *window);
    void setCurrentWindowArea(HolonWindowArea *windowArea);
    void setLayout();

public:
    enum TaskbarArea
    {
        Left,
        Right,
        Top,
        Bottom
    };
    Q_ENUM(TaskbarArea)


    HolonDesktop *const q_ptr;

    ~HolonDesktopPrivate();

    QString buttonStyleSheet() const;
    void hideWindowArea(HolonWindowArea *windowArea);
    int menuBorderWidth() const;
    QString menuStyleSheet() const;
    int menuWidth() const;
    const QSet<HolonSidebarDock *> &sidebarDocks() const;
    QString sidebarMoveShortcut() const;
    int sidebarSwitchButtonWidth() const;
    HolonTaskbar *taskbar() const;
    HolonDesktopPrivate::TaskbarArea taskbarArea() const;
    int taskbarPreferedHeight() const;
    int taskbarPreferedWidth() const;
    QString taskbarStyleSheet() const;
    int titleBarHeight() const;
    QString titleBarStyleSheet() const;
    void showWindowArea(HolonWindowArea *windowArea);
    QList<HolonAbstractWindow *> windows() const;

    void removeSidebar(HolonSidebar *sidebar);
    void resizeDocks();
    void restoreSidebar(HolonSidebar *sidebar);
    void saveDockWidgetWidth(HolonSidebarDock *dock, int width);
    HolonSidebarDock *sidebarDock(HolonSidebar *sidebar) const;
};

#endif // HOLONDESKTOP_P_H
