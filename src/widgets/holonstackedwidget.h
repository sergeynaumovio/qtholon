// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSTACKEDWIDGET_H
#define HOLONSTACKEDWIDGET_H

#include "qtholonglobal.h"
#include <QStackedWidget>

class HolonStackedWidgetPrivate;
class HolonAbstractTask;

class Q_HOLON_EXPORT HolonStackedWidget : public QStackedWidget
{
    Q_OBJECT

    const QScopedPointer<HolonStackedWidgetPrivate> d_ptr;

public:
    explicit HolonStackedWidget(const QString &group);
    ~HolonStackedWidget();

    void addTaskWidget(HolonAbstractTask *task, QWidget *widget);
    QString group() const;
    void setCurrentTask(HolonAbstractTask *task);
};

#endif // HOLONSTACKEDWIDGET_H
