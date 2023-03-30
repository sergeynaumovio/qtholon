// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKMODELBRANCH_H
#define HOLONTASKMODELBRANCH_H

#include "holonnamespace.h"
#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QObject>

class HolonAbstractTask;
class HolonDesktop;
class HolonTaskModel;
class HolonTaskModelBranchPrivate;

class Q_HOLON_EXPORT HolonTaskModelBranch : public QObject, public QLoaderSettings
{
    Q_OBJECT

    const QScopedPointer<HolonTaskModelBranchPrivate> d_ptr;

public:
    HolonTaskModelBranch(QLoaderSettings *settings, HolonTaskModel *taskTreeModel);
    HolonTaskModelBranch(QLoaderSettings *settings, HolonTaskModelBranch *taskTreeBranch);
    ~HolonTaskModelBranch();

    HolonDesktop *desktop() const;
    virtual QIcon icon(Holon::TreeItem expanded) const;
    bool isExpanded() const;
    void setExpanded(bool expanded);
    virtual QString title() const;
};

#endif // HOLONTASKMODELBRANCH_H
