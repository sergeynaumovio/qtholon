// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
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

#if !BUILD_WITH_PYSIDE

struct PyThreadState{};
static constexpr PyThreadState *PyEval_SaveThread() { return nullptr; }
static constexpr void PyEval_RestoreThread(PyThreadState *) {}

#endif

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
    setCurrentTask(desktop->currentTask());

    QObject::connect(combobox, &QComboBox::currentIndexChanged, combobox->parent(), [=, this](int index)
    {
        HolonAbstractTask *tsk = static_cast<HolonAbstractTask *>(combobox->model()->index(index, 0).internalPointer());
        desktop->setCurrentTask(tsk);
    });

    QObject::connect(openTaskTreeModel, &QAbstractItemModel::rowsInserted, combobox, [=](const QModelIndex &, int row)
    {
        combobox->setCurrentIndex(row);
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

    QToolButton *runButton = toolbar->addToolButton(icons->runIcon(), u"Run Task"_s);
    QToolButton *stopButton = toolbar->addToolButton(icons->stopIcon(), u"Stop Task"_s);
    stopButton->setDisabled(true);

    HolonTaskThread *taskThread = desktop->taskThread();

    const QString startTaskShortcut = desktop->value(u"startTaskShortcut"_s).toString();
    QShortcut *shortcut = new QShortcut(QKeySequence(startTaskShortcut), desktop);

    QObject::connect(shortcut, &QShortcut::activated, desktop, [=, this]()
    {
        if (taskThread->isRunning())
            taskThread->requestInterruption();
        else
        {
            runButton->setDisabled(true);
            stopButton->setEnabled(true);

            pyMainThreadState = PyEval_SaveThread();
            taskThread->start();
        }
    });

    QObject::connect(runButton, &QToolButton::clicked, q_ptr, [=]()
    {
        emit shortcut->activated();
    });

    QObject::connect(stopButton, &QToolButton::clicked, q_ptr, [=]()
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

        runButton->setEnabled(true);
        stopButton->setDisabled(true);
    });

    return toolbar;
}
