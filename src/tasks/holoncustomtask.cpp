// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncustomtask.h"
#include "holondesktop.h"
#include "holontaskmodelbranch.h"
#include "holonworkflowmodel.h"
#include "holonworkflowmodelbranch.h"
#include <QLabel>

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonTaskModelBranch *taskModelBranch)
:   HolonAbstractTask(settings, taskModelBranch)
{ }

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonWorkflowModelBranch *workflowModelBranch)
:   HolonAbstractTask(settings, workflowModelBranch)
{
    if (HolonWorkflowModel *workflowModel = workflowModelBranch->workflowModel())
    {
        if (HolonDesktop *desktop = workflowModel->desktop())
            desktop->addTask(this);
    }
}

HolonCustomTask::~HolonCustomTask()
{ }

int HolonCustomTask::exec()
{
    return 0;
}

QWidget *HolonCustomTask::widget(const QString &widgetRole) const
{
    if (widgetRole == role())
        return new QLabel(title());

    return HolonAbstractTask::widget(widgetRole);
}

