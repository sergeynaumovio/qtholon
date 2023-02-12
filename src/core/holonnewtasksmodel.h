// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONNEWTASKSMODEL_H
#define HOLONNEWTASKSMODEL_H

#include <QAbstractItemModel>
#include <QLoaderSettings>

class HolonDesktop;
class HolonNewTasksDir;
class HolonNewTasksModelPrivate;

class HolonNewTasksModel : public QAbstractItemModel, public QLoaderSettings
{
    Q_OBJECT

    const QScopedPointer<HolonNewTasksModelPrivate> d_ptr;

public:
    HolonNewTasksModel(QLoaderSettings *settings, HolonDesktop *desktop);
    ~HolonNewTasksModel();

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
};

#endif // HOLONNEWTASKSMODEL_H
