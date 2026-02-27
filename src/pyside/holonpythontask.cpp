// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonpythontask.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holontaskthread.h"

using namespace Qt::Literals::StringLiterals;

bool HolonPythonTask::isInterruptionRequested() const
{
    return desktop->taskThread()->isInterruptionRequested();
}

QWidget *HolonPythonTask::parametersWidget() const
{
    return desktop->currentTask()->parametersWidget();
}

bool HolonPythonTask::setValue(const QString &key, const QVariant &value)
{
    return desktop->currentTask()->setValue(key, value);
}

QVariant HolonPythonTask::value(const QString &key, const QVariant &defaultValue) const
{
    return desktop->currentTask()->value(key, defaultValue);
}
