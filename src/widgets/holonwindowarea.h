// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWAREA_H
#define HOLONWINDOWAREA_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QWidget>

class HolonAbstractWindow;
class HolonDesktop;
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

    void addWindow(HolonAbstractWindow *window);
    void closeWindow(HolonAbstractWindow *window);
    HolonDesktop *desktop() const;
    bool isChecked() const;
    virtual QIcon icon() const;
    QString shortcut() const;
    void splitWindow(HolonAbstractWindow *first, HolonAbstractWindow *second, Qt::Orientation orientation);
    virtual QString title() const;
};

#endif // HOLONWINDOWAREA_H
