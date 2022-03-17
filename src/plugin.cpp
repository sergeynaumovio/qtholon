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

#include "holoncore.h"
#include "holondesktop.h"
#include "holontaskbar.h"
#include "holonnewtaskmenu.h"
#include "holonsidebar.h"
#include "holonsidebarswitch.h"
#include "holontask.h"
#include "holonopentaskswindow.h"

#include <QLoaderPluginInterface>
#include <QLoaderSettings>
#include <QLoaderTree>
#include <QApplication>

class Plugin : public QObject, QLoaderPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "QtHolon")
    Q_INTERFACES(QLoaderPluginInterface)

public:
    QObject *object(QLoaderSettings *settings, QObject *parent) override
    {
        QByteArray className = settings->className();
        const char *shortName = className.data() + qstrlen("Holon");
        bool coreApp = !qobject_cast<QApplication*>(QCoreApplication::instance());

        if (!qstrcmp(shortName, "Desktop"))
        {
            if (coreApp)
                return new HolonCore(settings, parent);

            QWidget *widget = qobject_cast<QWidget*>(parent);
            if (!parent || (parent && widget))
                return new HolonDesktop(settings, widget);

            return parent;
        }

        if (!qstrcmp(shortName, "Taskbar"))
        {
            if (coreApp)
                return nullptr;

            HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent);
            if (desktop)
                return new HolonTaskbar(settings, desktop);

            return parent;
        }

        if (!qstrcmp(shortName, "NewTaskMenu"))
        {
            if (coreApp)
                return nullptr;

            HolonTaskbar *taskbar = qobject_cast<HolonTaskbar*>(parent);
            if (taskbar && !taskbar->findChild<HolonNewTaskMenu*>())
                return new HolonNewTaskMenu(settings, taskbar);

            return parent;
        }

        if (!qstrcmp(shortName, "Sidebar"))
        {
            if (coreApp)
                return nullptr;

            HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent);
            if (desktop)
                return new HolonSidebar(settings, desktop);

            return parent;
        }

        if (!qstrcmp(shortName, "SidebarSwitch"))
        {
            if (coreApp)
                return nullptr;

            HolonTaskbar *taskbar = qobject_cast<HolonTaskbar*>(parent);
            if (taskbar)
                return new HolonSidebarSwitch(settings, taskbar);

            return parent;
        }

        if (!qstrcmp(shortName, "OpenTasksWindow"))
        {
            if (coreApp)
                return nullptr;

            HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent);
            if (desktop)
                return new HolonOpenTasksWindow(settings, desktop);

            HolonSidebar *sidebar = qobject_cast<HolonSidebar*>(parent);
            if (sidebar)
                return new HolonOpenTasksWindow(settings, sidebar);

            return parent;
        }

        return settings->tree();
    }
};

#include "plugin.moc"
