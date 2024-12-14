// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskwindowattributeswindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holontaskwindowattributeswindow_p.h"

using namespace Qt::Literals::StringLiterals;

HolonTaskWindowAttributesWindow::HolonTaskWindowAttributesWindow(HolonTaskWindowAttributesWindowPrivate &d,
                                                                 QLoaderSettings *settings,
                                                                 QObject *parent)
:   HolonAbstractWindow(d, settings, parent)
{ }

HolonTaskWindowAttributesWindow::HolonTaskWindowAttributesWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{ }

HolonTaskWindowAttributesWindow::HolonTaskWindowAttributesWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(*new HolonTaskWindowAttributesWindowPrivate(this, parent->desktop()), settings, parent)
{ }

HolonTaskWindowAttributesWindow::~HolonTaskWindowAttributesWindow()
{ }

QWidget *HolonTaskWindowAttributesWindow::centralWidget() const
{
    return static_cast<HolonTaskWindowAttributesWindowPrivate *>(d_ptr.get())->centralWidget();
}
