// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncore.h"
#include "holoncore_p.h"

HolonCore::HolonCore(QLoaderSettings *settings, QObject *parent)
:   QObject(parent),
    QLoaderSettings(settings),
    d_ptr(new HolonCorePrivate)
{ }

HolonCore::~HolonCore()
{ }

void HolonCore::addModel(QAbstractItemModel *model, Holon::Tasks tasks)
{
    if (tasks == Holon::NewTasks)
        d_ptr->models.newTasks.append(model);
    else
        d_ptr->models.openTasks.append(model);
}
