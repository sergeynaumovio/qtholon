// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowmodel_p.h"
#include "holonabstracttask.h"
#include "holonworkflowitem.h"
#include "holonworkflowmodel.h"
#include "holonworkflowmodelbranch.h"
#include "holonworkflowmodelbranch_p.h"

using namespace Qt::Literals::StringLiterals;

HolonWorkflowModelPrivate::HolonWorkflowModelPrivate(HolonWorkflowModel *q, HolonCore *c, HolonDesktop *desk)
:   q_ptr(q),
    core(c),
    desktop(desk)
{ }

HolonWorkflowModelPrivate::~HolonWorkflowModelPrivate()
{
    qDeleteAll(branchList);
}

void HolonWorkflowModelPrivate::addBranch(HolonWorkflowModelBranch *branch)
{
    branchList.append(branch->d_ptr->rootItem);
}

QObject *HolonWorkflowModelPrivate::object(const QModelIndex &index) const
{
    if (index.isValid())
        if (QObject *obj = static_cast<QObject *>(index.internalPointer()))
            return obj;

    return q_ptr;
}

int HolonWorkflowModelPrivate::rowCount()
{
    if (once)
    {
        once = false;
        taskList = q_ptr->findChildren<HolonAbstractTask *>();
    }

    return taskList.size();
}

void HolonWorkflowModelPrivate::setCurrent(bool current)
{
    q_ptr->setValue(u"current"_s, current);
}
