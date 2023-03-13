// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondockwidgetsplit.h"
#include "holondockwidget.h"
#include "holontitlebar.h"
#include "holonwindowarea.h"

HolonDockWidgetSplit::HolonDockWidgetSplit(HolonWindowArea *parent)
:   QObject(parent),
    area(Qt::DockWidgetArea{})
{ }

HolonDockWidgetSplit::HolonDockWidgetSplit(Qt::DockWidgetArea position,
                                           HolonDockWidgetSplit *parent)
:   QObject(parent),
    area(position)
{ }

HolonDockWidgetItem::HolonDockWidgetItem(HolonDockWidget *widget,
                                         Qt::DockWidgetArea position,
                                         HolonDockWidgetSplit *parent)
:   QObject(parent),
    area(position),
    dock(widget)
{
    setObjectName(dock->objectName());
    dock->titleBar()->setDockWidgetArea(area);
}
