/****************************************************************************
**
** Copyright (C) 2023 Sergey Naumov
**
** Permission to use, copy, modify, and/or distribute this
** software for any purpose with or without fee is hereby granted.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
** THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
** CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
** LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
** NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
** CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**
****************************************************************************/

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
class QString;

class HolonDesktopPrivate
{
    Q_GADGET

    friend class HolonDesktop;
    HolonDesktopPrivateData &d;
    std::aligned_storage_t<376, sizeof (ptrdiff_t)> d_storage;

    HolonDesktopPrivate(HolonDesktop *q);

    void addSidebar(HolonSidebar *sidebar);
    void addWindow(HolonWindow *window);
    void resizeEvent(QResizeEvent *e);
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
    const QList<HolonSidebarDock *> &sidebarDocks() const;
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

    void removeDockWidget(HolonSidebarDock *dock);
    void resizeDocks();
    void restoreDockWidget(HolonSidebarDock *dock);
    void saveDockWidgetWidth(HolonSidebarDock *dock, int width);
    HolonSidebarDock *sidebarDock(HolonSidebar *sidebar) const;
};

#endif // HOLONDESKTOP_P_H
