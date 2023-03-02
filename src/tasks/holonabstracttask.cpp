// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstracttask.h"
#include "holonabstracttask_p.h"
#include "holonabstractwidget.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holontaskmodelbranch.h"
#include "holonworkflowmodel.h"
#include "holonworkflowmodelbranch.h"
#include <QLoaderTree>

HolonAbstractTask::HolonAbstractTask(QLoaderSettings *settings, HolonTaskModelBranch *taskModelBranch)
:   QObject(taskModelBranch),
    QLoaderSettings(settings),
    d_ptr(new HolonAbstractTaskPrivate)
{ }

HolonAbstractTask::HolonAbstractTask(QLoaderSettings *settings, HolonWorkflowModelBranch *workflowModelBranch)
:   QObject(workflowModelBranch),
    QLoaderSettings(settings),
    d_ptr(new HolonAbstractTaskPrivate(this, workflowModelBranch))
{ }

HolonAbstractTask::~HolonAbstractTask()
{ }

void HolonAbstractTask::addWidget(HolonAbstractWidget *widget)
{
    if (d_ptr->widgetList.contains(widget->group()))
        return;

    d_ptr->widgetList[widget->group()] = widget;
}

void HolonAbstractTask::addWindow(HolonAbstractWindow *window)
{
    if (d_ptr->windowList[window->group()].contains(window))
        return;

    d_ptr->windowList[window->group()].append(window);

    if (d_ptr->workflowModelBranch)
        if (HolonWorkflowModel *workflowModel = d_ptr->workflowModelBranch->workflowModel())
            if (HolonDesktop *desktop = workflowModel->desktop())
                desktop->addWindow(window);
}

HolonDesktop *HolonAbstractTask::desktop() const
{
    if (d_ptr)
        return d_ptr->desktop;

    return {};
}

QString HolonAbstractTask::group() const
{
    return value("group").toString();
}

bool HolonAbstractTask::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonWorkflowModelBranch *>(parent))
            return true;
    }

    return false;
}

bool HolonAbstractTask::isCurrent() const
{
    return value("current").toBool();
}

QString HolonAbstractTask::title() const
{
    return value("title", section().constLast()).toString();
}

QWidget *HolonAbstractTask::widget(const QString &group) const
{
    QWidget *widget{};

    if (d_ptr->widgetList.contains(group))
        widget = d_ptr->widgetList.value(group)->widget();

    return widget;
}

QList<HolonAbstractWindow *> HolonAbstractTask::windows(const QString &group) const
{
    return d_ptr->windowList.value(group);
}
