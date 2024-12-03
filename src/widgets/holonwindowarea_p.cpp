// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowarea_p.h"
#include "holonabstracttask.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holondockwidget.h"
#include "holondockwidgetsplitstate.h"
#include "holonid.h"
#include "holontitlebar.h"
#include "holonwindowarea.h"
#include <QLoaderTree>
#include <QMainWindow>

using namespace Qt::Literals::StringLiterals;

HolonWindowAreaPrivate::HolonWindowAreaPrivate(HolonDesktop *desk,
                                               HolonWindowArea *q,
                                               Holon::WindowType winType)
:   desktop(desk),
    q_ptr(q),
    mainWindow(new QMainWindow),
    windowType(winType)
{
    mainWindow->setDockOptions(QMainWindow::AllowNestedDocks);
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

        if (q_ptr->contains(u"dockWidgetSplitState"_s))
            if (!dockWidgetSplitState->restoreSplitState())
                q_ptr->emitWarning(u"dockWidgetSplitState format not valid, please close windows and split again"_s);
    }

    dockByWindow.insert(window, dock);

    if (mainWindowStateCache.isNull())
    {
        if (windowType == Holon::SidebarWindow)
            mainWindowStateCache = window->value(u"mainWindowState"_s).toByteArray();
        else
            mainWindowStateCache = q_ptr->value(u"mainWindowState"_s).toByteArray();
    }

    if (windowType == Holon::SidebarWindow)
        desktop->addWindow(window);

    if (dockByWindow.count() > 1)
        for (const HolonDockWidget *dockWidget: std::as_const(dockByWindow))
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
        for (HolonDockWidget *w : dockList)
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

        for (HolonDockWidget *w : dockList)
            w->hide();

        dock->show();
    }
    else
    {
        for (HolonDockWidget *w : dockList)
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
    q_ptr->setValue(u"mainWindowState"_s, mainWindow->saveState());
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
                                         Qt::Orientation splitOrientation)
{
    if (HolonDockWidget *firstDock = dockByWindow.value(first))
    {
        auto to = [](auto *object) -> QStringList
        {
            int id = HolonId::createChildId(object);
            QStringList section = object->section();
            section.append(QString::number(id));

            return section;
        };

        QStringList toSection;
        if (HolonAbstractTask *task = first->task())
            toSection = to(task);
        else if (HolonWindowArea *windowarea = qobject_cast<HolonWindowArea *>(first->parent()))
            toSection = to(windowarea);
        else
            return;

        QLoaderTree *tree = second->tree();
        QStringList fromSection = second->section();
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
        }
    }
}

