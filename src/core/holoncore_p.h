// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCORE_P_H
#define HOLONCORE_P_H

#include <QList>

class HolonTaskModel;
class HolonWorkflowModel;

class HolonCorePrivate
{
public:
    QList<HolonTaskModel *> taskModelList;
    QList<HolonWorkflowModel *> workflowModelList;

    HolonTaskModel *currentTaskModel{};
    HolonWorkflowModel *currentWorkflowModel{};
};

#endif // HOLONDESKTOP_P_H
