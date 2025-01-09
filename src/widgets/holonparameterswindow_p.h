// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPARAMETERSWINDOW_P_H
#define HOLONPARAMETERSWINDOW_P_H

#include "holontaskattributeswindow_p.h"

class HolonAbstractTask;
class HolonOpenTaskTreeModel;
class HolonParametersWindow;

class HolonParametersWindowPrivate : public HolonTaskAttributesWindowPrivate
{
public:
    HolonParametersWindowPrivate(HolonParametersWindow *q,
                                 HolonDesktop *desktop);

    void setCurrentTask(HolonAbstractTask *task);
    void setOpenTaskTreeModel(HolonOpenTaskTreeModel *openTaskTreeModel);
};

#endif // HOLONPARAMETERSWINDOW_P_H
