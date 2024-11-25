// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflow_p.h"
#include "holonabstracttask.h"
#include "holonworkflow.h"
#include "holonworkflowitem.h"

using namespace Qt::Literals::StringLiterals;

HolonWorkflowPrivate::HolonWorkflowPrivate(HolonWorkflow *q, HolonCore *c, HolonDesktop *desk)
:   q_ptr(q),
    core(c),
    desktop(desk),
    rootItem(new HolonWorkflowItem)
{ }

HolonWorkflowPrivate::~HolonWorkflowPrivate()
{
    delete rootItem;
}

void HolonWorkflowPrivate::addTask(HolonAbstractTask *task)
{
    uint id = task->objectName().toUInt();

    if (children.contains(id))
        children.value(id)->task = task;
    else
    {
        HolonWorkflowItem *item = new HolonWorkflowItem;
        item->task = task;

        rootItem->children.append(item);
        children.insert(id, item);
    }
}

QObject *HolonWorkflowPrivate::object(const QModelIndex &index) const
{
    if (index.isValid())
        if (QObject *obj = static_cast<QObject *>(index.internalPointer()))
            return obj;

    return q_ptr;
}

void HolonWorkflowPrivate::setCurrent(bool current)
{
    q_ptr->setValue(u"current"_s, current);
}
