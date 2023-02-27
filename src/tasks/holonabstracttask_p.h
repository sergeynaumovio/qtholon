// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTTASK_P_H
#define HOLONABSTRACTTASK_P_H

#include <QMap>

class HolonAbstractTask;
class HolonAbstractWidget;
class HolonAbstractWindow;
class HolonDesktop;
class HolonWorkflowModelBranch;

class HolonAbstractTaskPrivate
{
public:
    HolonAbstractTask *const q_ptr;
    HolonWorkflowModelBranch *const workflowModelBranch;
    HolonDesktop *const desktop;
    QMap<QString, QList<HolonAbstractWindow *>> windowList;
    QMap<QString, HolonAbstractWidget *> widgetList;

    HolonAbstractTaskPrivate(HolonAbstractTask *q = nullptr,
                             HolonWorkflowModelBranch *branch = nullptr);

    void setCurrent(bool current);
};

#endif // HOLONABSTRACTTASK_P_H
