// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKMODEL_P_H
#define HOLONTASKMODEL_P_H

#include <QHash>

class HolonDesktop;
class HolonTaskModel;
class HolonTaskModelBranch;
class QModelIndex;
class QObject;

class HolonTaskModelPrivate
{
public:
    HolonTaskModel *const q_ptr;
    HolonDesktop *const desktop;
    QHash<HolonTaskModelBranch *, bool> expanded;

    HolonTaskModelPrivate(HolonTaskModel *q, HolonDesktop *desk);

    QObject *object(const QModelIndex &index) const;
    void setCurrent(bool current);
};

#endif // HOLONTASKMODEL_P_H
