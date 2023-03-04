// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonmainwindow.h"
#include "holonabstracttask.h"
#include "holondesktop_p.h"
#include "holonsidebar.h"
#include "holonsidebardock.h"
#include <QLabel>
#include <QLayout>
#include <QLoaderTree>
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
    QHash<HolonAbstractTask *, HolonWindowArea*> taskWorkspacesWindowAreas;
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

    HolonWindowArea *addWindowArea(HolonAbstractTask *task)
    {
        if (QLoaderSettings *settings = task->tree()->settings(task))
        {
            HolonWindowArea *windowArea = new HolonWindowArea(settings, desktop_d.q_ptr);
            taskWorkspaces->addWidget(windowArea);
            taskWorkspacesWindowAreas.insert(task, windowArea);

            if (task->isCurrent())
                taskWorkspaces->setCurrentWidget(taskWorkspacesWindowAreas.value(task));

            return windowArea;
        }

        return {};
    }

    void addWindowArea(HolonWindowArea *windowArea)
    {
        centralWindowAreas->addWidget(windowArea);
    }

    void setCurrentTask(HolonAbstractTask *task)
    {
        if (taskWorkspacesWindowAreas.contains(task))
            taskWorkspaces->setCurrentWidget(taskWorkspacesWindowAreas.value(task));
    }

    void setCurrentWindowArea(HolonWindowArea *windowArea)
    {
        if (windowArea)
            centralWindowAreas->setCurrentWidget(windowArea);
        else
            centralWindowAreas->setCurrentWidget(taskWorkspaces);
    }

    HolonWindowArea *windowArea(HolonAbstractTask *task)
    {
        return taskWorkspacesWindowAreas.value(task);
    }
};

HolonMainWindow::HolonMainWindow(HolonDesktopPrivate &desktop_d, QWidget *parent)
:   d_ptr(this, desktop_d, parent)
{
    setParent(parent);
    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);
}

HolonMainWindow::~HolonMainWindow()
{ }

HolonSidebarDock *HolonMainWindow::addSidebar(HolonSidebar *sidebar)
{
    QString dockName = sidebar->value("group").toString();
    HolonSidebarDock *sidebarDock;

    if (dockName.isEmpty() || !d_ptr->groupDock.contains(dockName))
    {
        if (dockName.isEmpty())
            dockName = sidebar->section().constLast();

        sidebarDock = new HolonSidebarDock(d_ptr->desktop_d, dockName, this);
        addDockWidget(Qt::LeftDockWidgetArea, sidebarDock);

        if (!dockName.isEmpty())
            d_ptr->groupDock.insert(dockName, sidebarDock);
    }
    else
        sidebarDock = d_ptr->groupDock.value(dockName);

    sidebarDock->addSidebar(sidebar);

    return sidebarDock;
}

HolonWindowArea *HolonMainWindow::addWindowArea(HolonAbstractTask *task)
{
    return d_ptr->addWindowArea(task);
}

void HolonMainWindow::addWindowArea(HolonWindowArea *windowArea)
{
    d_ptr->addWindowArea(windowArea);
}

void HolonMainWindow::setCurrentTask(HolonAbstractTask *task)
{
    d_ptr->setCurrentTask(task);
}

void HolonMainWindow::setCurrentWindowArea(HolonWindowArea *windowArea)
{
    d_ptr->setCurrentWindowArea(windowArea);
}

HolonWindowArea *HolonMainWindow::windowArea(HolonAbstractTask *task)
{
    return d_ptr->windowArea(task);
}
