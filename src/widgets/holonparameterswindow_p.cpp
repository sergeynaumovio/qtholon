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

#define emit

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

    HolonThemeIcons *icons = desktop->theme()->icons();

    QIcon runIcon = icons->runTaskIcon();
    QIcon stopIcon = icons->stopIcon();

    QToolButton *execButton = toolbar->addToolButton(runIcon, u"Run Task"_s);

    HolonTaskThread *taskThread = desktop->taskThread();

    const QString startTaskShortcut = desktop->value(u"startTaskShortcut"_s).toString();
    QShortcut *shortcut = new QShortcut(QKeySequence(startTaskShortcut), desktop);

    QObject::connect(shortcut, &QShortcut::activated, desktop, [=, this]()
    {
        if (taskThread->isRunning())
            taskThread->requestInterruption();
        else
        {
            execButton->setIcon(stopIcon);
            execButton->setToolTip(u"Stop Task"_s);

            pyMainThreadState = PyEval_SaveThread();
            taskThread->start();
        }
    });

    QObject::connect(execButton, &QToolButton::clicked, q_ptr, [=]()
    {
        emit shortcut->activated();
    });

    QObject::connect(taskThread, &QThread::finished, q_ptr, [=, this]()
    {
        if (pyMainThreadState)
        {
            PyEval_RestoreThread(pyMainThreadState);
            pyMainThreadState = nullptr;
        }

        execButton->setIcon(runIcon);
        execButton->setToolTip(u"Run Task"_s);
    });

    return toolbar;
}
