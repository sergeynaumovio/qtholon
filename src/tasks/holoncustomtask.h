// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCUSTOMTASK_H
#define HOLONCUSTOMTASK_H

#include "holonabstracttask.h"
#include <QLoaderSettings>

class HolonCustomTaskPrivate;
class HolonTaskModelBranch;
class HolonWorkflowModelBranch;

class HolonCustomTask : public HolonAbstractTask
{
    Q_OBJECT

    const QScopedPointer<HolonCustomTaskPrivate> d_ptr;

public:
    HolonCustomTask(QLoaderSettings *settings, HolonTaskModelBranch *taskModelBranch);
    HolonCustomTask(QLoaderSettings *settings, HolonWorkflowModelBranch *workflowModelBranch);
    ~HolonCustomTask();

    int exec() override;
    QWidget *widget(const QString &group) const override;
};

#endif // HOLONCUSTOMTASK_H