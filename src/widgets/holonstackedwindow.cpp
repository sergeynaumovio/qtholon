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
:   HolonAbstractWindow(settings, parent),
    d_ptr(new HolonStackedWindowPrivate(this, parent->desktop()))
{
    parent->addWindow(this);
}

HolonStackedWindow::HolonStackedWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(new HolonStackedWindowPrivate(this, parent->desktop()))
{
    parent->addWindow(this);
}

HolonStackedWindow::~HolonStackedWindow()
{ }

void HolonStackedWindow::addWindow(HolonAbstractWindow *window)
{
    d_ptr->addWindow(window);
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
    d_ptr->setWindow(window);
}

QWidget *HolonStackedWindow::widget(int /*role*/) const
{
    return d_ptr->stackedWidget;
}
