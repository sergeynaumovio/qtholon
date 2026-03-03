// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractattributeswindow.h"
#include "holonabstractattributeswindow_p.h"
#include "holondesktop.h"
#include "holonsidebar.h"

using namespace Qt::Literals::StringLiterals;

HolonAbstractAttributesWindow::HolonAbstractAttributesWindow(HolonAbstractAttributesWindowPrivate &d,
                                                             QLoaderSettings *settings,
                                                             QObject *parent)
:   HolonAbstractWindow(d, settings, parent)
{ }

HolonAbstractAttributesWindow::HolonAbstractAttributesWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{ }

HolonAbstractAttributesWindow::HolonAbstractAttributesWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(*new HolonAbstractAttributesWindowPrivate(this, parent->desktop()), settings, parent)
{ }

HolonAbstractAttributesWindow::~HolonAbstractAttributesWindow()
{ }

QWidget *HolonAbstractAttributesWindow::centralWidget()
{
    Q_D(HolonAbstractAttributesWindow);
    return d->centralWidget();
}
