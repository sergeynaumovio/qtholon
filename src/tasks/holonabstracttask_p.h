// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTTASK_P_H
#define HOLONABSTRACTTASK_P_H

class HolonAbstractTask;
class HolonAbstractWidget;
class HolonAbstractWindow;
class HolonDesktop;
class HolonTaskFolder;
class HolonWorkflow;

using WidgetRole = int;

class HolonAbstractTaskPrivate
{
public:
    HolonAbstractTask *const q_ptr;
    HolonTaskFolder *const folder;
    HolonWorkflow *const workflow;
    HolonDesktop *const desktop;

    HolonAbstractTaskPrivate(HolonAbstractTask *q = nullptr,
                             HolonTaskFolder *folder = nullptr);

    HolonAbstractTaskPrivate(HolonAbstractTask *q = nullptr,
                             HolonWorkflow *workflow = nullptr);

    virtual ~HolonAbstractTaskPrivate();

    void setCurrent(bool current);
};

#endif // HOLONABSTRACTTASK_P_H
