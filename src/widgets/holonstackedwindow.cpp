// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonstackedwindow.h"
#include "holonabstractwindow_p.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holonstackedwidget.h"
#include "holonstackedwindow_p.h"
#include "holonwindowarea.h"
#include <QIcon>
#include <QLoaderTree>

HolonStackedWindow::HolonStackedWindow(HolonStackedWindowPrivate &d, QLoaderSettings *settings, QObject *parent)
:   HolonAbstractWindow(d, settings, parent)
{

}

HolonStackedWindow::HolonStackedWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   HolonAbstractWindow(*new HolonStackedWindowPrivate(this, parent->desktop()), settings, parent)
{
    parent->addWindow(this);
}

HolonStackedWindow::~HolonStackedWindow()
{ }

QWidget *HolonStackedWindow::centralWidget() const
{
    return d_func()->stackedWidget;
}

void HolonStackedWindow::addWindow(HolonAbstractWindow *window)
{
    Q_D(HolonStackedWindow);
    d->addWindow(window);
}

HolonDesktop *HolonStackedWindow::desktop() const
{
    return d_ptr->desktop;
}

bool HolonStackedWindow::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonSidebar *>(parent))
            return true;
    }

    return false;
}

void HolonStackedWindow::setWindow(HolonAbstractWindow *window)
{
    Q_D(HolonStackedWindow);
    d->setWindow(window);
}

QWidget *HolonStackedWindow::toolbarWidget() const
{
    return {};
}

HolonAbstractWindow *HolonStackedWindow::window() const
{
    Q_D(const HolonStackedWindow);
    return d->window();
}
