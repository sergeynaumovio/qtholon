// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskmodelbranch.h"
#include "holontaskmodel.h"
#include <QIcon>

HolonTaskModelBranch::HolonTaskModelBranch(QLoaderSettings *settings, HolonTaskModel *taskTreeModel)
:   QObject(taskTreeModel),
    QLoaderSettings(settings)
{ }

HolonTaskModelBranch::HolonTaskModelBranch(QLoaderSettings *settings, HolonTaskModelBranch *taskTreeBranch)
:   QObject(taskTreeBranch),
    QLoaderSettings(settings)
{ }

QIcon HolonTaskModelBranch::icon() const
{
    return QIcon(value("icon", ":/holon/holoniconlight.svg").toString());
}

QString HolonTaskModelBranch::title() const
{
    return value("title", section().constLast()).toString();
}
