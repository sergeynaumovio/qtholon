// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonopentasksmodel.h"
#include "holoncore.h"
#include "holondesktop.h"

class HolonOpenTasksModelPrivate
{
public:
    QLoaderSettings *settings;
};

HolonOpenTasksModel::HolonOpenTasksModel(QLoaderSettings *settings, HolonCore *core)
:   QAbstractItemModel(core),
    QLoaderSettings(settings),
    d_ptr(new HolonOpenTasksModelPrivate{settings})
{
    core->addModel(this, Holon::OpenTasks);
}

HolonOpenTasksModel::HolonOpenTasksModel(QLoaderSettings *settings, HolonDesktop *desktop)
:   QAbstractItemModel(desktop),
    QLoaderSettings(settings),
    d_ptr(new HolonOpenTasksModelPrivate{settings})
{
    desktop->addModel(this, Holon::OpenTasks);
}

HolonOpenTasksModel::~HolonOpenTasksModel()
{ }

int HolonOpenTasksModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}

QVariant HolonOpenTasksModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)

    return {};
}

QModelIndex HolonOpenTasksModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(row)
    Q_UNUSED(column)
    Q_UNUSED(parent)

    return {};
}

QModelIndex HolonOpenTasksModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)

    return {};
}

int HolonOpenTasksModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 1;
}
