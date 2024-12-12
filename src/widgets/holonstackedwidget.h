// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSTACKEDWIDGET_H
#define HOLONSTACKEDWIDGET_H

#include "holonnamespace.h"
#include <QStackedWidget>

class HolonAbstractTask;
class HolonAbstractWindow;
class HolonStackedWidgetPrivate;
class HolonTaskStackedWidgetPrivate;
class HolonWindowArea;
class HolonWindowAreaStackedWidgetPrivate;
class HolonWindowStackedWidgetPrivate;

class HolonStackedWidget : public QStackedWidget
{
    Q_OBJECT

protected:
    const QScopedPointer<HolonStackedWidgetPrivate> d_ptr;

    HolonStackedWidget(HolonStackedWidgetPrivate &d);

public:
    ~HolonStackedWidget();

    int role() const;
};


class HolonTaskStackedWidget : public HolonStackedWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonTaskStackedWidget)

public:
    explicit HolonTaskStackedWidget(int role = Holon::NoRole);
    ~HolonTaskStackedWidget();

    void addTaskWidget(HolonAbstractTask *task, QWidget *widget);
    void setCurrentTask(HolonAbstractTask *task);
    QWidget *taskWidget(HolonAbstractTask *task) const;
};


class HolonWindowStackedWidget : public HolonStackedWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonWindowStackedWidget)

public:
    explicit HolonWindowStackedWidget(int role = Holon::NoRole);
    ~HolonWindowStackedWidget();

    void addWindowWidget(HolonAbstractWindow *window, QWidget *widget);
    void removeWindowWidget(HolonAbstractWindow *window);
    void setCurrentWindow(HolonAbstractWindow *window);
};


class HolonWindowAreaStackedWidget : public HolonStackedWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonWindowAreaStackedWidget)

public:
    explicit HolonWindowAreaStackedWidget(int role = Holon::NoRole);
    ~HolonWindowAreaStackedWidget();

    void addWindowAreaWidget(HolonWindowArea *windowArea, QWidget *widget);
    void setCurrentWindowArea(HolonWindowArea *windowArea);
};

#endif // HOLONSTACKEDWIDGET_H
