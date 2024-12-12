// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskattributeswindow_p.h"
#include "holondesktop.h"
#include "holonstackedwidget.h"
#include "holontaskattributeswindow.h"

HolonTaskAttributesWindowPrivate::HolonTaskAttributesWindowPrivate(HolonTaskAttributesWindow *q, HolonDesktop *desk)
:   HolonAbstractWindowPrivate(q, desk)
{ }

QWidget *HolonTaskAttributesWindowPrivate::centralWidget()
{
    if (!q_ptr)
        return nullptr;

    if (stackedWidget)
        return stackedWidget;

    stackedWidget = new HolonTaskStackedWidget(q_ptr->metaObject()->metaType());

    return stackedWidget;
}
