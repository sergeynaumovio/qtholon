// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowmodel.h"
#include "holoncore.h"
#include "holondesktop.h"
#include "holontask.h"

class HolonOpenTasksModelPrivate
{
public:
    HolonWorkflowModel *const q_ptr;
    bool once{true};
    QList<HolonTask *> tasks;

    HolonOpenTasksModelPrivate(HolonWorkflowModel *q)
    :   q_ptr(q),
        tasks(q->findChildren<HolonTask *>().size())
    { }

    int rowCount()
    {
        if (once)
        {
            once = false;
            tasks = q_ptr->findChildren<HolonTask *>();
        }

        return tasks.size();
    }

    QObject *object(const QModelIndex &index) const
    {
        if (index.isValid())
            if (QObject *obj = static_cast<QObject *>(index.internalPointer()))
                return obj;

        return q_ptr;
    }
};

HolonWorkflowModel::HolonWorkflowModel(QLoaderSettings *settings, HolonCore *core)
:   HolonAbstractItemModel(settings, core),
    d_ptr(new HolonOpenTasksModelPrivate(this))
{
    core->addModel(this);
}

HolonWorkflowModel::HolonWorkflowModel(QLoaderSettings *settings, HolonDesktop *desktop)
:   HolonAbstractItemModel(settings, desktop),
    d_ptr(new HolonOpenTasksModelPrivate(this))
{
    desktop->addModel(this);
}

HolonWorkflowModel::~HolonWorkflowModel()
{ }

void HolonWorkflowModel::addBranch(HolonWorkflowModelBranch *branch)
{
    Q_UNUSED(branch)
}

QVariant HolonWorkflowModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole))
    {
        if (index.row() < d_ptr->tasks.size())
        {
            HolonTask *task = d_ptr->tasks.at(index.row());

            return task->title();
        }
    }

    return QVariant();
}

QModelIndex HolonWorkflowModel::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column);
}

bool HolonWorkflowModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(parent, position, position + rows);
    d_ptr->tasks.append(findChildren<HolonTask *>().constLast());
    endInsertRows();

    return true;
}

QModelIndex HolonWorkflowModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int HolonWorkflowModel::rowCount(const QModelIndex &) const
{
    return d_ptr->rowCount();
}

int HolonWorkflowModel::columnCount(const QModelIndex &) const
{
    return 1;
}
