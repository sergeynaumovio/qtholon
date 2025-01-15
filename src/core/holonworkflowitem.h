// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWITEM_H
#define HOLONWORKFLOWITEM_H

#include <QScopedStorage>

class HolonWorkflowItemPrivate;

class HolonWorkflowItem
{
    const QScopedStorage<HolonWorkflowItemPrivate, 40> d_ptr;

public:
    enum ItemType
    {
        Branch,
        Task,
        ConditionTrue,
        ConditionFalse,
        Reference,
        Address
    };

    HolonWorkflowItem(int taskId, ItemType taskType, HolonWorkflowItem *parent = nullptr);
    ~HolonWorkflowItem();

    void appendChild(HolonWorkflowItem *item);
    HolonWorkflowItem *child(int row);
    int childCount() const;
    int id() const;
    HolonWorkflowItem *parent();
    int row() const;
};

#endif // HOLONWORKFLOWITEM_H
