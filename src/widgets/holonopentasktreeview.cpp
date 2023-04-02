// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonopentasktreeview.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonthemeicons.h"
#include "holontheme.h"
#include "holonworkflowmodel.h"
#include <QCoreApplication>
#include <QEvent>
#include <QHeaderView>
#include <QMouseEvent>
#include <QPainter>
#include <QStyledItemDelegate>

class HolonTaskDelegate : public QStyledItemDelegate
{
    HolonOpenTaskTreeView *const view;

public:
    HolonTaskDelegate(HolonOpenTaskTreeView *parent);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

HolonTaskDelegate::HolonTaskDelegate(HolonOpenTaskTreeView *parent)
:   QStyledItemDelegate(parent),
    view(parent)
{ }

void HolonTaskDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool isMouseOver = option.state.testFlag(QStyle::State_MouseOver);

    if (isMouseOver)
    {
        QBrush brush = option.palette.alternateBase();
        painter->fillRect(option.rect, brush);
    }

    QStyledItemDelegate::paint(painter, option, index);

    if (isMouseOver && index.column() == 1)
    {
        HolonThemeIcons *icons = view->desktop()->currentTheme()->icons();
        QIcon icon = (option.state.testAnyFlag(QStyle::State_Selected) ? icons->closeBackgroundIcon()
                                                                       : icons->closeForegroundIcon());
        int iconSize = 16;
        int voffset = (option.rect.height() - iconSize) / 2;
        QRect iconRect(option.rect.right() - iconSize, option.rect.top() + voffset, iconSize, iconSize);

        icon.paint(painter, iconRect);
    }
}

class HolonOpenTaskTreeViewPrivate
{
public:
    HolonDesktop *const desktop;
    HolonWorkflowModel *const workflowModel;
    HolonTaskDelegate *const itemDelegate;
    bool once{true};

    HolonOpenTaskTreeViewPrivate(HolonDesktop *desk,
                             HolonTaskDelegate *delegate)
    :   desktop(desk),
        workflowModel(desktop->workflowModel()),
        itemDelegate(delegate)
    { }
};

bool HolonOpenTaskTreeView::eventFilter(QObject *object, QEvent *event)
{
    if (object == this && event->type() == QEvent::KeyPress && currentIndex().isValid())
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->modifiers() == Qt::NoModifier && (keyEvent->key() == Qt::Key_Delete ||
                                                        keyEvent->key() == Qt::Key_Backspace))
        {
            emit closeActivated(currentIndex());
        }

        return false;
    }

    if (object == viewport() && event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->modifiers() == Qt::NoModifier)
        {
            QModelIndex index = indexAt(mouseEvent->pos());

            if (mouseEvent->button() == Qt::MiddleButton || (index.isValid() &&
                                                             index.column() == 1 &&
                                                             mouseEvent->button() == Qt::LeftButton))
            {
                emit closeActivated(index);

                QPoint cursorPos = QCursor::pos();
                QWidget *vp = viewport();
                QMouseEvent e(QEvent::MouseMove, vp->mapFromGlobal(cursorPos), cursorPos, Qt::NoButton, {}, {});
                QCoreApplication::sendEvent(vp, &e);

                return true;
            }
        }
    }

    return false;
}

void HolonOpenTaskTreeView::showEvent(QShowEvent *)
{
    if (d_ptr->once && d_ptr->workflowModel)
    {
        setCurrentIndex(d_ptr->workflowModel->restoreCurrentIndex());
        d_ptr->once = false;
    }
}

HolonOpenTaskTreeView::HolonOpenTaskTreeView(HolonDesktop *desktop)
:   d_ptr(desktop, new HolonTaskDelegate(this))
{
    setItemDelegate(d_ptr->itemDelegate);
    setFrameStyle(QFrame::NoFrame);
    setRootIsDecorated(false);
    setUniformRowHeights(true);
    setTextElideMode(Qt::ElideMiddle);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    viewport()->setAttribute(Qt::WA_Hover);
    viewport()->installEventFilter(this);

    installEventFilter(this);

    if (d_ptr->workflowModel)
    {
        HolonOpenTaskTreeView::setModel(d_ptr->workflowModel);

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

        connect(this, &QTreeView::clicked, this, [=](const QModelIndex &index)
        {
            QObject *clickedObject = static_cast<QObject *>(index.internalPointer());
            if (HolonAbstractTask *task = qobject_cast<HolonAbstractTask *>(clickedObject))
                desktop->setCurrentTask(task);
        });

        connect(this, &HolonOpenTaskTreeView::closeActivated, this, [=](const QModelIndex &index)
        {
            d_ptr->workflowModel->removeRow(index.row());

            QObject *clickedObject = static_cast<QObject *>(index.internalPointer());
            if (HolonAbstractTask *task = qobject_cast<HolonAbstractTask *>(clickedObject))
                desktop->closeTask(task);
        });
    }
}

HolonOpenTaskTreeView::~HolonOpenTaskTreeView()
{ }

HolonDesktop *HolonOpenTaskTreeView::desktop() const
{
    return d_ptr->desktop;
}

void HolonOpenTaskTreeView::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);
    header()->hide();
    header()->setStretchLastSection(false);
    header()->setSectionResizeMode(0, QHeaderView::Stretch);
    header()->setSectionResizeMode(1, QHeaderView::Fixed);
    header()->setMinimumSectionSize(1);
    header()->resizeSection(1, 16);
}
