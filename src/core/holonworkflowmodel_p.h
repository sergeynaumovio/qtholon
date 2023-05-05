// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWMODEL_P_H
#define HOLONWORKFLOWMODEL_P_H

#include <QModelIndex>

class HolonAbstractTask;
class HolonCore;
class HolonDesktop;
class HolonWorkflowItem;
class HolonWorkflowModel;
class HolonWorkflowModelBranch;

class HolonWorkflowModelPrivate
{
public:
    HolonWorkflowModel *const q_ptr;
    HolonCore *const core;
    HolonDesktop *const desktop;
    bool once{true};
    QList<HolonWorkflowItem *> branchList;
    QList<HolonAbstractTask *> taskList;
    QModelIndex currentIndex;

    HolonWorkflowModelPrivate(HolonWorkflowModel *q, HolonCore *core, HolonDesktop *desktop);
    ~HolonWorkflowModelPrivate();

    void addBranch(HolonWorkflowModelBranch *branch);
    QObject *object(const QModelIndex &index) const;
    int rowCount();
    void setCurrent(bool current);
};

#endif // HOLONWORKFLOWMODEL_P_H
