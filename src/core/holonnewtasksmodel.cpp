// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonnewtasksmodel.h"
#include "holondesktop.h"

class HolonNewTasksModelPrivate
{
public:
    HolonNewTasksModel *const q_ptr;

    HolonNewTasksModelPrivate(HolonNewTasksModel *q)
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

HolonNewTasksModel::HolonNewTasksModel(QLoaderSettings *settings, HolonDesktop *desktop)
:   QAbstractItemModel(desktop),
    QLoaderSettings(settings),
    d_ptr(new HolonNewTasksModelPrivate(this))
{
    desktop->addModel(this, Holon::NewTasks);
}

HolonNewTasksModel::~HolonNewTasksModel()
{ }

int HolonNewTasksModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant HolonNewTasksModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole))
        return d_ptr->object(index)->objectName();

    return QVariant();
}

QModelIndex HolonNewTasksModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    if (QObject *parentObject = d_ptr->object(parent))
        if (QObject *childObject = parentObject->children().at(row))
            return createIndex(row, column, childObject);

    return QModelIndex();
}

QModelIndex HolonNewTasksModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    QObject *childObject = d_ptr->object(child);
    QObject *parentObject = childObject ? childObject->parent() : nullptr;

    if (!parentObject || parentObject == this)
        return QModelIndex();

    return createIndex(parentObject->children().count(), 0, parentObject);
}

int HolonNewTasksModel::rowCount(const QModelIndex &parent) const
{
    return d_ptr->object(parent)->children().count();
}
