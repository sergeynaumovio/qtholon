// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonparameterswindow_p.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holonopentasktreemodel.h"
#include "holonparameterswindow.h"
#include "holontitlebar.h"
#include <QComboBox>

HolonParametersWindowPrivate::HolonParametersWindowPrivate(HolonParametersWindow *q, HolonDesktop *desk)
:   HolonTaskAttributesWindowPrivate(q, desk)
{ }

void HolonParametersWindowPrivate::setCurrentTask(HolonAbstractTask *tsk)
{
    QComboBox *combobox = titleBar->windowComboBox();

    for (int row{}; row < combobox->count(); ++row)
    {
        if (tsk == static_cast<HolonAbstractTask *>(combobox->model()->index(row, 0).internalPointer()))
            return combobox->setCurrentIndex(row);
    }
}

void HolonParametersWindowPrivate::setOpenTaskTreeModel(HolonOpenTaskTreeModel *openTaskTreeModel)
{
    QComboBox *combobox = titleBar->windowComboBox();
    combobox->setModel(openTaskTreeModel);
    setCurrentTask(desktop->task());

    QObject::connect(combobox, &QComboBox::currentIndexChanged, combobox->parent(), [=, this](int index)
    {
        HolonAbstractTask *tsk = static_cast<HolonAbstractTask *>(combobox->model()->index(index, 0).internalPointer());
        desktop->setTask(tsk);
    });
}

