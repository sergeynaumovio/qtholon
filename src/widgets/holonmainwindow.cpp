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

    QStackedWidget *const centralWindowAreas;
    QStackedWidget *const taskWorkspaces;
    bool visibleTitleBar{};
    QMap<QString, HolonSidebarDock *> groupDock;

    HolonMainWindowPrivate(HolonMainWindow *q, HolonDesktopPrivate &desk_d, QWidget *parent)
    :   q_ptr(q),
        desktop_d(desk_d),
        external(qobject_cast<HolonMainWindow *>(parent)),
        centralWindowAreas(external ? new QStackedWidget(q) : nullptr),
        taskWorkspaces(centralWindowAreas ? new QStackedWidget(centralWindowAreas) : nullptr)
    {
        if (external)
        {
            q_ptr->setCentralWidget(centralWindowAreas);
            centralWindowAreas->addWidget(taskWorkspaces);
            taskWorkspaces->addWidget(new QLabel("Workspaces", taskWorkspaces));
            external->setCentralWidget(q_ptr);

            QShortcut *shortcut = new QShortcut(QKeySequence(desktop_d.sidebarMoveShortcut()), q_ptr);
            QObject::connect(shortcut, &QShortcut::activated, q_ptr, [this]()
            {
                visibleTitleBar = !visibleTitleBar;
                for (HolonSidebarDock *dock : desktop_d.sidebarDocks())
                    dock->showTitleBarWidget(visibleTitleBar);
            });
        }
        else
            parent->layout()->addWidget(q_ptr);
    }

    void addWindowArea(HolonWindowArea *windowArea)
    {
        centralWindowAreas->addWidget(windowArea);
    }
};

HolonMainWindow::HolonMainWindow(HolonDesktopPrivate &desktop_d, QWidget *parent)
:   d(*new (&d_storage) HolonMainWindowPrivate(this, desktop_d, parent))
{
    static_assert (sizeof (d_storage) == sizeof (HolonMainWindowPrivate));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonMainWindowPrivate));

    setParent(parent);
    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);
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

void HolonMainWindow::addWindowArea(HolonWindowArea *windowArea)
{
    d.addWindowArea(windowArea);
}

void HolonMainWindow::setCurrentWindowArea(HolonWindowArea *windowArea)
{
    if (windowArea)
        d.centralWindowAreas->setCurrentWidget(windowArea);
    else
        d.centralWindowAreas->setCurrentIndex(0);
}
