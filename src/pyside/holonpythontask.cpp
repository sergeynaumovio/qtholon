// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonpythontask.h"
#include "holonabstracttask.h"
#include "holondesktop.h"

using namespace Qt::Literals::StringLiterals;

QWidget *HolonPythonTask::parametersWidget() const
{
    return desktop->task()->parametersWidget();
}

bool HolonPythonTask::setValue(const QString &key, const QVariant &value)
{
    return desktop->task()->setValue(key, value);
}

QVariant HolonPythonTask::value(const QString &key, const QVariant &defaultValue) const
{
    return desktop->task()->value(key, defaultValue);
}
