// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonopentasksdir.h"
#include "holonopentasksmodel.h"
#include <QIcon>

HolonOpenTasksDir::HolonOpenTasksDir(QLoaderSettings *settings, HolonOpenTasksModel *openTasksModel)
:   QObject(openTasksModel),
    QLoaderSettings(settings)
{
    openTasksModel->addDir(this);
}

void HolonOpenTasksDir::addTask(HolonTask *task)
{
    Q_UNUSED(task)
}

QIcon HolonOpenTasksDir::icon() const
{
    return {};
}
