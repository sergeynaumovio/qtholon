// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowswindow.h"
#include "holondesktop.h"
#include "holonwindowarea.h"
#include <QBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLoaderTree>

class HolonWorkflowsWindowPrivate
{
public:
    QWidget *widget{};
};

HolonWorkflowsWindow::HolonWorkflowsWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonWindow(settings, parent)
{ }

HolonWorkflowsWindow::HolonWorkflowsWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   HolonWindow(settings, parent),
    d_ptr(new HolonWorkflowsWindowPrivate)
{
    parent->addWindow(this);
}

HolonWorkflowsWindow::~HolonWorkflowsWindow()
{ }

HolonWindow::Areas HolonWorkflowsWindow::areas() const
{
    return HolonWindow::Sidebar;
}

HolonWindow::Attributes HolonWorkflowsWindow::attributes() const
{
    return {};
}

QIcon HolonWorkflowsWindow::icon() const
{
    return {};
}

bool HolonWorkflowsWindow::isCopyable(const QStringList &to) const
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

QString HolonWorkflowsWindow::title() const
{
    return "Workflows";
}

QWidget *HolonWorkflowsWindow::toolbar() const
{
    return {};
}

QWidget *HolonWorkflowsWindow::widget() const
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
