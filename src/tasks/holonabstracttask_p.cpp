// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstracttask_p.h"
#include "holonabstracttask.h"
#include "holontaskmodel.h"
#include "holontaskmodelbranch.h"
#include "holonworkflowmodel.h"
#include "holonworkflowmodelbranch.h"
#include <QApplication>
#include <QMap>
#include <QStyle>

using namespace Qt::Literals::StringLiterals;

HolonAbstractTaskPrivate::HolonAbstractTaskPrivate(HolonAbstractTask *q,
                                                   HolonTaskModelBranch *branch)
:   q_ptr(q),
    taskModelBranch(branch),
    workflowModelBranch(nullptr),
    desktop(taskModelBranch->taskModel()->desktop())
{ }

HolonAbstractTaskPrivate::HolonAbstractTaskPrivate(HolonAbstractTask *q,
                                                   HolonWorkflowModelBranch *branch)
:   q_ptr(q),
    taskModelBranch(nullptr),
    workflowModelBranch(branch),
    desktop(workflowModelBranch->workflowModel()->desktop())
{ }

void HolonAbstractTaskPrivate::setCurrent(bool current)
{
    q_ptr->setValue(u"current"_s, current);
}
