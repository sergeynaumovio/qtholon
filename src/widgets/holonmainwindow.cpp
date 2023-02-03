// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

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
    QMap<QString, HolonSidebarDock *> groupDock;

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
{
    d.~HolonMainWindowPrivate();
}

HolonSidebarDock *HolonMainWindow::addSidebar(HolonSidebar *sidebar)
{
    QString dockName = sidebar->value("group").toString();
    HolonSidebarDock *sidebarDock;

    if (dockName.isEmpty() || !d.groupDock.contains(dockName))
    {
        if (dockName.isEmpty())
            dockName = sidebar->section().constLast();

        sidebarDock = new HolonSidebarDock(d.desktop_d, dockName, this);
        addDockWidget(Qt::LeftDockWidgetArea, sidebarDock);

        if (!dockName.isEmpty())
            d.groupDock.insert(dockName, sidebarDock);
    }
    else
        sidebarDock = d.groupDock.value(dockName);

    sidebarDock->addSidebar(sidebar);

    return sidebarDock;
}
