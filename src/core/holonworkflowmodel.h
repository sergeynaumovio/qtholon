// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWMODEL_H
#define HOLONWORKFLOWMODEL_H

#include "qtholonglobal.h"
#include <QAbstractItemModel>
#include <QLoaderSettings>

class HolonCore;
class HolonDesktop;
class HolonWorkflowModelBranch;
class HolonWorkflowModelPrivate;

class Q_HOLON_EXPORT HolonWorkflowModel : public QAbstractItemModel, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonWorkflowModelPrivate;
    const QScopedPointer<HolonWorkflowModelPrivate> d_ptr;

public:
    explicit HolonWorkflowModel(QLoaderSettings *settings, HolonCore *core);
    explicit HolonWorkflowModel(QLoaderSettings *settings, HolonDesktop *desktop);

    ~HolonWorkflowModel();

    void addBranch(HolonWorkflowModelBranch *branch);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    HolonCore *core() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    HolonDesktop *desktop() const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
    bool isCurrent() const;
    QModelIndex parent(const QModelIndex &child) const override;
    QModelIndex restoreCurrentIndex() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
};

#endif // HOLONWORKFLOWMODEL_H
