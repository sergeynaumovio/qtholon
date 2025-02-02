// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonpythontask.h"
#include "holonabstracttask.h"
#include "holondesktop.h"

QWidget *HolonPythonTask::parametersWidget() const
{
    return desktop->task()->parametersWidget();
}
