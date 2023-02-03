// Copyright (C) 2022 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include <QIcon>

HolonWindow::HolonWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   QObject(parent),
    QLoaderSettings(settings)
{
    parent->addWindow(this);
}

HolonWindow::HolonWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   QObject(parent),
    QLoaderSettings(settings)
{ }

HolonWindow::Areas HolonWindow::areas() const
{
    return {};
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
