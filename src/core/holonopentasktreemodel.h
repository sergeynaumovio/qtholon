// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONOPENTASKTREEMODEL_H
#define HOLONOPENTASKTREEMODEL_H

#include <QAbstractItemModel>
#include <QScopedStorage>

class HolonCore;
class HolonDesktop;
class HolonOpenTaskTreeModelPrivate;

class HolonOpenTaskTreeModel : public QAbstractItemModel
{
    Q_OBJECT

    QScopedStorage<HolonOpenTaskTreeModelPrivate, 56> d_ptr;

public:
    explicit HolonOpenTaskTreeModel(HolonCore *core);
    explicit HolonOpenTaskTreeModel(HolonDesktop *desktop);

    ~HolonOpenTaskTreeModel();

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
    QModelIndex parent(const QModelIndex &child) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QModelIndex restoreCurrentIndex() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
};

#endif // HOLONOPENTASKTREEMODEL_H
