// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncore.h"
#include "holoncore_p.h"

HolonCore::HolonCore(QLoaderSettings *settings, QObject *parent)
:   QObject(parent),
    QLoaderSettings(settings),
    d_ptr(new HolonCorePrivate)
{ }

HolonCore::~HolonCore()
{ }

void HolonCore::addTaskModel(HolonTaskModel *taskModel)
{
    d_ptr->taskModelList.append(taskModel);
}

void HolonCore::addWorkflowModel(HolonWorkflowModel *workflowModel)
{
    d_ptr->workflowModelList.append(workflowModel);
}
