// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonopentasksmodel.h"
#include "holoncore.h"
#include "holondesktop.h"

class HolonOpenTasksModelPrivate
{

};

HolonOpenTasksModel::HolonOpenTasksModel(QLoaderSettings *settings, HolonCore *core)
:   QObject(core),
    QLoaderSettings(settings),
    d_ptr(new HolonOpenTasksModelPrivate)
{
    core->addOpenTasksModel(this);
}

HolonOpenTasksModel::HolonOpenTasksModel(QLoaderSettings *settings, HolonDesktop *desktop)
:   QObject(desktop),
    QLoaderSettings(settings),
    d_ptr(new HolonOpenTasksModelPrivate)
{
    desktop->addOpenTasksModel(this);
}

HolonOpenTasksModel::~HolonOpenTasksModel()
{ }

void HolonOpenTasksModel::addDir(HolonOpenTasksDir *dir)
{
    Q_UNUSED(dir)
}
