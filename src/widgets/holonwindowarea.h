// Copyright (C) 2022 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWAREA_H
#define HOLONWINDOWAREA_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QWidget>

class HolonDesktop;
class HolonWindow;
class HolonWindowAreaPrivate;
class QStackedWidget;

class Q_HOLON_EXPORT HolonWindowArea : public QWidget, public QLoaderSettings
{
    Q_OBJECT

protected:
    const QScopedPointer<HolonWindowAreaPrivate> d_ptr;

    HolonWindowArea(HolonWindowAreaPrivate &d,
                    QLoaderSettings *settings);

    HolonWindowArea(HolonDesktop *desktop,
                    QLoaderSettings *settings,
                    QStackedWidget *parent);

public:
    ~HolonWindowArea();

    void addWindow(HolonWindow *window);
    void closeWindow(HolonWindow *window);
};

#endif // HOLONWINDOWAREA_H
