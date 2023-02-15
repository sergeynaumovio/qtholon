// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonnewtaskswindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holontask.h"
#include "holonopentasksdir.h"
#include <QBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QLoaderTree>
#include <QTreeView>
#include <QUuid>

class HolonNewTasksWindowPrivate
{
public:
    HolonNewTasksWindow *const q_ptr;
    QLoaderSettings *const settings;
    QLoaderTree *const tree;
    HolonDesktop *const desktop;
    QAbstractItemModel *model{};
    QTreeView *view{};


    HolonNewTasksWindowPrivate(HolonNewTasksWindow *q, QLoaderSettings *s, HolonDesktop *desk)
    :   q_ptr(q),
        settings(s),
        tree(s->tree()),
        desktop(desk)
    { }

    QWidget *widget()
    {
        if (!view)
            view = new QTreeView;

        if (desktop->models(Holon::NewTasks).size())
        {
            view->setModel(model = desktop->models(Holon::NewTasks).at(0));
            view->header()->hide();

            QTreeView::connect(view, &QTreeView::doubleClicked, view, [this](QModelIndex index)
            {
                QObject *clickedObject = static_cast<QObject *>(index.internalPointer());
                if (!qobject_cast<HolonTask *>(clickedObject))
                    return;

                QLoaderSettings *clickedObjectSettings = tree->settings(clickedObject);
                if (!clickedObjectSettings)
                    return;

                QList<QAbstractItemModel *> models = desktop->models(Holon::OpenTasks);
                QLoaderSettings *modelSettings = tree->settings(models.constFirst());
                if (!modelSettings)
                    return;

                QList<HolonOpenTasksDir *>
                dirs = models.constFirst()->findChildren<HolonOpenTasksDir *>(Qt::FindDirectChildrenOnly);

                if (dirs.isEmpty())
                    return;

                QStringList to = dirs.constFirst()->section();
                to.append(QUuid::createUuid().toString(QUuid::WithoutBraces));
                tree->copy(clickedObjectSettings->section(), to);
            });
        }

        return view;
    }
};

HolonNewTasksWindow::HolonNewTasksWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonWindow(settings, parent)
{ }

HolonNewTasksWindow::HolonNewTasksWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonWindow(settings, parent),
    d_ptr(new HolonNewTasksWindowPrivate(this, settings, parent->desktop()))
{
    parent->addWindow(this);
}

HolonNewTasksWindow::~HolonNewTasksWindow()
{ }

HolonWindow::Areas HolonNewTasksWindow::areas() const
{
    return HolonWindow::Sidebar;
}

HolonWindow::Attributes HolonNewTasksWindow::attributes() const
{
    return {};
}

QIcon HolonNewTasksWindow::icon() const
{
    return {};
}

bool HolonNewTasksWindow::isCopyable(const QStringList &to) const
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

QString HolonNewTasksWindow::title() const
{
    return "New Tasks";
}

QWidget *HolonNewTasksWindow::toolbar() const
{
    return {};
}

QWidget *HolonNewTasksWindow::widget() const
{
    return d_ptr->widget();
}
