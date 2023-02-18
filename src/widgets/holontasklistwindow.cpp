// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontasklistwindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holontask.h"
#include "holonworkflowmodel.h"
#include <QBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QListView>
#include <QLoaderTree>
#include <QUuid>

class HolonTaskListWindowPrivate
{
public:
    HolonTaskListWindow *const q_ptr;
    QLoaderSettings *const settings;
    QLoaderTree *const tree;
    HolonDesktop *const desktop;
    HolonWorkflowModel *workflowModel{};
    QListView *view{};


    HolonTaskListWindowPrivate(HolonTaskListWindow *q, QLoaderSettings *s, HolonDesktop *desk)
    :   q_ptr(q),
        settings(s),
        tree(s->tree()),
        desktop(desk)
    { }

    QWidget *widget()
    {
        if (!view)
            view = new QListView;

        if ((workflowModel = desktop->workflowModel()))
        {
            view->setModel(workflowModel);

            QListView::connect(view, &QListView::clicked, view, [](QModelIndex index)
            {
                QObject *clickedObject = static_cast<QObject *>(index.internalPointer());
                qDebug () << clickedObject;

                if (!qobject_cast<HolonTask *>(clickedObject))
                    return;
            });
        }

        return view;
    }
};

HolonTaskListWindow::HolonTaskListWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonWindow(settings, parent)
{ }

HolonTaskListWindow::HolonTaskListWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonWindow(settings, parent),
    d_ptr(new HolonTaskListWindowPrivate(this, settings, parent->desktop()))
{
    parent->addWindow(this);
}

HolonTaskListWindow::~HolonTaskListWindow()
{ }

HolonWindow::Areas HolonTaskListWindow::areas() const
{
    return HolonWindow::Sidebar;
}

HolonWindow::Attributes HolonTaskListWindow::attributes() const
{
    return {};
}

QIcon HolonTaskListWindow::icon() const
{
    return {};
}

bool HolonTaskListWindow::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonSidebar*>(parent))
            return true;
    }

    return false;
}

QString HolonTaskListWindow::title() const
{
    return "Open Tasks";
}

QWidget *HolonTaskListWindow::toolbar() const
{
    return {};
}

QWidget *HolonTaskListWindow::widget() const
{
    return d_ptr->widget();
}
