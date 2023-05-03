// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWITEM_H
#define HOLONWORKFLOWITEM_H

#include <QList>

class HolonAbstractTask;

class HolonWorkflowItem
{
public:
    HolonWorkflowItem *parent{};
    HolonAbstractTask *task{};
    QList<HolonWorkflowItem *> children;

    ~HolonWorkflowItem();
};

#endif // HOLONWORKFLOWITEM_H
