// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCUSTOMTASK_H
#define HOLONCUSTOMTASK_H

#include "holonabstracttask.h"
#include "holonnamespace.h"

class HolonTaskFoler;
class HolonWorkflowModelBranch;

class HolonCustomTask : public HolonAbstractTask
{
    Q_OBJECT

public:
    HolonCustomTask(QLoaderSettings *settings, HolonTaskFolder *folder);
    HolonCustomTask(QLoaderSettings *settings, HolonWorkflowModelBranch *workflowModelBranch);
    ~HolonCustomTask();

    int exec() override;
    QWidget *widget(int role = Holon::NoRole) const override;
};

#endif // HOLONCUSTOMTASK_H
