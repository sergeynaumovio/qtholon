// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowmodel.h"
#include "holonabstracttask.h"
#include "holonworkflow.h"
#include "holonworkflowitem.h"
#include <QIcon>

using namespace Qt::Literals::StringLiterals;

class HolonWorkflowModelPrivate
{
public:
    HolonWorkflow *const workflow;
    HolonWorkflowItem *const rootItem;

    HolonWorkflowModelPrivate(HolonWorkflow *w)
    :   workflow(w),
        rootItem(new HolonWorkflowItem(workflow->objectName().toInt(), HolonWorkflowItem::Branch))
    { }

    ~HolonWorkflowModelPrivate()
    {
        delete rootItem;
    }

    bool restoreStateFromPath(QStringView path)
    {
        auto fromString = [](QStringView element) -> HolonWorkflowItem::Icon
        {
            if (element == 't'_L1)
                return HolonWorkflowItem::Task;

            if (element == 'y'_L1)
                return HolonWorkflowItem::Yes;

            if (element == 'n'_L1)
                return HolonWorkflowItem::No;

            if (element == 'r'_L1)
                return HolonWorkflowItem::Reference;

            if (element == 'a'_L1)
                return HolonWorkflowItem::Address;

            return {};
        };

        HolonWorkflowItem *parent = rootItem;
        HolonWorkflowItem *item{};
        int taskId{};
        int last_i = path.count(u'/');

        int i{-1}, first_i{}, pre_last_i = last_i - 1;

        for (QStringView element : QStringTokenizer{path, u'/'})
        {
            ++i;

            if (i != last_i)
            {
                if (bool ok = (taskId = element.toInt(&ok), ok))
                {
                    if ((item = parent->findChild(taskId)))
                    {
                        parent = item;
                        continue;
                    }
                    else if (i == pre_last_i)
                        continue;
                }
            }
            else if (i != first_i)
            {
                if (HolonWorkflowItem::Icon icon = fromString(element))
                {
                    parent->appendChild(new HolonWorkflowItem(taskId, icon));
                    continue;
                }
            }

            return false;
        }

        return true;
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
    return 1;
}

QVariant HolonWorkflowModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DecorationRole)
    {
        HolonWorkflowItem *item = static_cast<HolonWorkflowItem *>(index.internalPointer());

        if (HolonAbstractTask *task = d_ptr->workflow->findChild<HolonAbstractTask *>(QString::number(item->taskId())))
            return task->icon();

        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        HolonWorkflowItem *item = static_cast<HolonWorkflowItem *>(index.internalPointer());

        if (HolonAbstractTask *task = d_ptr->workflow->findChild<HolonAbstractTask *>(QString::number(item->taskId())))
            return task->title();

        return QString(u"task id not found: "_s + QString::number(item->taskId()));
    }

    if (role == Qt::CheckStateRole)
        return Qt::Checked;

    return QVariant();
}

QModelIndex HolonWorkflowModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    HolonWorkflowItem *parentItem;

    if (!parent.isValid())
        parentItem = d_ptr->rootItem;
    else
        parentItem = static_cast<HolonWorkflowItem *>(parent.internalPointer());

    if (HolonWorkflowItem *childItem = parentItem->child(row))
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex HolonWorkflowModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    HolonWorkflowItem *childItem = static_cast<HolonWorkflowItem *>(index.internalPointer());
    HolonWorkflowItem *parentItem = childItem->parent();

    if (parentItem == d_ptr->rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

bool HolonWorkflowModel::restoreState()
{
    if (d_ptr->workflow->contains(u"modelState"_s))
    {
        const QString value(d_ptr->workflow->value(u"modelState"_s).toString());
        for (QStringView path : QStringTokenizer{value, u','})
        {
            if (path.isEmpty() || !d_ptr->restoreStateFromPath(path))
            {
                d_ptr->rootItem->clear();

                return false;
            }
        }
    }

    return true;
}

HolonWorkflowItem *HolonWorkflowModel::rootItem() const
{
    return d_ptr->rootItem;
}

int HolonWorkflowModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    HolonWorkflowItem *parentItem;

    if (!parent.isValid())
        parentItem = d_ptr->rootItem;
    else
        parentItem = static_cast<HolonWorkflowItem *>(parent.internalPointer());

    return parentItem->childCount();
}
