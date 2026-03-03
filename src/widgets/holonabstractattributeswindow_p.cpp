// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractattributeswindow_p.h"
#include "holonabstractattributeswindow.h"
#include "holondesktop.h"
#include "holonstackedwidget.h"

HolonAbstractAttributesWindowPrivate::HolonAbstractAttributesWindowPrivate(HolonAbstractAttributesWindow *q,
                                                                           HolonDesktop *desk)
:   HolonAbstractWindowPrivate(q, desk)
{ }

QWidget *HolonAbstractAttributesWindowPrivate::centralWidget()
{
    if (!q_ptr)
        return nullptr;

    if (stackedWidget)
        return stackedWidget;

    stackedWidget = new HolonWindowAreaStackedWidget(q_ptr->metaObject()->metaType());

    return stackedWidget;
}
