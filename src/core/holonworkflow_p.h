// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOW_P_H
#define HOLONWORKFLOW_P_H

#include <QModelIndex>

class HolonAbstractTask;
class HolonCore;
class HolonDesktop;
class HolonWorkflow;
class HolonWorkflowItem;

class HolonWorkflowPrivate
{
public:
    HolonWorkflow *const q_ptr;
    HolonCore *const core;
    HolonDesktop *const desktop;
    QList<HolonAbstractTask *> taskList;
    QModelIndex currentIndex;

    HolonWorkflowItem *rootItem;
    QHash<uint, HolonWorkflowItem *> children;

    HolonWorkflowPrivate(HolonWorkflow *q, HolonCore *core, HolonDesktop *desktop);
    ~HolonWorkflowPrivate();

    void addTask(HolonAbstractTask *task);
    QObject *object(const QModelIndex &index) const;
    void setCurrent(bool current);
};

#endif // HOLONWORKFLOW_P_H
