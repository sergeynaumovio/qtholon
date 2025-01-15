// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWMODEL_H
#define HOLONWORKFLOWMODEL_H

#include <QAbstractItemModel>
#include <QScopedStorage>

class HolonAbstractTask;
class HolonWorkflow;
class HolonWorkflowItem;
class HolonWorkflowModelPrivate;

class HolonWorkflowModel : public QAbstractItemModel
{
    Q_OBJECT

    QScopedStorage<HolonWorkflowModelPrivate, 16> d_ptr;

public:
    HolonWorkflowModel(HolonWorkflow *workflow);
    ~HolonWorkflowModel();

    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    bool restoreState();
    HolonWorkflowItem *rootItem() const;
    int rowCount(const QModelIndex &parent) const override;
};

#endif // HOLONWORKFLOWMODEL_H
