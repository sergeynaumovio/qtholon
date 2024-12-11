// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTTASKWINDOW_P_H
#define HOLONABSTRACTTASKWINDOW_P_H

#include "holonabstractwindow_p.h"

class HolonAbstractTaskWindowPrivate : public HolonAbstractWindowPrivate
{
public:
    HolonAbstractTask *const task;

    HolonAbstractTaskWindowPrivate(HolonAbstractWindow *q,
                                   HolonDesktop *desktop,
                                   HolonAbstractTask *task);
};

#endif // HOLONABSTRACTTASKWINDOW_P_H
