// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCUSTOMTASK_H
#define HOLONCUSTOMTASK_H

#include "holonabstracttask.h"
#include "holonnamespace.h"

class HolonTaskModelBranch;
class HolonWorkflowModelBranch;

class HolonCustomTask : public HolonAbstractTask
{
    Q_OBJECT

public:
    HolonCustomTask(QLoaderSettings *settings, HolonTaskModelBranch *taskModelBranch);
    HolonCustomTask(QLoaderSettings *settings, HolonWorkflowModelBranch *workflowModelBranch);
    ~HolonCustomTask();

    int exec() override;
    QWidget *widget(int role = Holon::NoRole) const override;
};

#endif // HOLONCUSTOMTASK_H
