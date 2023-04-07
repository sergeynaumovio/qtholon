// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstracttask.h"
#include "holonabstracttask_p.h"
#include "holonabstractwidget.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holontaskmodelbranch.h"
#include "holontheme.h"
#include "holonthemeicons.h"
#include "holonworkflowmodel.h"
#include "holonworkflowmodelbranch.h"
#include <QLoaderTree>

using namespace Qt::Literals::StringLiterals;

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
    if (d_ptr->widgetList.contains(widget->role()))
        return;

    d_ptr->widgetList[widget->role()] = widget;
}

void HolonAbstractTask::addWindow(HolonAbstractWindow *window)
{
    if (d_ptr->windowList[window->role()].contains(window))
        return;

    d_ptr->windowList[window->role()].append(window);

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

QIcon HolonAbstractTask::icon() const
{
    return desktop()->currentTheme()->icons()->taskIcon();
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
    return value(u"current"_s).toBool();
}

int HolonAbstractTask::role() const
{
    return Holon::ParametersRole;
}

QString HolonAbstractTask::title() const
{
    return value(u"title"_s, section().constLast()).toString();
}

QWidget *HolonAbstractTask::widget(int widgetRole) const
{
    QWidget *widget{};

    if (d_ptr->widgetList.contains(widgetRole))
        widget = d_ptr->widgetList.value(widgetRole)->widget();

    return widget;
}

QList<HolonAbstractWindow *> HolonAbstractTask::windows(int windowRole) const
{
    return d_ptr->windowList.value(windowRole);
}

