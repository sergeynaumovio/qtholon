// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractwindow_p.h"
#include "holonabstractwindow.h"

using namespace Qt::Literals::StringLiterals;

HolonAbstractWindowPrivate::HolonAbstractWindowPrivate(HolonAbstractWindow *q, HolonDesktop *desk)
:   q_ptr(q),
    desktop(desk)
{ }

void HolonAbstractWindowPrivate::setCurrent(bool current)
{
    q_ptr->setValue(u"current"_s, current);
}
