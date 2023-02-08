// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonmarkupcommanderwindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include <QBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QLoaderTree>

class HolonMarkupCommanderWindowPrivate
{
public:
    QWidget *widget{};
};

HolonMarkupCommanderWindow::HolonMarkupCommanderWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonWindow(settings, parent)
{ }

HolonMarkupCommanderWindow::HolonMarkupCommanderWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonWindow(settings, parent),
    d_ptr(new HolonMarkupCommanderWindowPrivate)
{
    parent->addWindow(this);
}

HolonMarkupCommanderWindow::~HolonMarkupCommanderWindow()
{ }

HolonWindow::Areas HolonMarkupCommanderWindow::areas() const
{
    return HolonWindow::Central;
}

HolonWindow::Attributes HolonMarkupCommanderWindow::attributes() const
{
    return {};
}

QIcon HolonMarkupCommanderWindow::icon() const
{
    return {};
}

bool HolonMarkupCommanderWindow::isCopyable(const QStringList &to) const
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

QString HolonMarkupCommanderWindow::title() const
{
    return "Markup Commander";
}

QWidget *HolonMarkupCommanderWindow::toolbar() const
{
    return {};
}

QWidget *HolonMarkupCommanderWindow::widget() const
{
    if (!d_ptr->widget)
    {
        d_ptr->widget = new QWidget(d_ptr->widget);
        {
            d_ptr->widget->setLayout(new QVBoxLayout(d_ptr->widget));
            {
                d_ptr->widget->layout()->addWidget(new QLabel("Markup Commander", d_ptr->widget));
            }
        }
    }

    return d_ptr->widget;
}
