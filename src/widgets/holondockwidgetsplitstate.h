// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDOCKWIDGETSPLITSTATE_H
#define HOLONDOCKWIDGETSPLITSTATE_H

#include <QObject>

class HolonDockWidget;
class HolonDockWidgetSplitState;
class HolonWindowArea;
class HolonWindowAreaPrivate;

class HolonDockWidgetSplit : public QObject
{
    Q_OBJECT

public:
    Qt::DockWidgetArea area{};

    HolonDockWidgetSplit(HolonDockWidgetSplitState *parent);
    HolonDockWidgetSplit(Qt::DockWidgetArea area, HolonDockWidgetSplit *parent);
};

class HolonDockWidgetItem : public QObject
{
    Q_OBJECT

public:
    Qt::DockWidgetArea area{};
    HolonDockWidget *dock{};

    HolonDockWidgetItem(HolonDockWidget *dock, Qt::DockWidgetArea area, HolonDockWidgetSplit *parent);
    HolonDockWidgetItem(QStringView objectName, Qt::DockWidgetArea area, HolonDockWidgetSplit *parent);
};

class HolonDockWidgetSplitState : public QObject
{
    Q_OBJECT

    HolonWindowAreaPrivate *const d_ptr;

    bool restoreSplitFromPath(QStringView path);
    void saveSplitState();
    void saveSplitStateRecursive(QStringList &splitState, const QString &section, QObject *parent);

public:
    HolonDockWidgetSplit *const rootSplit;

    HolonDockWidgetSplitState(HolonWindowAreaPrivate *d);

    void addSplit(HolonDockWidget *firstDock, HolonDockWidget *secondDock, Qt::Orientation orientation);
    void removeSplit(HolonDockWidget *dock);
    bool restoreSplitState();
    void setSplitItemDock(HolonDockWidget *dock);
};

#endif // HOLONDOCKWIDGETSPLITSTATE_H
