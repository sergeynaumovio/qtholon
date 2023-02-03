// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebar_p.h"

HolonSidebarPrivate::HolonSidebarPrivate(HolonDesktop *desktop, HolonSidebar *q)
:   HolonWindowAreaPrivate(desktop, q)
{ }

void HolonSidebarPrivate::setChecked(bool checked)
{
    Q_Q(HolonSidebar);
    q->setValue("checked", checked);
}
