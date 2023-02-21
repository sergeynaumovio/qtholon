// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncustomtask.h"
#include "holondesktop.h"
#include "holontaskmodelbranch.h"
#include "holonworkflowmodel.h"
#include "holonworkflowmodelbranch.h"
#include <QLabel>

class HolonCustomTaskPrivate
{

};

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonTaskModelBranch *taskModelBranch)
:   HolonAbstractTask(settings, taskModelBranch),
    d_ptr(new HolonCustomTaskPrivate)
{ }

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonWorkflowModelBranch *workflowModelBranch)
:   HolonAbstractTask(settings, workflowModelBranch),
    d_ptr(new HolonCustomTaskPrivate)
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

QWidget *HolonCustomTask::widget(const QString &widgetGroup) const
{
    if (widgetGroup == group())
        return new QLabel(title());

    return HolonAbstractTask::widget(widgetGroup);
}

