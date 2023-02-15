// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCUSTOMTASK_H
#define HOLONCUSTOMTASK_H

#include "holontask.h"
#include <QLoaderSettings>

class HolonCustomTaskPrivate;
class HolonNewTasksDir;
class HolonOpenTasksDir;

class HolonCustomTask : public HolonTask
{
    Q_OBJECT

    const QScopedPointer<HolonCustomTaskPrivate> d_ptr;

public:
    HolonCustomTask(QLoaderSettings *settings, HolonNewTasksDir *newTasksDir);
    HolonCustomTask(QLoaderSettings *settings, HolonOpenTasksDir *openTasksDir);

    int exec() override;
    QString title() const override;

    ~HolonCustomTask();
};

#endif // HOLONCUSTOMTASK_H
