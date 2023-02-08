// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncore.h"
#include "holoncustomtask.h"
#include "holondesktop.h"
#include "holonmarkupcommanderwindow.h"
#include "holonnewtasksdir.h"
#include "holonnewtasksmodel.h"
#include "holonnewtaskswindow.h"
#include "holonopentasksdir.h"
#include "holonopentasksmodel.h"
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

        if (!qstrcmp(shortName, "CustomTask"))
        {
            if (HolonNewTasksDir *newTaskDir = qobject_cast<HolonNewTasksDir*>(parent))
                return new HolonCustomTask(settings, newTaskDir);

            if (HolonOpenTasksDir *openTasksDir = qobject_cast<HolonOpenTasksDir*>(parent))
                return new HolonCustomTask(settings, openTasksDir);

            return parent;
        }

        if (!qstrcmp(shortName, "Desktop"))
        {
            if (coreApp)
                return new HolonCore(settings, parent);

            QWidget *widget = qobject_cast<QWidget*>(parent);
            if (!parent || (parent && widget))
                return new HolonDesktop(settings, widget);

            return parent;
        }

        if (!qstrcmp(shortName, "MarkupCommanderWindow"))
        {
            if (qobject_cast<HolonCore*>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonMarkupCommanderWindow(settings, desktop);

            return parent;
        }

        if (!qstrcmp(shortName, "NewTasksDir"))
        {
            if (HolonNewTasksModel *model = qobject_cast<HolonNewTasksModel*>(parent))
            {
                if (coreApp)
                    return nullptr;

                return new HolonNewTasksDir(settings, model);
            }

            return parent;
        }

        if (!qstrcmp(shortName, "NewTasksModel"))
        {
            if (qobject_cast<HolonCore*>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonNewTasksModel(settings, desktop);

            return parent;
        }

        if (!qstrcmp(shortName, "NewTasksWindow"))
        {
            if (qobject_cast<HolonCore*>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonNewTasksWindow(settings, desktop);

            if (HolonSidebar *sidebar = qobject_cast<HolonSidebar*>(parent))
                return new HolonNewTasksWindow(settings, sidebar);

            return parent;
        }

        if (!qstrcmp(shortName, "OpenTasksDir"))
        {
            if (HolonOpenTasksModel *model = qobject_cast<HolonOpenTasksModel*>(parent))
            {
                if (coreApp)
                    return nullptr;

                return new HolonOpenTasksDir(settings, model);
            }

            return parent;
        }

        if (!qstrcmp(shortName, "OpenTasksModel"))
        {
            if (HolonCore *core = qobject_cast<HolonCore*>(parent))
                return new HolonOpenTasksModel(settings, core);

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonOpenTasksModel(settings, desktop);

            return parent;
        }

        if (!qstrcmp(shortName, "OpenTasksWindow"))
        {
            if (qobject_cast<HolonCore*>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonOpenTasksWindow(settings, desktop);

            if (HolonSidebar *sidebar = qobject_cast<HolonSidebar*>(parent))
                return new HolonOpenTasksWindow(settings, sidebar);

            return parent;
        }

        if (!qstrcmp(shortName, "Sidebar"))
        {
            if (qobject_cast<HolonCore*>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonSidebar(settings, desktop);

            return parent;
        }

        return settings->tree();
    }
};

#include "plugin.moc"
