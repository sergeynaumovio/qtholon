// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTITEMMODEL_H
#define HOLONABSTRACTITEMMODEL_H

#include "qtholonglobal.h"
#include <QAbstractItemModel>
#include <QLoaderSettings>

class HolonCore;
class HolonDesktop;

class Q_HOLON_EXPORT HolonAbstractItemModel : public QAbstractItemModel, public QLoaderSettings
{
    Q_OBJECT

public:
    HolonAbstractItemModel(QLoaderSettings *settings, HolonCore *core);
    HolonAbstractItemModel(QLoaderSettings *settings, HolonDesktop *desktop);

    bool isCurrent() const;
    void setCurrent(bool current);
};

#endif // HOLONABSTRACTITEMMODEL_H
