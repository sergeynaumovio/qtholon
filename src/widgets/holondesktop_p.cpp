// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondesktop_p.h"
#include "holonabstracttask.h"
#include "holonabstracttask_p.h"
#include "holonabstractwindow.h"
#include "holonabstractwindow_p.h"
#include "holoncore_p.h"
#include "holondesktop.h"
#include "holonmainwindow.h"
#include "holonsidebar.h"
#include "holonsidebardock.h"
#include "holonsidebardock_p.h"
#include "holonstackedwidget.h"
#include "holontaskbar.h"
#include "holontaskmodel.h"
#include "holonwindowarea_p.h"
#include "holonwindowareaswitch.h"
#include "holonworkflowmodel.h"
#include <QLayout>
#include <QLoaderTree>
#include <QShortcut>
#include <QStackedWidget>
#include <QStringList>
#include <QRegularExpression>

class HolonDesktopPrivateData : public HolonCorePrivate
{
    void addSidebarWindow(HolonAbstractWindow *window);
    void addTaskStackedWidget(HolonTaskStackedWidget *taskStackedWidget, const QString &group);
    void addTaskWindow(HolonAbstractTask *task, HolonAbstractWindow *window);
    void addWindowAreaStackedWidget(HolonWindowAreaStackedWidget *windowAreaStackedWidget);

public:
    HolonDesktopPrivate &desktop_d;
    HolonDesktop *const q_ptr;
    const QString buttonStyleSheet;
    const QString menuStyleSheet;
    const QRegularExpression borderWidth{"^QWidget\\s*{[^}]*border:[^};]*(?<px>\\d+)px[^}]*}$"};
    const int menuBorderWidth;
    const int menuWidth;
    const QString sidebarMoveShortcut;
    const int sidebarSwitchButtonWidth;
    const HolonDesktopPrivate::TaskbarArea taskbarArea;
    const int taskbarPreferedHeight;
    const int taskbarPreferedWidth;
    const QString taskbarStyleSheet;
    const int titleBarHeight;
    const QString titleBarStyleSheet;

    QWidget *screen;
    QWidget *top;
    QWidget *middle;
    QWidget *bottom;
    QWidget *left;
    QWidget *right;

    HolonMainWindow *externalMainWindow;
    HolonMainWindow *internalMainWindow;
    int skipExternalMainWindowSaveState{2};
    HolonTaskbar *taskbar;

    QList<HolonAbstractTask *> taskList;
    QList<HolonAbstractWindow *> windowList;

    struct
    {
        QMap<HolonSidebarDock *, HolonMainWindow *> mainWindowByDock;
        QSet<HolonSidebarDock *> docks;
        QMap<HolonSidebar *, HolonSidebarDock *> dockBySidebar;
        QMap<HolonSidebarDock *, int> dockWidth;

        struct
        {
            QList<QDockWidget *> dockList;
            QList<int> widthList;

        } visible;

    } sidebars;

    QList<HolonAbstractWindow *> sidebarWindowList;
    QList<HolonTaskStackedWidget *> taskStackedWidgetList;
    QList<HolonWindowStackedWidget *> windowStackedWidgetList;
    QList<HolonWindowAreaStackedWidget *> windowAreaStackedWidgetList;

    QHash<HolonAbstractWindow *, HolonWindowArea *> windowAreaByTaskWindow;
    QHash<HolonAbstractWindow *, HolonWindowStackedWidget *> stackedWidgetByTaskWindow;

    HolonAbstractTask *currentTask{};
    HolonAbstractWindow *currentWindow{};
    HolonWindowArea *currentWindowArea{};

    HolonDesktopPrivateData(HolonDesktopPrivate &d, HolonDesktop *q);
    ~HolonDesktopPrivateData() { }

    void addSidebar(HolonSidebar *sidebar);
    void addTask(HolonAbstractTask *task);
    void addWidget(QWidget *widget, QWidget *parent);
    void addWindow(HolonAbstractWindow *window);
    void addWindowArea(HolonWindowArea *windowArea);
    void closeWindow(HolonAbstractWindow *window);
    void removeUncheckedDocks(HolonMainWindow *mainWindow);
    void setCurrentTask(HolonAbstractTask *task);
    void setCurrentWindow(HolonAbstractWindow *window);
    void setCurrentWindowArea(HolonWindowArea *windowArea);
    void setHBoxLayout(QWidget *&widget, const char *name, QWidget *parent);
    void setLayout();
    void setMainWindow(HolonMainWindow *&widget, const char *name, QWidget *parent);
    void setTaskbar();
    void setVBoxLayout();
    void setVBoxLayout(QWidget *&widget, const char *name, QWidget *parent);
};

void HolonDesktopPrivateData::addSidebarWindow(HolonAbstractWindow *window)
{
    if (sidebarWindowList.contains(window))
        return;

    QWidget *sidebarWidget = window->widget();
    HolonStackedWidget *stackedWidget = qobject_cast<HolonStackedWidget *>(sidebarWidget);
    if (!stackedWidget || taskStackedWidgetList.contains(stackedWidget))
        return;

    sidebarWindowList.append(window);

    if (HolonTaskStackedWidget *taskStackedWidget = qobject_cast<HolonTaskStackedWidget *>(stackedWidget))
        return addTaskStackedWidget(taskStackedWidget, window->group());

    if (HolonWindowAreaStackedWidget *windowAreaStackedWidget = qobject_cast<HolonWindowAreaStackedWidget *>(stackedWidget))
        return addWindowAreaStackedWidget(windowAreaStackedWidget);
}

void HolonDesktopPrivateData::addTaskStackedWidget(HolonTaskStackedWidget *taskStackedWidget,
                                                   const QString &group)
{
    taskStackedWidgetList.append(taskStackedWidget);

    for (HolonAbstractTask *task : taskList)
    {
        if (QWidget *widget = task->widget(group))
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
        HolonWindowArea *wa = internalMainWindow->windowArea(task);
        if (!wa)
            wa = internalMainWindow->addWindowArea(task);

        return wa;
    }();

    if (windowArea)
    {
        windowArea->addWindow(window);
        windowAreaByTaskWindow.insert(window, windowArea);
    }
    else
        return;

    if (task->isCurrent() && window->isCurrent())
        currentWindow = window;

    for (HolonTaskStackedWidget *taskStackedWidget : taskStackedWidgetList)
    {
        if (taskStackedWidget->group() == window->group())
        {
            QWidget *widget = taskStackedWidget->taskWidget(task);
            HolonWindowStackedWidget *windowStackedWidget = qobject_cast<HolonWindowStackedWidget *>(widget);

            if (widget && !windowStackedWidget)
                continue;

            if (!widget)
            {
                windowStackedWidget = new HolonWindowStackedWidget;
                taskStackedWidget->addTaskWidget(task, windowStackedWidget);
            }

            if ((widget = window->widget(taskStackedWidget->group())))
            {
                windowStackedWidget->addWindowWidget(window, widget);
                stackedWidgetByTaskWindow.insert(window, windowStackedWidget);
            }
        }
    }
}

void HolonDesktopPrivateData::addWindowAreaStackedWidget(HolonWindowAreaStackedWidget *windowAreaStackedWidget)
{
    windowAreaStackedWidgetList.append(windowAreaStackedWidget);

    HolonTaskStackedWidget *taskStackedWidget = new HolonTaskStackedWidget(windowAreaStackedWidget->group());
    windowAreaStackedWidget->addWindowAreaWidget(nullptr, taskStackedWidget);
    taskStackedWidgetList.append(taskStackedWidget);

    for (HolonAbstractTask *task : taskList)
    {
        QList<HolonAbstractWindow *> taskWindowList = task->windows(taskStackedWidget->group());
        if (taskWindowList.size())
        {
            HolonWindowStackedWidget *windowStackedWidget = new HolonWindowStackedWidget;
            taskStackedWidget->addTaskWidget(task, windowStackedWidget);

            if (task->isCurrent())
                taskStackedWidget->setCurrentTask(task);

            for (HolonAbstractWindow *taskWindow : taskWindowList)
                if (QWidget *widget = taskWindow->widget(taskStackedWidget->group()))
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
    buttonStyleSheet(q_ptr->value("buttonStyleSheet").toString()),
    menuStyleSheet(q_ptr->value("menuStyleSheet").toString()),
    menuBorderWidth([this]()
    {
        QRegularExpressionMatch match = borderWidth.match(menuStyleSheet);
        if (match.hasMatch())
            return match.captured("px").toInt();

        return 1;
    }()),
    menuWidth(q_ptr->value("menuWidth", 200).toInt()),
    sidebarMoveShortcut(q_ptr->value("sidebarMoveShortcut").toString()),
    sidebarSwitchButtonWidth(q_ptr->value("sidebarSwitchButtonWidth", 80).toInt()),
    taskbarArea([this]()
    {
        QString string = q_ptr->value("taskbarArea").toString();
        if (string == "left")
            return HolonDesktopPrivate::TaskbarArea::Left;

        if (string == "right")
            return HolonDesktopPrivate::TaskbarArea::Right;

        if (string == "top")
            return HolonDesktopPrivate::TaskbarArea::Top;

        return HolonDesktopPrivate::TaskbarArea::Bottom;
    }()),
    taskbarPreferedHeight(q_ptr->value("taskbarPreferedHeight", 40).toInt()),
    taskbarPreferedWidth(q_ptr->value("taskbarPreferedWidth", 50).toInt()),
    taskbarStyleSheet(q_ptr->value("taskbarStyleSheet").toString()),
    titleBarHeight(q_ptr->value("titleBarHeight", 10).toInt()),
    titleBarStyleSheet(q_ptr ->value("titleBarStyleSheet").toString())
{ }

void HolonDesktopPrivateData::addSidebar(HolonSidebar *sidebar)
{
    HolonSidebarDock *sidebarDock;

    if (sidebar->value("layout", "external").toString() == "external")
    {
        sidebarDock = externalMainWindow->addSidebar(sidebar);
        externalMainWindow->restoreState(q_ptr->value("externalMainWindowState").toByteArray());
        sidebars.mainWindowByDock.insert(sidebarDock, externalMainWindow);
        removeUncheckedDocks(externalMainWindow);
    }
    else
    {
        sidebarDock = internalMainWindow->addSidebar(sidebar);
        internalMainWindow->restoreState(q_ptr->value("internalMainWindowState").toByteArray());
        sidebars.mainWindowByDock.insert(sidebarDock, internalMainWindow);
        removeUncheckedDocks(internalMainWindow);
    }

    sidebars.docks.insert(sidebarDock);

    sidebars.dockBySidebar.insert(sidebar, sidebarDock);

    sidebars.visible.dockList.reserve(sidebars.dockWidth.size());
    sidebars.visible.widthList.reserve(sidebars.dockWidth.size());

    taskbar->sidebarSwitch()->addSidebar(sidebar);
}

void HolonDesktopPrivateData::addTask(HolonAbstractTask *task)
{
    if (taskList.contains(task))
        return;

    taskList.append(task);

    for (HolonTaskStackedWidget *taskStackedWidget : taskStackedWidgetList)
    {
        if (QWidget *widget = task->widget(taskStackedWidget->group()))
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
        return addTaskWindow(task, window);

    if (qobject_cast<HolonSidebar *>(window->parent()))
        return addSidebarWindow(window);
}

void HolonDesktopPrivateData::addWindowArea(HolonWindowArea *windowArea)
{
    internalMainWindow->addWindowArea(windowArea);
    taskbar->sidebarSwitch()->addWindowArea(windowArea);

    if (windowArea->isChecked())
    {
        internalMainWindow->setCurrentWindowArea(windowArea);
        currentWindowArea = windowArea;
    }
}

void HolonDesktopPrivateData::closeWindow(HolonAbstractWindow *window)
{
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
}

void HolonDesktopPrivateData::removeUncheckedDocks(HolonMainWindow *mainWindow)
{
    QMapIterator it(sidebars.mainWindowByDock);
    while(it.hasNext())
    {
        it.next();
        if (it.value() == mainWindow)
        {
            HolonSidebarDock *dock = it.key();
            QList<HolonSidebar *> sidebarList = dock->sidebars();
            int i{};

            for (; i < sidebarList.size(); ++i)
                if (sidebarList[i]->isChecked())
                    break;

            if (i == sidebarList.size())
                mainWindow->removeDockWidget(it.key());
        }
    }
}

void HolonDesktopPrivateData::setCurrentTask(HolonAbstractTask *task)
{
    for (HolonTaskStackedWidget *taskStackedWidget : taskStackedWidgetList)
        taskStackedWidget->setCurrentTask(task);

    internalMainWindow->setCurrentTask(task);
    currentTask = task;
}

void HolonDesktopPrivateData::setCurrentWindow(HolonAbstractWindow *window)
{
    for (HolonTaskStackedWidget *taskStackedWidget : taskStackedWidgetList)
    {
        if (HolonWindowStackedWidget *windowStackedWidget =
                qobject_cast<HolonWindowStackedWidget *>(taskStackedWidget->currentWidget()))
        {
            windowStackedWidget->setCurrentWindow(window);
        }
    }
    currentWindow = window;
}

void HolonDesktopPrivateData::setCurrentWindowArea(HolonWindowArea *windowArea)
{
    internalMainWindow->setCurrentWindowArea(windowArea);
    currentWindowArea = windowArea;
}

void HolonDesktopPrivateData::setHBoxLayout(QWidget *&widget, const char *name, QWidget *parent)
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
    setVBoxLayout(screen, "Screen", q_ptr);
    {
        setVBoxLayout(top, "ScreenTop", screen);
        setHBoxLayout(middle, "ScreenMiddle", screen);
        {
            setHBoxLayout(left, "ScreenLeft", middle);
            setMainWindow(externalMainWindow, "ScreenCenterExternal", middle);
            setMainWindow(internalMainWindow, "ScreenCenterInternal", externalMainWindow);
            setHBoxLayout(right, "ScreenRight", middle);
        }
        setVBoxLayout(bottom, "ScreenBottom", screen);
    }
    setTaskbar();
}

void HolonDesktopPrivateData::setMainWindow(HolonMainWindow *&widget, const char *name, QWidget *parent)
{
    widget = new HolonMainWindow(desktop_d, parent);
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

void HolonDesktopPrivateData::setVBoxLayout()
{
    q_ptr->setLayout(new QVBoxLayout(q_ptr));
    q_ptr->layout()->setContentsMargins({});
    q_ptr->layout()->setSpacing(0);
}

void HolonDesktopPrivateData::setVBoxLayout(QWidget *&widget, const char *name, QWidget *parent)
{
    widget = new QWidget(parent);
    widget->setObjectName(name);
    widget->setLayout(new QVBoxLayout(widget));
    widget->layout()->setContentsMargins({});
    widget->layout()->setSpacing(0);
    parent->layout()->addWidget(widget);
}

HolonDesktopPrivate::HolonDesktopPrivate(HolonDesktop *q)
:   d(*new (&d_storage) HolonDesktopPrivateData(*this, q)),
    q_ptr(q)
{
    static_assert (sizeof (d_storage) == sizeof (HolonDesktopPrivateData));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonDesktopPrivateData));

    if (q->contains("saveShortcut") == QLoaderSettings::Value)
    {
        QShortcut *shortcut = new QShortcut(q);
        {
            shortcut->setKey(QKeySequence(q->value("saveShortcut").toString()));
            QObject::connect(shortcut, &QShortcut::activated, q, [q] { q->tree()->save(); });
        }
    }
}

void HolonDesktopPrivate::addModel(HolonTaskModel *model)
{
    if (d.models.tasks.isEmpty() || model->isCurrent())
        d.models.current.task = model;

    d.models.tasks.append(model);

}

void HolonDesktopPrivate::addModel(HolonWorkflowModel *model)
{
    if (d.models.workflows.isEmpty() || model->isCurrent())
        d.models.current.workflow = model;

    d.models.workflows.append(model);
}

void HolonDesktopPrivate::addSidebar(HolonSidebar *sidebar)
{
    d.addSidebar(sidebar);
}

void HolonDesktopPrivate::addTask(HolonAbstractTask *task)
{
    d.addTask(task);
}

void HolonDesktopPrivate::addWindow(HolonAbstractWindow *window)
{
    d.addWindow(window);
}

void HolonDesktopPrivate::addWindowArea(HolonWindowArea *windowArea)
{
    d.addWindowArea(windowArea);
}

void HolonDesktopPrivate::closeWindow(HolonAbstractWindow *window)
{
    d.closeWindow(window);
}

HolonAbstractTask *HolonDesktopPrivate::currentTask()
{
    return d.currentTask;
}

void HolonDesktopPrivate::resizeEvent(QResizeEvent *)
{
    if (d.skipExternalMainWindowSaveState)
        d.skipExternalMainWindowSaveState--;
}

void HolonDesktopPrivate::saveMainWindowState()
{
    if (!d.skipExternalMainWindowSaveState)
    {
        q_ptr->setValue("externalMainWindowState", d.externalMainWindow->saveState());
        q_ptr->setValue("internalMainWindowState", d.internalMainWindow->saveState());
    }
}

void HolonDesktopPrivate::setCurrentTask(HolonAbstractTask *task)
{
    if (task == d.currentTask)
        return;

    if (d.currentTask)
        d.currentTask->d_ptr->setCurrent(false);

    d.setCurrentTask(task);

    if (task)
        task->d_ptr->setCurrent(true);
}

void HolonDesktopPrivate::setCurrentWindow(HolonAbstractWindow *window)
{
    if (window == d.currentWindow)
        return;

    if (d.currentWindow)
        d.currentWindow->d_ptr->setCurrent(false);

    d.setCurrentWindow(window);

    if (window)
        window->d_ptr->setCurrent(true);
}

void HolonDesktopPrivate::setCurrentWindowArea(HolonWindowArea *windowArea)
{
    if (windowArea == d.currentWindowArea)
        return;

    if (d.currentWindowArea)
        d.currentWindowArea->d_ptr->setChecked(false);

    d.setCurrentWindowArea(windowArea);

    if (windowArea)
        windowArea->d_ptr->setChecked(true);
}

void HolonDesktopPrivate::setLayout()
{
    d.setLayout();
}

HolonTaskModel *HolonDesktopPrivate::taskModel() const
{
    return d.models.current.task;
}

HolonWorkflowModel *HolonDesktopPrivate::workflowModel() const
{
    return d.models.current.workflow;
}

HolonDesktopPrivate::~HolonDesktopPrivate()
{
    d.~HolonDesktopPrivateData();
}

QString HolonDesktopPrivate::buttonStyleSheet() const
{
    return d.buttonStyleSheet;
}

int HolonDesktopPrivate::menuBorderWidth() const
{
    return d.menuBorderWidth;
}

QString HolonDesktopPrivate::menuStyleSheet() const
{
    return d.menuStyleSheet;
}

int HolonDesktopPrivate::menuWidth() const
{
    return d.menuWidth;
}

const QSet<HolonSidebarDock *> &HolonDesktopPrivate::sidebarDocks() const
{
    return d.sidebars.docks;
}

int HolonDesktopPrivate::titleBarHeight() const
{
    return d.titleBarHeight;
}

QString HolonDesktopPrivate::titleBarStyleSheet() const
{
    return d.titleBarStyleSheet;
}

QString HolonDesktopPrivate::sidebarMoveShortcut() const
{
    return d.sidebarMoveShortcut;
}

int HolonDesktopPrivate::sidebarSwitchButtonWidth() const
{
    return d.sidebarSwitchButtonWidth;
}

HolonTaskbar *HolonDesktopPrivate::taskbar() const
{
    return d.taskbar;
}

HolonDesktopPrivate::TaskbarArea HolonDesktopPrivate::taskbarArea() const
{
    return d.taskbarArea;
}

int HolonDesktopPrivate::taskbarPreferedHeight() const
{
    return d.taskbarPreferedHeight;
}

int HolonDesktopPrivate::taskbarPreferedWidth() const
{
    return d.taskbarPreferedWidth;
}

QString HolonDesktopPrivate::taskbarStyleSheet() const
{
    return d.taskbarStyleSheet;
}

QList<HolonAbstractWindow *> HolonDesktopPrivate::windows() const
{
    return d.windowList;
}

void HolonDesktopPrivate::removeSidebar(HolonSidebar *sidebar)
{
    HolonSidebarDock *dock = d.sidebars.dockBySidebar.value(sidebar);

    if (sidebar == dock->d.currentSidebar())
    {
        d.sidebars.mainWindowByDock.value(dock)->removeDockWidget(dock);
        resizeDocks();
    }

    sidebar->d_ptr->setChecked(false);
}

void HolonDesktopPrivate::resizeDocks()
{
    QMapIterator it(d.sidebars.dockWidth);
    while(it.hasNext())
    {
        it.next();
        if (d.sidebars.mainWindowByDock.value(it.key()) == d.externalMainWindow &&
            it.key()->isVisible())
        {
            d.sidebars.visible.dockList.append(it.key());
            d.sidebars.visible.widthList.append(it.value());
        }
    }

    d.externalMainWindow->resizeDocks(d.sidebars.visible.dockList,
                                      d.sidebars.visible.widthList,
                                      Qt::Horizontal);

    d.sidebars.visible.dockList.clear();
    d.sidebars.visible.widthList.clear();

    saveMainWindowState();
}

void HolonDesktopPrivate::restoreSidebar(HolonSidebar *sidebar)
{
    HolonSidebarDock *dock = d.sidebars.dockBySidebar.value(sidebar);
    dock->d.setSidebar(sidebar);
    d.sidebars.mainWindowByDock.value(dock)->restoreDockWidget(dock);
    sidebar->d_ptr->setChecked(true);
    resizeDocks();
}

void HolonDesktopPrivate::saveDockWidgetWidth(HolonSidebarDock *dock, int width)
{
    d.sidebars.dockWidth[dock] = width;

    saveMainWindowState();
}

HolonSidebarDock *HolonDesktopPrivate::sidebarDock(HolonSidebar *sidebar) const
{
    return d.sidebars.dockBySidebar.value(sidebar);
}
