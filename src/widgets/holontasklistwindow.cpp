// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontasklistwindow.h"
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
    HolonDesktop *const desktop;
    HolonWorkflowModel *workflowModel{};
    QListView *view{};


    HolonTaskListWindowPrivate(HolonTaskListWindow *q = nullptr,
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
    d_ptr(this, settings, parent->desktop())
{
    parent->addWindow(this);
}

HolonTaskListWindow::~HolonTaskListWindow()
{ }

HolonAbstractWindow::Areas HolonTaskListWindow::areas() const
{
    return HolonAbstractWindow::Sidebar | HolonAbstractWindow::areas();
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
        if (qobject_cast<HolonSidebar *>(parent))
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
