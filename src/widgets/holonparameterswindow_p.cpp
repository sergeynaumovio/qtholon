// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonparameterswindow_p.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holonopentasktreemodel.h"
#include "holonparameterswindow.h"
#include "holontaskthread.h"
#include "holonthemeicons.h"
#include "holontitlebar.h"
#include "holontoolbar.h"
#include <QComboBox>
#include <QShortcut>
#include <QToolButton>

using namespace Qt::Literals::StringLiterals;

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

QWidget *HolonParametersWindowPrivate::toolbarWidget()
{
    if (!q_ptr)
        return nullptr;

    if (toolbar)
        return toolbar;

    toolbar = new HolonToolBar;

    QIcon runIcon;
    QIcon stopIcon;

    QToolButton *execButton = toolbar->addToolButton(runIcon, u"Run Task"_s);

    HolonTaskThread *taskThread = desktop->taskThread();

    auto stopButton = [=]()
    {
        execButton->setIcon(stopIcon);
        execButton->setToolTip(u"Stop Task"_s);
        taskThread->start();
    };

    const QString startTaskShortcut = desktop->value(u"startTaskShortcut"_s).toString();
    QShortcut *shortcut = new QShortcut(QKeySequence(startTaskShortcut), desktop);

    QObject::connect(shortcut, &QShortcut::activated, desktop, [=]()
    {
        stopButton();
    });

    QObject::connect(execButton, &QToolButton::clicked, q_ptr, [=, this]()
    {
        if (taskThread->isRunning())
            taskThread->requestInterruption();

       stopButton();
    });

    QObject::connect(taskThread, &QThread::finished, q_ptr, [=, this]()
    {
        execButton->setIcon(runIcon);
        execButton->setToolTip(u"Run Task"_s);
    });

    return toolbar;
}
