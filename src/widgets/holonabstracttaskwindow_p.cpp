// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstracttaskwindow_p.h"

HolonAbstractTaskWindowPrivate::HolonAbstractTaskWindowPrivate(HolonAbstractWindow *q,
                                                               HolonDesktop *desk,
                                                               HolonAbstractTask *tsk)
:   HolonAbstractWindowPrivate(q, desk),
    task(tsk)
{ }

