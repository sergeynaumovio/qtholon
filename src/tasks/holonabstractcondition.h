// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTCONDITION_H
#define HOLONABSTRACTCONDITION_H

#include "holonabstracttask.h"

class Q_HOLON_EXPORT HolonAbstractCondition : public HolonAbstractTask
{
    Q_OBJECT

protected:
    HolonAbstractCondition(QLoaderSettings *settings, HolonTaskFolder *folder);
    HolonAbstractCondition(QLoaderSettings *settings, HolonWorkflow *workflow);

public:
    ~HolonAbstractCondition();

    QIcon icon() const override;
};

#endif // HOLONABSTRACTCONDITION_H
