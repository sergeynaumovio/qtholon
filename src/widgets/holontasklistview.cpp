// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontasklistview.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonworkflowmodel.h"
#include <QPainter>
#include <QStyledItemDelegate>

class HolonTaskDelegate : public QStyledItemDelegate
{
public:
    HolonTaskDelegate(QObject *parent);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

HolonTaskDelegate::HolonTaskDelegate(QObject *parent)
:   QStyledItemDelegate(parent)
{ }

void HolonTaskDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (option.state.testFlag(QStyle::State_MouseOver))
    {
        QBrush brush = option.palette.alternateBase();
        painter->fillRect(option.rect, brush);
    }

    QStyledItemDelegate::paint(painter, option, index);
}

class HolonTaskListViewPrivate
{
public:
    HolonWorkflowModel *const workflowModel;
    HolonTaskDelegate *const itemDelegate;
    bool once{true};

    HolonTaskListViewPrivate(HolonWorkflowModel *model,
                             HolonTaskDelegate *delegate)
    :   workflowModel(model),
        itemDelegate(delegate)
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
:   d_ptr(desktop->workflowModel(), new HolonTaskDelegate(this))
{
    setItemDelegate(d_ptr->itemDelegate);
    setFrameStyle(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    viewport()->setAttribute(Qt::WA_Hover);

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
