/****************************************************************************
**
** Copyright (C) 2021 Sergey Naumov
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

#include "holonlabel.h"
#include "holonmain.h"
#include "holonsidebar.h"
#include "holonsidebararea.h"
#include "holonsplitted.h"

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

        if (!qstrcmp(className, "HolonMain"))
        {
            QWidget *widget = qobject_cast<QWidget*>(parent);
            if (!parent || (parent && widget))
                return new HolonMain(settings, widget);

            return parent;
        }

        if (!qstrcmp(className, "HolonSplitted"))
        {
            HolonMain *main = qobject_cast<HolonMain*>(parent);
            if (main && !main->findChild<HolonSplitted*>())
                return new HolonSplitted(settings, main);

            HolonSplitted *splitted = qobject_cast<HolonSplitted*>(parent);
            if (splitted && !splitted->findChild<HolonSplitted*>())
                return new HolonSplitted(settings, splitted);

            return parent;
        }

        if (!qstrcmp(className, "HolonSidebarArea"))
        {
            HolonSplitted *splitted = qobject_cast<HolonSplitted*>(parent);

            if (splitted && splitted->mainWindow()->findChildren<HolonSidebarArea*>().size() < 9)
                return new HolonSidebarArea(settings, splitted);

            return parent;
        }

        if (!qstrcmp(className, "HolonSidebar"))
        {
            HolonSidebarArea *area = qobject_cast<HolonSidebarArea*>(parent);
            if (area && area->mainWindow()->findChildren<HolonSidebar*>().size() < 9)
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
            HolonMain *main = qobject_cast<HolonMain*>(parent);
            if (main && !main->findChild<HolonNewTask*>("", Qt::FindDirectChildrenOnly))
                return new HolonNewTask(settings, main);

            HolonTiled *tiled = qobject_cast<HolonTiled*>(parent);
            if (tiled && !tiled->findChild<HolonNewTask*>())
                return new HolonNewTask(settings, tiled);

            return parent;
        }

        if (!qstrcmp(className, "HolonOpenTasks"))
        {
            HolonMain *main = qobject_cast<HolonMain*>(parent);
            if (main && !main->findChild<HolonOpenTasks*>("", Qt::FindDirectChildrenOnly))
                return new HolonOpenTasks(settings, main);

            HolonTiled *tiled = qobject_cast<HolonTiled*>(parent);
            if (tiled && !tiled->findChild<HolonOpenTasks*>())
                return new HolonOpenTasks(settings, tiled);

            return parent;
        }

        return nullptr;
    }
};

#include "plugin.moc"
