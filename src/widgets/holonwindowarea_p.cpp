// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowarea_p.h"
#include "holonabstracttask.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holondockwidget.h"
#include "holondockwidgetsplit.h"
#include "holontitlebar.h"
#include "holonwindowarea.h"
#include <QLoaderTree>
#include <QMainWindow>

struct HolonDockWidgetSplitArea
{
    Qt::DockWidgetArea first;
    Qt::DockWidgetArea second;
};

void HolonWindowAreaPrivate::addSplit(HolonDockWidget *firstDock,
                                      HolonDockWidget *secondDock,
                                      Qt::Orientation orientation)
{
    HolonDockWidgetSplitArea splitArea = [orientation]() -> HolonDockWidgetSplitArea
    {
        if (orientation == Qt::Horizontal)
            return {Qt::LeftDockWidgetArea, Qt::RightDockWidgetArea};

        return {Qt::TopDockWidgetArea, Qt::BottomDockWidgetArea};
    }();

    if (HolonDockWidgetItem *firstItem = rootSplit->findChild<HolonDockWidgetItem *>(firstDock->objectName()))
    {
        if (HolonDockWidgetSplit *parentSplit = qobject_cast<HolonDockWidgetSplit *>(firstItem->parent()))
        {
            if (parentSplit == rootSplit && parentSplit->children().size() == 1)
            {
                firstItem->area = splitArea.first;
                firstItem->dock->setArea(firstItem->area);

                new HolonDockWidgetItem(secondDock, splitArea.second, parentSplit);
            }
            else
            {
                HolonDockWidgetSplit *split = new HolonDockWidgetSplit(firstItem->area, parentSplit);

                firstItem->setParent(split);
                firstItem->area = splitArea.first;
                firstItem->dock->setArea(firstItem->area);

                new HolonDockWidgetItem(secondDock, splitArea.second, split);
            }
        }
    }
    else
    {
        new HolonDockWidgetItem(firstDock, splitArea.first, rootSplit);
        new HolonDockWidgetItem(secondDock, splitArea.second, rootSplit);
    }

    saveSplitState();
}

void HolonWindowAreaPrivate::removeSplit(HolonDockWidget *firstDock)
{
    if (HolonDockWidgetItem *firstItem = rootSplit->findChild<HolonDockWidgetItem *>(firstDock->objectName()))
    {
        if (HolonDockWidgetSplit *parentSplit = qobject_cast<HolonDockWidgetSplit *>(firstItem->parent()))
        {
            QList<HolonDockWidgetItem *> itemList = parentSplit->findChildren<HolonDockWidgetItem *>(Qt::FindDirectChildrenOnly);
            itemList.removeOne(firstItem);

            if (itemList.size())
            {
                if (parentSplit == rootSplit)
                    delete firstItem;
                else
                {
                    HolonDockWidgetItem *secondItem = itemList.first();
                    secondItem->setParent(parentSplit->parent());
                    secondItem->area = parentSplit->area;
                    secondItem->dock->setArea(secondItem->area);
                    delete parentSplit;
                }

                saveSplitState();

                return;
            }

            QList<HolonDockWidgetSplit *> splitList = parentSplit->findChildren<HolonDockWidgetSplit *>(Qt::FindDirectChildrenOnly);

            if (splitList.size())
            {
                HolonDockWidgetSplit *split = splitList.first();
                QObjectList splitChildren = split->children();

                for (QObject *object : splitChildren)
                    object->setParent(parentSplit);

                delete firstItem;
                delete split;

                saveSplitState();
            }
        }
    }
}

void HolonWindowAreaPrivate::restoreSplitPath(const QStringList &path, QObject *parent)
{
    auto area = [this](const QString &element) -> Qt::DockWidgetArea
    {
        if (element == "l")
            return Qt::LeftDockWidgetArea;

        if (element == "r")
            return Qt::RightDockWidgetArea;

        if (element == "t")
            return Qt::TopDockWidgetArea;

        if (element == "b")
            return Qt::BottomDockWidgetArea;

        q_ptr->emitWarning("dockWidgetSplitState is not valid");

        return {};
    };

    for (const QString &element : path)
    {
        if (HolonDockWidgetSplit *split = qobject_cast<HolonDockWidgetSplit *>(parent))
        {
            bool isNumber;
            if ((element.toInt(&isNumber), isNumber))
            {
                if ((parent = parent->findChild<HolonDockWidgetItem *>(element, Qt::FindDirectChildrenOnly)))
                    continue;
                else
                    parent = new HolonDockWidgetItem(element, {}, split);
            }
            else if ((parent = parent->findChild<HolonDockWidgetSplit *>(element, Qt::FindDirectChildrenOnly)))
                continue;
            else
                parent = new HolonDockWidgetSplit(area(element), split);
        }
        else if (HolonDockWidgetItem *item = qobject_cast<HolonDockWidgetItem *>(parent))
            item->area = area(element);
    }
}

void HolonWindowAreaPrivate::restoreSplitState()
{
    QStringList state = q_ptr->value("dockWidgetSplitState").toStringList();
    for (const QString &path : state)
    {
        QStringList list = path.split('/');

        if (list.first() == "")
            list.removeFirst();

        restoreSplitPath(list, rootSplit);
    }
}

void HolonWindowAreaPrivate::saveSplitState()
{
    QStringList splitState;
    saveSplitStateRecursive(splitState, "", rootSplit);
    q_ptr->setValue("dockWidgetSplitState", splitState);
}

void HolonWindowAreaPrivate::saveSplitStateRecursive(QStringList &splitState, const QString &section, QObject *parent)
{
    QObjectList children = parent->children();

    for (QObject *child : children)
    {
        QString childSection = section;

        if (HolonDockWidgetSplit *split = qobject_cast<HolonDockWidgetSplit *>(child))
            childSection += '/' + toString(split->area);
        else if (HolonDockWidgetItem *item = qobject_cast<HolonDockWidgetItem *>(child))
        {
           childSection += '/' + item->objectName() + '/' + toString(item->area);
           splitState.append(childSection);
        }

        saveSplitStateRecursive(splitState, childSection, child);
    }
}

void HolonWindowAreaPrivate::setSplitItemDock(HolonDockWidget *dock)
{
    if (HolonDockWidgetItem *item = rootSplit->findChild<HolonDockWidgetItem *>(dock->objectName()))
    {
        item->dock = dock;
        dock->setArea(item->area);
    }
}

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

    if (!rootSplit)
    {
        rootSplit = new HolonDockWidgetSplit(q_ptr);

        if (q_ptr->contains("dockWidgetSplitState"))
            restoreSplitState();
    }

    if (dockList.count())
        defaultDock->hide();

    dockByWindow.insert(window, dock);

    if (!qobject_cast<HolonAbstractTask *>(window->parent()))
        desktop->addWindow(window);

    if (dockByWindow.count() > 1)
        for (const HolonDockWidget *dockWidget: std::as_const(dockByWindow))
            dockWidget->titleBar()->showControlButtons();

    if (!window->tree()->isLoaded())
        mainWindow->restoreState(mainWindowState());

    setSplitItemDock(dock);
}

void HolonWindowAreaPrivate::closeWindow(HolonAbstractWindow *window)
{
    HolonDockWidget *dock = dockByWindow.value(window);
    dockByWindow.remove(window);
    mainWindow->removeDockWidget(dock);
    removeSplit(dock);
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

                    addSplit(firstDock, secondDock, splitOrientation);
                }
            }
        }
    }
}

