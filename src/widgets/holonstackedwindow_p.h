// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSTACKEDWINDOW_P_H
#define HOLONSTACKEDWINDOW_P_H

#include "holonabstractwindow_p.h"
#include "holonstackedwindow.h"

class HolonAbstractWindow;
class HolonDesktop;
class HolonStackedWindow;
class HolonWindowStackedWidget;

class HolonStackedWindowPrivate : public HolonAbstractWindowPrivate
{
public:
    HolonWindowStackedWidget *const stackedWidget;
    HolonAbstractWindow *currentWindow{};

    HolonStackedWindowPrivate(HolonStackedWindow *q, HolonDesktop *desk);

    void addWindow(HolonAbstractWindow *window);
    void setWindow(HolonAbstractWindow *window);
    HolonAbstractWindow *window() const;
};

#endif // HOLONSTACKEDWINDOW_P_H
