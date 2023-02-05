// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSIDEBARDOCK_P_H
#define HOLONSIDEBARDOCK_P_H

#include <stddef.h>
#include <type_traits>

class HolonDesktopPrivate;
class HolonMainWindow;
class HolonSidebar;
class HolonSidebarDock;
class HolonSidebarDockPrivateData;
template<typename> class QList;
class QStackedWidget;
class QString;

class HolonSidebarDockPrivate
{
    HolonSidebarDockPrivateData &d;
    std::aligned_storage_t<88, sizeof (ptrdiff_t)> d_storage;

public:
    HolonSidebarDockPrivate(HolonDesktopPrivate &desktop_d,
                            HolonSidebarDock *q,
                            const QString &name,
                            HolonMainWindow *parent);

    void addSidebar(HolonSidebar *sidebar);
    HolonSidebar *currentSidebar() const;
    HolonDesktopPrivate &desktop_d() const;
    void setSidebar(HolonSidebar *sidebar);
    const QList<HolonSidebar *> &sidebars() const;
    void showTitleBarWidget(bool show) const;
};

#endif // HOLONSIDEBARDOCK_P_H
