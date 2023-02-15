// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
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

    friend class HolonDesktopPrivate;
    friend class HolonWindowAreaPrivate;

protected:
    const QScopedPointer<HolonWindowAreaPrivate> d_ptr;

    HolonWindowArea(HolonWindowAreaPrivate &d, QLoaderSettings *settings);

public:
    HolonWindowArea(QLoaderSettings *settings, HolonDesktop *desktop);
    ~HolonWindowArea();

    void addWindow(HolonWindow *window);
    void closeWindow(HolonWindow *window);
    bool isChecked() const;
    virtual QIcon icon() const;
    QString shortcut() const;
    virtual QString title() const;
};

#endif // HOLONWINDOWAREA_H
