// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstracttask.h"
#include "holonabstracttask_p.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holontaskfolder.h"
#include "holontheme.h"
#include "holonthemeicons.h"
#include "holonworkflow.h"
#include <QLoaderTree>

using namespace Qt::Literals::StringLiterals;

HolonAbstractTask::HolonAbstractTask(QLoaderSettings *settings, HolonTaskFolder *folder)
:   QObject(folder),
    QLoaderSettings(this, settings),
    d_ptr(new HolonAbstractTaskPrivate(this, folder))
{ }

HolonAbstractTask::HolonAbstractTask(QLoaderSettings *settings, HolonWorkflow *workflow)
:   QObject(workflow),
    QLoaderSettings(this, settings),
    d_ptr(new HolonAbstractTaskPrivate(this, workflow))
{
    if (bool ok = (objectName().toUInt(&ok), ok))
    {
        if (!contains(u"open"_s))
            setValue(u"open"_s, true);

        workflow->addTask(this);
    }
    else
        emitError(u"task id not valid"_s);
}

HolonAbstractTask::~HolonAbstractTask()
{ }

void HolonAbstractTask::addWindow(HolonAbstractWindow *window)
{
    if (d_ptr->windowList[window->role()].contains(window))
        return;

    d_ptr->windowList[window->role()].append(window);

    if (d_ptr->workflow)
        d_ptr->workflow->desktop()->addWindow(window);
}

QWidget *HolonAbstractTask::customWidget(QMetaType /*taskAttributesWindow*/) const
{
    return {};
}

HolonDesktop *HolonAbstractTask::desktop() const
{
    if (d_ptr)
        return d_ptr->desktop;

    return {};
}

QIcon HolonAbstractTask::icon() const
{
    return desktop()->theme()->icons()->taskIcon();
}

bool HolonAbstractTask::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);

        if (qobject_cast<HolonWorkflow *>(parent))
            return true;
    }

    return false;
}

bool HolonAbstractTask::isCurrent() const
{
    return value(u"current"_s).toBool();
}

bool HolonAbstractTask::isOpen() const
{
    return value(u"open"_s).toBool();
}

QString HolonAbstractTask::title() const
{
    return value(u"title"_s, section().constLast()).toString();
}

QList<HolonAbstractWindow *> HolonAbstractTask::windows(int windowRole) const
{
    return d_ptr->windowList.value(windowRole);
}

