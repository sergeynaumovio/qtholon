/****************************************************************************
**
** Copyright (C) 2023 Sergey Naumov
**
** Permission to use, copy, modify, and/or distribute this
** software for any purpose with or without fee is hereby granted.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
** THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
** CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
** LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
** NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
** CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**
****************************************************************************/

#include "holondesktop_p.h"
#include "holonmainwindow.h"
#include "holonsidebar.h"
#include "holonsidebardock.h"
#include <QLabel>
#include <QShortcut>
#include <QStackedWidget>

class HolonMainWindowPrivate
{
public:
    HolonMainWindow *const q_ptr;
    HolonDesktopPrivate &desktop_d;
    QStackedWidget *const workspaces;
    QList<HolonSidebarDock *> docks;
    bool visibleTitleBar{};

    HolonMainWindowPrivate(HolonMainWindow *q, HolonDesktopPrivate &desk_d)
    :   q_ptr(q),
        desktop_d(desk_d),
        workspaces(new QStackedWidget(q))
    { }
};

HolonMainWindow::HolonMainWindow(HolonDesktopPrivate &desktop_d, QWidget *parent)
:   d_ptr(new HolonMainWindowPrivate(this, desktop_d))
{
    setParent(parent);

    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);
    setCentralWidget(d_ptr->workspaces);
    d_ptr->workspaces->addWidget(new QLabel("Workspaces", d_ptr->workspaces));

    QShortcut *shortcut = new QShortcut(QKeySequence(desktop_d.sidebarMoveShortcut()), this);
    connect(shortcut, &QShortcut::activated, this, [this]()
    {
        d_ptr->visibleTitleBar = !d_ptr->visibleTitleBar;
        for (HolonSidebarDock *area : d_ptr->docks)
            area->showTitleBarWidget(d_ptr->visibleTitleBar);
    });
}

HolonMainWindow::~HolonMainWindow()
{ }

void HolonMainWindow::addSidebar(HolonSidebar *sidebar)
{
    QString name = sidebar->value("group").toString();

    if (name.isEmpty())
        name = sidebar->section().constLast();

    HolonSidebarDock *sidebarDock = new HolonSidebarDock(d_ptr->desktop_d, name, this);
    addDockWidget(Qt::LeftDockWidgetArea, sidebarDock);
    d_ptr->docks.append(sidebarDock);
    sidebarDock->addSidebar(sidebar);
}
