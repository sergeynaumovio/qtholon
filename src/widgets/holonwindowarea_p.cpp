// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowarea_p.h"
#include "holonabstracttask.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holondockwidget.h"
#include "holondockwidgetsplitstate.h"
#include "holontitlebar.h"
#include "holonwindowarea.h"
#include <QLoaderTree>
#include <QMainWindow>

HolonWindowAreaPrivate::HolonWindowAreaPrivate(HolonDesktop *desk, HolonWindowArea *q)
:   desktop(desk),
    q_ptr(q),
    mainWindow(new QMainWindow)
{
    mainWindow->setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);
}

HolonWindowAreaPrivate::~HolonWindowAreaPrivate()
{ }

void HolonWindowAreaPrivate::addWindow(HolonAbstractWindow *window)
{
    HolonDockWidget *dock = new HolonDockWidget(desktop, mainWindow, window, this);
    dockList.append(dock);

    if (!dockWidgetSplitState)
    {
        dockWidgetSplitState = new HolonDockWidgetSplitState(this);

        if (q_ptr->contains("dockWidgetSplitState"))
            dockWidgetSplitState->restoreSplitState();
    }

    dockByWindow.insert(window, dock);

    if (!qobject_cast<HolonAbstractTask *>(window->parent()))
        desktop->addWindow(window);

    if (dockByWindow.count() > 1)
        for (const HolonDockWidget *dockWidget: std::as_const(dockByWindow))
        {
            dockWidget->titleBar()->showControlButtons();
            dockWidget->titleBar()->hideSidebarButton();
        }

    if (!window->tree()->isLoaded())
        mainWindow->restoreState(mainWindowState());

    dockWidgetSplitState->setSplitItemDock(dock);
}

void HolonWindowAreaPrivate::closeWindow(HolonAbstractWindow *window)
{
    HolonDockWidget *dock = dockByWindow.value(window);
    dockByWindow.remove(window);
    mainWindow->removeDockWidget(dock);
    dockWidgetSplitState->removeSplit(dock);
    dock->deleteLater();
    window->deleteLater();

    dockList.removeOne(dock);

    if(maximized)
    {
        for (HolonDockWidget *w : dockList)
            w->show();

        maximized = false;
    }

    if (dockByWindow.count() == 1)
    {
        HolonDockWidget *dock = dockByWindow.first();
        dock->titleBar()->hideControlButtons();
        dock->titleBar()->showSplitButton();

        if (qobject_cast<HolonWindowArea *>(q_ptr))
            dock->titleBar()->showSidebarButton();
    }
}

void HolonWindowAreaPrivate::emitWarning(const QString &warning) const
{
    q_ptr->emitWarning(warning);
}

QByteArray HolonWindowAreaPrivate::mainWindowState() const
{
    return q_ptr->value("mainWindowState").toByteArray();
}

void HolonWindowAreaPrivate::maximizeWindow(HolonDockWidget *dock)
{
    if (maximized)
    {
        mainWindowStateBeforeMaximized = mainWindow->saveState();

        for (HolonDockWidget *w : dockList)
            w->hide();

        dock->show();
    }
    else
    {
        for (HolonDockWidget *w : dockList)
            w->show();

        mainWindow->restoreState(mainWindowStateBeforeMaximized);
    }
}

void HolonWindowAreaPrivate::saveMainWindowState()
{
    if (dockList.size() > 1)
        q_ptr->setValue("mainWindowState", mainWindow->saveState());
}

void HolonWindowAreaPrivate::setChecked(bool checked)
{
    q_ptr->setValue("checked", checked);
}

void HolonWindowAreaPrivate::setValue(const QString &key, const QVariant &value)
{
    q_ptr->setValue(key, value);
}

void HolonWindowAreaPrivate::splitWindow(HolonAbstractWindow *first,
                                         HolonAbstractWindow *second,
                                         Qt::Orientation splitOrientation)
{
    if (HolonDockWidget *firstDock = dockByWindow.value(first))
    {
        if (HolonAbstractTask *task = first->task())
        {
            QSet<int> windows;
            for (QObject *o : task->children())
            {
                if (qobject_cast<HolonAbstractWindow *>(o))
                    windows.insert(o->objectName().toInt());
            }

            int i{};
            for (; i < windows.count(); ++i)
                if (!windows.contains(i))
                    break;

            QStringList to = task->section();
            to.append(QString::number(i));

            QLoaderTree *tree = second->tree();
            tree->copy(second->section(), to);

            if ((second = qobject_cast<HolonAbstractWindow *>(tree->object(to))))
            {
                if (HolonDockWidget *secondDock = dockByWindow.value(second))
                {
                    if (!firstDock->orientation() || firstDock->orientation() == splitOrientation)
                    {
                        if (splitOrientation == Qt::Horizontal)
                        {
                            mainWindow->splitDockWidget(firstDock, secondDock, Qt::Vertical);
                            mainWindow->splitDockWidget(firstDock, secondDock, Qt::Horizontal);
                        }
                        else
                        {
                            mainWindow->splitDockWidget(firstDock, secondDock, Qt::Horizontal);
                            mainWindow->splitDockWidget(firstDock, secondDock, Qt::Vertical);
                        }
                    }
                    else
                        mainWindow->splitDockWidget(firstDock, secondDock, splitOrientation);

                    firstDock->setOrientation(splitOrientation);
                    secondDock->setOrientation(splitOrientation);

                    dockWidgetSplitState->addSplit(firstDock, secondDock, splitOrientation);
                }
            }
        }
    }
}

