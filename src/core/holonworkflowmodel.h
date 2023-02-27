// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWMODEL_H
#define HOLONWORKFLOWMODEL_H

#include "holonabstractitemmodel.h"
#include "qtholonglobal.h"

class HolonWorkflowModelBranch;
class HolonOpenTasksModelPrivate;

class Q_HOLON_EXPORT HolonWorkflowModel : public HolonAbstractItemModel
{
    Q_OBJECT

    const QScopedPointer<HolonOpenTasksModelPrivate> d_ptr;

public:
    HolonWorkflowModel(QLoaderSettings *settings, HolonCore *core);
    HolonWorkflowModel(QLoaderSettings *settings, HolonDesktop *desktop);

    ~HolonWorkflowModel();

    void addBranch(HolonWorkflowModelBranch *branch);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    HolonCore *core() const;
    QModelIndex restoreCurrentIndex() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    HolonDesktop *desktop() const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
};

#endif // HOLONWORKFLOWMODEL_H
