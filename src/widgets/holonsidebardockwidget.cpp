// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebardockwidget.h"
#include "holondesktop_p.h"
#include "holonsidebardockwidget_p.h"
#include "holonsidebarmainwindow.h"

void HolonSidebarDockWidget::resizeEvent(QResizeEvent *)
{
    if (isVisible())
        d_ptr->desktop_d().saveSidebarMainWindowState(d_ptr->sidebarMainWindow());
}

HolonSidebarDockWidget::HolonSidebarDockWidget(HolonDesktopPrivate &desktop_d,
                                               const QString &name,
                                               HolonSidebarMainWindow *parent)
:   QDockWidget(parent),
    d_ptr(desktop_d, this, name, parent)
{
    connect(this, &QDockWidget::dockLocationChanged, this, [this]()
    {
        if (isVisible())
            d_ptr->desktop_d().saveSidebarMainWindowState(d_ptr->sidebarMainWindow());
    });
}

HolonSidebarDockWidget::~HolonSidebarDockWidget()
{ }

void HolonSidebarDockWidget::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->addSidebar(sidebar);
}

QList<HolonSidebar *> HolonSidebarDockWidget::sidebars() const
{
    return d_ptr->sidebars();
}

void HolonSidebarDockWidget::showTitleBarWidget(bool show)
{
    d_ptr->showTitleBarWidget(show);
}
