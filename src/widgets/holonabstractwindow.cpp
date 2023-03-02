// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractwindow.h"
#include "holonabstractwindow_p.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonwindowarea.h"
#include <QIcon>

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

HolonAbstractWindow::Attributes HolonAbstractWindow::attributes() const
{
    return WindowMinMaxButtonsHint | WindowCloseButtonHint;
}

HolonDesktop *HolonAbstractWindow::desktop() const
{
    return d_ptr->desktop;
}

QString HolonAbstractWindow::group() const
{
    return value("group").toString();
}

QIcon HolonAbstractWindow::icon() const
{
    return {};
}

bool HolonAbstractWindow::isCurrent() const
{
    return value("current").toBool();
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
