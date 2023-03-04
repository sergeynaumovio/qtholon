// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontasktreewindow.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holontaskmodel.h"
#include "holonworkflowmodelbranch.h"
#include "holonworkflowmodel.h"
#include <QBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QLoaderTree>
#include <QTreeView>
#include <QUuid>

class HolonTaskTreeWindowPrivate
{
public:
    HolonTaskTreeWindow *const q_ptr;
    QLoaderSettings *const settings;
    QLoaderTree *const tree;
    HolonDesktop *const desktop;
    HolonTaskModel *taskTreeModel{};
    QTreeView *view{};


    HolonTaskTreeWindowPrivate(HolonTaskTreeWindow *q, QLoaderSettings *s, HolonDesktop *desk)
    :   q_ptr(q),
        settings(s),
        tree(s->tree()),
        desktop(desk)
    { }

    QWidget *widget()
    {
        if (view)
            return view;

        view = new QTreeView;

        if ((taskTreeModel = desktop->taskModel()))
        {
            view->setModel(taskTreeModel);
            view->header()->hide();

            QTreeView::connect(view, &QTreeView::doubleClicked, view, [this](QModelIndex index)
            {
                QObject *clickedObject = static_cast<QObject *>(index.internalPointer());
                if (!qobject_cast<HolonAbstractTask *>(clickedObject))
                    return;

                QLoaderSettings *clickedObjectSettings = tree->settings(clickedObject);
                if (!clickedObjectSettings)
                    return;

                HolonWorkflowModel *workflowModel = desktop->workflowModel();
                if (!workflowModel)
                    return;

                QLoaderSettings *workflowModelSettings = tree->settings(workflowModel);
                if (!workflowModelSettings)
                    return;

                QList<HolonWorkflowModelBranch *>
                branchList = workflowModel->findChildren<HolonWorkflowModelBranch *>(Qt::FindDirectChildrenOnly);

                if (branchList.isEmpty())
                    return;

                QStringList to = branchList.constFirst()->section();
                to.append(QUuid::createUuid().toString(QUuid::WithoutBraces));
                tree->copy(clickedObjectSettings->section(), to);

                workflowModel->insertRow(workflowModel->rowCount());
            });
        }

        return view;
    }
};

HolonTaskTreeWindow::HolonTaskTreeWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonTaskTreeWindow::HolonTaskTreeWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(new HolonTaskTreeWindowPrivate(this, settings, parent->desktop()))
{
    parent->addWindow(this);
}

HolonTaskTreeWindow::~HolonTaskTreeWindow()
{ }

HolonAbstractWindow::Area HolonTaskTreeWindow::area() const
{
    return HolonAbstractWindow::Sidebar;
}

HolonAbstractWindow::Attributes HolonTaskTreeWindow::attributes() const
{
    return {};
}

QIcon HolonTaskTreeWindow::icon() const
{
    return {};
}

bool HolonTaskTreeWindow::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonSidebar *>(parent))
            return true;
    }

    return false;
}

QString HolonTaskTreeWindow::title() const
{
    return "Project Tasks";
}

QWidget *HolonTaskTreeWindow::toolbar() const
{
    return {};
}

QWidget *HolonTaskTreeWindow::widget(const QString &group) const
{
    if (group == QString())
        return d_ptr->widget();

    return {};
}
