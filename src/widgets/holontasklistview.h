// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKLISTVIEW_H
#define HOLONTASKLISTVIEW_H

#include <QListView>
#include <QScopedStorage>

class HolonDesktop;
class HolonTaskListViewPrivate;
class HolonWorkflowModel;

class HolonTaskListView : public QListView
{
    Q_OBJECT

    const QScopedStorage<HolonTaskListViewPrivate, 16> d_ptr;

protected:
    void showEvent(QShowEvent *) override;

public:
    HolonTaskListView(HolonDesktop *desktop);
    ~HolonTaskListView();
};

#endif // HOLONTASKLISTVIEW_H
