// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowmodel.h"
#include "holonabstracttask.h"
#include "holoncore.h"
#include "holondesktop.h"
#include "holonworkflowmodel_p.h"

using namespace Qt::Literals::StringLiterals;

HolonWorkflowModel::HolonWorkflowModel(QLoaderSettings *settings, HolonCore *core)
:   QAbstractItemModel(core),
    QLoaderSettings(this, settings),
    d_ptr(new HolonWorkflowModelPrivate(this, core, nullptr))
{
    core->addWorkflowModel(this);
}

HolonWorkflowModel::HolonWorkflowModel(QLoaderSettings *settings, HolonDesktop *desktop)
:   QAbstractItemModel(desktop),
    QLoaderSettings(this, settings),
    d_ptr(new HolonWorkflowModelPrivate(this, nullptr, desktop))
{
    desktop->addWorkflowModel(this);
}

HolonWorkflowModel::~HolonWorkflowModel()
{ }

void HolonWorkflowModel::addBranch(HolonWorkflowModelBranch *branch)
{
    d_ptr->addBranch(branch);
}

int HolonWorkflowModel::columnCount(const QModelIndex &) const
{
    return 2;
}

HolonCore *HolonWorkflowModel::core() const
{
    return d_ptr->core;
}

QVariant HolonWorkflowModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.column())
        return QVariant();

    if (index.row() < d_ptr->taskList.size())
    {
        HolonAbstractTask *task = d_ptr->taskList.at(index.row());

        if (role == Qt::DecorationRole)
            return task->icon();

        if (role == Qt::DisplayRole)
            return task->title();
    }

    return QVariant();
}

HolonDesktop *HolonWorkflowModel::desktop() const
{
    return d_ptr->desktop;
}

QModelIndex HolonWorkflowModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid())
        return QModelIndex();

    QModelIndex index = createIndex(row, column, d_ptr->taskList.at(row));

    HolonAbstractTask *task = d_ptr->taskList.at(row);
    if (task->isCurrent())
        d_ptr->currentIndex = index;

    return index;
}

bool HolonWorkflowModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(parent, position, position + rows);
    d_ptr->taskList.append(findChildren<HolonAbstractTask *>().constLast());
    endInsertRows();

    return true;
}

bool HolonWorkflowModel::isCurrent() const
{
    return value(u"current"_s).toBool();
}

QModelIndex HolonWorkflowModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

QModelIndex HolonWorkflowModel::restoreCurrentIndex() const
{
    return d_ptr->currentIndex;
}

int HolonWorkflowModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return d_ptr->rowCount();
}
