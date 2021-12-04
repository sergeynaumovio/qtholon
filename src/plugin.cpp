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

#include "holonmainwindow.h"
#include "holonareasidebars.h"
#include "holonsidebar.h"

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

        if (!qstrcmp(className, "HolonMainWindow"))
        {
            QWidget *widget = qobject_cast<QWidget*>(parent);
            if (!parent || (parent && widget))
                return new HolonMainWindow(settings, widget);

            return parent;
        }

        if (!qstrcmp(className, "HolonTopSideBar"))
        {
            HolonMainWindow *mainwindow = qobject_cast<HolonMainWindow*>(parent);
            if (mainwindow && !mainwindow->findChild<HolonTopSideBar*>())
                return new HolonTopSideBar(settings, mainwindow);

            return parent;
        }

        if (!qstrcmp(className, "HolonRightSideBar"))
        {
            HolonMainWindow *mainwindow = qobject_cast<HolonMainWindow*>(parent);
            if (mainwindow && !mainwindow->findChild<HolonRightSideBar*>())
                return new HolonRightSideBar(settings, mainwindow);

            return parent;
        }

        if (!qstrcmp(className, "HolonBottomSideBar"))
        {
            HolonMainWindow *mainwindow = qobject_cast<HolonMainWindow*>(parent);
            if (mainwindow && !mainwindow->findChild<HolonBottomSideBar*>())
                return new HolonBottomSideBar(settings, mainwindow);

            return parent;
        }

        if (!qstrcmp(className, "HolonLeftSideBar"))
        {
            HolonMainWindow *mainwindow = qobject_cast<HolonMainWindow*>(parent);
            if (mainwindow && !mainwindow->findChild<HolonLeftSideBar*>())
                return new HolonLeftSideBar(settings, mainwindow);

            return parent;
        }

        if (!qstrcmp(className, "HolonSideBar"))
        {
            HolonAreaSideBar *area = qobject_cast<HolonAreaSideBar*>(parent);
            HolonMainWindow *mainwindow = qobject_cast<HolonMainWindow*>(area->parent());
            if (area && mainwindow && mainwindow->findChildren<HolonSideBar*>().size() < 9)
                return new HolonSideBar(settings, area);

            return parent;
        }

        if (!qstrcmp(className, "HolonNewTask"))
        {
            HolonMainWindow *mainwindow = qobject_cast<HolonMainWindow*>(parent);
            if (mainwindow && !mainwindow->findChild<HolonNewTask*>("", Qt::FindDirectChildrenOnly))
                return new HolonNewTask(settings, mainwindow);

            HolonSideBar *sidebar = qobject_cast<HolonSideBar*>(parent);
            if (sidebar && !sidebar->findChild<HolonNewTask*>())
                return new HolonNewTask(settings, sidebar);

            return parent;
        }

        if (!qstrcmp(className, "HolonOpenTasks"))
        {
            HolonMainWindow *mainwindow = qobject_cast<HolonMainWindow*>(parent);
            if (mainwindow && !mainwindow->findChild<HolonOpenTasks*>("", Qt::FindDirectChildrenOnly))
                return new HolonOpenTasks(settings, mainwindow);

            HolonSideBar *sidebar = qobject_cast<HolonSideBar*>(parent);
            if (sidebar && !sidebar->findChild<HolonOpenTasks*>())
                return new HolonOpenTasks(settings, sidebar);

            return parent;
        }

        return nullptr;
    }
};

#include "plugin.moc"
