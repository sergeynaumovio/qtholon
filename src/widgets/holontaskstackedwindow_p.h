// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKSTACKEDWINDOW_P_H
#define HOLONTASKSTACKEDWINDOW_P_H

#include "holonstackedwindow_p.h"

class HolonTaskStackedWindow;

class HolonTaskStackedWindowPrivate : public HolonStackedWindowPrivate
{
public:
    HolonAbstractTask *const task;

    HolonTaskStackedWindowPrivate(HolonTaskStackedWindow *q, HolonDesktop *desk, HolonAbstractTask *task);
};

#endif // HOLONTASKSTACKEDWINDOW_P_H
