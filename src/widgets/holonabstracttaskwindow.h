// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTTASKWINDOW_H
#define HOLONABSTRACTTASKWINDOW_H

#include "holonabstractwindow.h"

class HolonAbstractTaskWindowPrivate;

class Q_HOLON_EXPORT HolonAbstractTaskWindow : public HolonAbstractWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonAbstractTaskWindow)

protected:
    HolonAbstractTaskWindow(HolonAbstractTaskWindowPrivate &d, QLoaderSettings *settings, QObject *parent);
    HolonAbstractTaskWindow(QLoaderSettings *settings, HolonAbstractTask *parent);
    HolonAbstractTaskWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonAbstractTaskWindow(QLoaderSettings *settings, HolonStackedWindow *parent);

public:
    ~HolonAbstractTaskWindow();

    virtual QWidget *settingsWidget() const = 0;
    HolonAbstractTask *task() const;
};

#endif // HOLONABSTRACTTASKWINDOW_H
