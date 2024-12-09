// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPARAMETERSWINDOW_P_H
#define HOLONPARAMETERSWINDOW_P_H

#include "holonabstractwindow_p.h"

class HolonOpenTaskTreeModel;
class HolonTaskStackedWidget;
class QWidget;

class HolonParametersWindowPrivate : public HolonAbstractWindowPrivate
{
    HolonTaskStackedWidget *stackedWidget{};

public:
    HolonParametersWindowPrivate(HolonAbstractWindow *q,
                                 HolonDesktop *desktop,
                                 HolonAbstractTask *task = nullptr);

    void setOpenTaskTreeModel(HolonOpenTaskTreeModel *openTaskTreeModel);
    QWidget *widget();
};

#endif // HOLONPARAMETERSWINDOW_P_H
