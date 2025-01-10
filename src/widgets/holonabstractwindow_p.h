// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTWINDOW_P_H
#define HOLONABSTRACTWINDOW_P_H

class HolonAbstractTask;
class HolonAbstractWindow;
class HolonDesktop;
class HolonTitleBar;

class HolonAbstractWindowPrivate
{
public:
    HolonAbstractWindow *const q_ptr;
    HolonDesktop *const desktop;
    HolonTitleBar *titleBar;

    HolonAbstractWindowPrivate(HolonAbstractWindow *q, HolonDesktop *desktop);
    virtual ~HolonAbstractWindowPrivate();

    void setCurrent(bool current);
};

#endif // HOLONABSTRACTWINDOW_P_H
