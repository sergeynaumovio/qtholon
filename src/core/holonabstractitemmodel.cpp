// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractitemmodel.h"
#include "holoncore.h"
#include "holondesktop.h"

HolonAbstractItemModel::HolonAbstractItemModel(QLoaderSettings *settings, HolonCore *core)
:   QAbstractItemModel(core),
    QLoaderSettings(settings)
{ }

HolonAbstractItemModel::HolonAbstractItemModel(QLoaderSettings *settings, HolonDesktop *desktop)
:   QAbstractItemModel(desktop),
    QLoaderSettings(settings)
{ }

bool HolonAbstractItemModel::isCurrent() const
{
    return value("current").toBool();
}

void HolonAbstractItemModel::setCurrent(bool current)
{
    setValue("current", current);
}
