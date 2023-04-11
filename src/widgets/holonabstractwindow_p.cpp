// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractwindow_p.h"
#include "holonabstractwindow.h"

using namespace Qt::Literals::StringLiterals;

HolonAbstractWindowPrivate::HolonAbstractWindowPrivate(HolonAbstractWindow *q,
                                                       HolonDesktop *desk,
                                                       HolonAbstractTask *t)
:   q_ptr(q),
    desktop(desk),
    task(t)
{ }

void HolonAbstractWindowPrivate::saveSidebarState(const QByteArray &state)
{
    q_ptr->setValue(u"mainWindowState"_s, state);
}

void HolonAbstractWindowPrivate::setCurrent(bool current)
{
    q_ptr->setValue(u"current"_s, current);
}
