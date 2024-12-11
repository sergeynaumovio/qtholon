// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSIDEBAR_P_H
#define HOLONSIDEBAR_P_H

#include "holonsidebar.h"
#include "holonwindowarea_p.h"

class HolonDesktopPrivate;
class HolonSidebar;

class HolonSidebarPrivate : public HolonWindowAreaPrivate
{
    Q_DECLARE_PUBLIC(HolonSidebar)

    int mainWindowNestingIndex;

public:
    HolonDesktopPrivate *desktop_d;

    HolonSidebarPrivate(HolonSidebar *q, HolonDesktop *desktop);
    void saveSidebarMainWindowNestingState(const QByteArray &state);
};

#endif // HOLONSIDEBAR_P_H
