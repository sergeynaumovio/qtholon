// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractwindow.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonwindowarea.h"
#include <QIcon>

class HolonAbstractWindowPrivate
{
public:
    HolonDesktop *const desktop;

    HolonAbstractWindowPrivate(HolonDesktop *desk)
    :   desktop(desk)
    { }
};

HolonAbstractWindow::HolonAbstractWindow(QLoaderSettings *settings, HolonAbstractTask *parent)
:   QObject(parent),
    QLoaderSettings(settings),
    d_ptr(new HolonAbstractWindowPrivate(parent->desktop()))
{ }

HolonAbstractWindow::HolonAbstractWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   QObject(parent),
    QLoaderSettings(settings),
    d_ptr(new HolonAbstractWindowPrivate(parent))
{ }


HolonAbstractWindow::HolonAbstractWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   QObject(parent),
    QLoaderSettings(settings),
    d_ptr(new HolonAbstractWindowPrivate(parent->desktop()))
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

QString HolonAbstractWindow::title() const
{
    return {};
}

QWidget *HolonAbstractWindow::toolbar() const
{
    return {};
}
