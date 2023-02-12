// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonnewtaskswindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include <QBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QIcon>
#include <QLoaderTree>
#include <QTreeView>

class HolonNewTasksWindowPrivate
{
public:
    HolonNewTasksWindow *const q_ptr;
    HolonDesktop *desktop;
    QTreeView *view{};

    HolonNewTasksWindowPrivate(HolonNewTasksWindow *q, HolonDesktop *desk)
    :   q_ptr(q),
        desktop(desk)
    { }

    QWidget *widget()
    {
        if (!view)
            view = new QTreeView;

        if (desktop->models(HolonDesktop::NewTasks).size())
        {
            view->setModel(desktop->models(HolonDesktop::NewTasks).at(0));
            view->header()->hide();
        }

        return view;
    }
};

HolonNewTasksWindow::HolonNewTasksWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonWindow(settings, parent)
{ }

HolonNewTasksWindow::HolonNewTasksWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonWindow(settings, parent),
    d_ptr(new HolonNewTasksWindowPrivate(this, parent->desktop()))
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
