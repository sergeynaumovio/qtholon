// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonstackedwindow_p.h"
#include "holondesktop.h"
#include "holonstackedwidget.h"
#include "holontitlebar.h"

using namespace Qt::Literals::StringLiterals;

HolonStackedWindowPrivate::HolonStackedWindowPrivate(HolonStackedWindow *q, HolonDesktop *desk)
:   HolonAbstractWindowPrivate(q, desk),
    stackedWidget(new HolonWindowStackedWidget)
{ }

void HolonStackedWindowPrivate::addWindow(HolonAbstractWindow *window)
{
    stackedWidget->addWindowWidget(window, window->centralWidget());

    if (!window->contains(u"current"_s))
        window->d_ptr->setCurrent(true);

    titleBar->addWindow(window);
    desktop->addWindow(window);
}

HolonAbstractWindow *HolonStackedWindowPrivate::currentWindow() const
{
    return stackedWidget->currentWindow();
}

void HolonStackedWindowPrivate::setCurrentWindow(HolonAbstractWindow *window)
{
    if (currentWindow())
        currentWindow()->d_ptr->setCurrent(false);

    stackedWidget->setCurrentWindow(window);
    window->d_ptr->setCurrent(true);
}

