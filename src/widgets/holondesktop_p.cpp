// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondesktop_p.h"
#include "holonabstracttask.h"
#include "holonabstracttask_p.h"
#include "holonabstractwindow.h"
#include "holonabstractwindow_p.h"
#include "holoncore_p.h"
#include "holondesktop.h"
#include "holonmainwindow.h"
#include "holonopentasktreemodel.h"
#include "holonopentasktreeview.h"
#include "holonprojecttasktreewindow.h"
#include "holonprojecttasktreewindow_p.h"
#include "holonsidebar.h"
#include "holonsidebar_p.h"
#include "holonsidebardockwidget.h"
#include "holonsidebardockwidget_p.h"
#include "holonsidebarmainwindow.h"
#include "holonstackedwidget.h"
#include "holontaskbar.h"
#include "holontheme.h"
#include "holontheme_p.h"
#include "holonthemestyle.h"
#include "holonthemestyle_p.h"
#include "holonwindowareaswitch.h"
#include "holonworkflow.h"
#include <QApplication>
#include <QLayout>
#include <QLoaderTree>
#include <QRegularExpression>
#include <QShortcut>
#include <QStackedWidget>
#include <QStringList>

using namespace Qt::Literals::StringLiterals;
using MainWindowNestingIndex = int;

class HolonDesktopPrivateData : public HolonCorePrivate
{
    void addSidebarWindow(HolonAbstractWindow *window, HolonSidebar *sidebar);
    void addTaskStackedWidget(HolonTaskStackedWidget *taskStackedWidget, int role);
    void addTaskWindow(HolonAbstractTask *task, HolonAbstractWindow *window);
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
    QList<HolonAbstractWindow *> windowList;

    struct
    {
        QMap<HolonAbstractWindow *, HolonSidebar *> byWindow;
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
    HolonTheme *theme{};

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

void HolonDesktopPrivateData::addSidebarWindow(HolonAbstractWindow *window, HolonSidebar *sidebar)
{
    if (sidebars.byWindow.contains(window))
        return;

    if (HolonProjectTaskTreeWindow *projectTaskTreeWindow = qobject_cast<HolonProjectTaskTreeWindow *>(window))
        projectTaskTreeWindow->d_ptr->setOpenTaskTreeModel(openTaskTreeModel);

    QWidget *sidebarWidget = window->widget();

    if (HolonOpenTaskTreeView *openTaskTreeView = qobject_cast<HolonOpenTaskTreeView *>(sidebarWidget))
        openTaskTreeView->setModel(openTaskTreeModel);

    HolonStackedWidget *stackedWidget = qobject_cast<HolonStackedWidget *>(sidebarWidget);
    if (!sidebarWidget || taskStackedWidgetList.contains(stackedWidget))
        return;

    sidebars.byWindow.insert(window, sidebar);

    if (HolonTaskStackedWidget *taskStackedWidget = qobject_cast<HolonTaskStackedWidget *>(stackedWidget))
        return addTaskStackedWidget(taskStackedWidget, window->role());

    if (HolonWindowAreaStackedWidget *windowAreaStackedWidget = qobject_cast<HolonWindowAreaStackedWidget *>(stackedWidget))
        return addWindowAreaStackedWidget(windowAreaStackedWidget);
}

void HolonDesktopPrivateData::addTaskStackedWidget(HolonTaskStackedWidget *taskStackedWidget, int role)
{
    taskStackedWidgetList.append(taskStackedWidget);

    for (HolonAbstractTask *task : taskList)
    {
        if (QWidget *widget = task->widget(role))
        {
            taskStackedWidget->addTaskWidget(task, widget);

            if (task->isCurrent())
                taskStackedWidget->setCurrentTask(task);
        }
    }
}

void HolonDesktopPrivateData::addTaskWindow(HolonAbstractTask *task, HolonAbstractWindow *window)
{
    if (stackedWidgetByTaskWindow.contains(window))
        return;

    HolonWindowArea *windowArea = [task, this]()
    {
        HolonWindowArea *wa = mainWindow->windowArea(task);
        if (!wa)
            wa = mainWindow->addWindowArea(task);

        return wa;
    }();

    if (!windowArea)
        return;

    windowArea->addWindow(window);
    windowAreaByTaskWindow.insert(window, windowArea);

    if (window->isCurrent())
    {
        currentTaskWindow[task] = window;

        if (task->isCurrent())
        {
            currentWindow = window;
            window->widget()->setFocus();
        }
    }

    for (HolonTaskStackedWidget *taskStackedWidget : taskStackedWidgetList)
    {
        if (taskStackedWidget->role() != window->role())
            continue;

        QWidget *widget = taskStackedWidget->taskWidget(task);
        HolonWindowStackedWidget *windowStackedWidget = qobject_cast<HolonWindowStackedWidget *>(widget);

        if (widget && !windowStackedWidget)
            continue;

        if (!widget)
        {
            windowStackedWidget = new HolonWindowStackedWidget;
            taskStackedWidget->addTaskWidget(task, windowStackedWidget);
        }

        if (!(widget = window->widget(taskStackedWidget->role())))
            continue;

        windowStackedWidget->addWindowWidget(window, widget);
        stackedWidgetByTaskWindow.insert(window, windowStackedWidget);

        if (window->isCurrent())
            currentTaskWindow[task] = window;
    }
}

void HolonDesktopPrivateData::addWindowAreaStackedWidget(HolonWindowAreaStackedWidget *windowAreaStackedWidget)
{
    windowAreaStackedWidgetList.append(windowAreaStackedWidget);

    HolonTaskStackedWidget *taskStackedWidget = new HolonTaskStackedWidget(windowAreaStackedWidget->role());
    windowAreaStackedWidget->addWindowAreaWidget(nullptr, taskStackedWidget);
    taskStackedWidgetList.append(taskStackedWidget);

    for (HolonAbstractTask *task : taskList)
    {
        QList<HolonAbstractWindow *> taskWindowList = task->windows(taskStackedWidget->role());
        if (taskWindowList.size())
        {
            HolonWindowStackedWidget *windowStackedWidget = new HolonWindowStackedWidget;
            taskStackedWidget->addTaskWidget(task, windowStackedWidget);

            if (task->isCurrent())
                taskStackedWidget->setCurrentTask(task);

            for (HolonAbstractWindow *taskWindow : taskWindowList)
                if (QWidget *widget = taskWindow->widget(taskStackedWidget->role()))
                {
                    windowStackedWidget->addWindowWidget(taskWindow, widget);
                    stackedWidgetByTaskWindow.insert(taskWindow, windowStackedWidget);
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
    }())
{
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
        else if (qobject_cast<HolonTheme *>(object))
            desktop_d.emitWarning(u"current theme already set"_s);
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

    for (HolonTaskStackedWidget *taskStackedWidget : taskStackedWidgetList)
    {
        if (QWidget *widget = task->widget(taskStackedWidget->role()))
            taskStackedWidget->addTaskWidget(task, widget);
    }

    if (task->isCurrent())
        currentTask = task;
}

void HolonDesktopPrivateData::addWidget(QWidget *widget, QWidget *parent)
{
    parent->layout()->addWidget(widget);
}

void HolonDesktopPrivateData::addWindow(HolonAbstractWindow *window)
{
    if (window->parent() == q_ptr)
    {
        if (windowList.contains(window))
            return;

        windowList.append(window);

        return;
    }

    if (HolonAbstractTask *task = qobject_cast<HolonAbstractTask *>(window->parent()))
    {
        if (window->role() == Holon::NoRole)
            return desktop_d.emitWarning(u"window role is not set"_s);

        return addTaskWindow(task, window);
    }

    if (HolonSidebar *sidebar = qobject_cast<HolonSidebar *>(window->parent()))
        return addSidebarWindow(window, sidebar);
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
    if (!openTaskTreeModel)
        openTaskTreeModel = new HolonOpenTaskTreeModel(q_ptr);

    addObject(workflow, workflowList, currentWorkflow);
}

void HolonDesktopPrivateData::closeTask(HolonAbstractTask *task)
{
    Q_UNUSED(task)
}

void HolonDesktopPrivateData::closeWindow(HolonAbstractWindow *window)
{
    if (HolonSidebar *sidebar = sidebars.byWindow.value(window))
    {
        sidebar->closeWindow(window);
        sidebars.byWindow.remove(window);

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
    for (HolonTaskStackedWidget *taskStackedWidget : taskStackedWidgetList)
    {
        taskStackedWidget->setCurrentTask(task);

        if (auto *windowStackedWidget = qobject_cast<HolonWindowStackedWidget *>(taskStackedWidget->currentWidget()))
            windowStackedWidget->setCurrentWindow(currentTaskWindow.value(task));
    }

    mainWindow->setCurrentTask(task);
    currentTask = task;

    if (HolonAbstractWindow *window = currentTaskWindow.value(task))
        if (QWidget *widget = window->widget())
            widget->setFocus();
}

void HolonDesktopPrivateData::setCurrentWindow(HolonAbstractWindow *window)
{
    for (HolonTaskStackedWidget *taskStackedWidget : taskStackedWidgetList)
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

void HolonDesktopPrivateData::setTheme(HolonTheme *theme)
{
    if (!this->theme)
        this->theme = theme;
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
    if (q->contains(u"saveShortcut"_s) == QLoaderSettings::Value)
    {
        QShortcut *shortcut = new QShortcut(q);
        {
            shortcut->setKey(QKeySequence(q->value(u"saveShortcut"_s).toString()));
            QObject::connect(shortcut, &QShortcut::activated, q, [q] { q->tree()->save(); });
        }
    }
}

void HolonDesktopPrivate::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->addSidebar(sidebar);
    sidebar->d_func()->desktop_d = this;
}

void HolonDesktopPrivate::addTask(HolonAbstractTask *task)
{
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

HolonAbstractTask *HolonDesktopPrivate::currentTask() const
{
    return d_ptr->currentTask;
}

void HolonDesktopPrivate::emitWarning(const QString &warning) const
{
    q_ptr->emitWarning(warning);
}

void HolonDesktopPrivate::setCurrentTask(HolonAbstractTask *task)
{
    if (task == d_ptr->currentTask)
        return;

    if (d_ptr->currentTask)
        d_ptr->currentTask->d_ptr->setCurrent(false);

    d_ptr->setCurrentTask(task);

    if (task)
        task->d_ptr->setCurrent(true);
}

void HolonDesktopPrivate::setCurrentWindow(HolonAbstractWindow *window)
{
    if (window == d_ptr->currentWindow)
        return;

    if (d_ptr->currentWindow && d_ptr->currentTaskWindow.value(d_ptr->currentTask) == d_ptr->currentWindow)
        d_ptr->currentWindow->d_ptr->setCurrent(false);

    d_ptr->setCurrentWindow(window);

    if (window)
        window->d_ptr->setCurrent(true);
}

void HolonDesktopPrivate::setCurrentWindowArea(HolonWindowArea *windowArea)
{
    if (windowArea == d_ptr->currentWindowArea)
        return;

    if (d_ptr->currentWindowArea)
        d_ptr->currentWindowArea->d_ptr->setChecked(false);

    d_ptr->setCurrentWindowArea(windowArea);

    if (windowArea)
        windowArea->d_ptr->setChecked(true);
}

void HolonDesktopPrivate::setLayout()
{
    d_ptr->setLayout();
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

QList<HolonAbstractWindow *> HolonDesktopPrivate::windows() const
{
    return d_ptr->windowList;
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
    firstSidebar->d_func()->saveSidebarMainWindowState(sidebarMainWindow->saveState());
}

void HolonDesktopPrivate::saveSidebarState(HolonAbstractWindow *firstSidebarWindow)
{
    HolonSidebar *sidebar = d_ptr->sidebars.byWindow.value(firstSidebarWindow);
    firstSidebarWindow->d_ptr->saveSidebarState(sidebar->d_func()->mainWindow->saveState());
}

HolonSidebarDockWidget *HolonDesktopPrivate::sidebarDockWidget(HolonSidebar *sidebar) const
{
    return d_ptr->sidebars.dockBySidebar.value(sidebar);
}
