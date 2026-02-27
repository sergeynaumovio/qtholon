// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonpythontaskwindow.h"
#include "holonabstracttaskwindow.h"
#include "holondesktop.h"

using namespace Qt::Literals::StringLiterals;

bool HolonPythonTaskWindow::setValue(const QString &key, const QVariant &value)
{
    return desktop->currentTaskWindow()->setValue(key, value);
}

QVariant HolonPythonTaskWindow::value(const QString &key, const QVariant &defaultValue) const
{
    return desktop->currentTaskWindow()->value(key, defaultValue);
}
