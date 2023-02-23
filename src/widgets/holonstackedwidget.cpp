// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonstackedwidget.h"
#include <QHash>

class HolonStackedWidgetPrivate
{
    HolonStackedWidget *const q_ptr;
    const QString widgetGroup;
    QHash<HolonAbstractTask *, QWidget *> taskWidget;

public:
    HolonStackedWidgetPrivate(HolonStackedWidget *q, const QString &group)
    :   q_ptr(q),
        widgetGroup(group)
    { }

    void addTaskWidget(HolonAbstractTask *task, QWidget *widget)
    {
        taskWidget.insert(task, widget);
        q_ptr->addWidget(widget);
    }

    QString group() const
    {
        return widgetGroup;
    }

    void setCurrentTask(HolonAbstractTask *task)
    {
        if (taskWidget.contains(task))
            q_ptr->setCurrentWidget(taskWidget.value(task));
    }
};

HolonStackedWidget::HolonStackedWidget(const QString &group)
:   QStackedWidget(),
    d_ptr(new HolonStackedWidgetPrivate(this, group))
{ }

HolonStackedWidget::~HolonStackedWidget()
{ }

void HolonStackedWidget::addTaskWidget(HolonAbstractTask *task, QWidget *widget)
{
    d_ptr->addTaskWidget(task, widget);
}

QString HolonStackedWidget::group() const
{
    return d_ptr->group();
}

void HolonStackedWidget::setCurrentTask(HolonAbstractTask *task)
{
    d_ptr->setCurrentTask(task);
}
