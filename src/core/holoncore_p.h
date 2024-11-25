// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONCORE_P_H
#define HOLONCORE_P_H

#include <QList>

class HolonWorkflow;

class HolonCorePrivate
{
public:
    QList<HolonWorkflow *> workflowList;
};

#endif // HOLONDESKTOP_P_H
