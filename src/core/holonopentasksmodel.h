// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONOPENTASKSMODEL_H
#define HOLONOPENTASKSMODEL_H

#include "qtholonglobal.h"
#include <QLoaderSettings>

class HolonCore;
class HolonDesktop;
class HolonOpenTasksDir;
class HolonOpenTasksModelPrivate;

class Q_HOLON_EXPORT HolonOpenTasksModel : public QObject, public QLoaderSettings
{
    Q_OBJECT

    const QScopedPointer<HolonOpenTasksModelPrivate> d_ptr;

public:
    HolonOpenTasksModel(QLoaderSettings *settings, HolonCore *core);
    HolonOpenTasksModel(QLoaderSettings *settings, HolonDesktop *desktop);

    ~HolonOpenTasksModel();

    void addDir(HolonOpenTasksDir *dir);
};

#endif // HOLONOPENTASKSMODEL_H
