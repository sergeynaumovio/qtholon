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
    HolonWindowArea *windowArea{};
};

HolonAbstractWindow::HolonAbstractWindow(QLoaderSettings *settings, HolonAbstractTask *parent)
:   QObject(parent),
    QLoaderSettings(settings)
{ }

HolonAbstractWindow::HolonAbstractWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   QObject(parent),
    QLoaderSettings(settings)
{ }


HolonAbstractWindow::HolonAbstractWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   QObject(parent),
    QLoaderSettings(settings),
    d_ptr(new HolonAbstractWindowPrivate{parent})
{ }

HolonAbstractWindow::~HolonAbstractWindow()
{ }

HolonAbstractWindow::Areas HolonAbstractWindow::areas() const
{
    return Central | Sidebar;
}

HolonAbstractWindow::Attributes HolonAbstractWindow::attributes() const
{
    return WindowMinMaxButtonsHint | WindowCloseButtonHint;
}

void HolonAbstractWindow::close()
{
    if (d_ptr && d_ptr->windowArea)
        d_ptr->windowArea->closeWindow(this);
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
