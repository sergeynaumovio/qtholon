// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDESKTOP_P_H
#define HOLONDESKTOP_P_H

#include <QMetaType>

class HolonDesktop;
class HolonDesktopPrivateData;
class HolonSidebar;
class HolonSidebarDock;
class HolonTaskbar;
class HolonWindow;
template<typename> class QList;
class QResizeEvent;
class QStackedWidget;
class QString;

class HolonDesktopPrivate
{
    Q_GADGET

    friend class HolonDesktop;
    friend class HolonDesktopPrivateData;
    HolonDesktopPrivateData &d;
    std::aligned_storage_t<360, sizeof (ptrdiff_t)> d_storage;

    HolonDesktopPrivate(HolonDesktop *q);

    void addSidebar(HolonSidebar *sidebar);
    void addWindow(HolonWindow *window);
    void resizeEvent(QResizeEvent *e);
    void saveMainWindowState();
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
    int menuBorder() const;
    QString menuStyleSheet() const;
    int menuWidth() const;
    const QSet<HolonSidebarDock *> &sidebarDocks() const;
    QString sidebarMoveShortcut() const;
    int sidebarSwitchButtonWidth() const;
    HolonTaskbar *taskbar() const;
    HolonDesktopPrivate::TaskbarArea taskbarArea() const;
    int taskbarPreferedHeight() const;
    int taskbarPreferedWidth() const ;
    QString taskbarStyleSheet() const;
    int titleBarHeight() const;
    QString titleBarStyleSheet() const;
    QList<HolonWindow *> windowList() const;

    void removeSidebar(HolonSidebar *sidebar);
    void resizeDocks();
    void restoreSidebar(HolonSidebar *sidebar);
    void saveDockWidgetWidth(HolonSidebarDock *dock, int width);
    HolonSidebarDock *sidebarDock(HolonSidebar *sidebar) const;
};

#endif // HOLONDESKTOP_P_H
