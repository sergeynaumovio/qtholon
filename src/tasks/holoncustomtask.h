// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCUSTOMTASK_H
#define HOLONCUSTOMTASK_H

#include "holonabstracttask.h"
#include <QScopedStorage>

class HolonCustomTaskPrivate;

class HolonCustomTask : public HolonAbstractTask
{
    Q_OBJECT

    const QScopedStorage<HolonCustomTaskPrivate, 16> d_ptr;

public:
    HolonCustomTask(QLoaderSettings *settings, HolonTaskFolder *folder);
    HolonCustomTask(QLoaderSettings *settings, HolonWorkflow *workflow);
    ~HolonCustomTask();

    bool exec() override;
    QWidget *parametersWidget() override;
};

#endif // HOLONCUSTOMTASK_H
