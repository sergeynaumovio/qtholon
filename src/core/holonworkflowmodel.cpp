// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowmodel.h"
#include "holonabstracttask.h"
#include "holoncore.h"
#include "holondesktop.h"

using namespace Qt::Literals::StringLiterals;

class HolonOpenTasksModelPrivate
{
public:
    HolonWorkflowModel *const q_ptr;
    HolonCore *const core;
    HolonDesktop *const desktop;
    bool once{true};
    QList<HolonAbstractTask *> taskList;
    QModelIndex currentIndex;

    HolonOpenTasksModelPrivate(HolonWorkflowModel *q, HolonCore *c, HolonDesktop *desk)
    :   q_ptr(q),
        core(c),
        desktop(desk)
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
:   QAbstractItemModel(core),
    QLoaderSettings(settings),
    d_ptr(new HolonOpenTasksModelPrivate(this, core, nullptr))
{
    core->addWorkflowModel(this);
}

HolonWorkflowModel::HolonWorkflowModel(QLoaderSettings *settings, HolonDesktop *desktop)
:   QAbstractItemModel(desktop),
    QLoaderSettings(settings),
    d_ptr(new HolonOpenTasksModelPrivate(this, nullptr, desktop))
{
    desktop->addWorkflowModel(this);
}

HolonWorkflowModel::~HolonWorkflowModel()
{ }

void HolonWorkflowModel::addBranch(HolonWorkflowModelBranch *branch)
{
    Q_UNUSED(branch)
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
