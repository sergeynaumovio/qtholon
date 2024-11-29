// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflow_p.h"
#include "holonabstracttask.h"
#include "holonworkflow.h"

using namespace Qt::Literals::StringLiterals;

HolonWorkflowPrivate::HolonWorkflowPrivate(HolonWorkflow *q, HolonCore *c, HolonDesktop *desk)
:   q_ptr(q),
    core(c),
    desktop(desk)
{ }

HolonWorkflowPrivate::~HolonWorkflowPrivate()
{ }

void HolonWorkflowPrivate::addTask(HolonAbstractTask *task)
{
    Q_UNUSED(task)
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
