// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskfoldermodel.h"
#include "holontaskfolder.h"
#include "holonabstracttask.h"
#include "holondesktop.h"

static QObject *objectFromIndex(const QModelIndex &index)
{
    return static_cast<QObject *>(index.internalPointer());
}

HolonTaskFolderModel::HolonTaskFolderModel(HolonDesktop *desk, QObject *parent)
:   QAbstractItemModel(parent),
    desktop(desk)
{ }

HolonTaskFolderModel::~HolonTaskFolderModel()
{ }

int HolonTaskFolderModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant HolonTaskFolderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DecorationRole)
    {
        QObject *object = objectFromIndex(index);

        if (HolonTaskFolder *folder = qobject_cast<HolonTaskFolder *>(object))
        {
            if (folder->isExpanded())
                return  folder->icon(Holon::Expanded);

            return folder->icon(Holon::Collapsed);
        }

        if (HolonAbstractTask *task = qobject_cast<HolonAbstractTask *>(object))
            return task->icon();

        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        QObject *object = objectFromIndex(index);

        if (HolonTaskFolder *folder = qobject_cast<HolonTaskFolder *>(object))
            return folder->title();

        if (HolonAbstractTask *task = qobject_cast<HolonAbstractTask *>(object))
            return task->title();

        return object->objectName();
    }

    return QVariant();
}

QModelIndex HolonTaskFolderModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid ())
        return createIndex(row, column, desktop->findChildren<HolonTaskFolder *>(Qt::FindDirectChildrenOnly).at(row));

    return createIndex(row, column, objectFromIndex(parent)->children().at(row));
}

QModelIndex HolonTaskFolderModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

     QObject *parent = objectFromIndex(child)->parent();

     if (qobject_cast<HolonDesktop *>(parent))
         return QModelIndex();

     return createIndex(parent->children().count(), 0, parent);
}

int HolonTaskFolderModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return desktop->findChildren<HolonTaskFolder *>(Qt::FindDirectChildrenOnly).count();

    if (HolonTaskFolder *folder = qobject_cast<HolonTaskFolder *>(objectFromIndex (parent)))
        return folder->children().count();

    return 0;
}
