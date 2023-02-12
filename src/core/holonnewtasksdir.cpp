// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonnewtasksdir.h"
#include "holonnewtasksmodel.h"
#include <QIcon>

HolonNewTasksDir::HolonNewTasksDir(QLoaderSettings *settings, HolonNewTasksModel *newTasksModel)
:   QObject(newTasksModel),
    QLoaderSettings(settings)
{ }

HolonNewTasksDir::HolonNewTasksDir(QLoaderSettings *settings, HolonNewTasksDir *newTaskDir)
:   QObject(newTaskDir),
    QLoaderSettings(settings)
{ }

QIcon HolonNewTasksDir::icon() const
{
    return QIcon(value("icon", ":/holon/holoniconlight.svg").toString());
}

QString HolonNewTasksDir::title() const
{
    return value("title", section().constLast()).toString();
}
