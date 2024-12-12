// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractwindow.h"
#include "holonabstractwindow_p.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonstackedwindow.h"
#include "holonwindowarea.h"

using namespace Qt::Literals::StringLiterals;

HolonAbstractWindow::HolonAbstractWindow(HolonAbstractWindowPrivate &d, QLoaderSettings *settings, QObject *parent)
:   QObject(parent),
    QLoaderSettings(this, settings),
    d_ptr(&d)
{ }

HolonAbstractWindow::HolonAbstractWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   QObject(parent),
    QLoaderSettings(this, settings),
    d_ptr(new HolonAbstractWindowPrivate(this, parent))
{ }

HolonAbstractWindow::HolonAbstractWindow(QLoaderSettings *settings, HolonStackedWindow *parent)
:   QObject(parent),
    QLoaderSettings(this, settings),
    d_ptr(new HolonAbstractWindowPrivate(this, parent->desktop()))
{ }

HolonAbstractWindow::HolonAbstractWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   QObject(parent),
    QLoaderSettings(this, settings),
    d_ptr(new HolonAbstractWindowPrivate(this, parent->desktop()))
{ }

HolonAbstractWindow::~HolonAbstractWindow()
{ }

HolonDesktop *HolonAbstractWindow::desktop() const
{
    return d_ptr->desktop;
}

QIcon HolonAbstractWindow::icon() const
{
    return {};
}

bool HolonAbstractWindow::isCurrent() const
{
    return value(u"current"_s).toBool();
}

int HolonAbstractWindow::role() const
{
    return Holon::NoRole;
}

QString HolonAbstractWindow::title() const
{
    return {};
}
