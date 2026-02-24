// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonstackedwindow.h"
#include "holonabstractwindow_p.h"
#include "holondesktop.h"
#include "holonid.h"
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

QWidget *HolonStackedWindow::centralWidget()
{
    return d_func()->stackedWidget;
}

void HolonStackedWindow::addWindow(HolonAbstractWindow *window)
{
    Q_D(HolonStackedWindow);
    d->addWindow(window);
}

HolonAbstractWindow *HolonStackedWindow::currentWindow() const
{
    Q_D(const HolonStackedWindow);
    return d->currentWindow ();
}

HolonDesktop *HolonStackedWindow::desktop() const
{
    return d_ptr->desktop;
}

bool HolonStackedWindow::isCopyable(QStringView to) const
{
    QStringView toParent = HolonId::parentSection(to);

    if (toParent.size())
    {
        if (QObject *parent = tree()->object(toParent))
            if (qobject_cast<HolonSidebar *>(parent))
                return true;
    }

    return false;
}

void HolonStackedWindow::setCurrentWindow(HolonAbstractWindow *window)
{
    Q_D(HolonStackedWindow);
    d->setCurrentWindow(window);
}

QWidget *HolonStackedWindow::toolbarWidget()
{
    return {};
}

