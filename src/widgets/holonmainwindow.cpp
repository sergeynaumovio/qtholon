// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonmainwindow.h"
#include "holonabstracttask.h"
#include "holondesktop_p.h"
#include "holonwindowarea.h"
#include <QHash>
#include <QLoaderTree>
#include <QStackedWidget>

using namespace Qt::Literals::StringLiterals;

class HolonMainWindowPrivate
{
public:
    HolonMainWindow *const q_ptr;
    HolonDesktopPrivate &desktop_d;

    QStackedWidget *const centralWindowAreas;
    QStackedWidget *const taskWorkspaces;
    QHash<HolonAbstractTask *, HolonWindowArea*> taskWorkspacesWindowAreas;

    HolonMainWindowPrivate(HolonMainWindow *q, HolonDesktopPrivate &desk_d)
    :   q_ptr(q),
        desktop_d(desk_d),
        centralWindowAreas(new QStackedWidget(q)),
        taskWorkspaces(new QStackedWidget(centralWindowAreas))
    {
        q_ptr->setCentralWidget(centralWindowAreas);
        centralWindowAreas->addWidget(taskWorkspaces);
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
        if (HolonWindowArea *windowArea = qobject_cast<HolonWindowArea *>(taskWorkspaces->currentWidget()))
            desktop_d.cacheMainWindowState(windowArea);

        if (HolonWindowArea *windowArea = taskWorkspacesWindowAreas.value(task))
        {
            desktop_d.restoreMainWindowStateFromCache(windowArea);
            taskWorkspaces->setCurrentWidget(windowArea);
        }
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

HolonMainWindow::HolonMainWindow(HolonDesktopPrivate &desktop_d)
:   d_ptr(this, desktop_d)
{
    setDockOptions(QMainWindow::AllowNestedDocks);
}

HolonMainWindow::~HolonMainWindow()
{ }

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
