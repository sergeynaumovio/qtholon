// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractwindow.h"
#include "holonabstractwindow_p.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonwindowarea.h"
#include <QIcon>

using namespace Qt::Literals::StringLiterals;

HolonAbstractWindow::HolonAbstractWindow(QLoaderSettings *settings, HolonAbstractTask *parent)
:   QObject(parent),
    QLoaderSettings(settings),
    d_ptr(new HolonAbstractWindowPrivate(this, parent->desktop(), parent))
{ }

HolonAbstractWindow::HolonAbstractWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   QObject(parent),
    QLoaderSettings(settings),
    d_ptr(new HolonAbstractWindowPrivate(this, parent))
{ }

HolonAbstractWindow::HolonAbstractWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   QObject(parent),
    QLoaderSettings(settings),
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

HolonAbstractTask *HolonAbstractWindow::task() const
{
    return d_ptr->task;
}

QString HolonAbstractWindow::title() const
{
    return {};
}

QWidget *HolonAbstractWindow::toolbar() const
{
    return {};
}
