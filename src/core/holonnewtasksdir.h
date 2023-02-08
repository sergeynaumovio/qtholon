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

    void addDir(HolonNewTasksDir *dir);
    void addTask(HolonTask *task);
    virtual QIcon icon() const;
};

#endif // HOLONNEWTASKSDIR_H
