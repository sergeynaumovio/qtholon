// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSTACKEDWIDGET_H
#define HOLONSTACKEDWIDGET_H

#include "qtholonglobal.h"
#include <QStackedWidget>

class HolonAbstractTask;
class HolonAbstractWindow;
class HolonStackedWidgetPrivate;
class HolonTaskStackedWidgetPrivate;
class HolonWindowArea;
class HolonWindowAreaStackedWidgetPrivate;
class HolonWindowStackedWidgetPrivate;

class Q_HOLON_EXPORT HolonStackedWidget : public QStackedWidget
{
    Q_OBJECT

protected:
    const QScopedPointer<HolonStackedWidgetPrivate> d_ptr;

    HolonStackedWidget(HolonStackedWidgetPrivate &d);

public:
    ~HolonStackedWidget();

    QString group() const;
};


class Q_HOLON_EXPORT HolonTaskStackedWidget : public HolonStackedWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonTaskStackedWidget)

public:
    explicit HolonTaskStackedWidget(const QString &group = QString());
    ~HolonTaskStackedWidget();

    void addTaskWidget(HolonAbstractTask *task, QWidget *widget);
    void setCurrentTask(HolonAbstractTask *task);
    QWidget *taskWidget(HolonAbstractTask *task) const;
};


class Q_HOLON_EXPORT HolonWindowStackedWidget : public HolonStackedWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonWindowStackedWidget)

public:
    explicit HolonWindowStackedWidget(const QString &group = QString());
    ~HolonWindowStackedWidget();

    void addWindowWidget(HolonAbstractWindow *window, QWidget *widget);
    void setCurrentWindow(HolonAbstractWindow *window);
};


class Q_HOLON_EXPORT HolonWindowAreaStackedWidget : public HolonStackedWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonWindowAreaStackedWidget)

public:
    explicit HolonWindowAreaStackedWidget(const QString &group = QString());
    ~HolonWindowAreaStackedWidget();

    void addWindowAreaWidget(HolonWindowArea *windowArea, QWidget *widget);
    void setCurrentWindowArea(HolonWindowArea *windowArea);
};

#endif // HOLONSTACKEDWIDGET_H
