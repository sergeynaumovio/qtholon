// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncore.h"
#include "holoncustomtask.h"
#include "holondesktop.h"
#include "holonmessageswidget.h"
#include "holonmessageswindow.h"
#include "holonopentasktreewindow.h"
#include "holonparameterswindow.h"
#include "holonprojecttasktreewindow.h"
#include "holonsettingswindow.h"
#include "holonsidebar.h"
#include "holontaskfolder.h"
#include "holonterminalwindow.h"
#include "holontheme.h"
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
        bool coreApp = !qobject_cast<QApplication *>(QCoreApplication::instance());

        if (!qstrcmp(shortName, "CustomTask"))
        {
            if (HolonTaskFolder *folder = qobject_cast<HolonTaskFolder *>(parent))
                return new HolonCustomTask(settings, folder);

            if (HolonWorkflowModelBranch *openTasksDir = qobject_cast<HolonWorkflowModelBranch *>(parent))
                return new HolonCustomTask(settings, openTasksDir);

            return parent;
        }

        if (!qstrcmp(shortName, "Desktop"))
        {
            if (coreApp)
                return new HolonCore(settings, parent);

            QWidget *widget = qobject_cast<QWidget *>(parent);
            if (!parent || (parent && widget))
                return new HolonDesktop(settings, widget);

            return parent;
        }

        if (!qstrcmp(shortName, "MessagesWidget"))
        {
            if (qobject_cast<HolonCore *>(parent))
                return nullptr;

            if (HolonAbstractTask *task = qobject_cast<HolonAbstractTask *>(parent))
                return new HolonMessagesWidget(settings, task);

            return parent;
        }

        if (!qstrcmp(shortName, "MessagesWindow"))
        {
            if (qobject_cast<HolonCore *>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(parent))
                return new HolonMessagesWindow(settings, desktop);

            if (HolonSidebar *sidebar = qobject_cast<HolonSidebar *>(parent))
                return new HolonMessagesWindow(settings, sidebar);

            return parent;
        }

        if (!qstrcmp(shortName, "OpenTaskTreeWindow"))
        {
            if (qobject_cast<HolonCore *>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(parent))
                return new HolonOpenTaskTreeWindow(settings, desktop);

            if (HolonSidebar *sidebar = qobject_cast<HolonSidebar *>(parent))
                return new HolonOpenTaskTreeWindow(settings, sidebar);

            return parent;
        }

        if (!qstrcmp(shortName, "ParametersWindow"))
        {
            if (qobject_cast<HolonCore *>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(parent))
                return new HolonParametersWindow(settings, desktop);

            if (HolonSidebar *sidebar = qobject_cast<HolonSidebar *>(parent))
                return new HolonParametersWindow(settings, sidebar);

            return parent;
        }

        if (!qstrcmp(shortName, "ProjectTaskTreeWindow"))
        {
            if (qobject_cast<HolonCore *>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(parent))
                return new HolonProjectTaskTreeWindow(settings, desktop);

            if (HolonSidebar *sidebar = qobject_cast<HolonSidebar *>(parent))
                return new HolonProjectTaskTreeWindow(settings, sidebar);

            return parent;
        }

        if (!qstrcmp(shortName, "SettingsWindow"))
        {
            if (qobject_cast<HolonCore*>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(parent))
                return new HolonSettingsWindow(settings, desktop);

            if (HolonSidebar *sidebar = qobject_cast<HolonSidebar *>(parent))
                return new HolonSettingsWindow(settings, sidebar);

            return parent;
        }

        if (!qstrcmp(shortName, "Sidebar"))
        {
            if (qobject_cast<HolonCore *>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(parent))
                return new HolonSidebar(settings, desktop);

            return parent;
        }

        if (!qstrcmp(shortName, "TaskFolder"))
        {
            if (qobject_cast<HolonCore *>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(parent))
                return new HolonTaskFolder(settings, desktop);

            if (HolonTaskFolder *folder = qobject_cast<HolonTaskFolder *>(parent))
                return new HolonTaskFolder(settings, folder);

            return parent;
        }

        if (!qstrcmp(shortName, "TerminalWindow"))
        {
            if (qobject_cast<HolonCore *>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(parent))
                return new HolonTerminalWindow(settings, desktop);

            if (HolonAbstractTask *task = qobject_cast<HolonAbstractTask *>(parent))
                return new HolonTerminalWindow(settings, task);

            return parent;
        }

        if (!qstrcmp(shortName, "Theme"))
        {
            if (qobject_cast<HolonCore *>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(parent))
                return new HolonTheme(settings, desktop);

            return parent;
        }

        if (!qstrcmp(shortName, "WindowArea"))
        {
            if (qobject_cast<HolonCore *>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(parent))
                return new HolonWindowArea(settings, desktop);

            return parent;
        }

        if (!qstrcmp(shortName, "WorkflowModel"))
        {
            if (HolonCore *core = qobject_cast<HolonCore *>(parent))
                return new HolonWorkflowModel(settings, core);

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(parent))
                return new HolonWorkflowModel(settings, desktop);

            return parent;
        }

        if (!qstrcmp(shortName, "WorkflowModelBranch"))
        {
            if (HolonWorkflowModel *model = qobject_cast<HolonWorkflowModel *>(parent))
            {
                if (coreApp)
                    return nullptr;

                return new HolonWorkflowModelBranch(settings, model);
            }

            return parent;
        }

        if (!qstrcmp(shortName, "WorkflowWindow"))
        {
            if (qobject_cast<HolonCore *>(parent))
                return nullptr;

            if (HolonDesktop *desktop = qobject_cast<HolonDesktop *>(parent))
                return new HolonWorkflowWindow(settings, desktop);

            if (HolonWindowArea *windowArea = qobject_cast<HolonWindowArea *>(parent))
                return new HolonWorkflowWindow(settings, windowArea);

            return parent;
        }

        return settings->tree();
    }
};

#include "plugin.moc"
