// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonparameterswindow_p.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holonopentasktreemodel.h"
#include "holontitlebar.h"
#include "holonstackedwidget.h"
#include <QComboBox>

HolonParametersWindowPrivate::HolonParametersWindowPrivate(HolonAbstractWindow *q,
                                                           HolonDesktop *desk,
                                                           HolonAbstractTask *t)
:   HolonAbstractWindowPrivate(q, desk, t)
{ }

void HolonParametersWindowPrivate::setOpenTaskTreeModel(HolonOpenTaskTreeModel *openTaskTreeModel)
{
    QComboBox *combobox = titleBar->windowComboBox();
    combobox->setModel(openTaskTreeModel);

    for (int index{}; index < combobox->count(); ++index)
    {
        HolonAbstractTask *task = static_cast<HolonAbstractTask *>(combobox->model()->index(index, 0).internalPointer());

        if (task == desktop->task())
            combobox->setCurrentIndex(index);
    }

    combobox->connect(combobox, &QComboBox::currentIndexChanged, combobox->parent(), [=, this](int index)
    {
        HolonAbstractTask *task = static_cast<HolonAbstractTask *>(combobox->model()->index(index, 0).internalPointer());
        desktop->setTask(task);
    });
}

QWidget *HolonParametersWindowPrivate::widget()
{
    if (!q_ptr)
        return nullptr;

    if (stackedWidget)
        return stackedWidget;

    stackedWidget = new HolonTaskStackedWidget(q_ptr->role());

    return stackedWidget;
}

