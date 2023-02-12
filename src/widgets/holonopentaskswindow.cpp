// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonopentaskswindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include <QBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QLoaderTree>

class HolonOpenTasksWindowPrivate
{
public:
    QWidget *widget{};
};

HolonOpenTasksWindow::HolonOpenTasksWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonWindow(settings, parent)
{ }

HolonOpenTasksWindow::HolonOpenTasksWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonWindow(settings, parent),
    d_ptr(new HolonOpenTasksWindowPrivate)
{
    parent->addWindow(this);
}

HolonOpenTasksWindow::~HolonOpenTasksWindow()
{ }

HolonWindow::Areas HolonOpenTasksWindow::areas() const
{
    return HolonWindow::Sidebar;
}

HolonWindow::Attributes HolonOpenTasksWindow::attributes() const
{
    return {};
}

QIcon HolonOpenTasksWindow::icon() const
{
    return {};
}

bool HolonOpenTasksWindow::isCopyable(const QStringList &to) const
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

QString HolonOpenTasksWindow::title() const
{
    return "Open Tasks";
}

QWidget *HolonOpenTasksWindow::toolbar() const
{
    return {};
}

QWidget *HolonOpenTasksWindow::widget() const
{
    if (!d_ptr->widget)
    {
        d_ptr->widget = new QWidget(d_ptr->widget);
        {
            d_ptr->widget->setLayout(new QVBoxLayout(d_ptr->widget));
            {
                d_ptr->widget->layout()->addWidget(new QLabel("Open Tasks", d_ptr->widget));
            }
        }
    }

    return d_ptr->widget;
}
