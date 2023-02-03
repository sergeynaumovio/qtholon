// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebardock.h"
#include "holondesktop_p.h"
#include "holonmainwindow.h"
#include "holonsidebar.h"
#include "holonsidebardock_p.h"
#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QResizeEvent>
#include <QStackedWidget>


void HolonSidebarDock::resizeEvent(QResizeEvent *e)
{
    d.desktop_d().saveDockWidgetWidth(this, e->size().width());
}

HolonSidebarDock::HolonSidebarDock(HolonDesktopPrivate &desktop_d,
                                   const QString &name,
                                   HolonMainWindow *parent)
:   QDockWidget(parent),
    d(*new (&d_storage) HolonSidebarDockPrivate(desktop_d, this, name, parent))
{
    static_assert (sizeof (d_storage) == sizeof (HolonSidebarDockPrivate));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonSidebarDockPrivate));
}

HolonSidebarDock::~HolonSidebarDock()
{ }

void HolonSidebarDock::addSidebar(HolonSidebar *sidebar)
{
    d.stackedWidget()->addWidget(sidebar);
    d.stackedWidget()->setCurrentWidget(sidebar);
}

void HolonSidebarDock::showTitleBarWidget(bool show)
{
    d.showTitleBarWidget(show);
}
