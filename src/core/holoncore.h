// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCORE_H
#define HOLONCORE_H

#include "qtholonglobal.h"
#include <QLoaderSettings>

class HolonCorePrivate;
class HolonTaskModel;
class HolonWorkflowModel;

class Q_HOLON_EXPORT HolonCore : public QObject, public QLoaderSettings
{
    Q_OBJECT

    const QScopedPointer<HolonCorePrivate> d_ptr;

public:
    HolonCore(QLoaderSettings *settings, QObject *parent);
    ~HolonCore();

    void addModel(HolonTaskModel *model);
    void addModel(HolonWorkflowModel *model);
};

#endif // HOLONCORE_H
