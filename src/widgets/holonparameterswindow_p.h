// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPARAMETERSWINDOW_P_H
#define HOLONPARAMETERSWINDOW_P_H

#include "holontaskattributeswindow_p.h"

class HolonOpenTaskTreeModel;
class HolonParametersWindow;

class HolonParametersWindowPrivate : public HolonTaskAttributesWindowPrivate
{
public:
    HolonParametersWindowPrivate(HolonParametersWindow *q,
                                 HolonDesktop *desktop);

    void setOpenTaskTreeModel(HolonOpenTaskTreeModel *openTaskTreeModel);
};

#endif // HOLONPARAMETERSWINDOW_P_H
