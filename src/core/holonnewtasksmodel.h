// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONNEWTASKSMODEL_H
#define HOLONNEWTASKSMODEL_H

#include "qtholonglobal.h"
#include <QLoaderSettings>

class HolonDesktop;
class HolonNewTasksDir;
class HolonNewTasksModelPrivate;

class Q_HOLON_EXPORT HolonNewTasksModel : public QObject, public QLoaderSettings
{
    Q_OBJECT

    const QScopedPointer<HolonNewTasksModelPrivate> d_ptr;

public:
    HolonNewTasksModel(QLoaderSettings *settings, HolonDesktop *desktop);
    ~HolonNewTasksModel();

    void addDir(HolonNewTasksDir *dir);
};

#endif // HOLONNEWTASKSMODEL_H
