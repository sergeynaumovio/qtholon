// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonnewtasksdir.h"
#include "holonnewtasksmodel.h"
#include <QIcon>

HolonNewTasksDir::HolonNewTasksDir(QLoaderSettings *settings, HolonNewTasksModel *newTasksModel)
:   QObject(newTasksModel),
    QLoaderSettings(settings)
{
    newTasksModel->addDir(this);
}

HolonNewTasksDir::HolonNewTasksDir(QLoaderSettings *settings, HolonNewTasksDir *newTaskDir)
:   QObject(newTaskDir),
    QLoaderSettings(settings)
{
    newTaskDir->addDir(this);
}

void HolonNewTasksDir::addDir(HolonNewTasksDir *dir)
{
    Q_UNUSED(dir)
}

void HolonNewTasksDir::addTask(HolonTask *task)
{
    Q_UNUSED(task)
}

QIcon HolonNewTasksDir::icon() const
{
    return {};
}
