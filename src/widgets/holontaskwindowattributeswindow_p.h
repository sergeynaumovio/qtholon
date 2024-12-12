// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKWINDOWATTRIBUTESWINDOW_P_H
#define HOLONTASKWINDOWATTRIBUTESWINDOW_P_H

#include "holonabstractwindow_p.h"

class HolonTaskWindowAttributesWindow;
class HolonWindowAreaStackedWidget;
class QWidget;

class HolonTaskWindowAttributesWindowPrivate : public HolonAbstractWindowPrivate
{
    HolonWindowAreaStackedWidget *stackedWidget{};

public:
    HolonTaskWindowAttributesWindowPrivate(HolonTaskWindowAttributesWindow *q, HolonDesktop *desktop);

    QWidget *centralWidget();
};

#endif // HOLONTASKWINDOWATTRIBUTESWINDOW_P_H
