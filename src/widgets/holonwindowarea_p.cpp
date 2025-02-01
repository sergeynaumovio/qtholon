// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowarea_p.h"
#include "holonabstracttask.h"
#include "holonabstracttaskwindow.h"
#include "holondesktop.h"
#include "holondockwidget.h"
#include "holondockwidgetsplitstate.h"
#include "holonid.h"
#include "holonstackedwindow_p.h"
#include "holontaskstackedwindow.h"
#include "holontitlebar.h"
#include "holonwindowarea.h"
#include "holonwindowareastate.h"
#include <QComboBox>
#include <QLoaderTree>
#include <QMainWindow>

using namespace Qt::Literals::StringLiterals;

HolonWindowAreaPrivate::HolonWindowAreaPrivate(HolonWindowArea *q, HolonDesktop *desk)
:   windowAreaState(&mainWindow),
    q_ptr(q),
    desktop(desk),
    mainWindow(new QMainWindow)
{
    mainWindow->setDockOptions(QMainWindow::AllowNestedDocks);
}

HolonWindowAreaPrivate::~HolonWindowAreaPrivate()
{ }

void HolonWindowAreaPrivate::addWindow(HolonAbstractWindow *window)
{
    HolonDockWidget *dock = new HolonDockWidget(desktop, mainWindow, window, this);
    window->d_ptr->titleBar = dock->titleBar();

    if (!qobject_cast<HolonStackedWindow *>(window))
        dock->titleBar()->addWindow(window);

    if (!dockWidgetSplitState)
    {
        dockWidgetSplitState = new HolonDockWidgetSplitState(this);

        if (q_ptr->contains(u"dockWidgetSplitState"_s))
            if (!dockWidgetSplitState->restoreSplitState())
                q_ptr->emitWarning(u"dockWidgetSplitState format not valid, please close windows and split again"_s);
    }

    dockByWindow.insert(window, dock);
    dockList.append(dock);

    if (mainWindowStateCache.isNull())
        mainWindowStateCache = q_ptr->value(u"mainWindowState"_s).toByteArray();

    if (!qobject_cast<HolonAbstractTaskWindow *>(window) && !qobject_cast<HolonAbstractTask *>(window->parent()))
        desktop->addWindow(window);

    if (dockByWindow.count() > 1)
        for (const HolonDockWidget *dockWidget : std::as_const(dockByWindow))
        {
            dockWidget->titleBar()->showControlButtons();
            dockWidget->titleBar()->hideSidebarButton();
        }

    if (!window->tree()->isLoaded())
        restoreMainWindowStateFromCache();

    dockWidgetSplitState->setSplitItemDock(dock);
}

Qt::DockWidgetArea HolonWindowAreaPrivate::area() const
{
    QString str = q_ptr->value(u"area"_s, u"left"_s).toString();

    if (str == "left"_L1)
        return Qt::LeftDockWidgetArea;

    if (str == "right"_L1)
        return Qt::RightDockWidgetArea;

    if (str == "top"_L1)
        return Qt::TopDockWidgetArea;

    return Qt::BottomDockWidgetArea;
}

void HolonWindowAreaPrivate::cacheMainWindowState()
{
    mainWindowStateCache = mainWindow->saveState();
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
        for (HolonDockWidget *w : std::as_const(dockList))
            w->show();

        maximized = false;
    }

    if (dockByWindow.count() == 1)
    {
        dock = dockByWindow.first();
        dock->titleBar()->hideControlButtons();
        dock->titleBar()->showSplitButton();

        if (qobject_cast<HolonWindowArea *>(q_ptr))
            dock->titleBar()->showSidebarButton();
    }
}

void HolonWindowAreaPrivate::maximizeWindow(HolonDockWidget *dock)
{
    if (maximized)
    {
        cacheMainWindowState();

        for (HolonDockWidget *w : std::as_const(dockList))
            w->hide();

        dock->show();
    }
    else
    {
        for (HolonDockWidget *w : std::as_const(dockList))
            w->show();

        restoreMainWindowStateFromCache();
    }
}

void HolonWindowAreaPrivate::restoreMainWindowStateFromCache()
{
    mainWindow->restoreState(mainWindowStateCache);
}

void HolonWindowAreaPrivate::saveWindowAreaState()
{
    q_ptr->setValue(u"mainWindowState"_s, windowAreaState->save());
}

void HolonWindowAreaPrivate::setChecked(bool checked)
{
    q_ptr->setValue(u"checked"_s, checked);
}

void HolonWindowAreaPrivate::setValue(const QString &key, const QVariant &value)
{
    q_ptr->setValue(key, value);
}

void HolonWindowAreaPrivate::splitWindow(HolonAbstractWindow *window, Qt::Orientation orientation)
{
    splitWindow(window, window, orientation);
}

void HolonWindowAreaPrivate::splitWindow(HolonAbstractWindow *first,
                                         HolonAbstractWindow *second,
                                         Qt::Orientation splitOrientation,
                                         int secondWindowComboboxIndex)
{
    if (HolonDockWidget *firstDock = dockByWindow.value(first))
    {
        auto to = [](auto *object) -> QString
        {
            int id = HolonId::createChildId(object);
            QString section = object->section();
            section += u'/' + QString::number(id);

            return section;
        };

        QString toSection;
        if (HolonTaskStackedWindow *taskStackedWindow = qobject_cast<HolonTaskStackedWindow *>(first))
            toSection = to(taskStackedWindow->task());
        else if (HolonWindowArea *windowarea = qobject_cast<HolonWindowArea *>(first->parent()))
            toSection = to(windowarea);
        else if (HolonAbstractTaskWindow *taskWindow = qobject_cast<HolonAbstractTaskWindow *>(first))
            toSection = to(taskWindow->task());
        else
            return;

        QStringView fromSection;
        if (qobject_cast<HolonStackedWindow *>(first))
            fromSection = first->section();
        else
            fromSection = second->section();

        QLoaderTree *tree = second->tree();
        tree->copy(fromSection, toSection);

        if ((second = qobject_cast<HolonAbstractWindow *>(tree->object(toSection))))
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

            if (qobject_cast<HolonStackedWindow *>(second))
                second->d_ptr->titleBar->windowComboBox()->setCurrentIndex(secondWindowComboboxIndex);
        }
    }
}

