// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskstackedwindow_p.h"
#include "holondesktop.h"
#include "holontaskstackedwindow.h"

HolonTaskStackedWindowPrivate::HolonTaskStackedWindowPrivate(HolonTaskStackedWindow *q,
                                                             HolonDesktop *desk,
                                                             HolonAbstractTask *tsk)
:   HolonStackedWindowPrivate(q, desk),
    task(tsk)
{ }
