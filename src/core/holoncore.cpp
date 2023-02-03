// Copyright (C) 2022 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncore.h"

HolonCore::HolonCore(QLoaderSettings *settings, QObject *parent)
:   QObject(parent),
    QLoaderSettings(settings)
{ }

void HolonCore::addTask(HolonTask */*task*/)
{

}
