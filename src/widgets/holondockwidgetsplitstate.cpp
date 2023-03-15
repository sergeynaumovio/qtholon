// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondockwidgetsplitstate.h"
#include "holondockwidget.h"
#include "holontitlebar.h"
#include "holonwindowarea.h"
#include "holonwindowarea_p.h"

static
QString toString(Qt::DockWidgetArea area)
{
    switch (area) {
    case Qt::LeftDockWidgetArea: return "l";
    case Qt::RightDockWidgetArea: return "r";
    case Qt::TopDockWidgetArea: return "t";
    case Qt::BottomDockWidgetArea: return "b";
    case Qt::DockWidgetArea_Mask:
    case Qt::NoDockWidgetArea: return {};
    }

    return {};
}

HolonDockWidgetSplit::HolonDockWidgetSplit(HolonDockWidgetSplitState *parent)
:   QObject(parent)
{ }

HolonDockWidgetSplit::HolonDockWidgetSplit(Qt::DockWidgetArea position,
                                           HolonDockWidgetSplit *parent)
:   QObject(parent),
    area(position)
{
    setObjectName(toString(area));
}

HolonDockWidgetItem::HolonDockWidgetItem(HolonDockWidget *widget,
                                         Qt::DockWidgetArea position,
                                         HolonDockWidgetSplit *parent)
:   QObject(parent),
    area(position),
    dock(widget)
{
    setObjectName(dock->objectName());
    dock->titleBar()->setDockWidgetArea(area);
}

HolonDockWidgetItem::HolonDockWidgetItem(const QString &objectName,
                                         Qt::DockWidgetArea position,
                                         HolonDockWidgetSplit *parent)
:   QObject(parent),
    area(position)
{
    setObjectName(objectName);
}

struct HolonDockWidgetSplitArea
{
    Qt::DockWidgetArea first;
    Qt::DockWidgetArea second;
};

void HolonDockWidgetSplitState::addSplit(HolonDockWidget *firstDock,
                                         HolonDockWidget *secondDock,
                                         Qt::Orientation orientation)
{
    HolonDockWidgetSplitArea splitArea = [orientation]() -> HolonDockWidgetSplitArea
    {
        if (orientation == Qt::Horizontal)
            return {Qt::LeftDockWidgetArea, Qt::RightDockWidgetArea};

        return {Qt::TopDockWidgetArea, Qt::BottomDockWidgetArea};
    }();

    if (rootSplit->children().isEmpty())
    {
        firstDock->titleBar()->showSplitButton();
        secondDock->titleBar()->showSplitButton();
    }

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

void HolonDockWidgetSplitState::removeSplit(HolonDockWidget *firstDock)
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
                    if (secondItem->dock)
                    {
                        secondItem->setParent(parentSplit->parent());
                        secondItem->area = parentSplit->area;
                        secondItem->dock->setArea(secondItem->area);
                    }

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

bool HolonDockWidgetSplitState::restoreSplitPath(const QStringList &path, QObject *parent)
{
    auto fromString = [](const QString &element) -> Qt::DockWidgetArea
    {
        if (element == "l")
            return Qt::LeftDockWidgetArea;

        if (element == "r")
            return Qt::RightDockWidgetArea;

        if (element == "t")
            return Qt::TopDockWidgetArea;

        if (element == "b")
            return Qt::BottomDockWidgetArea;

        return {};
    };

    for (const QString &element : path)
    {
        if (HolonDockWidgetSplit *split = qobject_cast<HolonDockWidgetSplit *>(parent))
        {
            QObject *object;
            bool isNumber;
            if ((element.toInt(&isNumber), isNumber))
            {
                if ((object = parent->findChild<HolonDockWidgetItem *>(element, Qt::FindDirectChildrenOnly)))
                {
                    parent = object;
                    continue;
                }
                else if (parent->children().size() < 2)
                    parent = new HolonDockWidgetItem(element, {}, split);
                else
                    return false;
            }
            else if ((object = parent->findChild<HolonDockWidgetSplit *>(element, Qt::FindDirectChildrenOnly)))
            {
                parent = object;
                continue;
            }
            else if (parent->children().size() < 2)
            {
                if (Qt::DockWidgetArea area = fromString(element))
                    parent = new HolonDockWidgetSplit(area, split);
                else
                    return false;
            }
            else
                return false;
        }
        else if (HolonDockWidgetItem *item = qobject_cast<HolonDockWidgetItem *>(parent))
        {
            if (Qt::DockWidgetArea area = fromString(element))
                item->area = area;
            else
                return false;
        }
    }

    return true;
}

void HolonDockWidgetSplitState::restoreSplitState()
{
    QStringList state = QString(d_ptr->q_ptr->value("dockWidgetSplitState").toByteArray()).split(',');
    for (const QString &path : state)
    {
        QStringList list = path.split('/');

        if (list.first() == "")
            list.removeFirst();

        if (!restoreSplitPath(list, rootSplit))
        {
            for (QObject *child : rootSplit->children())
                delete child;

            d_ptr->emitWarning("dockWidgetSplitState format is not valid, please close windows and split again");

            return;
        }
    }
}

void HolonDockWidgetSplitState::saveSplitState()
{
    QStringList splitState;
    saveSplitStateRecursive(splitState, "", rootSplit);
    d_ptr->setValue("dockWidgetSplitState", splitState.join(',').toLocal8Bit());
}

void HolonDockWidgetSplitState::saveSplitStateRecursive(QStringList &splitState, const QString &section, QObject *parent)
{
    QObjectList children = parent->children();

    for (QObject *child : children)
    {
        QString childSection = section;

        if (HolonDockWidgetSplit *split = qobject_cast<HolonDockWidgetSplit *>(child))
            childSection += '/' + toString(split->area);
        else if (HolonDockWidgetItem *item = qobject_cast<HolonDockWidgetItem *>(child))
        {
            if (item->dock)
            {
                childSection += '/' + item->objectName() + '/' + toString(item->area);
                splitState.append(childSection);
            }
        }

        saveSplitStateRecursive(splitState, childSection, child);
    }
}

void HolonDockWidgetSplitState::setSplitItemDock(HolonDockWidget *dock)
{
    if (HolonDockWidgetItem *item = rootSplit->findChild<HolonDockWidgetItem *>(dock->objectName()))
    {
        item->dock = dock;
        dock->setArea(item->area);
    }
}

HolonDockWidgetSplitState::HolonDockWidgetSplitState(HolonWindowAreaPrivate *d)
:   QObject(d->q_ptr),
    d_ptr(d),
    rootSplit(new HolonDockWidgetSplit(this))
{ }
