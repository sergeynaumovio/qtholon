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
#include <QLayout>
#include <QShortcut>
#include <QStackedWidget>

class HolonMainWindowPrivate
{
public:
    HolonMainWindow *const q_ptr;
    HolonDesktopPrivate &desktop_d;

    HolonMainWindow *const external;
    QStackedWidget *const workspaces;
    bool visibleTitleBar{};

    HolonMainWindowPrivate(HolonMainWindow *q, HolonDesktopPrivate &desk_d, QWidget *parent)
    :   q_ptr(q),
        desktop_d(desk_d),
        external(qobject_cast<HolonMainWindow *>(parent)),
        workspaces(external ? new QStackedWidget(q) : nullptr)
    { }
};

HolonMainWindow::HolonMainWindow(HolonDesktopPrivate &desktop_d, QWidget *parent)
:   d(*new (&d_storage) HolonMainWindowPrivate(this, desktop_d, parent))
{
    static_assert (sizeof (d_storage) == sizeof (HolonMainWindowPrivate));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonMainWindowPrivate));

    setParent(parent);

    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);

    if (d.external)
    {
        setCentralWidget(d.workspaces);
        d.workspaces->addWidget(new QLabel("Workspaces", d.workspaces));
        d.external->setCentralWidget(this);

        QShortcut *shortcut = new QShortcut(QKeySequence(desktop_d.sidebarMoveShortcut()), this);
        connect(shortcut, &QShortcut::activated, this, [this]()
        {
            d.visibleTitleBar = !d.visibleTitleBar;
            for (HolonSidebarDock *dock : d.desktop_d.sidebarDocks())
                dock->showTitleBarWidget(d.visibleTitleBar);
        });
    }
    else
        parent->layout()->addWidget(this);
}

HolonMainWindow::~HolonMainWindow()
{ }

HolonSidebarDock *HolonMainWindow::addSidebar(HolonSidebar *sidebar)
{
    QString name = sidebar->value("group").toString();

    if (name.isEmpty())
        name = sidebar->section().constLast();

    HolonSidebarDock *sidebarDock = new HolonSidebarDock(d.desktop_d, name, this);
    addDockWidget(Qt::LeftDockWidgetArea, sidebarDock);
    sidebarDock->addSidebar(sidebar);

    return sidebarDock;
}
