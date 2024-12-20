// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebar_p.h"
#include "holondesktop_p.h"

using namespace Qt::Literals::StringLiterals;

HolonSidebarPrivate::HolonSidebarPrivate(HolonSidebar *q, HolonDesktop *desktop)
:   HolonWindowAreaPrivate(q, desktop)
{ }

void HolonSidebarPrivate::saveSidebarMainWindowNestingState(const QByteArray &state)
{
    Q_Q(HolonSidebar);
    q->setValue(u"mainWindowNestingState"_s, state);
}

