// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskmodelbranch.h"
#include "holontaskmodel.h"
#include <QIcon>

using namespace Qt::Literals::StringLiterals;

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
    return QIcon(value(u"icon"_s, u":/holon/holoniconlight.svg"_s).toString());
}

QString HolonTaskModelBranch::title() const
{
    return value(u"title"_s, section().constLast()).toString();
}
