// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontasklistwindow.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holontasklistview.h"
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
        if (view)
            return view;

        return view = new HolonTaskListView(desktop);
    }
};

HolonTaskListWindow::HolonTaskListWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonTaskListWindow::HolonTaskListWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(new HolonTaskListWindowPrivate(this, settings, parent->desktop()))
{
    parent->addWindow(this);
}

HolonTaskListWindow::~HolonTaskListWindow()
{ }

HolonAbstractWindow::Area HolonTaskListWindow::area() const
{
    return HolonAbstractWindow::Sidebar;
}

HolonAbstractWindow::Attributes HolonTaskListWindow::attributes() const
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

QWidget *HolonTaskListWindow::widget(const QString &group) const
{
    if (group == QString())
        return d_ptr->widget();

    return {};
}
