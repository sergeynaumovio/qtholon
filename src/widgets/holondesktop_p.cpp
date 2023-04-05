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
#include "holontaskmodel_p.h"
#include "holontheme.h"
#include "holontheme_p.h"
#include "holonwindowarea_p.h"
#include "holonwindowareaswitch.h"
#include "holonworkflowmodel.h"
#include <QLayout>
#include <QLoaderTree>
#include <QRegularExpression>
#include <QShortcut>
#include <QStackedWidget>
#include <QStringList>

using namespace Qt::Literals::StringLiterals;

class HolonDesktopPrivateData : public HolonCorePrivate
{
    void addSidebarWindow(HolonAbstractWindow *window, HolonSidebar *sidebar);
    void addTaskStackedWidget(HolonTaskStackedWidget *taskStackedWidget, const QString &group);
    void addTaskWindow(HolonAbstractTask *task, HolonAbstractWindow *window);
    void addWindowAreaStackedWidget(HolonWindowAreaStackedWidget *windowAreaStackedWidget);

public:
    HolonDesktopPrivate &desktop_d;
    HolonDesktop *const q_ptr;
    const QString buttonStyleSheet;
    const QString menuStyleSheet;
    const QRegularExpression borderWidth{u"^QWidget\\s*{[^}]*border:[^};]*(?<px>\\d+)px[^}]*}$"_s};
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
    QList<HolonTheme *> themeList;
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

    QMap<HolonAbstractWindow *, HolonSidebar *> sidebarByWindow;
    QList<HolonTaskStackedWidget *> taskStackedWidgetList;
    QList<HolonWindowStackedWidget *> windowStackedWidgetList;
    QList<HolonWindowAreaStackedWidget *> windowAreaStackedWidgetList;

    QHash<HolonAbstractWindow *, HolonWindowArea *> windowAreaByTaskWindow;
    QHash<HolonAbstractWindow *, HolonWindowStackedWidget *> stackedWidgetByTaskWindow;

    HolonAbstractTask *currentTask{};
    HolonTheme *currentTheme{};
    QHash<HolonAbstractTask *, HolonAbstractWindow *> currentTaskWindow;
    HolonAbstractWindow *currentWindow{};
    HolonWindowArea *currentWindowArea{};

    HolonDesktopPrivateData(HolonDesktopPrivate &d, HolonDesktop *q);
    ~HolonDesktopPrivateData() { }

    void addModel(HolonTaskModel *model);
    void addSidebar(HolonSidebar *sidebar);
    void addTask(HolonAbstractTask *task);
    void addTheme(HolonTheme *theme);
    void addWidget(QWidget *widget, QWidget *parent);
    void addWindow(HolonAbstractWindow *window);
    void addWindowArea(HolonWindowArea *windowArea);
    void closeTask(HolonAbstractTask *task);
    void closeWindow(HolonAbstractWindow *window);
    void removeUncheckedDocks(HolonMainWindow *mainWindow);
    void setCurrentTask(HolonAbstractTask *task);
    void setCurrentWindow(HolonAbstractWindow *window);
    void setCurrentWindowArea(HolonWindowArea *windowArea);
    void setHBoxLayout(QWidget *&widget, const QString &name, QWidget *parent);
    void setLayout();
    void setMainWindow(HolonMainWindow *&widget, const QString &name, QWidget *parent);
    void setTaskbar();
    void setVBoxLayout();
    void setVBoxLayout(QWidget *&widget, const QString &name, QWidget *parent);
};

void HolonDesktopPrivateData::addSidebarWindow(HolonAbstractWindow *window, HolonSidebar *sidebar)
{
    if (sidebarByWindow.contains(window))
        return;

    QWidget *sidebarWidget = window->widget();
    HolonStackedWidget *stackedWidget = qobject_cast<HolonStackedWidget *>(sidebarWidget);
    if (!sidebarWidget || taskStackedWidgetList.contains(stackedWidget))
        return;

    sidebarByWindow.insert(window, sidebar);

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
        if (taskStackedWidget->group() != window->group())
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

        if (!(widget = window->widget(taskStackedWidget->group())))
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
    buttonStyleSheet(q_ptr->value(u"buttonStyleSheet"_s).toString()),
    menuStyleSheet(q_ptr->value(u"menuStyleSheet"_s).toString()),
    menuBorderWidth([this]()
    {
        QRegularExpressionMatch match = borderWidth.match(menuStyleSheet);
        if (match.hasMatch())
            return match.captured(u"px"_s).toInt();

        return 1;
    }()),
    menuWidth(q_ptr->value(u"menuWidth"_s, 200).toInt()),
    sidebarMoveShortcut(q_ptr->value(u"sidebarMoveShortcut"_s).toString()),
    sidebarSwitchButtonWidth(q_ptr->value(u"sidebarSwitchButtonWidth"_s, 80).toInt()),
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
    taskbarPreferedHeight(q_ptr->value(u"taskbarPreferedHeight"_s, 40).toInt()),
    taskbarPreferedWidth(q_ptr->value(u"taskbarPreferedWidth"_s, 50).toInt()),
    taskbarStyleSheet(q_ptr->value(u"taskbarStyleSheet"_s).toString()),
    titleBarHeight(q_ptr->value(u"titleBarHeight"_s, 10).toInt()),
    titleBarStyleSheet(q_ptr ->value(u"titleBarStyleSheet"_s).toString())
{ }

void HolonDesktopPrivateData::addModel(HolonTaskModel *model)
{
    if (taskModelList.contains(model))
        return;

    taskModelList.append(model);

    if (model->isCurrent() && !currentTaskModel)
        currentTaskModel = model;
    else
        desktop_d.emitWarning(u"current task model already set"_s);
}

void HolonDesktopPrivateData::addSidebar(HolonSidebar *sidebar)
{
    HolonSidebarDock *sidebarDock;

    if (sidebar->value(u"layout"_s, u"external"_s).toString() == "external"_L1)
    {
        sidebarDock = externalMainWindow->addSidebar(sidebar);
        externalMainWindow->restoreState(q_ptr->value(u"externalMainWindowState"_s).toByteArray());
        sidebars.mainWindowByDock.insert(sidebarDock, externalMainWindow);
        removeUncheckedDocks(externalMainWindow);
    }
    else
    {
        sidebarDock = internalMainWindow->addSidebar(sidebar);
        internalMainWindow->restoreState(q_ptr->value(u"internalMainWindowState"_s).toByteArray());
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

void HolonDesktopPrivateData::addTheme(HolonTheme *theme)
{
    if (themeList.contains(theme))
        return;

    themeList.append(theme);

    if (theme->isCurrent() && !currentTheme)
        currentTheme = theme;
    else
        desktop_d.emitWarning(u"current theme already set"_s);
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
        if (window->group().isEmpty())
            return desktop_d.emitWarning(u"window group is not set"_s);

        return addTaskWindow(task, window);
    }

    if (HolonSidebar *sidebar = qobject_cast<HolonSidebar *>(window->parent()))
        return addSidebarWindow(window, sidebar);
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

void HolonDesktopPrivateData::closeTask(HolonAbstractTask *task)
{
    Q_UNUSED(task)
}

void HolonDesktopPrivateData::closeWindow(HolonAbstractWindow *window)
{
    if (HolonSidebar *sidebar = sidebarByWindow.value(window))
    {
        sidebar->closeWindow(window);
        sidebarByWindow.remove(window);

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
    {
        taskStackedWidget->setCurrentTask(task);

        if (HolonWindowStackedWidget *windowStackedWidget =
                qobject_cast<HolonWindowStackedWidget *>(taskStackedWidget->currentWidget()))
        {
            windowStackedWidget->setCurrentWindow(currentTaskWindow.value(task));
        }
    }

    internalMainWindow->setCurrentTask(task);
    currentTask = task;

    if (HolonAbstractWindow *window = currentTaskWindow.value(task))
        if (QWidget *widget = window->widget())
            widget->setFocus();
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
    currentTaskWindow[currentTask] = window;
}

void HolonDesktopPrivateData::setCurrentWindowArea(HolonWindowArea *windowArea)
{
    internalMainWindow->setCurrentWindowArea(windowArea);
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
            setMainWindow(externalMainWindow, u"ScreenCenterExternal"_s, middle);
            setMainWindow(internalMainWindow, u"ScreenCenterInternal"_s, externalMainWindow);
            setHBoxLayout(right, u"ScreenRight"_s, middle);
        }
        setVBoxLayout(bottom, u"ScreenBottom"_s, screen);
    }
    setTaskbar();
}

void HolonDesktopPrivateData::setMainWindow(HolonMainWindow *&widget, const QString &name, QWidget *parent)
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

void HolonDesktopPrivate::addModel(HolonTaskModel *model)
{
    d_ptr->addModel(model);
}

void HolonDesktopPrivate::addModel(HolonWorkflowModel *model)
{
    if (d_ptr->models.workflows.isEmpty() || model->isCurrent())
        d_ptr->models.current.workflow = model;

    d_ptr->models.workflows.append(model);
}

void HolonDesktopPrivate::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->addSidebar(sidebar);
}

void HolonDesktopPrivate::addTask(HolonAbstractTask *task)
{
    d_ptr->addTask(task);
}

void HolonDesktopPrivate::addTheme(HolonTheme *theme)
{
    d_ptr->addTheme(theme);
}

void HolonDesktopPrivate::addWindow(HolonAbstractWindow *window)
{
    d_ptr->addWindow(window);
}

void HolonDesktopPrivate::addWindowArea(HolonWindowArea *windowArea)
{
    d_ptr->addWindowArea(windowArea);
}

void HolonDesktopPrivate::closeTask(HolonAbstractTask *task)
{
    d_ptr->closeTask(task);
}

void HolonDesktopPrivate::closeWindow(HolonAbstractWindow *window)
{
    d_ptr->closeWindow(window);
}

HolonAbstractTask *HolonDesktopPrivate::currentTask() const
{
    return d_ptr->currentTask;
}

HolonTaskModel *HolonDesktopPrivate::currentTaskModel() const
{
    return d_ptr->currentTaskModel;
}

HolonTheme *HolonDesktopPrivate::currentTheme() const
{
    if (!d_ptr->currentTheme)
    {
        d_ptr->currentTheme = new HolonTheme(q_ptr->tree()->settings(q_ptr), q_ptr);
        q_ptr->emitWarning(u"create default theme"_s);
    }

    return d_ptr->currentTheme;
}

void HolonDesktopPrivate::emitWarning(const QString &warning) const
{
    q_ptr->emitWarning(warning);
}

void HolonDesktopPrivate::resizeEvent(QResizeEvent *)
{
    if (d_ptr->skipExternalMainWindowSaveState)
        d_ptr->skipExternalMainWindowSaveState--;
}

void HolonDesktopPrivate::saveMainWindowState()
{
    if (!d_ptr->skipExternalMainWindowSaveState)
    {
        q_ptr->setValue(u"externalMainWindowState"_s, d_ptr->externalMainWindow->saveState());
        q_ptr->setValue(u"internalMainWindowState"_s, d_ptr->internalMainWindow->saveState());
    }
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

void HolonDesktopPrivate::setCurrentTaskModel(HolonTaskModel *model)
{
    if (model == d_ptr->currentTaskModel)
        return;

    if (d_ptr->currentTaskModel)
    {
        d_ptr->currentTaskModel->d_ptr->setCurrent(false);
        model->d_ptr->setCurrent(true);
        q_ptr->emitWarning(u"task model change will take effect after restart"_s);
    }
}

void HolonDesktopPrivate::setCurrentTheme(HolonTheme *theme)
{
    if (theme == d_ptr->currentTheme)
        return;

    if (d_ptr->currentTheme)
    {
        QLoaderSettings *themeSettings = q_ptr->tree()->settings(d_ptr->currentTheme);
        if (themeSettings != q_ptr)
        {
            d_ptr->currentTheme->d_ptr->setCurrent(false);
            theme->d_ptr->setCurrent(true);
            q_ptr->emitWarning(u"theme change will take effect after restart"_s);
        }
    }
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

HolonWorkflowModel *HolonDesktopPrivate::workflowModel() const
{
    return d_ptr->models.current.workflow;
}

HolonDesktopPrivate::~HolonDesktopPrivate()
{ }

QString HolonDesktopPrivate::buttonStyleSheet() const
{
    return d_ptr->buttonStyleSheet;
}

void HolonDesktopPrivate::hideWindowArea(HolonWindowArea *windowArea)
{
    taskbar()->sidebarSwitch()->hideWindowArea(windowArea);
}

int HolonDesktopPrivate::menuBorderWidth() const
{
    return d_ptr->menuBorderWidth;
}

QString HolonDesktopPrivate::menuStyleSheet() const
{
    return d_ptr->menuStyleSheet;
}

int HolonDesktopPrivate::menuWidth() const
{
    return d_ptr->menuWidth;
}

const QSet<HolonSidebarDock *> &HolonDesktopPrivate::sidebarDocks() const
{
    return d_ptr->sidebars.docks;
}

int HolonDesktopPrivate::titleBarHeight() const
{
    return d_ptr->titleBarHeight;
}

QString HolonDesktopPrivate::titleBarStyleSheet() const
{
    return d_ptr->titleBarStyleSheet;
}

void HolonDesktopPrivate::showWindowArea(HolonWindowArea *windowArea)
{
    taskbar()->sidebarSwitch()->showWindowArea(windowArea);
}

QString HolonDesktopPrivate::sidebarMoveShortcut() const
{
    return d_ptr->sidebarMoveShortcut;
}

int HolonDesktopPrivate::sidebarSwitchButtonWidth() const
{
    return d_ptr->sidebarSwitchButtonWidth;
}

HolonTaskbar *HolonDesktopPrivate::taskbar() const
{
    return d_ptr->taskbar;
}

HolonDesktopPrivate::TaskbarArea HolonDesktopPrivate::taskbarArea() const
{
    return d_ptr->taskbarArea;
}

int HolonDesktopPrivate::taskbarPreferedHeight() const
{
    return d_ptr->taskbarPreferedHeight;
}

int HolonDesktopPrivate::taskbarPreferedWidth() const
{
    return d_ptr->taskbarPreferedWidth;
}

QString HolonDesktopPrivate::taskbarStyleSheet() const
{
    return d_ptr->taskbarStyleSheet;
}

QList<HolonAbstractWindow *> HolonDesktopPrivate::windows() const
{
    return d_ptr->windowList;
}

void HolonDesktopPrivate::removeSidebar(HolonSidebar *sidebar)
{
    HolonSidebarDock *dock = d_ptr->sidebars.dockBySidebar.value(sidebar);

    if (sidebar == dock->d_ptr->currentSidebar())
    {
        d_ptr->sidebars.mainWindowByDock.value(dock)->removeDockWidget(dock);
        resizeDocks();
    }

    sidebar->d_ptr->setChecked(false);
}

void HolonDesktopPrivate::resizeDocks()
{
    QMapIterator it(d_ptr->sidebars.dockWidth);
    while(it.hasNext())
    {
        it.next();
        if (d_ptr->sidebars.mainWindowByDock.value(it.key()) == d_ptr->externalMainWindow &&
            it.key()->isVisible())
        {
            d_ptr->sidebars.visible.dockList.append(it.key());
            d_ptr->sidebars.visible.widthList.append(it.value());
        }
    }

    d_ptr->externalMainWindow->resizeDocks(d_ptr->sidebars.visible.dockList,
                                           d_ptr->sidebars.visible.widthList,
                                           Qt::Horizontal);

    d_ptr->sidebars.visible.dockList.clear();
    d_ptr->sidebars.visible.widthList.clear();

    saveMainWindowState();
}

void HolonDesktopPrivate::restoreSidebar(HolonSidebar *sidebar)
{
    HolonSidebarDock *dock = d_ptr->sidebars.dockBySidebar.value(sidebar);
    dock->d_ptr->setSidebar(sidebar);
    d_ptr->sidebars.mainWindowByDock.value(dock)->restoreDockWidget(dock);
    sidebar->d_ptr->setChecked(true);
    resizeDocks();
}

void HolonDesktopPrivate::saveDockWidgetWidth(HolonSidebarDock *dock, int width)
{
    d_ptr->sidebars.dockWidth[dock] = width;

    saveMainWindowState();
}

HolonSidebarDock *HolonDesktopPrivate::sidebarDock(HolonSidebar *sidebar) const
{
    return d_ptr->sidebars.dockBySidebar.value(sidebar);
}
