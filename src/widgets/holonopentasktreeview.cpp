// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonopentasktreeview.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonopentasktreemodel.h"
#include "holonthemeicons.h"
#include "holontheme.h"
#include <QApplication>
#include <QEvent>
#include <QHeaderView>
#include <QMouseEvent>
#include <QPainter>
#include <QStyledItemDelegate>

#define emit

class HolonTaskDelegate : public QStyledItemDelegate
{
    HolonOpenTaskTreeView *const view;
    const int iconsSize;

public:
    HolonTaskDelegate(HolonOpenTaskTreeView *parent);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

HolonTaskDelegate::HolonTaskDelegate(HolonOpenTaskTreeView *parent)
:   QStyledItemDelegate(parent),
    view(parent),
    iconsSize(QApplication::style()->pixelMetric(QStyle::PM_ListViewIconSize))
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
        HolonThemeIcons *icons = view->desktop()->theme()->icons();
        QIcon icon = (option.state.testAnyFlag(QStyle::State_Selected) ? icons->closeBackgroundIcon()
                                                                       : icons->closeForegroundIcon());
        int voffset = (option.rect.height() - iconsSize) / 2;
        QRect iconRect(option.rect.right() - iconsSize, option.rect.top() + voffset, iconsSize, iconsSize);
        icon.paint(painter, iconRect);
    }
}

class HolonOpenTaskTreeViewPrivate
{
public:
    HolonDesktop *const desktop;
    QAbstractItemModel *openTaskTreeModel;
    HolonTaskDelegate *const itemDelegate;

    HolonOpenTaskTreeViewPrivate(HolonDesktop *desk,
                                 HolonTaskDelegate *delegate)
    :   desktop(desk),
        itemDelegate(delegate)
    { }
};

bool HolonOpenTaskTreeView::eventFilter(QObject *object, QEvent *event)
{
    if (object == this)
    {
        if (currentIndex().isValid())
        {
            if (event->type() == QEvent::KeyPress)
            {
                QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
                if (keyEvent->modifiers() == Qt::NoModifier && (keyEvent->key() == Qt::Key_Delete ||
                                                                keyEvent->key() == Qt::Key_Backspace))
                {
                    emit closeActivated(currentIndex());
                }
            }
        }
        else if (event->type() == QEvent::Show && d_ptr->openTaskTreeModel)
            if (HolonOpenTaskTreeModel *model = qobject_cast<HolonOpenTaskTreeModel *>(d_ptr->openTaskTreeModel))
                setCurrentIndex(model->restoreCurrentIndex());

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
}

HolonOpenTaskTreeView::~HolonOpenTaskTreeView()
{ }

HolonDesktop *HolonOpenTaskTreeView::desktop() const
{
    return d_ptr->desktop;
}

void HolonOpenTaskTreeView::setCurrentTask(HolonAbstractTask *task)
{
    QAbstractItemModel *model = d_ptr->openTaskTreeModel;

    for (int row{}; row < model->rowCount(); ++row)
    {
        QModelIndex index = model->index(row, 0);

        if (task == static_cast<HolonAbstractTask *>(index.internalPointer()))
            return setCurrentIndex(index);
    }
}

void HolonOpenTaskTreeView::setModel(QAbstractItemModel *model)
{
    d_ptr->openTaskTreeModel = model;

    QTreeView::setModel(model);
    header()->hide();
    header()->setStretchLastSection(false);
    header()->setSectionResizeMode(0, QHeaderView::Stretch);
    header()->setSectionResizeMode(1, QHeaderView::Fixed);
    header()->setMinimumSectionSize(1);
    header()->resizeSection(1, QApplication::style()->pixelMetric(QStyle::PM_ListViewIconSize));

    connect(model, &QAbstractItemModel::rowsInserted, this, [=, this](const QModelIndex &, int row)
    {
        QModelIndex index = model->index(row, 0);
        HolonAbstractTask *addedTask = static_cast<HolonAbstractTask *>(index.internalPointer());
        desktop()->setTask(addedTask);
        setCurrentIndex(index);
    });

    connect(this, &QTreeView::clicked, this, [=, this](const QModelIndex &index)
    {
        HolonAbstractTask *clickedTask = static_cast<HolonAbstractTask *>(index.internalPointer());
        desktop()->setTask(clickedTask);
    });

    connect(this, &HolonOpenTaskTreeView::closeActivated, this, [=, this](const QModelIndex &index)
    {
        model->removeRow(index.row());

        HolonAbstractTask *clickedTask = static_cast<HolonAbstractTask *>(index.internalPointer());
        desktop()->closeTask(clickedTask);
    });
}
