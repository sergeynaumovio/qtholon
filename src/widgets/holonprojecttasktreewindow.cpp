// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonprojecttasktreewindow.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holontaskmodel.h"
#include "holontaskmodelbranch.h"
#include "holonworkflowmodelbranch.h"
#include "holonworkflowmodel.h"
#include <QBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QLoaderTree>
#include <QTreeView>
#include <QUuid>

using namespace Qt::Literals::StringLiterals;

class HolonProjectTaskTreeWindowPrivate
{
public:
    HolonProjectTaskTreeWindow *const q_ptr;
    QLoaderSettings *const settings;
    HolonDesktop *const desktop;
    HolonTaskModel *taskTreeModel{};
    QTreeView *view{};


    HolonProjectTaskTreeWindowPrivate(HolonProjectTaskTreeWindow *q = nullptr,
                               QLoaderSettings *s = nullptr,
                               HolonDesktop *desk = nullptr)
    :   q_ptr(q),
        settings(s),
        desktop(desk)
    { }

    QWidget *widget()
    {
        if (!q_ptr)
            return nullptr;

        if (view)
            return view;

        view = new QTreeView;

        if ((taskTreeModel = desktop->taskModel()))
        {
            view->setFrameStyle(QFrame::NoFrame);
            view->setModel(taskTreeModel);
            view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            view->header()->hide();

            QLoaderTree *const tree = settings->tree();

            QTreeView::connect(view, &QTreeView::doubleClicked, view, [=, this](QModelIndex index)
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

            auto setExpanded = [](const QModelIndex &index, bool expanded)
            {
                QObject *object = static_cast<QObject *>(index.internalPointer());
                if (HolonTaskModelBranch *dir = qobject_cast<HolonTaskModelBranch *>(object))
                    dir->setExpanded(expanded);
            };
            QTreeView::connect(view, &QTreeView::collapsed, view, [=](QModelIndex index){ setExpanded(index, false); });
            QTreeView::connect(view, &QTreeView::expanded, view, [=](QModelIndex index){ setExpanded(index, true); });
        }

        return view;
    }
};

HolonProjectTaskTreeWindow::HolonProjectTaskTreeWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonProjectTaskTreeWindow::HolonProjectTaskTreeWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(this, settings, parent->desktop())
{
    parent->addWindow(this);
}

HolonProjectTaskTreeWindow::~HolonProjectTaskTreeWindow()
{ }

Holon::WindowFlags HolonProjectTaskTreeWindow::flags() const
{
    return Holon::SidebarWindow | Holon::WindowAllButtonsHint;
}

QIcon HolonProjectTaskTreeWindow::icon() const
{
    return {};
}

bool HolonProjectTaskTreeWindow::isCopyable(const QStringList &to) const
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

QString HolonProjectTaskTreeWindow::title() const
{
    return u"Project Tasks"_s;
}

QWidget *HolonProjectTaskTreeWindow::toolbar() const
{
    return {};
}

QWidget *HolonProjectTaskTreeWindow::widget(const QString &group) const
{
    if (group == QString())
        return d_ptr->widget();

    return {};
}
