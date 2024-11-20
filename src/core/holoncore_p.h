// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCORE_P_H
#define HOLONCORE_P_H

#include <QList>

class HolonTaskModel;
class HolonWorkflowModel;

class HolonCorePrivate
{
public:
    QList<HolonWorkflowModel *> workflowModelList;
    HolonWorkflowModel *currentWorkflowModel{};
};

#endif // HOLONDESKTOP_P_H
