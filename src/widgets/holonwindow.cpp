// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindow.h"
#include "holondesktop.h"
#include "holonwindowarea.h"
#include <QIcon>

class HolonWindowPrivate
{
public:
    HolonWindowArea *windowArea{};
};

HolonWindow::HolonWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   QObject(parent),
    QLoaderSettings(settings)
{
    parent->addWindow(this);
}

HolonWindow::HolonWindow(QLoaderSettings *settings, HolonWindowArea *parent)
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
    if (d_ptr && d_ptr->windowArea)
        d_ptr->windowArea->closeWindow(this);
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
