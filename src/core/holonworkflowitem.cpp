// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowitem.h"
#include <QList>

class HolonWorkflowItemPrivate
{
public:
    HolonWorkflowItem *parentItem{};
    QList<HolonWorkflowItem *> childItems;
    int taskId;
    HolonWorkflowItem::Icon icon;

    HolonWorkflowItemPrivate(int taskIndex, HolonWorkflowItem::Icon ico)
    :   taskId(taskIndex),
        icon(ico)
    { }

    ~HolonWorkflowItemPrivate()
    {
        qDeleteAll(childItems);
    }
};

HolonWorkflowItem::HolonWorkflowItem(int taskId, HolonWorkflowItem::Icon icon)
:   d_ptr(taskId, icon)
{ }

HolonWorkflowItem::~HolonWorkflowItem()
{ }

void HolonWorkflowItem::appendChild(HolonWorkflowItem *item)
{
    item->d_ptr->parentItem = this;
    d_ptr->childItems.append(item);
}

HolonWorkflowItem *HolonWorkflowItem::child(int row) const
{
    return d_ptr->childItems.value(row);
}

int HolonWorkflowItem::childCount() const
{
    return d_ptr->childItems.count();
}

void HolonWorkflowItem::clear()
{
    qDeleteAll(d_ptr->childItems);
    d_ptr->childItems.clear();
}

HolonWorkflowItem *HolonWorkflowItem::findChild(int taskIndex) const
{
    for (HolonWorkflowItem *item : d_ptr->childItems)
        if (item->taskId () == taskIndex)
            return item;

    return nullptr;
}

HolonWorkflowItem::Icon HolonWorkflowItem::icon() const
{
    return d_ptr->icon;
}

HolonWorkflowItem *HolonWorkflowItem::parent()
{
    return d_ptr->parentItem;
}

int HolonWorkflowItem::taskId() const
{
    return d_ptr->taskId;
}

int HolonWorkflowItem::row() const
{
    if (d_ptr->parentItem)
        return d_ptr->parentItem->d_ptr->childItems.indexOf(this);

    return 0;
}
