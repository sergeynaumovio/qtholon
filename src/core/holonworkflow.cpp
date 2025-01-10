// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflow.h"
#include "holonabstracttask.h"
#include "holoncore.h"
#include "holondesktop.h"
#include "holonworkflow_p.h"

using namespace Qt::Literals::StringLiterals;

HolonWorkflow::HolonWorkflow(QLoaderSettings *settings, HolonCore *core)
:   QObject(core),
    QLoaderSettings(this, settings),
    d_ptr(this, core, nullptr)
{ }

HolonWorkflow::HolonWorkflow(QLoaderSettings *settings, HolonDesktop *desktop)
:   QObject(desktop),
    QLoaderSettings(this, settings),
    d_ptr(this, nullptr, desktop)
{
    desktop->addWorkflow(this);
}

HolonWorkflow::HolonWorkflow(QLoaderSettings *settings, HolonWorkflow *workflow)
:   QObject(workflow),
    QLoaderSettings(this, settings),
    d_ptr(this, nullptr, workflow->desktop())
{ }

HolonWorkflow::~HolonWorkflow()
{ }

void HolonWorkflow::addTask(HolonAbstractTask *task)
{
    d_ptr->addTask(task);
}

HolonCore *HolonWorkflow::core() const
{
    return d_ptr->core;
}

HolonDesktop *HolonWorkflow::desktop() const
{
    return d_ptr->desktop;
}

bool HolonWorkflow::isCurrent() const
{
    return value(u"current"_s).toBool();
}

QString HolonWorkflow::title() const
{
    return value(u"title"_s).toString ();
}
