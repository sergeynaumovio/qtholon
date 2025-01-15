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
class HolonWorkflowModel;

class HolonWorkflowPrivate
{
public:
    HolonWorkflow *const q_ptr;
    HolonCore *const core;
    HolonDesktop *const desktop;
    HolonWorkflowModel *const model;
    QList<HolonAbstractTask *> taskList;
    QModelIndex currentIndex;

    HolonWorkflowPrivate(HolonWorkflow *q, HolonCore *core, HolonDesktop *desktop);
    ~HolonWorkflowPrivate();

    void addTask(HolonAbstractTask *task);
    int exec();
    QObject *object(const QModelIndex &index) const;
    void setCurrent(bool current);
};

#endif // HOLONWORKFLOW_P_H
