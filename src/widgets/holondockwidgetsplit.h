// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDOCKWIDGETSPLIT_H
#define HOLONDOCKWIDGETSPLIT_H

#include <QObject>

class HolonDockWidget;
class HolonWindowArea;

class HolonDockWidgetSplit : public QObject
{
    Q_OBJECT

public:
    const Qt::DockWidgetArea area;

    HolonDockWidgetSplit(HolonWindowArea *parent);
    HolonDockWidgetSplit(Qt::DockWidgetArea area, HolonDockWidgetSplit *parent);
};

class HolonDockWidgetItem : public QObject
{
    Q_OBJECT

public:
    Qt::DockWidgetArea area{};
    HolonDockWidget *const dock;

    HolonDockWidgetItem(HolonDockWidget *dock, Qt::DockWidgetArea area, HolonDockWidgetSplit *parent);
};

#endif // HOLONDOCKWIDGETSPLIT_H
