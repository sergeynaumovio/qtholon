// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontasklistview.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonworkflowmodel.h"

void HolonTaskListView::showEvent(QShowEvent *)
{
    if (once && workflowModel)
    {
        setCurrentIndex(workflowModel->restoreCurrentIndex());
        once = false;
    }
}

HolonTaskListView::HolonTaskListView(HolonDesktop *desktop)
:   workflowModel(desktop->workflowModel())
{
    if (workflowModel)
    {
        setModel(workflowModel);

        connect(workflowModel, &QAbstractItemModel::rowsInserted, this, [=, this](const QModelIndex &, int row)
        {
            QModelIndex index = workflowModel->index(row);
            QObject *addedObject = static_cast<QObject *>(index.internalPointer());
            if (HolonAbstractTask *task = qobject_cast<HolonAbstractTask *>(addedObject))
            {
                desktop->setCurrentTask(task);
                setCurrentIndex(index);
            }
        });

        connect(this, &QListView::clicked, this, [=](QModelIndex index)
        {
            QObject *clickedObject = static_cast<QObject *>(index.internalPointer());
            if (HolonAbstractTask *task = qobject_cast<HolonAbstractTask *>(clickedObject))
                desktop->setCurrentTask(task);
        });
    }
}
