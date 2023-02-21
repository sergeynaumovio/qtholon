// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTWIDGET_H
#define HOLONABSTRACTWIDGET_H

#include "qtholonglobal.h"
#include <QLoaderSettings>

class HolonAbstractTask;

class Q_HOLON_EXPORT HolonAbstractWidget : public QObject, public QLoaderSettings
{
    Q_OBJECT

protected:
    HolonAbstractWidget(QLoaderSettings *settings, HolonAbstractTask *parent);

public:
    ~HolonAbstractWidget();

    QString group() const;
    virtual QWidget *widget() const = 0;
};

#endif // HOLONABSTRACTWIDGET_H
