// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncustomtask.h"
#include "holondesktop.h"
#include "holontaskfolder.h"
#include "holonworkflowmodel.h"
#include "holonworkflowmodelbranch.h"
#include <QLabel>

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonTaskFolder *folder)
:   HolonAbstractTask(settings, folder)
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

QWidget *HolonCustomTask::widget(int widgetRole) const
{
    if (widgetRole == role())
        return new QLabel(title());

    return HolonAbstractTask::widget(widgetRole);
}

