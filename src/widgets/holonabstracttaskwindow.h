// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTTASKWINDOW_H
#define HOLONABSTRACTTASKWINDOW_H

#include "holonabstractwindow.h"

class HolonAbstractTask;
class HolonAbstractTaskWindowPrivate;
class HolonTaskStackedWindow;

class Q_HOLON_EXPORT HolonAbstractTaskWindow : public HolonAbstractWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonAbstractTaskWindow)

    friend class HolonPythonTaskWindow;

protected:
    HolonAbstractTaskWindow(HolonAbstractTaskWindowPrivate &d, QLoaderSettings *settings, QObject *parent);
    HolonAbstractTaskWindow(QLoaderSettings *settings, HolonAbstractTask *parent);
    HolonAbstractTaskWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonAbstractTaskWindow(QLoaderSettings *settings, HolonTaskStackedWindow *parent);

public:
    ~HolonAbstractTaskWindow();

    virtual QWidget *customWidget(QMetaType taskWindowAttributesWindow);
    virtual QWidget *settingsWidget() = 0;
    HolonAbstractTask *task() const;
};

#endif // HOLONABSTRACTTASKWINDOW_H
