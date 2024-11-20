// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKFOLDERMODEL_H
#define HOLONTASKFOLDERMODEL_H

#include <QAbstractItemModel>

class HolonCore;
class HolonDesktop;
class HolonTaskFolder;
class HolonTaskFolderModelPrivate;

class HolonTaskFolderModel : public QAbstractItemModel
{
    Q_OBJECT

    HolonDesktop *const desktop;

public:
    explicit HolonTaskFolderModel(HolonDesktop *desktop, QObject *parent);
    ~HolonTaskFolderModel();

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
};

#endif // HOLONTASKFOLDERMODEL_H
