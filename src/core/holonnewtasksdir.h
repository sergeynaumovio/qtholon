// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONNEWTASKSDIR_H
#define HOLONNEWTASKSDIR_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QObject>

class HolonNewTasksModel;
class HolonTask;

class Q_HOLON_EXPORT HolonNewTasksDir : public QObject, public QLoaderSettings
{
    Q_OBJECT

public:
    HolonNewTasksDir(QLoaderSettings *settings, HolonNewTasksModel *newTasksModel);
    HolonNewTasksDir(QLoaderSettings *settings, HolonNewTasksDir *newTaskDir);

    virtual QIcon icon() const;
    virtual QString title() const;
};

#endif // HOLONNEWTASKSDIR_H
