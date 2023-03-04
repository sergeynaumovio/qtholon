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
    d_ptr->desktop_d().saveDockWidgetWidth(this, e->size().width());
}

HolonSidebarDock::HolonSidebarDock(HolonDesktopPrivate &desktop_d,
                                   const QString &name,
                                   HolonMainWindow *parent)
:   QDockWidget(parent),
    d_ptr(desktop_d, this, name, parent)
{ }

HolonSidebarDock::~HolonSidebarDock()
{ }

void HolonSidebarDock::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->addSidebar(sidebar);
}

QList<HolonSidebar *> HolonSidebarDock::sidebars() const
{
    return d_ptr->sidebars();
}

void HolonSidebarDock::showTitleBarWidget(bool show)
{
    d_ptr->showTitleBarWidget(show);
}
