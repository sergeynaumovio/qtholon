// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTTASK_P_H
#define HOLONABSTRACTTASK_P_H

#include <QIcon>
#include <QMap>

class HolonAbstractTask;
class HolonAbstractWidget;
class HolonAbstractWindow;
class HolonDesktop;
class HolonTaskModelBranch;
class HolonWorkflowModelBranch;

using WidgetRole = int;

class HolonAbstractTaskPrivate
{
public:
    HolonAbstractTask *const q_ptr;
    HolonTaskModelBranch *const taskModelBranch;
    HolonWorkflowModelBranch *const workflowModelBranch;
    HolonDesktop *const desktop;
    QMap<WidgetRole, QList<HolonAbstractWindow *>> windowList;
    QMap<WidgetRole, HolonAbstractWidget *> widgetList;

    HolonAbstractTaskPrivate(HolonAbstractTask *q = nullptr,
                             HolonTaskModelBranch *branch = nullptr);

    HolonAbstractTaskPrivate(HolonAbstractTask *q = nullptr,
                             HolonWorkflowModelBranch *branch = nullptr);

    void setCurrent(bool current);
};

#endif // HOLONABSTRACTTASK_P_H
