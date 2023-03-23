// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontasklistview.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonworkflowmodel.h"

class HolonTaskListViewPrivate
{
public:
    HolonWorkflowModel *const workflowModel;
    bool once{true};

    HolonTaskListViewPrivate(HolonWorkflowModel *model)
    :   workflowModel(model)
    { }
};

void HolonTaskListView::showEvent(QShowEvent *)
{
    if (d_ptr->once && d_ptr->workflowModel)
    {
        setCurrentIndex(d_ptr->workflowModel->restoreCurrentIndex());
        d_ptr->once = false;
    }
}

HolonTaskListView::HolonTaskListView(HolonDesktop *desktop)
:   d_ptr(desktop->workflowModel())
{
    setFrameStyle(QFrame::NoFrame);

    if (d_ptr->workflowModel)
    {
        setModel(d_ptr->workflowModel);

        connect(d_ptr->workflowModel, &QAbstractItemModel::rowsInserted, this, [=, this](const QModelIndex &, int row)
        {
            QModelIndex index = d_ptr->workflowModel->index(row);
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

HolonTaskListView::~HolonTaskListView()
{ }
