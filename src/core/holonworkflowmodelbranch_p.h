// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWMODELBRANCH_P_H
#define HOLONWORKFLOWMODELBRANCH_P_H

#include <QHash>

class HolonAbstractTask;
class HolonWorkflowItem;
class HolonWorkflowModel;
class HolonWorkflowModelBranch;

class HolonWorkflowModelBranchPrivate
{
    bool restoreItemFromPath(const QString &path);

public:
    HolonWorkflowModelBranch *const q_ptr;
    HolonWorkflowModel *const workflowModel;
    bool expanded{};
    HolonWorkflowItem *rootItem;
    QHash<uint, HolonWorkflowItem *> children;

    HolonWorkflowModelBranchPrivate(HolonWorkflowModelBranch *q, HolonWorkflowModel *model);

    void addTask(HolonAbstractTask *task);
    bool restoreTreeState();
};

#endif // HOLONWORKFLOWMODELBRANCH_P_H
