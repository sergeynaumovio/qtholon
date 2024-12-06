// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebarmainwindow.h"
#include "holonsidebar.h"
#include "holonsidebardockwidget.h"

using namespace Qt::Literals::StringLiterals;

class HolonSidebarMainWindowPrivate
{
public:
    HolonSidebarMainWindow *const q_ptr;
    HolonDesktopPrivate &desktop_d;
    QList<HolonSidebar *> sidebarList;

    QByteArray state;
    QMap<QString, HolonSidebarDockWidget *> groupDock;

    HolonSidebarMainWindowPrivate(HolonSidebarMainWindow *q, HolonDesktopPrivate &desk_d)
    :   q_ptr(q),
        desktop_d(desk_d)
    { }
};

HolonSidebarMainWindow::HolonSidebarMainWindow(HolonDesktopPrivate &desktop_d)
:   d_ptr(this, desktop_d)
{
    setDockOptions(QMainWindow::AnimatedDocks);
}

HolonSidebarMainWindow::~HolonSidebarMainWindow()
{ }

HolonSidebarDockWidget *HolonSidebarMainWindow::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->sidebarList.append(sidebar);

    QString dockName = sidebar->group();
    HolonSidebarDockWidget *sidebarDock;

    if (dockName.isEmpty() || !d_ptr->groupDock.contains(dockName))
    {
        if (dockName.isEmpty())
            dockName = sidebar->section().constLast();

        sidebarDock = new HolonSidebarDockWidget(d_ptr->desktop_d, dockName, this);
        addDockWidget(Qt::LeftDockWidgetArea, sidebarDock);

        if (!dockName.isEmpty())
            d_ptr->groupDock.insert(dockName, sidebarDock);
    }
    else
        sidebarDock = d_ptr->groupDock.value(dockName);

    sidebarDock->addSidebar(sidebar);

    if (d_ptr->state.isNull())
        d_ptr->state = sidebar->mainWindowNestingState();

    restoreState(d_ptr->state);

    return sidebarDock;
}

QList<HolonSidebar *> HolonSidebarMainWindow::sidebars() const
{
    return d_ptr->sidebarList;
}
