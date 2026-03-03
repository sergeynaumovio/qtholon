// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTATTRIBUTESWINDOW_P_H
#define HOLONABSTRACTATTRIBUTESWINDOW_P_H

#include "holonabstractwindow_p.h"

class HolonAbstractAttributesWindow;
class HolonWindowAreaStackedWidget;
class QWidget;

class HolonAbstractAttributesWindowPrivate : public HolonAbstractWindowPrivate
{
    HolonWindowAreaStackedWidget *stackedWidget{};

public:
    HolonAbstractAttributesWindowPrivate(HolonAbstractAttributesWindow *q, HolonDesktop *desktop);

    QWidget *centralWidget();
};

#endif // HOLONABSTRACTATTRIBUTESWINDOW_P_H
