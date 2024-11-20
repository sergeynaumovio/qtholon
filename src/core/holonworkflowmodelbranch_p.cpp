// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowmodelbranch_p.h"
#include "holonabstracttask.h"
#include "holonworkflowitem.h"
#include "holonworkflowmodelbranch.h"

using namespace Qt::Literals::StringLiterals;

bool HolonWorkflowModelBranchPrivate::restoreItemFromPath(const QString &path)
{
    QStringList list = path.split(u'/');

    if (list.first() == ""_L1)
        list.removeFirst();

    return true;
}

HolonWorkflowModelBranchPrivate::HolonWorkflowModelBranchPrivate(HolonWorkflowModelBranch *q,
                                                                 HolonWorkflowModel *model)
:   q_ptr(q),
    workflowModel(model),
    rootItem(new HolonWorkflowItem)
{ }

void HolonWorkflowModelBranchPrivate::addTask(HolonAbstractTask *task)
{
    uint id = task->objectName().toUInt();

    if (HolonWorkflowItem *item = children.value(id))
        item->task = task;
    else
    {
        item = new HolonWorkflowItem;
        item->task = task;
        rootItem->children.append(item);
        children.insert(id, item);
    }
}

bool HolonWorkflowModelBranchPrivate::restoreTreeState()
{
    QString value(q_ptr->value(u"taskTreeState"_s).toString());
    QStringList list = value.split(u',');
    bool isValid{true};

    for (const QString &path : list)
        if (bool error = !restoreItemFromPath(path))
        {
            isValid = false;
            Q_UNUSED(error)
        }

    return isValid;
}
