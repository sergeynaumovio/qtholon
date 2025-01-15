// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCORE_H
#define HOLONCORE_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QObject>

class HolonCorePrivate;
class HolonTaskModel;

class Q_HOLON_EXPORT HolonCore : public QObject, public QLoaderSettings
{
    Q_OBJECT

    const QScopedPointer<HolonCorePrivate> d_ptr;

public:
    explicit HolonCore(QLoaderSettings *settings, QObject *parent);
    ~HolonCore();
};

#endif // HOLONCORE_H
