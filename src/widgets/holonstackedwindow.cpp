// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonstackedwindow.h"
#include "holonabstractwindow_p.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonstackedwidget.h"
#include "holonstackedwindow_p.h"
#include "holonwindowarea.h"
#include <QIcon>

HolonStackedWindow::HolonStackedWindow(QLoaderSettings *settings, HolonAbstractTask *parent)
:   HolonAbstractWindow(*new HolonStackedWindowPrivate(this, parent->desktop()), settings, parent)
{
    parent->addWindow(this);
}

HolonStackedWindow::HolonStackedWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   HolonAbstractWindow(*new HolonStackedWindowPrivate(this, parent->desktop()), settings, parent)
{
    parent->addWindow(this);
}

HolonStackedWindow::~HolonStackedWindow()
{ }

void HolonStackedWindow::addWindow(HolonAbstractWindow *window)
{
    Q_D(HolonStackedWindow);
    d->addWindow(window);
}

HolonDesktop *HolonStackedWindow::desktop() const
{
    return d_ptr->desktop;
}

Holon::WindowFlags HolonStackedWindow::flags() const
{
    return Holon::WindowAllButtonsHint;
}

void HolonStackedWindow::setWindow(HolonAbstractWindow *window)
{
    Q_D(HolonStackedWindow);
    d->setWindow(window);
}

QWidget *HolonStackedWindow::widget(int /*role*/) const
{
    Q_D(const HolonStackedWindow);
    return d->stackedWidget;
}
