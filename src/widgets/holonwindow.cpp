// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include <QIcon>

class HolonWindowPrivate
{
public:
    HolonSidebar *sidebar{};
};

HolonWindow::HolonWindow(QLoaderSettings *&settings, HolonDesktop *parent)
:   QObject(parent),
    QLoaderSettings(settings)
{
    parent->addWindow(this);
}

HolonWindow::HolonWindow(QLoaderSettings *&settings, HolonSidebar *parent)
:   QObject(parent),
    QLoaderSettings(settings),
    d_ptr(new HolonWindowPrivate{parent})
{ }

HolonWindow::~HolonWindow()
{ }

HolonWindow::Areas HolonWindow::areas() const
{
    return Central | Sidebar;
}

HolonWindow::Attributes HolonWindow::attributes() const
{
    return WindowMinMaxButtonsHint | WindowCloseButtonHint;
}

void HolonWindow::close()
{
    if (d_ptr && d_ptr->sidebar)
        d_ptr->sidebar->closeWindow(this);
}

QIcon HolonWindow::icon() const
{
    return {};
}

QString HolonWindow::title() const
{
    return {};
}

QWidget *HolonWindow::toolbar() const
{
    return {};
}

QWidget *HolonWindow::widget() const
{
    return {};
}
