// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPARAMETERSWINDOW_P_H
#define HOLONPARAMETERSWINDOW_P_H

#include "holontaskattributeswindow_p.h"

#if BUILD_WITH_PYSIDE

#include <sbkpython.h>

#else

struct PyThreadState;

#endif

class HolonAbstractTask;
class HolonOpenTaskTreeModel;
class HolonParametersWindow;
class HolonToolBar;

class HolonParametersWindowPrivate : public HolonTaskAttributesWindowPrivate
{
    HolonToolBar *toolbar{};
    PyThreadState *pyMainThreadState{};

public:
    HolonParametersWindowPrivate(HolonParametersWindow *q,
                                 HolonDesktop *desktop);

    void setCurrentTask(HolonAbstractTask *task);
    void setOpenTaskTreeModel(HolonOpenTaskTreeModel *openTaskTreeModel);
    QWidget *toolbarWidget();
};

#endif // HOLONPARAMETERSWINDOW_P_H
