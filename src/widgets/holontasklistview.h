// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKLISTVIEW_H
#define HOLONTASKLISTVIEW_H

#include <QListView>

class HolonDesktop;
class HolonWorkflowModel;

class HolonTaskListView : public QListView
{
    Q_OBJECT

    HolonWorkflowModel *const workflowModel;
    bool once{true};

protected:
    void showEvent(QShowEvent *) override;

public:
    HolonTaskListView(HolonDesktop *desktop);
};

#endif // HOLONTASKLISTVIEW_H
