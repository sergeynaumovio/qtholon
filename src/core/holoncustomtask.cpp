// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncustomtask.h"
#include "holontaskmodelbranch.h"
#include "holonworkflowmodelbranch.h"

class HolonCustomTaskPrivate
{

};

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonTaskModelBranch *taskModelBranch)
:   HolonTask(settings, taskModelBranch),
    d_ptr(new HolonCustomTaskPrivate)
{ }

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonWorkflowModelBranch *workflowModelBranch)
:   HolonTask(settings, workflowModelBranch),
    d_ptr(new HolonCustomTaskPrivate)
{ }

int HolonCustomTask::exec()
{
    return 0;
}

HolonCustomTask::~HolonCustomTask()
{ }
