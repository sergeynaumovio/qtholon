// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONOPENTASKTREEVIEW_H
#define HOLONOPENTASKTREEVIEW_H

#include <QScopedStorage>
#include <QTreeView>

class HolonDesktop;
class HolonOpenTaskTreeViewPrivate;
class HolonWorkflowModel;

class HolonOpenTaskTreeView : public QTreeView
{
    Q_OBJECT

    const QScopedStorage<HolonOpenTaskTreeViewPrivate, 32> d_ptr;

protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    void showEvent(QShowEvent *) override;

Q_SIGNALS:
    void closeActivated(const QModelIndex &index);

public:
    HolonOpenTaskTreeView(HolonDesktop *desktop);
    ~HolonOpenTaskTreeView();

    HolonDesktop *desktop() const;
    void setModel(QAbstractItemModel *model) override;
};

#endif // HOLONOPENTASKTREEVIEW_H
