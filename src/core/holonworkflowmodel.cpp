// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowmodel.h"
#include "holonabstracttask.h"
#include "holoncore.h"
#include "holondesktop.h"

class HolonOpenTasksModelPrivate
{
public:
    HolonWorkflowModel *const q_ptr;
    HolonCore *const core;
    HolonDesktop *const desktop;
    bool once{true};
    QList<HolonAbstractTask *> taskList;

    HolonOpenTasksModelPrivate(HolonWorkflowModel *q, HolonCore *c, HolonDesktop *desk)
    :   q_ptr(q),
        core(c),
        desktop(desk),
        taskList(q->findChildren<HolonAbstractTask *>().size())
    { }

    int rowCount()
    {
        if (once)
        {
            once = false;
            taskList = q_ptr->findChildren<HolonAbstractTask *>();
        }

        return taskList.size();
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
    d_ptr(new HolonOpenTasksModelPrivate(this, core, nullptr))
{
    core->addModel(this);
}

HolonWorkflowModel::HolonWorkflowModel(QLoaderSettings *settings, HolonDesktop *desktop)
:   HolonAbstractItemModel(settings, desktop),
    d_ptr(new HolonOpenTasksModelPrivate(this, nullptr, desktop))
{
    desktop->addModel(this);
}

HolonWorkflowModel::~HolonWorkflowModel()
{ }

void HolonWorkflowModel::addBranch(HolonWorkflowModelBranch *branch)
{
    Q_UNUSED(branch)
}

int HolonWorkflowModel::columnCount(const QModelIndex &) const
{
    return 1;
}

HolonCore *HolonWorkflowModel::core() const
{
    return d_ptr->core;
}

QVariant HolonWorkflowModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole))
    {
        if (index.row() < d_ptr->taskList.size())
        {
            HolonAbstractTask *task = d_ptr->taskList.at(index.row());

            return task->title();
        }
    }

    return QVariant();
}

HolonDesktop *HolonWorkflowModel::desktop() const
{
    return d_ptr->desktop;
}

QModelIndex HolonWorkflowModel::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column, d_ptr->taskList.at(row));
}

bool HolonWorkflowModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(parent, position, position + rows);
    d_ptr->taskList.append(findChildren<HolonAbstractTask *>().constLast());
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

