// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonpythontaskwindow.h"
#include "holonabstracttaskwindow.h"
#include "holondesktop.h"

using namespace Qt::Literals::StringLiterals;

bool HolonPythonTaskWindow::setValue(const QString &key, const QVariant &value)
{
    return desktop->taskWindow()->setValue(key, value);
}

QVariant HolonPythonTaskWindow::value(const QString &key, const QVariant &defaultValue) const
{
    return desktop->taskWindow()->value(key, defaultValue);
}
