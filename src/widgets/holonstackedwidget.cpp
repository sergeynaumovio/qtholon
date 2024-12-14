// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonstackedwidget.h"
#include "holonabstracttask.h"
#include "holonabstractwindow.h"
#include <QHash>

class HolonStackedWidgetPrivate
{
    QMetaType stackedWindowType{};

protected:
    HolonStackedWidget *const q_ptr;

public:
    HolonStackedWidgetPrivate(HolonStackedWidget *q, QMetaType windowType)
    :   stackedWindowType(windowType),
        q_ptr(q)
    { }

    virtual ~HolonStackedWidgetPrivate() { }

    QMetaType windowType() const { return stackedWindowType; }
};

class HolonTaskStackedWidgetPrivate : public HolonStackedWidgetPrivate
{
    QHash<HolonAbstractTask *, QWidget *> taskWidget;

public:
    HolonTaskStackedWidgetPrivate(HolonTaskStackedWidget *q, QMetaType windowType)
    :   HolonStackedWidgetPrivate(q, windowType)
    { }

    void addTaskWidget(HolonAbstractTask *task, QWidget *widget)
    {
        taskWidget.insert(task, widget);
        q_ptr->addWidget(widget);

        if (task->isCurrent())
            q_ptr->setCurrentWidget(taskWidget.value(task));
    }

    void setCurrentTask(HolonAbstractTask *task)
    {
        if (taskWidget.contains(task))
            q_ptr->setCurrentWidget(taskWidget.value(task));
    }

    QWidget *widget(HolonAbstractTask *task) const
    {
        return taskWidget.value(task);
    }
};

class HolonWindowStackedWidgetPrivate : public HolonStackedWidgetPrivate
{
    QHash<HolonAbstractWindow *, QWidget *> windowWidget;

public:
    HolonWindowStackedWidgetPrivate(HolonWindowStackedWidget *q, QMetaType windowType)
    :   HolonStackedWidgetPrivate(q, windowType)
    { }

    void addWindowWidget(HolonAbstractWindow *window, QWidget *widget)
    {
        windowWidget.insert(window, widget);
        q_ptr->addWidget(widget);

        if (window->isCurrent())
            q_ptr->setCurrentWidget(windowWidget.value(window));
    }

    void removeWindowWidget(HolonAbstractWindow *window)
    {
        if (QWidget *widget = windowWidget.value(window))
        {
            windowWidget.remove(window);
            q_ptr->removeWidget(widget);
            widget->deleteLater();
        }
    }

    void setCurrentWindow(HolonAbstractWindow *window)
    {
        if (windowWidget.contains(window))
            q_ptr->setCurrentWidget(windowWidget.value(window));
    }
};

class HolonWindowAreaStackedWidgetPrivate : public HolonStackedWidgetPrivate
{
    QHash<HolonWindowArea *, QWidget *> windowAreaWidget;

public:
    HolonWindowAreaStackedWidgetPrivate(HolonWindowAreaStackedWidget *q, QMetaType windowType)
    :   HolonStackedWidgetPrivate(q, windowType)
    { }

    void addWindowAreaWidget(HolonWindowArea *windowArea, QWidget *widget)
    {
        windowAreaWidget.insert(windowArea, widget);
        q_ptr->addWidget(widget);
    }

    void setCurrentWindowArea(HolonWindowArea *windowArea)
    {
        if (windowAreaWidget.contains(windowArea))
            q_ptr->setCurrentWidget(windowAreaWidget.value(windowArea));
    }
};


HolonStackedWidget::HolonStackedWidget(HolonStackedWidgetPrivate &d)
:   QStackedWidget(),
    d_ptr(&d)
{ }

HolonStackedWidget::~HolonStackedWidget()
{ }

QMetaType HolonStackedWidget::windowType() const
{
    return d_ptr->windowType();
}


HolonTaskStackedWidget::HolonTaskStackedWidget(QMetaType windowType)
:   HolonStackedWidget(*new HolonTaskStackedWidgetPrivate(this, windowType))
{ }

HolonTaskStackedWidget::~HolonTaskStackedWidget()
{ }

void HolonTaskStackedWidget::addTaskWidget(HolonAbstractTask *task, QWidget *widget)
{
    Q_D(HolonTaskStackedWidget);
    d->addTaskWidget(task, widget);
}

void HolonTaskStackedWidget::setCurrentTask(HolonAbstractTask *task)
{
    Q_D(HolonTaskStackedWidget);
    d->setCurrentTask(task);
}

QWidget *HolonTaskStackedWidget::taskWidget(HolonAbstractTask *task) const
{
    Q_D(const HolonTaskStackedWidget);
    return d->widget(task);
}


HolonWindowStackedWidget::HolonWindowStackedWidget(QMetaType windowType)
:   HolonStackedWidget(*new HolonWindowStackedWidgetPrivate(this, windowType))
{ }

HolonWindowStackedWidget::~HolonWindowStackedWidget()
{ }

void HolonWindowStackedWidget::addWindowWidget(HolonAbstractWindow *window, QWidget *widget)
{
    Q_D(HolonWindowStackedWidget);
    d->addWindowWidget(window, widget);
}

void HolonWindowStackedWidget::removeWindowWidget(HolonAbstractWindow *window)
{
    Q_D(HolonWindowStackedWidget);
    d->removeWindowWidget(window);
}

void HolonWindowStackedWidget::setCurrentWindow(HolonAbstractWindow *window)
{
    Q_D(HolonWindowStackedWidget);
    d->setCurrentWindow(window);
}


HolonWindowAreaStackedWidget::HolonWindowAreaStackedWidget(QMetaType windowType)
:   HolonStackedWidget(*new HolonWindowAreaStackedWidgetPrivate(this, windowType))
{ }

HolonWindowAreaStackedWidget::~HolonWindowAreaStackedWidget()
{ }

void HolonWindowAreaStackedWidget::addWindowAreaWidget(HolonWindowArea *windowArea, QWidget *widget)
{
    Q_D(HolonWindowAreaStackedWidget);
    d->addWindowAreaWidget(windowArea, widget);
}

void HolonWindowAreaStackedWidget::setCurrentWindowArea(HolonWindowArea *windowArea)
{
    Q_D(HolonWindowAreaStackedWidget);
    d->setCurrentWindowArea(windowArea);
}
