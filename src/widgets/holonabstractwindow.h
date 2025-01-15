// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTWINDOW_H
#define HOLONABSTRACTWINDOW_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QObject>

class HolonAbstractWindowPrivate;
class HolonDesktop;
class HolonStackedWindow;
class HolonWindowArea;

class Q_HOLON_EXPORT HolonAbstractWindow : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonAbstractWindowPrivate;
    friend class HolonDesktopPrivate;
    friend class HolonStackedWindowPrivate;
    friend class HolonWindowAreaPrivate;

protected:
    const QScopedPointer<HolonAbstractWindowPrivate> d_ptr;

    HolonAbstractWindow(HolonAbstractWindowPrivate &d, QLoaderSettings *settings, QObject *parent);
    HolonAbstractWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonAbstractWindow(QLoaderSettings *settings, HolonStackedWindow *parent);
    HolonAbstractWindow(QLoaderSettings *settings, HolonWindowArea *parent);

public:
    ~HolonAbstractWindow();

    virtual QWidget *centralWidget() = 0;
    HolonDesktop *desktop() const;
    virtual QIcon icon() const;
    bool isCurrent() const;
    virtual QString title() const;
    virtual QWidget *toolbarWidget() = 0;
};

#endif // HOLONABSTRACTWINDOW_H
