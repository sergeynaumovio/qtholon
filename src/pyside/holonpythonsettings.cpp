// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonpythonsettings.h"
#include "holonabstracttask.h"
#include "holondesktop.h"

using namespace Qt::Literals::StringLiterals;

bool HolonPythonSettings::setValue(const QString &key, const QVariant &value)
{
    return desktop->task()->setValue(key, value);
}

QVariant HolonPythonSettings::value(const QString &key, const QVariant &defaultValue) const
{
    return desktop->task()->value(key, defaultValue);
}
