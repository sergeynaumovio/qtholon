// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractwindow_p.h"
#include "holonabstractwindow.h"

HolonAbstractWindowPrivate::HolonAbstractWindowPrivate(HolonAbstractWindow *q,
                                                       HolonDesktop *desk,
                                                       HolonAbstractTask *t)
:   q_ptr(q),
    desktop(desk),
    task(t)
{ }

void HolonAbstractWindowPrivate::setCurrent(bool current)
{
    q_ptr->setValue("current", current);
}