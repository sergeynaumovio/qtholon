// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonnewtaskswindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include <QBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QLoaderTree>

class HolonNewTasksWindowPrivate
{
public:
    QWidget *widget{};
};

HolonNewTasksWindow::HolonNewTasksWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonWindow(settings, parent)
{ }

HolonNewTasksWindow::HolonNewTasksWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonWindow(settings, parent),
    d_ptr(new HolonNewTasksWindowPrivate)
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
    if (!d_ptr->widget)
    {
        d_ptr->widget = new QWidget(d_ptr->widget);
        {
            d_ptr->widget->setLayout(new QVBoxLayout(d_ptr->widget));
            {
                d_ptr->widget->layout()->addWidget(new QLabel("New Tasks", d_ptr->widget));
            }
        }
    }

    return d_ptr->widget;
}
