// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
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
    d_ptr(new HolonWorkflowPrivate(this, core, nullptr))
{ }

HolonWorkflow::HolonWorkflow(QLoaderSettings *settings, HolonDesktop *desktop)
:   QObject(desktop),
    QLoaderSettings(this, settings),
    d_ptr(new HolonWorkflowPrivate(this, nullptr, desktop))
{
    desktop->addWorkflow(this);
}

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
