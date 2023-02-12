// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCORE_P_H
#define HOLONCORE_P_H

#include <QList>

class QAbstractItemModel;

class HolonCorePrivate
{
public:
    struct
    {
        QList<QAbstractItemModel *> newTasks;
        QList<QAbstractItemModel *> openTasks;

    } models;
};

#endif // HOLONDESKTOP_P_H
