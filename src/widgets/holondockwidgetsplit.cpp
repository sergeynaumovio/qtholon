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
{
    setObjectName(toString(area));
}

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

HolonDockWidgetItem::HolonDockWidgetItem(const QString &objectName,
                                         Qt::DockWidgetArea position,
                                         HolonDockWidgetSplit *parent)
:   QObject(parent),
    area(position)
{
    setObjectName(objectName);
}

QString toString(Qt::DockWidgetArea area)
{
    switch (area) {
    case Qt::LeftDockWidgetArea: return "l";
    case Qt::RightDockWidgetArea: return "r";
    case Qt::TopDockWidgetArea: return "t";
    case Qt::BottomDockWidgetArea: return "b";
    case Qt::DockWidgetArea_Mask:
    case Qt::NoDockWidgetArea: return QString();
    }
}
