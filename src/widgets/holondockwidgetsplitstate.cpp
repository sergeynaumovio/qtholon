// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondockwidgetsplitstate.h"
#include "holondockwidget.h"
#include "holontitlebar.h"
#include "holonwindowarea.h"
#include "holonwindowarea_p.h"

using namespace Qt::Literals::StringLiterals;

static
QString toString(Qt::DockWidgetArea area)
{
    switch (area) {
    case Qt::LeftDockWidgetArea: return u"l"_s;
    case Qt::RightDockWidgetArea: return u"r"_s;
    case Qt::TopDockWidgetArea: return u"t"_s;
    case Qt::BottomDockWidgetArea: return u"b"_s;
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

bool HolonDockWidgetSplitState::restoreSplitFromPath(const QString &path)
{
    auto fromString = [](const QString &element) -> Qt::DockWidgetArea
    {
        if (element == 'l'_L1)
            return Qt::LeftDockWidgetArea;

        if (element == 'r'_L1)
            return Qt::RightDockWidgetArea;

        if (element == 't'_L1)
            return Qt::TopDockWidgetArea;

        if (element == 'b'_L1)
            return Qt::BottomDockWidgetArea;

        return {};
    };

    QObject *parent = rootSplit;
    QStringList list = path.split(u'/');

    if (list.first() == ""_L1)
        list.removeFirst();

    for (const QString &element : list)
    {
        if (HolonDockWidgetSplit *split = qobject_cast<HolonDockWidgetSplit *>(parent))
        {
            QObject *object;
            if (bool ok = (element.toUInt(&ok), ok))
            {
                if ((object = parent->findChild<HolonDockWidgetItem *>(element, Qt::FindDirectChildrenOnly)))
                {
                    parent = object;
                    continue;
                }
                else if (parent->children().size() < 2)
                {
                    parent = new HolonDockWidgetItem(element, {}, split);
                    continue;
                }
            }
            else if ((object = parent->findChild<HolonDockWidgetSplit *>(element, Qt::FindDirectChildrenOnly)))
            {
                parent = object;
                continue;
            }
            else if (parent->children().size() < 2)
            {
                if (Qt::DockWidgetArea area = fromString(element))
                {
                    parent = new HolonDockWidgetSplit(area, split);
                    continue;
                }
            }
        }
        else if (HolonDockWidgetItem *item = qobject_cast<HolonDockWidgetItem *>(parent))
        {
            if (Qt::DockWidgetArea area = fromString(element))
            {
                item->area = area;
                continue;
            }
        }

        return false;
    }

    return true;
}

bool HolonDockWidgetSplitState::restoreSplitState()
{
    QByteArray value(d_ptr->q_ptr->value(u"dockWidgetSplitState"_s).toByteArray());
    QStringList list = QString(QLatin1StringView(value.data())).split(u',');
    for (const QString &path : list)
    {
        if (!restoreSplitFromPath(path))
        {
            qDeleteAll(rootSplit->children());
            return false;
        }
    }

    return true;
}

void HolonDockWidgetSplitState::saveSplitState()
{
    QStringList splitState;
    saveSplitStateRecursive(splitState, u""_s, rootSplit);
    d_ptr->setValue(u"dockWidgetSplitState"_s, splitState.join(u',').toLocal8Bit());
}

void HolonDockWidgetSplitState::saveSplitStateRecursive(QStringList &splitState, const QString &section, QObject *parent)
{
    QObjectList children = parent->children();

    for (QObject *child : children)
    {
        QString childSection = section;

        if (HolonDockWidgetSplit *split = qobject_cast<HolonDockWidgetSplit *>(child))
            childSection += u'/' + toString(split->area);
        else if (HolonDockWidgetItem *item = qobject_cast<HolonDockWidgetItem *>(child))
        {
            if (item->dock)
            {
                childSection += u'/' + item->objectName() + u'/' + toString(item->area);
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
