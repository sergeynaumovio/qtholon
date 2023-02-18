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
    struct
    {
        QList<HolonTaskModel *> tasks;
        QList<HolonWorkflowModel *> workflows;

        struct
        {
            HolonTaskModel *task{};
            HolonWorkflowModel *workflow{};

        } current;

    } models;
};

#endif // HOLONDESKTOP_P_H
