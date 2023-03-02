// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTWINDOW_P_H
#define HOLONABSTRACTWINDOW_P_H

class HolonAbstractTask;
class HolonAbstractWindow;
class HolonDesktop;

class HolonAbstractWindowPrivate
{
public:
    HolonAbstractWindow *const q_ptr;
    HolonDesktop *const desktop;
    HolonAbstractTask *const task;

    HolonAbstractWindowPrivate(HolonAbstractWindow *q,
                               HolonDesktop *desktop,
                               HolonAbstractTask *task = nullptr);

    void setCurrent(bool current);
};

#endif // HOLONABSTRACTWINDOW_P_H
