// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKATTRIBUTESWINDOW_P_H
#define HOLONTASKATTRIBUTESWINDOW_P_H

#include "holonabstractwindow_p.h"

class HolonTaskAttributesWindow;
class HolonTaskStackedWidget;
class QWidget;

class HolonTaskAttributesWindowPrivate : public HolonAbstractWindowPrivate
{
    HolonTaskStackedWidget *stackedWidget{};

public:
    HolonTaskAttributesWindowPrivate(HolonTaskAttributesWindow *q, HolonDesktop *desktop);

    QWidget *centralWidget();
};

#endif // HOLONTASKATTRIBUTESWINDOW_P_H
