// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncore.h"
#include "holondesktop.h"
#include "holonopentaskswindow.h"
#include "holonsidebar.h"
#include <QApplication>
#include <QLoaderPluginInterface>
#include <QLoaderSettings>
#include <QLoaderTree>

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

        if (!qstrcmp(shortName, "Sidebar"))
        {
            if (coreApp)
                return nullptr;

            HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent);
            if (desktop)
                return new HolonSidebar(settings, desktop);

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
