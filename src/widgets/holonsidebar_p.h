// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSIDEBAR_P_H
#define HOLONSIDEBAR_P_H

#include "holonsidebar.h"
#include "holonwindowarea_p.h"

class HolonSidebar;

class HolonSidebarPrivate : public HolonWindowAreaPrivate
{
    Q_DECLARE_PUBLIC(HolonSidebar)

public:
    HolonSidebarPrivate(HolonDesktop *desktop, HolonSidebar *q);
};

#endif // HOLONSIDEBAR_P_H
