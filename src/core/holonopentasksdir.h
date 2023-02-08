// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONOPENTASKSDIR_H
#define HOLONOPENTASKSDIR_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QObject>

class HolonOpenTasksModel;
class HolonTask;

class Q_HOLON_EXPORT HolonOpenTasksDir : public QObject, public QLoaderSettings
{
    Q_OBJECT

public:
    HolonOpenTasksDir(QLoaderSettings *settings, HolonOpenTasksModel *openTasksModel);

    void addTask(HolonTask *task);
    virtual QIcon icon() const;
};

#endif // HOLONOPENTASKSDIR_H
