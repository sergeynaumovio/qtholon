// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonprojecttasktreewindow_p.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonid.h"
#include "holonopentasktreemodel.h"
#include "holontaskfolder.h"
#include "holontaskfoldermodel.h"
#include "holonprojecttasktreewindow.h"
#include "holontaskthread.h"
#include "holonworkflow.h"
#include "qloadertree.h"
#include <QHeaderView>
#include <QTreeView>

HolonProjectTaskTreeWindowPrivate::HolonProjectTaskTreeWindowPrivate(HolonProjectTaskTreeWindow *q,
                                                                     QLoaderSettings *s,
                                                                     HolonDesktop *desk)
:   q_ptr(q),
    settings(s),
    desktop(desk),
    model(desktop ? new HolonTaskFolderModel(desktop, q) : nullptr)
{ }

QWidget *HolonProjectTaskTreeWindowPrivate::widget()
{
    if (!q_ptr)
        return nullptr;

    if (view)
        return view;

    view = new QTreeView;

    view->setFrameStyle(QFrame::NoFrame);
    view->setModel(model);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->header()->hide();

    QLoaderTree *const tree = settings->tree();

    QTreeView::connect(view, &QTreeView::doubleClicked, view, [=, this](QModelIndex index)
    {
        if (desktop->taskThread()->isRunning())
            return;

        QObject *clickedObject = static_cast<QObject *>(index.internalPointer());
        if (!qobject_cast<HolonAbstractTask *>(clickedObject))
            return;

        QLoaderSettings *clickedObjectSettings = tree->settings(clickedObject);
        if (!clickedObjectSettings)
            return;

        [=, this]()
        {
            HolonWorkflow *workflow = desktop->workflow();
            if (!workflow)
                return;

            QString to = workflow->section();
            to += u'/' + QString::number(HolonId::createChildId(workflow));
            tree->copy(clickedObjectSettings->section(), to);
        }();

        openTaskTreeModel->insertRow(openTaskTreeModel->rowCount());
    });

    auto setExpanded = [](const QModelIndex &index, bool expanded)
    {
        QObject *object = static_cast<QObject *>(index.internalPointer());

        if (HolonTaskFolder *folder = qobject_cast<HolonTaskFolder *>(object))
            folder->setExpanded(expanded);
    };

    QTreeView::connect(view, &QTreeView::collapsed, view, [=](QModelIndex index){ setExpanded(index, false); });
    QTreeView::connect(view, &QTreeView::expanded, view, [=](QModelIndex index){ setExpanded(index, true); });

    return view;
}

void HolonProjectTaskTreeWindowPrivate::setOpenTaskTreeModel(HolonOpenTaskTreeModel *treeModel)
{
    openTaskTreeModel = treeModel;
}
