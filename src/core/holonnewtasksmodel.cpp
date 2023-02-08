// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonnewtasksmodel.h"
#include "holondesktop.h"

class HolonNewTasksModelPrivate
{

};

HolonNewTasksModel::HolonNewTasksModel(QLoaderSettings *settings, HolonDesktop *desktop)
:   QObject(desktop),
    QLoaderSettings(settings),
    d_ptr(new HolonNewTasksModelPrivate)
{
    desktop->setNewTasksModel(this);
}

HolonNewTasksModel::~HolonNewTasksModel()
{ }

void HolonNewTasksModel::addDir(HolonNewTasksDir *dir)
{
    Q_UNUSED(dir)
}
