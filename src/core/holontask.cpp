// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontask.h"
#include "holonnewtasksdir.h"
#include "holonopentasksdir.h"
#include <QLoaderTree>

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

bool HolonTask::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonOpenTasksDir *>(parent))
            return true;
    }

    return false;

}

QString HolonTask::title() const
{
    return value("title", section().constLast()).toString();
}
