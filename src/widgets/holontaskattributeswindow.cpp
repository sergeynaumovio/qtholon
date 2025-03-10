// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskattributeswindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holontaskattributeswindow_p.h"

HolonTaskAttributesWindow::HolonTaskAttributesWindow(HolonTaskAttributesWindowPrivate &d, QLoaderSettings *settings, QObject *parent)
:   HolonAbstractWindow(d, settings, parent)
{ }

HolonTaskAttributesWindow::HolonTaskAttributesWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{ }

HolonTaskAttributesWindow::HolonTaskAttributesWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(*new HolonTaskAttributesWindowPrivate(this, parent->desktop()), settings, parent)
{ }

HolonTaskAttributesWindow::~HolonTaskAttributesWindow()
{ }

QWidget *HolonTaskAttributesWindow::centralWidget()
{
    Q_D(HolonTaskAttributesWindow);
    return d->centralWidget();
}
