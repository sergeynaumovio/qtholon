// Copyright (C) 2022 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontask.h"
#include "holoncore.h"
#include "holondesktop.h"

HolonTask::HolonTask(QLoaderSettings *settings, HolonCore *parent)
:   QObject(parent),
    QLoaderSettings(settings),
    d_ptr(new HolonTaskData)
{
    parent->addTask(this);
}

HolonTask::HolonTask(QLoaderSettings *settings, HolonDesktop *parent)
:   QObject(parent),
    QLoaderSettings(settings),
    d_ptr(new HolonTaskData)
{
    parent->addTask(this);
}

HolonTask::~HolonTask()
{ }

int HolonTask::exec()
{
    return 0;
}
