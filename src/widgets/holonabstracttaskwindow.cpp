// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstracttaskwindow.h"
#include "holonabstracttask.h"
#include "holonabstracttaskwindow_p.h"
#include "holondesktop.h"
#include "holontaskstackedwindow.h"

using namespace Qt::Literals::StringLiterals;

static HolonAbstractTask *task(HolonTaskStackedWindow *parent)
{
    return qobject_cast<HolonAbstractTask *>(parent->parent());
}

HolonAbstractTaskWindow::HolonAbstractTaskWindow(HolonAbstractTaskWindowPrivate &d,
                                                 QLoaderSettings *settings,
                                                 QObject *parent)
:   HolonAbstractWindow(d, settings, parent)
{ }

HolonAbstractTaskWindow::HolonAbstractTaskWindow(QLoaderSettings *settings, HolonAbstractTask *parent)
:   HolonAbstractWindow(*new HolonAbstractTaskWindowPrivate(this, parent->desktop(), parent), settings, parent)
{ }

HolonAbstractTaskWindow::HolonAbstractTaskWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(*new HolonAbstractTaskWindowPrivate(this, parent, nullptr), settings, parent)
{ }

HolonAbstractTaskWindow::HolonAbstractTaskWindow(QLoaderSettings *settings, HolonTaskStackedWindow *parent)
:   HolonAbstractWindow(*new HolonAbstractTaskWindowPrivate(this, parent->desktop(), ::task(parent)), settings, parent)
{ }

HolonAbstractTaskWindow::~HolonAbstractTaskWindow()
{ }

QWidget *HolonAbstractTaskWindow::customWidget(QMetaType /*taskWindowAttributesWindow*/) const
{
    return {};
}

HolonAbstractTask *HolonAbstractTaskWindow::task() const
{
    return d_func()->task;
}
