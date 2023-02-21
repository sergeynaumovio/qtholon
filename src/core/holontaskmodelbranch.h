// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKMODELBRANCH_H
#define HOLONTASKMODELBRANCH_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QObject>

class HolonAbstractTask;
class HolonTaskModel;

class Q_HOLON_EXPORT HolonTaskModelBranch : public QObject, public QLoaderSettings
{
    Q_OBJECT

public:
    HolonTaskModelBranch(QLoaderSettings *settings, HolonTaskModel *taskTreeModel);
    HolonTaskModelBranch(QLoaderSettings *settings, HolonTaskModelBranch *taskTreeBranch);

    virtual QIcon icon() const;
    virtual QString title() const;
};

#endif // HOLONTASKMODELBRANCH_H
