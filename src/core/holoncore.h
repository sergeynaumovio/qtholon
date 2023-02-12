// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCORE_H
#define HOLONCORE_H

#include "holonnamespace.h"
#include "qtholonglobal.h"
#include <QLoaderSettings>

class HolonCorePrivate;
class HolonOpenTasksModel;
class QAbstractItemModel;

class Q_HOLON_EXPORT HolonCore : public QObject, public QLoaderSettings
{
    Q_OBJECT

    const QScopedPointer<HolonCorePrivate> d_ptr;

public:
    HolonCore(QLoaderSettings *settings, QObject *parent);
    ~HolonCore();

    void addModel(QAbstractItemModel *model, Holon::Tasks tasks);
};

#endif // HOLONCORE_H
