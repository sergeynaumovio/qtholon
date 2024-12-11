// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCUSTOMTASK_H
#define HOLONCUSTOMTASK_H

#include "holonabstracttask.h"

class HolonCustomTask : public HolonAbstractTask
{
    Q_OBJECT

public:
    HolonCustomTask(QLoaderSettings *settings, HolonTaskFolder *folder);
    HolonCustomTask(QLoaderSettings *settings, HolonWorkflow *workflow);
    ~HolonCustomTask();

    int exec() override;
    QWidget *parametersWidget() const override;
};

#endif // HOLONCUSTOMTASK_H
