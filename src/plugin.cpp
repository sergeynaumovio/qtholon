// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncore.h"
#include "holoncustomtask.h"
#include "holondesktop.h"
#include "holontasklistwindow.h"
#include "holontaskmodel.h"
#include "holontaskmodelbranch.h"
#include "holontasktreewindow.h"
#include "holonsidebar.h"
#include "holonterminalwindow.h"
#include "holonworkflowmodel.h"
#include "holonworkflowmodelbranch.h"
#include "holonworkflowwindow.h"
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
            if (HolonTaskModelBranch *newTaskDir = qobject_cast<HolonTaskModelBranch*>(parent))
                return new HolonCustomTask(settings, newTaskDir);

            if (HolonWorkflowModelBranch *openTasksDir = qobject_cast<HolonWorkflowModelBranch*>(parent))
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

        if (!qstrcmp(shortName, "TaskListWindow"))
        {
            if (qobject_cast<HolonCore*>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonTaskListWindow(settings, desktop);

            if (HolonSidebar *sidebar = qobject_cast<HolonSidebar*>(parent))
                return new HolonTaskListWindow(settings, sidebar);

            return parent;
        }

        if (!qstrcmp(shortName, "TaskModel"))
        {
            if (qobject_cast<HolonCore*>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonTaskModel(settings, desktop);

            return parent;
        }

        if (!qstrcmp(shortName, "TaskModelBranch"))
        {
            if (HolonTaskModel *model = qobject_cast<HolonTaskModel*>(parent))
            {
                if (coreApp)
                    return nullptr;

                return new HolonTaskModelBranch(settings, model);
            }

            return parent;
        }


        if (!qstrcmp(shortName, "TaskTreeWindow"))
        {
            if (qobject_cast<HolonCore*>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonTaskTreeWindow(settings, desktop);

            if (HolonSidebar *sidebar = qobject_cast<HolonSidebar*>(parent))
                return new HolonTaskTreeWindow(settings, sidebar);

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

        if (!qstrcmp(shortName, "TerminalWindow"))
        {
            if (qobject_cast<HolonCore*>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonTerminalWindow(settings, desktop);

            if (HolonSidebar *sidebar = qobject_cast<HolonSidebar*>(parent))
                return new HolonTerminalWindow(settings, sidebar);

            return parent;
        }

        if (!qstrcmp(shortName, "WindowArea"))
        {
            if (qobject_cast<HolonCore*>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonWindowArea(settings, desktop);

            return parent;
        }

        if (!qstrcmp(shortName, "WorkflowModel"))
        {
            if (HolonCore *core = qobject_cast<HolonCore*>(parent))
                return new HolonWorkflowModel(settings, core);

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonWorkflowModel(settings, desktop);

            return parent;
        }

        if (!qstrcmp(shortName, "WorkflowModelBranch"))
        {
            if (HolonWorkflowModel *model = qobject_cast<HolonWorkflowModel*>(parent))
            {
                if (coreApp)
                    return nullptr;

                return new HolonWorkflowModelBranch(settings, model);
            }

            return parent;
        }

        if (!qstrcmp(shortName, "WorkflowWindow"))
        {
            if (qobject_cast<HolonCore*>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop*>(parent))
                return new HolonWorkflowWindow(settings, desktop);

            if (HolonWindowArea *windowArea = qobject_cast<HolonWindowArea*>(parent))
                return new HolonWorkflowWindow(settings, windowArea);

            return parent;
        }

        return settings->tree();
    }
};

#include "plugin.moc"
