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

#include "holondesktop.h"
#include "holonmainwindow.h"
#include "holonsidebar.h"
#include "holonsidebardock.h"
#include <QLabel>
#include <QShortcut>
#include <QStackedWidget>

class HolonMainWindowPrivate
{
public:
    QList<HolonSidebarDock*> areas;
    bool visibleTitleBar{};
    QStackedWidget *workspaces;

    HolonMainWindowPrivate(QStackedWidget *ws)
    :   workspaces(ws)
    { }
};

HolonMainWindow::HolonMainWindow(HolonDesktop *desktop, QWidget *parent)
:   d_ptr(new HolonMainWindowPrivate(new QStackedWidget(this)))
{
    setParent(parent);

    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);

    for (const QString &name : desktop->sidebarAreaList())
    {
        HolonSidebarDock *area = new HolonSidebarDock(name, desktop, this);
        d_ptr->areas.append(area);
        addDockWidget(Qt::LeftDockWidgetArea, area);
    }

    for (HolonSidebarDock *area : d_ptr->areas)
        area->setSidebarAreasAdded(true);

    setCentralWidget(d_ptr->workspaces);
    d_ptr->workspaces->addWidget(new QLabel("Workspaces", d_ptr->workspaces));

    QString sidebarMoveShortcut = desktop->value("sidebarMoveShortcut").toString();
    QShortcut *shortcut = new QShortcut(QKeySequence(sidebarMoveShortcut), this);
    connect(shortcut, &QShortcut::activated, this, [this]()
    {
        d_ptr->visibleTitleBar = !d_ptr->visibleTitleBar;
        for (HolonSidebarDock *area : d_ptr->areas)
            area->showTitleBarWidget(d_ptr->visibleTitleBar);
    });
}

HolonMainWindow::~HolonMainWindow()
{ }

void HolonMainWindow::addSidebar(HolonSidebar *sidebar)
{
    for (HolonSidebarDock *area : d_ptr->areas)
    {
        if (area->objectName() == sidebar->sidebarArea())
        {
            area->addSidebar(sidebar);
            break;
        }
    }
}
