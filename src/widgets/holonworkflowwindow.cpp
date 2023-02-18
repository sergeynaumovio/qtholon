// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowwindow.h"
#include "holondesktop.h"
#include "holonwindowarea.h"
#include <QBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLoaderTree>

class HolonWorkflowWindowPrivate
{
public:
    QWidget *widget{};
};

HolonWorkflowWindow::HolonWorkflowWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonWindow(settings, parent)
{ }

HolonWorkflowWindow::HolonWorkflowWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   HolonWindow(settings, parent),
    d_ptr(new HolonWorkflowWindowPrivate)
{
    parent->addWindow(this);
}

HolonWorkflowWindow::~HolonWorkflowWindow()
{ }

HolonWindow::Areas HolonWorkflowWindow::areas() const
{
    return HolonWindow::Sidebar;
}

HolonWindow::Attributes HolonWorkflowWindow::attributes() const
{
    return {};
}

QIcon HolonWorkflowWindow::icon() const
{
    return {};
}

bool HolonWorkflowWindow::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonWindowArea *>(parent))
            return true;
    }

    return false;
}

QString HolonWorkflowWindow::title() const
{
    return "Workflow";
}

QWidget *HolonWorkflowWindow::toolbar() const
{
    return {};
}

QWidget *HolonWorkflowWindow::widget() const
{
    if (!d_ptr->widget)
    {
        d_ptr->widget = new QWidget(d_ptr->widget);
        {
            d_ptr->widget->setLayout(new QVBoxLayout(d_ptr->widget));
            {
                d_ptr->widget->layout()->addWidget(new QLabel("Workflows", d_ptr->widget));
            }
        }
    }

    return d_ptr->widget;
}
