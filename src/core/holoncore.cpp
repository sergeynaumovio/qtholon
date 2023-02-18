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

void HolonCore::addModel(HolonTaskModel *model)
{
    d_ptr->models.tasks.append(model);
}

void HolonCore::addModel(HolonWorkflowModel *model)
{
    d_ptr->models.workflows.append(model);
}
