// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONOPENTASKSMODEL_H
#define HOLONOPENTASKSMODEL_H

#include <QAbstractItemModel>
#include <QLoaderSettings>

class HolonCore;
class HolonDesktop;
class HolonOpenTasksDir;
class HolonOpenTasksModelPrivate;

class HolonOpenTasksModel : public QAbstractItemModel, public QLoaderSettings
{
    Q_OBJECT

    const QScopedPointer<HolonOpenTasksModelPrivate> d_ptr;

public:
    HolonOpenTasksModel(QLoaderSettings *settings, HolonCore *core);
    HolonOpenTasksModel(QLoaderSettings *settings, HolonDesktop *desktop);

    ~HolonOpenTasksModel();

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
};

#endif // HOLONOPENTASKSMODEL_H
