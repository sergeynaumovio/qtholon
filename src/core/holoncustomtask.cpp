// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncustomtask.h"
#include "holonnewtasksdir.h"
#include "holonopentasksdir.h"

class HolonCustomTaskPrivate
{

};

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonNewTasksDir *newTasksDir)
:   HolonTask(settings, newTasksDir),
    d_ptr(new HolonCustomTaskPrivate)
{ }

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonOpenTasksDir *openTasksDir)
:   HolonTask(settings, openTasksDir),
    d_ptr(new HolonCustomTaskPrivate)
{ }

int HolonCustomTask::exec()
{
    return 0;
}

QString HolonCustomTask::title() const
{
    return "Custom Task";
}

HolonCustomTask::~HolonCustomTask()
{ }
