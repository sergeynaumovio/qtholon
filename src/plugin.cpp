/****************************************************************************
**
** Copyright (C) 2021, 2022 Sergey Naumov
**
** Permission to use, copy, modify, and/or distribute this
** software for any purpose with or without fee is hereby granted.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
** THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
** CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
** LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
** NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
** CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**
****************************************************************************/

#include "holondesktop.h"
#include "holontaskbar.h"
#include "holonmenu.h"
#include "holonsplitted.h"
#include "holonsidebararea.h"
#include "holonsidebar.h"
#include "holonlabel.h"

#include "holonnewtask.h"
#include "holonopentasks.h"

#include <QLoaderInterface>
#include <QLoaderSettings>

class Plugin : public QObject, QLoaderInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "QtHolon")
    Q_INTERFACES(QLoaderInterface)

public:
    QObject *object(QLoaderSettings *settings, QObject *parent) override
    {
        const char *className = settings->className();

        if (!qstrcmp(className, "HolonDesktop"))
        {
            QWidget *widget = qobject_cast<QWidget*>(parent);
            if (!parent || (parent && widget))
                return new HolonDesktop(settings, widget);

            return parent;
        }

        if (!qstrcmp(className, "HolonTaskbar"))
        {
            HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent);
            if (desktop && !desktop->findChild<HolonTaskbar*>())
                return new HolonTaskbar(settings, desktop);

            return parent;
        }

        if (!qstrcmp(className, "HolonMenu"))
        {
            HolonTaskbar *taskbar = qobject_cast<HolonTaskbar*>(parent);
            if (taskbar && !taskbar->findChild<HolonMenu*>())
                return new HolonMenu(settings, taskbar);

            return parent;
        }

        if (!qstrcmp(className, "HolonSplitted"))
        {
            HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent);
            if (desktop && !desktop->findChild<HolonSplitted*>())
                return new HolonSplitted(settings, desktop);

            HolonSplitted *splitted = qobject_cast<HolonSplitted*>(parent);
            if (splitted && !splitted->findChild<HolonSplitted*>())
                return new HolonSplitted(settings, splitted);

            return parent;
        }

        if (!qstrcmp(className, "HolonSidebarArea"))
        {
            HolonSplitted *splitted = qobject_cast<HolonSplitted*>(parent);

            if (splitted)
                return new HolonSidebarArea(settings, splitted);

            return parent;
        }

        if (!qstrcmp(className, "HolonSidebar"))
        {
            HolonSidebarArea *area = qobject_cast<HolonSidebarArea*>(parent);
            if (area)
                return new HolonSidebar(settings, area);

            return parent;
        }

        if (!qstrcmp(className, "HolonLabel"))
        {
            HolonSplitted *splitted = qobject_cast<HolonSplitted*>(parent);
            if (splitted)
                return new HolonLabel(settings, splitted);

            HolonSidebar *sidebar = qobject_cast<HolonSidebar*>(parent);
            if (sidebar)
                return new HolonLabel(settings, sidebar);

            return parent;
        }

        if (!qstrcmp(className, "HolonNewTask"))
        {
            HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent);
            if (desktop && !desktop->findChild<HolonNewTask*>("", Qt::FindDirectChildrenOnly))
                return new HolonNewTask(settings, desktop);

            HolonTiled *tiled = qobject_cast<HolonTiled*>(parent);
            if (tiled && !tiled->findChild<HolonNewTask*>())
                return new HolonNewTask(settings, tiled);

            return parent;
        }

        if (!qstrcmp(className, "HolonOpenTasks"))
        {
            HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent);
            if (desktop && !desktop->findChild<HolonOpenTasks*>("", Qt::FindDirectChildrenOnly))
                return new HolonOpenTasks(settings, desktop);

            HolonTiled *tiled = qobject_cast<HolonTiled*>(parent);
            if (tiled && !tiled->findChild<HolonOpenTasks*>())
                return new HolonOpenTasks(settings, tiled);

            return parent;
        }

        return nullptr;
    }
};

#include "plugin.moc"
