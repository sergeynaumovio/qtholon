// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskmodel.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holontaskmodelbranch.h"

class HolonTaskModelPrivate
{
public:
    HolonTaskModel *const q_ptr;

    HolonTaskModelPrivate(HolonTaskModel *q)
    :   q_ptr(q)
    { }

    QObject *object(const QModelIndex &index) const
    {
        if (index.isValid())
            if (QObject *obj = static_cast<QObject *>(index.internalPointer()))
                return obj;

        return q_ptr;
    }
};

HolonTaskModel::HolonTaskModel(QLoaderSettings *settings, HolonDesktop *desktop)
:   HolonAbstractItemModel(settings, desktop),
    d_ptr(new HolonTaskModelPrivate(this))
{
    desktop->addModel(this);
}

HolonTaskModel::~HolonTaskModel()
{ }

int HolonTaskModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant HolonTaskModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole))
    {
        QObject *object = d_ptr->object(index);

        if (HolonTaskModelBranch *dir = qobject_cast<HolonTaskModelBranch *>(object))
            return dir->title();

        if (HolonAbstractTask *task = qobject_cast<HolonAbstractTask *>(object))
            return task->title();

        return d_ptr->object(index)->objectName();
    }

    return QVariant();
}

QModelIndex HolonTaskModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    if (QObject *parentObject = d_ptr->object(parent))
        if (QObject *childObject = parentObject->children().at(row))
            return createIndex(row, column, childObject);

    return QModelIndex();
}

QModelIndex HolonTaskModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    QObject *childObject = d_ptr->object(child);
    QObject *parentObject = childObject ? childObject->parent() : nullptr;

    if (!parentObject || parentObject == this)
        return QModelIndex();

    return createIndex(parentObject->children().count(), 0, parentObject);
}

int HolonTaskModel::rowCount(const QModelIndex &parent) const
{
    if (d_ptr->object(parent) == this || qobject_cast<HolonTaskModelBranch *>(d_ptr->object(parent)))
        return d_ptr->object(parent)->children().count();

    return 0;
}
