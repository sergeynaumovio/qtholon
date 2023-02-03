// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSIDEBARDOCK_H
#define HOLONSIDEBARDOCK_H

#include <QDockWidget>

class HolonDesktop;
class HolonDesktopPrivate;
class HolonMainWindow;
class HolonSidebar;
class HolonSidebarDockPrivate;

class HolonSidebarDock : public QDockWidget
{
    Q_OBJECT

    friend class HolonDesktopPrivate;
    HolonSidebarDockPrivate &d;
    std::aligned_storage_t<64, sizeof (ptrdiff_t)> d_storage;

protected:
    void resizeEvent(QResizeEvent *e) override;

public:
    HolonSidebarDock(HolonDesktopPrivate &desktop_d,
                     const QString &name,
                     HolonMainWindow *parent);

    ~HolonSidebarDock();

    void addSidebar(HolonSidebar *sidebar);
    void showTitleBarWidget(bool show);
};

#endif // HOLONSIDEBARDOCK_H
