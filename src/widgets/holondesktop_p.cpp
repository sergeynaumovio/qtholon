// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondesktop_p.h"
#include "holonabstracttask.h"
#include "holonabstracttask_p.h"
#include "holonabstracttaskwindow.h"
#include "holonabstractwindow.h"
#include "holonabstractwindow_p.h"
#include "holoncore_p.h"
#include "holondesktop.h"
#include "holonmainwindow.h"
#include "holonopentasktreemodel.h"
#include "holonopentasktreeview.h"
#include "holonparameterswindow.h"
#include "holonparameterswindow_p.h"
#include "holonprojecttasktreewindow.h"
#include "holonprojecttasktreewindow_p.h"

#if BUILD_WITH_PYSIDE

#include "holonpythonbindings.h"
#include "holonpythontask.h"
#include "holonpythontaskwindow.h"

#endif

#include "holonsettingswindow.h"
#include "holonsidebar.h"
#include "holonsidebar_p.h"
#include "holonsidebardockwidget.h"
#include "holonsidebardockwidget_p.h"
#include "holonsidebarmainwindow.h"
#include "holonstackedwidget.h"
#include "holontaskbar.h"
#include "holontaskstackedwindow.h"
#include "holontaskthread.h"
#include "holontheme.h"
#include "holontheme_p.h"
#include "holonthemestyle.h"
#include "holonthemestyle_p.h"
#include "holonwindowareaswitch.h"
#include "holonworkflow.h"
#include "holonworkflowwindow.h"
#include <QApplication>
#include <QLayout>
#include <QLoaderTree>
#include <QShortcut>

using namespace Qt::Literals::StringLiterals;
using MainWindowNestingIndex = int;

class HolonDesktopPrivateData : public HolonCorePrivate
{

#if BUILD_WITH_PYSIDE

    HolonPythonTask pyTask;
    HolonPythonTaskWindow pyTaskWindow;

#else

    // fix d_ptr size
    [[maybe_unused]] std::byte pyTask[8];
    [[maybe_unused]] std::byte pyTaskWindow[8];

#endif

    void addSidebarWindow(HolonAbstractWindow *window);
    void addTasksCustomWidgets(HolonTaskStackedWidget *taskStackedWidget, QMetaType sidebarWindow);
    void addTasksParametersWidgets(HolonTaskStackedWidget *taskStackedWidget);
    void addTasksWidgets(HolonTaskStackedWidget *taskStackedWidget, auto widgetFrom);
    void addTaskWindow(auto *window);
    void addWindowAreaStackedWidget(HolonWindowAreaStackedWidget *windowAreaStackedWidget);

public:
    HolonDesktopPrivate &desktop_d;
    HolonDesktop *const q_ptr;
    const QString sidebarMoveShortcut;
    const HolonDesktopPrivate::TaskbarArea taskbarArea;

    QWidget *screen;
    QWidget *top;
    QWidget *middle;
    QWidget *bottom;
    QWidget *left;
    QWidget *right;

    HolonMainWindow *mainWindow;

    HolonTaskbar *taskbar;

    QList<HolonAbstractTask *> taskList;
    QList<HolonTheme *> themeList;

    struct
    {
        QMap<HolonSidebarDockWidget *, HolonSidebarMainWindow *> mainWindowByDock;
        QMap<MainWindowNestingIndex, HolonSidebarMainWindow *> mainWindowByNestingIndex;
        QSet<HolonSidebarDockWidget *> docks;
        QMap<HolonSidebar *, HolonSidebarDockWidget *> dockBySidebar;
        bool visibleTitleBar{};

    } sidebars;

    QList<HolonTaskStackedWidget *> taskStackedWidgetList;
    QList<HolonWindowStackedWidget *> windowStackedWidgetList;
    QList<HolonWindowAreaStackedWidget *> windowAreaStackedWidgetList;

    QHash<HolonAbstractWindow *, HolonWindowArea *> windowAreaByTaskWindow;
    QHash<HolonAbstractWindow *, HolonWindowStackedWidget *> stackedWidgetByTaskWindow;

    HolonAbstractTask *currentTask{};
    QHash<HolonAbstractTask *, HolonAbstractWindow *> currentTaskWindow;
    HolonAbstractWindow *currentWindow{};
    HolonWindowArea *currentWindowArea{};
    HolonWorkflow *currentWorkflow{};
    HolonOpenTaskTreeModel *openTaskTreeModel{};
    QHash<HolonAbstractWindow *, HolonOpenTaskTreeView *> openTaskTreeViewByWindow;
    QList<HolonParametersWindow *> parametersWindows;
    HolonTheme *theme{};

    HolonTaskThread *const taskThread;

    HolonDesktopPrivateData(HolonDesktopPrivate &d, HolonDesktop *q);
    ~HolonDesktopPrivateData() { }

    void addObject(auto *object, auto &list, auto *&current);
    void addSidebar(HolonSidebar *sidebar);
    void addTask(HolonAbstractTask *task);
    void addWidget(QWidget *widget, QWidget *parent);
    void addWindow(HolonAbstractWindow *window);
    void addWindowArea(HolonWindowArea *windowArea);
    void addWorkflow(HolonWorkflow *workflow);
    void closeTask(HolonAbstractTask *task);
    void closeWindow(HolonAbstractWindow *window);

#if BUILD_WITH_PYSIDE

    bool initPythonBindings();

#endif

    void removeUncheckedSidebars(HolonSidebarMainWindow *sidebarMainWindow);
    void setCurrentTask(HolonAbstractTask *task);
    void setCurrentWindow(HolonAbstractWindow *window);
    void setCurrentWindowArea(HolonWindowArea *windowArea);
    void setHBoxLayout(QWidget *&widget, const QString &name, QWidget *parent);
    void setLayout();
    void setMainWindow(HolonMainWindow *&widget, const QString &name, QWidget *parent);
    void setTaskbar();
    void setTheme(HolonTheme *theme);
    void setVBoxLayout();
    void setVBoxLayout(QWidget *&widget, const QString &name, QWidget *parent);
};

void HolonDesktopPrivateData::addSidebarWindow(HolonAbstractWindow *window)
{
    if (!openTaskTreeModel)
        openTaskTreeModel = new HolonOpenTaskTreeModel(q_ptr);

    if (HolonProjectTaskTreeWindow *projectTaskTreeWindow = qobject_cast<HolonProjectTaskTreeWindow *>(window))
        projectTaskTreeWindow->d_ptr->setOpenTaskTreeModel(openTaskTreeModel);

    if (HolonParametersWindow *parametersWindow = qobject_cast<HolonParametersWindow *>(window))
    {
        parametersWindow->d_func()->setOpenTaskTreeModel(openTaskTreeModel);
        HolonTaskStackedWidget *taskStackedWidget = static_cast<HolonTaskStackedWidget *>(window->centralWidget());
        addTasksParametersWidgets(taskStackedWidget);
        parametersWindows.append(parametersWindow);
        return;
    }

    if (QWidget *widget = window->centralWidget())
    {
        if (HolonOpenTaskTreeView *openTaskTreeView = qobject_cast<HolonOpenTaskTreeView *>(widget))
        {
            HolonStackedWindow *stacked = qobject_cast<HolonStackedWindow *>(window->parent());
            HolonAbstractWindow *sidebarWindow = (stacked ? stacked : window);
            openTaskTreeViewByWindow.insert(sidebarWindow, openTaskTreeView);

            openTaskTreeView->setModel(openTaskTreeModel);

            if (currentTask)
                openTaskTreeView->setCurrentTask(currentTask);

            return;
        }

        if (HolonStackedWidget *stackedWidget = qobject_cast<HolonStackedWidget *>(widget))
        {
            if (HolonTaskStackedWidget *taskStackedWidget = qobject_cast<HolonTaskStackedWidget *>(stackedWidget))
                return addTasksCustomWidgets(taskStackedWidget, window->metaObject()->metaType());

            if (HolonWindowAreaStackedWidget *windowAreaStackedWidget = qobject_cast<HolonWindowAreaStackedWidget *>(stackedWidget))
                return addWindowAreaStackedWidget(windowAreaStackedWidget);
        }
    }
}

void HolonDesktopPrivateData::addTasksCustomWidgets(HolonTaskStackedWidget *taskStackedWidget, QMetaType sidebarWindow)
{
    addTasksWidgets(taskStackedWidget, [=](HolonAbstractTask *task){ return task->customWidget(sidebarWindow); });
}

void HolonDesktopPrivateData::addTasksParametersWidgets(HolonTaskStackedWidget *taskStackedWidget)
{
    addTasksWidgets(taskStackedWidget, [](HolonAbstractTask *task){ return task->parametersWidget(); });
}

void HolonDesktopPrivateData::addTasksWidgets(HolonTaskStackedWidget *taskStackedWidget, auto widgetFrom)
{
    taskStackedWidgetList.append(taskStackedWidget);
    HolonAbstractTask *current = currentTask;

    for (HolonAbstractTask *task : std::as_const(taskList))
    {
        currentTask = task;

        /// if (task->isCurrent()) TODO: add parametersWidget only for the current task, next load on demand
        if (QWidget *widget = widgetFrom(task))
        {
            taskStackedWidget->addTaskWidget(task, widget);

            if (task->isCurrent())
            {
                taskStackedWidget->setCurrentTask(task);
                current = task;
            }
        }
    }

    currentTask = current;
}

void HolonDesktopPrivateData::addTaskWindow(auto *window)
{
    if (stackedWidgetByTaskWindow.contains(window))
        return;

    HolonAbstractTask *task = window->task();

    HolonWindowArea *windowArea = [task, this]()
    {
        if (HolonWindowArea *wa = mainWindow->windowArea(task))
            return wa;

        return mainWindow->addWindowArea(task);
    }();

    if (!windowArea)
        return;

    if (qobject_cast<HolonAbstractTask *>(window->parent()))
        windowArea->addWindow(window);

    windowAreaByTaskWindow.insert(window, windowArea);

    if (qobject_cast<HolonTaskStackedWindow *>(window))
        return;

    HolonAbstractTaskWindow *taskWindow = qobject_cast<HolonAbstractTaskWindow *>(window);

    if (taskWindow->isCurrent())
    {
        currentTaskWindow[task] = taskWindow;

        if (task->isCurrent())
        {
            currentWindow = taskWindow;
            taskWindow->centralWidget()->setFocus();
        }
    }

    QMetaType settingsType = QMetaType::fromType<HolonSettingsWindow>();

    for (HolonTaskStackedWidget *taskStackedWidget : std::as_const(taskStackedWidgetList))
    {
        QMetaType windowType = taskStackedWidget->windowType();
        QWidget *widget = taskStackedWidget->taskWidget(task);
        HolonWindowStackedWidget *windowStackedWidget = qobject_cast<HolonWindowStackedWidget *>(widget);

        if (widget && !windowStackedWidget)
            continue;

        if (!widget)
        {
            windowStackedWidget = new HolonWindowStackedWidget(windowType);
            taskStackedWidget->addTaskWidget(task, windowStackedWidget);
        }

        if ((widget = (windowType == settingsType ? taskWindow->settingsWidget() :
                                                    taskWindow->customWidget(windowType))))
        {
            windowStackedWidget->addWindowWidget(taskWindow, widget);
            stackedWidgetByTaskWindow.insert(taskWindow, windowStackedWidget);

            if (taskWindow->isCurrent())
                currentTaskWindow[task] = taskWindow;
        }
    }
}

void HolonDesktopPrivateData::addWindowAreaStackedWidget(HolonWindowAreaStackedWidget *windowAreaStackedWidget)
{
    windowAreaStackedWidgetList.append(windowAreaStackedWidget);

    QMetaType windowType = windowAreaStackedWidget->windowType();
    QMetaType settingsType = QMetaType::fromType<HolonSettingsWindow>();
    HolonTaskStackedWidget *taskStackedWidget = new HolonTaskStackedWidget(windowType);
    windowAreaStackedWidget->addWindowAreaWidget(nullptr, taskStackedWidget);
    taskStackedWidgetList.append(taskStackedWidget);

    for (HolonAbstractTask *task : std::as_const(taskList))
    {
        QList<HolonAbstractTaskWindow *> taskWindowList = task->findChildren<HolonAbstractTaskWindow *>();
        if (taskWindowList.size())
        {
            HolonWindowStackedWidget *windowStackedWidget = new HolonWindowStackedWidget(windowType);
            taskStackedWidget->addTaskWidget(task, windowStackedWidget);

            if (task->isCurrent())
                taskStackedWidget->setCurrentTask(task);

            for (HolonAbstractTaskWindow *taskWindow : std::as_const(taskWindowList))
            {
                if (QWidget *widget = (windowType == settingsType ? taskWindow->settingsWidget() :
                                                                    taskWindow->customWidget(windowType)))
                {
                    windowStackedWidget->addWindowWidget(taskWindow, widget);
                    stackedWidgetByTaskWindow.insert(taskWindow, windowStackedWidget);
                }
            }
        }
    }
}

HolonDesktopPrivateData::HolonDesktopPrivateData(HolonDesktopPrivate &d, HolonDesktop *q)
:   desktop_d(d),
    q_ptr(q),
    sidebarMoveShortcut(q_ptr->value(u"sidebarMoveShortcut"_s).toString()),
    taskbarArea([this]()
    {
        QString string = q_ptr->value(u"taskbarArea"_s).toString();
        if (string == "left"_L1)
            return HolonDesktopPrivate::TaskbarArea::Left;

        if (string == "right"_L1)
            return HolonDesktopPrivate::TaskbarArea::Right;

        if (string == "top"_L1)
            return HolonDesktopPrivate::TaskbarArea::Top;

        return HolonDesktopPrivate::TaskbarArea::Bottom;
    }()),
    taskThread(new HolonTaskThread(q))
{
    taskThread->setObjectName(u"task"_s);

    QShortcut *shortcut = new QShortcut(QKeySequence(sidebarMoveShortcut), q_ptr);
    QObject::connect(shortcut, &QShortcut::activated, q_ptr, [this]()
    {
        sidebars.visibleTitleBar = !sidebars.visibleTitleBar;
        for (HolonSidebarDockWidget *dock : std::as_const(sidebars.docks))
            dock->showTitleBarWidget(sidebars.visibleTitleBar);
    });
}

void HolonDesktopPrivateData::addObject(auto *object, auto &list, auto *&current)
{
    if (list.contains(object))
        return;

    list.append(object);

    if (object->isCurrent())
    {
        if (!current)
            current = object;
        else if (qobject_cast<HolonWorkflow *>(object))
            desktop_d.emitWarning(u"current workflow already set"_s);
    }
}

void HolonDesktopPrivateData::addSidebar(HolonSidebar *sidebar)
{
    HolonSidebarDockWidget *sidebarDock;
    int index = sidebar->mainWindowNestingIndex();
    HolonSidebarMainWindow *sidebarMainWindow = sidebars.mainWindowByNestingIndex.value(index);

    if (!sidebarMainWindow)
    {
        QMapIterator<MainWindowNestingIndex, HolonSidebarMainWindow *> it(sidebars.mainWindowByNestingIndex);
        it.next();
        while (it.hasNext() && it.key() < index)
            it.next();

        if (it.hasNext())
            it.previous();

        sidebarMainWindow = new HolonSidebarMainWindow(desktop_d);

        it.value()->takeCentralWidget();
        it.value()->setCentralWidget(sidebarMainWindow);

        if (it.hasNext())
        {
            it.next();
            sidebarMainWindow->setCentralWidget(it.value());
        }

        sidebars.mainWindowByNestingIndex.insert(index, sidebarMainWindow);
        sidebars.mainWindowByNestingIndex.last()->setCentralWidget(mainWindow);
    }

    sidebarDock = sidebarMainWindow->addSidebar(sidebar);
    sidebars.mainWindowByDock.insert(sidebarDock, sidebarMainWindow);
    removeUncheckedSidebars(sidebarMainWindow);

    sidebars.docks.insert(sidebarDock);
    sidebars.dockBySidebar.insert(sidebar, sidebarDock);

    taskbar->sidebarSwitch()->addSidebar(sidebar);
}

void HolonDesktopPrivateData::addTask(HolonAbstractTask *task)
{
    if (taskList.contains(task))
        return;

    taskList.append(task);

    if (task->isCurrent())
        currentTask = task;

    for (HolonTaskStackedWidget *taskStackedWidget : std::as_const(taskStackedWidgetList))
    {
        if (taskStackedWidget->windowType() == QMetaType::fromType<HolonParametersWindow>())
        {
            if (QWidget *widget = task->parametersWidget())
                taskStackedWidget->addTaskWidget(task, widget);
        }
        else if (QWidget *widget = task->customWidget(taskStackedWidget->windowType()))
            taskStackedWidget->addTaskWidget(task, widget);
    }
}

void HolonDesktopPrivateData::addWidget(QWidget *widget, QWidget *parent)
{
    parent->layout()->addWidget(widget);
}

void HolonDesktopPrivateData::addWindow(HolonAbstractWindow *window)
{
    if (HolonTaskStackedWindow *taskStackedWindow = qobject_cast<HolonTaskStackedWindow *>(window))
        return addTaskWindow(taskStackedWindow);

    if (HolonAbstractTaskWindow *taskWindow = qobject_cast<HolonAbstractTaskWindow *>(window))
    {
        if (qobject_cast<HolonAbstractTask *>(window->parent()) ||
            qobject_cast<HolonTaskStackedWindow *>(window->parent()))
        {
            return addTaskWindow(taskWindow);
        }

        return;
    }

    if (qobject_cast<HolonSidebar *>(window->parent()))
        return addSidebarWindow(window);

    if (HolonStackedWindow *stacked = qobject_cast<HolonStackedWindow *>(window->parent()))
        if (qobject_cast<HolonSidebar *>(stacked->parent()))
            return addSidebarWindow(window);
}

void HolonDesktopPrivateData::addWindowArea(HolonWindowArea *windowArea)
{
    mainWindow->addWindowArea(windowArea);
    taskbar->sidebarSwitch()->addWindowArea(windowArea);

    if (windowArea->isChecked())
    {
        mainWindow->setCurrentWindowArea(windowArea);
        currentWindowArea = windowArea;
    }
}

void HolonDesktopPrivateData::addWorkflow(HolonWorkflow *workflow)
{
    addObject(workflow, workflowList, currentWorkflow);
}

void HolonDesktopPrivateData::closeTask(HolonAbstractTask *task)
{
    Q_UNUSED(task)
}

void HolonDesktopPrivateData::closeWindow(HolonAbstractWindow *window)
{
    if (HolonSidebar *sidebar = qobject_cast<HolonSidebar *>(window->parent()))
    {
        sidebar->closeWindow(window);

        if (openTaskTreeViewByWindow.contains(window))
            openTaskTreeViewByWindow.remove(window);

        return;
    }

    if (qobject_cast<HolonWorkflowWindow *>(window))
    {
        HolonWindowArea *windowArea = qobject_cast<HolonWindowArea *>(window->parent());
        windowArea->closeWindow(window);

        return;
    }

    if (HolonWindowArea *windowArea = windowAreaByTaskWindow.value(window))
    {
        windowArea->closeWindow(window);
        windowAreaByTaskWindow.remove(window);
    }

    if (HolonWindowStackedWidget *stackedWidget = stackedWidgetByTaskWindow.value(window))
    {
        stackedWidget->removeWindowWidget(window);
        stackedWidgetByTaskWindow.remove(window);
    }

    if (window == currentWindow)
    {
        currentWindow = nullptr;
        currentTaskWindow.remove(currentTask);
    }
}

#if BUILD_WITH_PYSIDE

bool HolonDesktopPrivateData::initPythonBindings()
{
    pyTask.desktop = q_ptr;
    pyTaskWindow.desktop = q_ptr;

    if (!HolonPythonBindings::init() ||
        !HolonPythonBindings::bind(0, "task", &pyTask) ||
        !HolonPythonBindings::bind(1, "taskWindow", &pyTaskWindow))
    {
        return false;
    }

    return true;
}

#endif

void HolonDesktopPrivateData::removeUncheckedSidebars(HolonSidebarMainWindow *sidebarMainWindow)
{
    QMapIterator it(sidebars.mainWindowByDock);
    while(it.hasNext())
    {
        it.next();
        if (it.value() == sidebarMainWindow)
        {
            HolonSidebarDockWidget *dock = it.key();
            QList<HolonSidebar *> sidebarList = dock->sidebars();
            int i{};

            for (; i < sidebarList.size(); ++i)
                if (sidebarList[i]->isChecked())
                    break;

            if (i == sidebarList.size())
                sidebarMainWindow->removeDockWidget(it.key());
        }
    }
}

void HolonDesktopPrivateData::setCurrentTask(HolonAbstractTask *task)
{
    // set current widget for parameters, messages, settings etc.
    for (HolonTaskStackedWidget *taskStackedWidget : std::as_const(taskStackedWidgetList))
    {
        taskStackedWidget->setCurrentTask(task);

        // set current widget for task window settings
        if (auto *windowStackedWidget = qobject_cast<HolonWindowStackedWidget *>(taskStackedWidget->currentWidget()))
            windowStackedWidget->setCurrentWindow(currentTaskWindow.value(task));
    }

    mainWindow->setCurrentTask(task);
    currentTask = task;

    for (HolonOpenTaskTreeView *view : std::as_const(openTaskTreeViewByWindow))
        view->setCurrentTask(task);

    // set current index for combobox
    for (HolonParametersWindow *window : std::as_const(parametersWindows))
        window->d_func()->setCurrentTask(task);

    if (HolonAbstractWindow *window = currentTaskWindow.value(task))
        if (QWidget *widget = window->centralWidget())
        {
            if (HolonWindowStackedWidget *stacked = qobject_cast<HolonWindowStackedWidget *>(widget))
                widget = stacked->currentWidget();

            widget->setFocus();
        }
}

void HolonDesktopPrivateData::setCurrentWindow(HolonAbstractWindow *window)
{
    for (HolonTaskStackedWidget *taskStackedWidget : std::as_const(taskStackedWidgetList))
        if (auto *windowStackedWidget = qobject_cast<HolonWindowStackedWidget *>(taskStackedWidget->currentWidget()))
            windowStackedWidget->setCurrentWindow(window);

    currentWindow = window;
    currentTaskWindow[currentTask] = window;
}

void HolonDesktopPrivateData::setCurrentWindowArea(HolonWindowArea *windowArea)
{
    mainWindow->setCurrentWindowArea(windowArea);
    currentWindowArea = windowArea;
}

void HolonDesktopPrivateData::setHBoxLayout(QWidget *&widget, const QString &name, QWidget *parent)
{
    widget = new QWidget(parent);
    widget->setObjectName(name);
    widget->setLayout(new QHBoxLayout(widget));
    widget->layout()->setContentsMargins({});
    widget->layout()->setSpacing(0);
    parent->layout()->addWidget(widget);
}

void HolonDesktopPrivateData::setLayout()
{
    setVBoxLayout();
    setVBoxLayout(screen, u"Screen"_s, q_ptr);
    {
        setVBoxLayout(top, u"ScreenTop"_s, screen);
        setHBoxLayout(middle, u"ScreenMiddle"_s, screen);
        {
            setHBoxLayout(left, u"ScreenLeft"_s, middle);
            setMainWindow(mainWindow, u"ScreenCenter"_s, middle);
            setHBoxLayout(right, u"ScreenRight"_s, middle);
        }
        setVBoxLayout(bottom, u"ScreenBottom"_s, screen);
    }
    setTaskbar();
}

void HolonDesktopPrivateData::setMainWindow(HolonMainWindow *&widget, const QString &name, QWidget *parent)
{
    HolonSidebarMainWindow *sidebarMainWindow = new HolonSidebarMainWindow(desktop_d);
    parent->layout()->addWidget(sidebarMainWindow);
    sidebars.mainWindowByNestingIndex.insert(0, sidebarMainWindow);

    widget = new HolonMainWindow(desktop_d);
    sidebarMainWindow->setCentralWidget(widget);
    widget->setObjectName(name);
}

void HolonDesktopPrivateData::setTaskbar()
{
    QWidget *parent{};

    switch (taskbarArea) {
    case HolonDesktopPrivate::Top: parent = top; break;
    case HolonDesktopPrivate::Bottom: parent = bottom; break;
    case HolonDesktopPrivate::Left: parent = left; break;
    case HolonDesktopPrivate::Right: parent = right; break;
    }

    taskbar = new HolonTaskbar(desktop_d);

    addWidget(taskbar, parent);
}

void HolonDesktopPrivateData::setTheme(HolonTheme *thm)
{
    if (!theme)
        theme = thm;
    else
        desktop_d.emitWarning(u"theme already set"_s);
}

void HolonDesktopPrivateData::setVBoxLayout()
{
    q_ptr->setLayout(new QVBoxLayout(q_ptr));
    q_ptr->layout()->setContentsMargins({});
    q_ptr->layout()->setSpacing(0);
}

void HolonDesktopPrivateData::setVBoxLayout(QWidget *&widget, const QString &name, QWidget *parent)
{
    widget = new QWidget(parent);
    widget->setObjectName(name);
    widget->setLayout(new QVBoxLayout(widget));
    widget->layout()->setContentsMargins({});
    widget->layout()->setSpacing(0);
    parent->layout()->addWidget(widget);
}

HolonDesktopPrivate::HolonDesktopPrivate(HolonDesktop *q)
:   d_ptr(*this, q),
    q_ptr(q)
{
    if (q->contains(u"saveShortcut"_s))
    {
        QShortcut *shortcut = new QShortcut(q);
        {
            shortcut->setKey(QKeySequence(q->value(u"saveShortcut"_s).toString()));
            QObject::connect(shortcut, &QShortcut::activated, q, [q] { q->tree()->save(); });
        }
    }

#if BUILD_WITH_PYSIDE

    if (!d_ptr->initPythonBindings())
        q_ptr->emitWarning(u"Python module not loaded"_s);

#endif

}

void HolonDesktopPrivate::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->addSidebar(sidebar);
    sidebar->d_func()->desktop_d = this;
}

void HolonDesktopPrivate::addTask(HolonAbstractTask *task)
{
    if (!task)
        return;

    if (q_ptr->taskThread()->isRunning())
        return q_ptr->emitWarning(u"task not added"_s);

    d_ptr->addTask(task);
}

void HolonDesktopPrivate::addWindow(HolonAbstractWindow *window)
{
    d_ptr->addWindow(window);
}

void HolonDesktopPrivate::addWindowArea(HolonWindowArea *windowArea)
{
    d_ptr->addWindowArea(windowArea);
}

void HolonDesktopPrivate::addWorkflow(HolonWorkflow *workflow)
{
    d_ptr->addWorkflow(workflow);
}

void HolonDesktopPrivate::closeTask(HolonAbstractTask *task)
{
    d_ptr->closeTask(task);
}

void HolonDesktopPrivate::closeWindow(HolonAbstractWindow *window)
{
    d_ptr->closeWindow(window);
}

void HolonDesktopPrivate::closeWindowArea(HolonWindowArea *windowArea)
{
    taskbar()->sidebarSwitch()->closeWindowArea(windowArea);
}

void HolonDesktopPrivate::emitWarning(const QString &warning) const
{
    q_ptr->emitWarning(warning);
}

void HolonDesktopPrivate::setLayout()
{
    d_ptr->setLayout();
}

void HolonDesktopPrivate::setTask(HolonAbstractTask *task)
{
    if (!task || task == d_ptr->currentTask)
        return;

    if (d_ptr->currentTask)
        d_ptr->currentTask->d_ptr->setCurrent(false);

    d_ptr->setCurrentTask(task);

    task->d_ptr->setCurrent(true);
}

void HolonDesktopPrivate::setWindow(HolonAbstractWindow *window)
{
    if (window == d_ptr->currentWindow)
        return;

    if (d_ptr->currentWindow && d_ptr->currentTaskWindow.value(d_ptr->currentTask) == d_ptr->currentWindow)
        d_ptr->currentWindow->d_ptr->setCurrent(false);

    d_ptr->setCurrentWindow(window);

    if (window)
        window->d_ptr->setCurrent(true);
}

void HolonDesktopPrivate::setWindowArea(HolonWindowArea *windowArea)
{
    if (windowArea == d_ptr->currentWindowArea)
        return;

    if (d_ptr->currentWindowArea)
        d_ptr->currentWindowArea->d_ptr->setChecked(false);

    d_ptr->setCurrentWindowArea(windowArea);

    if (windowArea)
        windowArea->d_ptr->setChecked(true);
}

void HolonDesktopPrivate::setTheme(HolonTheme *theme)
{
    d_ptr->setTheme(theme);
    theme->d_ptr->desktop_d = this;

    if (theme == d_ptr->theme)
    {
        theme->d_ptr->style->d_ptr->desktop_d = this;
        QApplication::setStyle(theme->d_ptr->style);
    }
}

HolonAbstractTask *HolonDesktopPrivate::task() const
{
    return d_ptr->currentTask;
}

HolonTaskThread *HolonDesktopPrivate::taskThread() const
{
    return d_ptr->taskThread;
}

HolonAbstractTaskWindow *HolonDesktopPrivate::taskWindow() const
{
    return qobject_cast<HolonAbstractTaskWindow *>(d_ptr->currentTaskWindow.value(d_ptr->currentTask));
}

HolonTheme *HolonDesktopPrivate::theme() const
{
    if (!d_ptr->theme)
    {
        d_ptr->theme = new HolonTheme(q_ptr->tree()->settings(q_ptr), q_ptr);
        q_ptr->emitWarning(u"create default theme"_s);
    }

    return d_ptr->theme;
}

HolonDesktopPrivate::~HolonDesktopPrivate()
{ }

void HolonDesktopPrivate::cacheMainWindowState(HolonWindowArea *windowArea)
{
    windowArea->d_ptr->cacheMainWindowState();
}

HolonTaskbar *HolonDesktopPrivate::taskbar() const
{
    return d_ptr->taskbar;
}

HolonDesktopPrivate::TaskbarArea HolonDesktopPrivate::taskbarArea() const
{
    return d_ptr->taskbarArea;
}

HolonWorkflow *HolonDesktopPrivate::workflow() const
{
    return d_ptr->currentWorkflow;
}

void HolonDesktopPrivate::removeSidebar(HolonSidebar *sidebar)
{
    HolonSidebarDockWidget *dock = d_ptr->sidebars.dockBySidebar.value(sidebar);
    dock->setUpdatesEnabled(false);

    if (sidebar == dock->d_ptr->currentSidebar())
        d_ptr->sidebars.mainWindowByDock.value(dock)->removeDockWidget(dock);

    sidebar->d_ptr->setChecked(false);
}

void HolonDesktopPrivate::restoreSidebar(HolonSidebar *sidebar)
{
    HolonSidebarDockWidget *dock = d_ptr->sidebars.dockBySidebar.value(sidebar);
    dock->setUpdatesEnabled(true);
    dock->d_ptr->setSidebar(sidebar);
    d_ptr->sidebars.mainWindowByDock.value(dock)->restoreDockWidget(dock);
    sidebar->d_ptr->setChecked(true);
}

void HolonDesktopPrivate::restoreMainWindowStateFromCache(HolonWindowArea *windowArea)
{
    windowArea->d_ptr->restoreMainWindowStateFromCache();
}

void HolonDesktopPrivate::saveSidebarMainWindowState(HolonSidebarMainWindow *sidebarMainWindow)
{
    HolonSidebar *firstSidebar = sidebarMainWindow->sidebars().constFirst();
    firstSidebar->d_func()->saveSidebarMainWindowNestingState(sidebarMainWindow->saveState());
}

HolonSidebarDockWidget *HolonDesktopPrivate::sidebarDockWidget(HolonSidebar *sidebar) const
{
    return d_ptr->sidebars.dockBySidebar.value(sidebar);
}
