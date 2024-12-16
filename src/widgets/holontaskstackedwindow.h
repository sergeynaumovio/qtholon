// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKSTACKEDWINDOW_H
#define HOLONTASKSTACKEDWINDOW_H

#include "holonstackedwindow.h"

class HolonAbstractTask;
class HolonAbstractTaskWindow;
class HolonTaskStackedWindowPrivate;

class Q_HOLON_EXPORT HolonTaskStackedWindow : public HolonStackedWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonTaskStackedWindow)

public:
    HolonTaskStackedWindow(QLoaderSettings *settings, HolonAbstractTask *parent);

    ~HolonTaskStackedWindow();

    bool isCopyable(const QStringList &to) const;
    HolonAbstractTask *task() const;
};

#endif // HOLONTASKSTACKEDWINDOW_H
