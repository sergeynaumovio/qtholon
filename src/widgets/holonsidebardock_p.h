// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSIDEBARDOCK_P_H
#define HOLONSIDEBARDOCK_P_H

#include "holonalignedstorage.h"

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
    const HolonAlignedStorage<HolonSidebarDockPrivateData, 88> d_ptr;

public:
    HolonSidebarDockPrivate(HolonDesktopPrivate &desktop_d,
                            HolonSidebarDock *q,
                            const QString &name,
                            HolonMainWindow *parent);

    ~HolonSidebarDockPrivate();

    void addSidebar(HolonSidebar *sidebar);
    HolonSidebar *currentSidebar() const;
    HolonDesktopPrivate &desktop_d() const;
    void setSidebar(HolonSidebar *sidebar);
    const QList<HolonSidebar *> &sidebars() const;
    void showTitleBarWidget(bool show) const;
};

#endif // HOLONSIDEBARDOCK_P_H
