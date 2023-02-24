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
    QWidget *central{};

    QWidget *widget()
    {
        if (central)
            return central;

        central = new QWidget;
        central->setLayout(new QVBoxLayout(central));
        central->layout()->addWidget(new QLabel("Workflows", central));

        return central;
    }
};

HolonWorkflowWindow::HolonWorkflowWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonWorkflowWindow::HolonWorkflowWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(new HolonWorkflowWindowPrivate)
{
    parent->addWindow(this);
}

HolonWorkflowWindow::~HolonWorkflowWindow()
{ }

HolonAbstractWindow::Area HolonWorkflowWindow::area() const
{
    return HolonAbstractWindow::Central;
}

HolonAbstractWindow::Attributes HolonWorkflowWindow::attributes() const
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

QWidget *HolonWorkflowWindow::widget(Area area) const
{
    if (area == Central)
        return d_ptr->widget();

    return {};
}
