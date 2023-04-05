// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKMODEL_H
#define HOLONTASKMODEL_H

#include "qtholonglobal.h"
#include <QAbstractItemModel>
#include <QLoaderSettings>

class HolonCore;
class HolonDesktop;
class HolonTaskModelBranch;
class HolonTaskModelPrivate;

class Q_HOLON_EXPORT HolonTaskModel : public QAbstractItemModel, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonDesktopPrivate;
    friend class HolonTaskModelPrivate;
    const QScopedPointer<HolonTaskModelPrivate> d_ptr;

public:
    HolonTaskModel(QLoaderSettings *settings, HolonDesktop *desktop);
    ~HolonTaskModel();

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    HolonDesktop *desktop() const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    bool isCurrent() const;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
};

#endif // HOLONTASKMODEL_H
