// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
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

void HolonStackedWindowPrivate::setWindow(HolonAbstractWindow *window)
{
    if (currentWindow)
        currentWindow->d_ptr->setCurrent(false);

    currentWindow = window;

    stackedWidget->setCurrentWindow(window);
    window->d_ptr->setCurrent(true);
}

HolonAbstractWindow *HolonStackedWindowPrivate::window() const
{
    return stackedWidget->window();
}
