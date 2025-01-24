// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstracttask.h"
#include "holonabstracttask_p.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holonid.h"
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
    if (!qobject_cast<HolonWorkflow *>(workflow->parent()))
    {
        emitError(u"parent object not valid"_s);
        return;
    }

    if (bool ok = (objectName().toUInt(&ok), ok))
    {
        if (!contains(u"open"_s))
            setValue(u"open"_s, true);

        workflow->addTask(this);
    }
    else
        emitError(u"task id not valid"_s);
}

bool HolonAbstractTask::setValue(const QString &key, const QVariant &value)
{
    return QLoaderSettings::setValue(key, value);
}

HolonAbstractTask::~HolonAbstractTask()
{ }

void HolonAbstractTask::addWindow(HolonAbstractWindow *window)
{
    if (d_ptr->workflow)
        d_ptr->workflow->desktop()->addWindow(window);
}

QWidget *HolonAbstractTask::customWidget(QMetaType /*taskAttributesWindow*/)
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

bool HolonAbstractTask::isCopyable(QStringView to) const
{
    QStringView toParent = HolonId::parentSection(to);

    if (toParent.size())
    {
        if (QObject *parent = tree()->object(toParent))
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
    return value(u"title"_s, section()).toString();
}

QVariant HolonAbstractTask::value(const QString &key, const QVariant &defaultValue) const
{
    return QLoaderSettings::value(key, defaultValue);
}
