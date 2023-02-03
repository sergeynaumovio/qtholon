// Copyright (C) 2022 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCORE_H
#define HOLONCORE_H

#include "qtholonglobal.h"
#include <QLoaderSettings>

class HolonTask;

class Q_HOLON_EXPORT HolonCore : public QObject, public QLoaderSettings
{
    Q_OBJECT

public:
    HolonCore(QLoaderSettings *settings, QObject *parent);

    void addTask(HolonTask *task);
};

#endif // HOLONCORE_H
