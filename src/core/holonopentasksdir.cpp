// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonopentasksdir.h"
#include "holonopentasksmodel.h"
#include <QIcon>

HolonOpenTasksDir::HolonOpenTasksDir(QLoaderSettings *settings, HolonOpenTasksModel *openTasksModel)
:   QObject(openTasksModel),
    QLoaderSettings(settings)
{ }


QIcon HolonOpenTasksDir::icon() const
{
    return QIcon(value("icon", ":/holon/holoniconlight.svg").toString());
}

QString HolonOpenTasksDir::title() const
{
    return value("title", section().constLast()).toString();
}
