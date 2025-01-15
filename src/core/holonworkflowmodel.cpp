// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowmodel.h"
#include "holonworkflow.h"
#include "holonworkflowitem.h"

using namespace Qt::Literals::StringLiterals;

class HolonWorkflowModelPrivate
{
public:
    HolonWorkflow *const workflow;
    const QScopedPointer<HolonWorkflowItem> rootItem;

    HolonWorkflowModelPrivate(HolonWorkflow *w)
    :   workflow(w),
        rootItem(new HolonWorkflowItem(workflow->objectName().toInt(), HolonWorkflowItem::Branch))
    { }

    bool restoreState()
    {
        return false;
    }
};

HolonWorkflowModel::HolonWorkflowModel(HolonWorkflow *workflow)
:   QAbstractItemModel(workflow),
    d_ptr(workflow)
{ }

HolonWorkflowModel::~HolonWorkflowModel()
{ }

int HolonWorkflowModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

QVariant HolonWorkflowModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index);
    Q_UNUSED(role);

    return QVariant();
}

QModelIndex HolonWorkflowModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    return QModelIndex();
}

QModelIndex HolonWorkflowModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);

    return QModelIndex();
}

bool HolonWorkflowModel::restoreState()
{
    if (d_ptr->workflow->contains(u"modelState"_s))
        return d_ptr->restoreState();

    return true;
}

HolonWorkflowItem *HolonWorkflowModel::rootItem() const
{
    return d_ptr->rootItem.get();
}

int HolonWorkflowModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 0;
}
