// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonparameterswindow_p.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holonopentasktreemodel.h"
#include "holontitlebar.h"
#include "holonstackedwidget.h"
#include <QComboBox>

HolonParametersWindowPrivate::HolonParametersWindowPrivate(HolonAbstractWindow *q, HolonDesktop *desk)
:   HolonAbstractWindowPrivate(q, desk)
{ }

void HolonParametersWindowPrivate::setOpenTaskTreeModel(HolonOpenTaskTreeModel *openTaskTreeModel)
{
    QComboBox *combobox = titleBar->windowComboBox();
    combobox->setModel(openTaskTreeModel);

    for (int index{}; index < combobox->count(); ++index)
    {
        HolonAbstractTask *tsk = static_cast<HolonAbstractTask *>(combobox->model()->index(index, 0).internalPointer());

        if (tsk == desktop->task())
        {
            combobox->setCurrentIndex(index);
            break;
        }
    }

    QObject::connect(combobox, &QComboBox::currentIndexChanged, combobox->parent(), [=, this](int index)
    {
        HolonAbstractTask *tsk = static_cast<HolonAbstractTask *>(combobox->model()->index(index, 0).internalPointer());
        desktop->setTask(tsk);
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

