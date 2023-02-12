// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontask.h"
#include "holonnewtasksdir.h"
#include "holonopentasksdir.h"

HolonTask::HolonTask(QLoaderSettings *settings, HolonNewTasksDir *newTasksDir)
:   QObject(newTasksDir),
    QLoaderSettings(settings),
    d_ptr(new HolonTaskData)
{ }

HolonTask::HolonTask(QLoaderSettings *settings, HolonOpenTasksDir *openTasksDir)
:   QObject(openTasksDir),
    QLoaderSettings(settings),
    d_ptr(new HolonTaskData)
{ }

HolonTask::~HolonTask()
{ }
