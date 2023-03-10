// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowarea_p.h"
#include "holonabstracttask.h"
#include "holonabstractwindow.h"
#include "holondockwidget.h"
#include "holontitlebar.h"
#include "holonwindowarea.h"
#include "holondesktop.h"
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
    dock->setObjectName(std::as_const(window)->section().last());
    dockList.append(dock);

    if (dockList.count())
        defaultDock->hide();

    dockByWindow.insert(window, dock);

    if (!qobject_cast<HolonAbstractTask *>(window->parent()))
        desktop->addWindow(window);

    if (dockByWindow.count() > 1)
        for (const HolonDockWidget *dockWidget: std::as_const(dockByWindow))
            dockWidget->titleBar()->showControlButtons();

    if (!window->tree()->isLoaded())
        mainWindow->restoreState(state());
}

void HolonWindowAreaPrivate::maximizeWindow(HolonDockWidget *dock)
{
    if (maximized)
    {
        mainWindowState = mainWindow->saveState();

        for (HolonDockWidget *w : dockList)
            w->hide();

        dock->show();
    }
    else
    {
        for (HolonDockWidget *w : dockList)
            w->show();

        mainWindow->restoreState(mainWindowState);
    }
}

void HolonWindowAreaPrivate::saveState()
{
    if (dockList.size() > 1)
        q_ptr->setValue("mainWindowState", mainWindow->saveState());
}

void HolonWindowAreaPrivate::closeWindow(HolonAbstractWindow *window)
{
    HolonDockWidget *dock = dockByWindow.value(window);
    dockByWindow.remove(window);
    mainWindow->removeDockWidget(dock);
    dock->deleteLater();
    window->deleteLater();

    dockList.removeOne(dock);

    if (!dockList.count())
        defaultDock->show();
    else if(maximized)
    {
        for (HolonDockWidget *w : dockList)
            w->show();

        maximized = false;
    }

    if (dockByWindow.count() == 1)
        dockByWindow.first()->titleBar()->hideControlButtons();
}

void HolonWindowAreaPrivate::setChecked(bool checked)
{
    q_ptr->setValue("checked", checked);
}

void HolonWindowAreaPrivate::setDefaultDockWidget()
{
    mainWindow->setParent(q_ptr);
    defaultDock = new HolonDockWidget(desktop, mainWindow, q_ptr);
    defaultDock->setObjectName("dafaultDock");
}

void HolonWindowAreaPrivate::splitWindow(HolonAbstractWindow *first,
                                         HolonAbstractWindow *second,
                                         Qt::Orientation splitOrientation)
{
    if (HolonDockWidget *firstDock = dockByWindow.value(first))
    {
        QSize firstDockSize = firstDock->size();

        if (HolonAbstractTask *task = first->task())
        {
            QSet<int> windows;
            for (QObject *o : task->children())
                windows.insert(o->objectName().toInt());

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
                    QList<QDockWidget *> dockPair{firstDock, secondDock};
                    QList<int> width(2);
                    QList<int> height(2);

                    mainWindow->splitDockWidget(firstDock, secondDock, splitOrientation);

                    if (splitOrientation == Qt::Horizontal)
                    {
                        width[0] = firstDockSize.width() / 2;
                        height[0] = firstDockSize.height();
                    }
                    else
                    {
                        width[0] = firstDockSize.width();
                        height[0] = firstDockSize.height() / 2;
                    }

                    width[1] = width[0];
                    height[1] = height[0];

                    mainWindow->resizeDocks(dockPair, width, Qt::Horizontal);
                    mainWindow->resizeDocks(dockPair, height, Qt::Vertical);
                }
            }
        }
    }
}

QByteArray HolonWindowAreaPrivate::state() const
{
    return q_ptr->value("mainWindowState").toByteArray();
}
