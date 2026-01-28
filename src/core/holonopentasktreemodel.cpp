// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonopentasktreemodel.h"
#include "holonabstracttask.h"
#include "holoncore.h"
#include "holondesktop.h"
#include "holonworkflow.h"

using namespace Qt::Literals::StringLiterals;

class HolonOpenTaskTreeModelPrivate
{
public:
    HolonOpenTaskTreeModel *const q_ptr;
    QList<HolonAbstractTask *> openTaskList;
    QModelIndex currentIndex;

    HolonOpenTaskTreeModelPrivate(HolonOpenTaskTreeModel *q)
    :   q_ptr(q)
    {
        const auto workflows = q_ptr->QObject::parent()->findChildren<HolonWorkflow *>();
        for (HolonWorkflow *workflow : workflows)
        {
            const auto tasks = workflow->findChildren<HolonAbstractTask *>(Qt::FindDirectChildrenOnly);
            for (HolonAbstractTask *task : tasks)
                if (task->isOpen())
                    openTaskList.append(task);
        }
    }
};

HolonOpenTaskTreeModel::HolonOpenTaskTreeModel(HolonCore *core)
:   QAbstractItemModel(core),
    d_ptr(this)
{ }

HolonOpenTaskTreeModel::HolonOpenTaskTreeModel(HolonDesktop *desktop)
:   QAbstractItemModel(desktop),
    d_ptr(this)
{ }

HolonOpenTaskTreeModel::~HolonOpenTaskTreeModel()
{ }

int HolonOpenTaskTreeModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant HolonOpenTaskTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.column())
        return QVariant();

    if (index.row() < d_ptr->openTaskList.size())
    {
        HolonAbstractTask *task = d_ptr->openTaskList.at(index.row());

        if (role == Qt::DecorationRole)
            return task->icon();

        if (role == Qt::DisplayRole)
            return task->title();
    }

    return QVariant();
}

QModelIndex HolonOpenTaskTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() || row < 0 || row > d_ptr->openTaskList.size() || column < 0 || column > 1)
        return QModelIndex();

    QModelIndex index = createIndex(row, column, d_ptr->openTaskList.at(row));

    HolonAbstractTask *task = d_ptr->openTaskList.at(row);
    if (task->isCurrent() && !d_ptr->currentIndex.isValid())
        d_ptr->currentIndex = index;

    return index;
}

bool HolonOpenTaskTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(parent, position, position + rows);
    if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(QObject::parent()))
        d_ptr->openTaskList.append(desktop->workflow()->findChildren<HolonAbstractTask *>().constLast());
    endInsertRows();

    return true;
}

QModelIndex HolonOpenTaskTreeModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

QModelIndex HolonOpenTaskTreeModel::restoreCurrentIndex() const
{
    return d_ptr->currentIndex;
}

int HolonOpenTaskTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return d_ptr->openTaskList.size();
}
