// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstracttask_p.h"
#include "holonabstracttask.h"
#include "holonworkflowmodelbranch.h"
#include "holonworkflowmodel.h"
#include <QMap>

HolonAbstractTaskPrivate::HolonAbstractTaskPrivate(HolonAbstractTask *q, HolonWorkflowModelBranch *branch)
:   q_ptr(q),
    workflowModelBranch(branch),
    desktop([this]() -> HolonDesktop *
    {
        if (workflowModelBranch)
            if (HolonWorkflowModel *workflowModel = workflowModelBranch->workflowModel())
                return workflowModel->desktop();

        return {};
    }())
{ }

void HolonAbstractTaskPrivate::setCurrent(bool current)
{
    q_ptr->setValue("current", current);
}