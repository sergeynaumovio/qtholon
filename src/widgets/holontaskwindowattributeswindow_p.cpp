// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskwindowattributeswindow_p.h"
#include "holondesktop.h"
#include "holonstackedwidget.h"
#include "holontaskwindowattributeswindow.h"

HolonTaskWindowAttributesWindowPrivate::HolonTaskWindowAttributesWindowPrivate(HolonTaskWindowAttributesWindow *q,
                                                                               HolonDesktop *desk)
:   HolonAbstractWindowPrivate(q, desk)
{ }

QWidget *HolonTaskWindowAttributesWindowPrivate::centralWidget()
{
    if (!q_ptr)
        return nullptr;

    if (stackedWidget)
        return stackedWidget;

    stackedWidget = new HolonWindowAreaStackedWidget(q_ptr->metaObject()->metaType());

    return stackedWidget;
}
