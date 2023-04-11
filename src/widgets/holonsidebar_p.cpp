// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebar_p.h"
#include "holondesktop_p.h"

using namespace Qt::Literals::StringLiterals;

HolonSidebarPrivate::HolonSidebarPrivate(HolonDesktop *desktop, HolonSidebar *q)
:   HolonWindowAreaPrivate(desktop, q, Holon::SidebarWindow)
{ }

void HolonSidebarPrivate::saveWindowAreaState()
{
    HolonAbstractWindow *firstSidebarWindow = dockByWindow.firstKey();
    desktop_d->saveSidebarState(firstSidebarWindow);
}

void HolonSidebarPrivate::saveSidebarMainWindowState(const QByteArray &state)
{
    Q_Q(HolonSidebar);
    q->setValue(u"mainWindowState"_s, state);
}

