// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncustomtask.h"
#include "holondesktop.h"
#include "holontaskfolder.h"
#include "holonworkflow.h"
#include <QLabel>

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonTaskFolder *folder)
:   HolonAbstractTask(settings, folder)
{ }

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonWorkflow *workflow)
:   HolonAbstractTask(settings, workflow)
{
    workflow->desktop()->addTask(this);
}

HolonCustomTask::~HolonCustomTask()
{ }

bool HolonCustomTask::exec()
{
    return true;
}

QWidget *HolonCustomTask::parametersWidget()
{
    return new QLabel(title());
}

