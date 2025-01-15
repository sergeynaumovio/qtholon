// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowitem.h"
#include <QList>

class HolonWorkflowItemPrivate
{
public:
    HolonWorkflowItem *parentItem;
    QList<HolonWorkflowItem *> childItems;
    int itemId;
    HolonWorkflowItem::ItemType itemType;

    HolonWorkflowItemPrivate(int taskIndex, HolonWorkflowItem::ItemType taskType, HolonWorkflowItem *parent)
    :   parentItem(parent),
        itemId(taskIndex),
        itemType(taskType)
    { }

    ~HolonWorkflowItemPrivate()
    {
        qDeleteAll(childItems);
    }
};

HolonWorkflowItem::HolonWorkflowItem(int taskId, HolonWorkflowItem::ItemType taskType, HolonWorkflowItem *parent)
:   d_ptr(taskId, taskType, parent)
{ }

HolonWorkflowItem::~HolonWorkflowItem()
{ }

void HolonWorkflowItem::appendChild(HolonWorkflowItem *item)
{
    d_ptr->childItems.append(item);
}

HolonWorkflowItem *HolonWorkflowItem::child(int row)
{
    return d_ptr->childItems.value(row);
}

int HolonWorkflowItem::childCount() const
{
    return d_ptr->childItems.count();
}

int HolonWorkflowItem::id() const
{
    return d_ptr->itemId;
}

HolonWorkflowItem *HolonWorkflowItem::parent()
{
    return d_ptr->parentItem;
}

int HolonWorkflowItem::row() const
{
    if (d_ptr->parentItem)
        return d_ptr->parentItem->d_ptr->childItems.indexOf(this);

    return 0;
}
