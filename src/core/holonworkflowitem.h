// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWITEM_H
#define HOLONWORKFLOWITEM_H

#include <QMetaObject>
#include <QScopedStorage>

class HolonWorkflowItemPrivate;

class HolonWorkflowItem
{
    Q_GADGET

    const QScopedStorage<HolonWorkflowItemPrivate, 40> d_ptr;

public:
    enum Icon
    {
        Branch,
        Task,
        Yes,
        No,
        Reference,
        Address
    };
    Q_ENUM(Icon)

    HolonWorkflowItem(int taskId, HolonWorkflowItem::Icon icon);
    ~HolonWorkflowItem();

    void appendChild(HolonWorkflowItem *item);
    HolonWorkflowItem *child(int row) const;
    int childCount() const;
    void clear();
    HolonWorkflowItem *findChild(int taskId) const;
    HolonWorkflowItem::Icon icon() const;
    HolonWorkflowItem *parent();
    int taskId() const;
    int row() const;
};

#endif // HOLONWORKFLOWITEM_H
