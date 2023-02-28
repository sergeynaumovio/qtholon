// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTWINDOW_H
#define HOLONABSTRACTWINDOW_H

#include "qtholonglobal.h"
#include <QLoaderSettings>

class HolonAbstractTask;
class HolonDesktop;
class HolonWindowArea;
class HolonAbstractWindowPrivate;

class Q_HOLON_EXPORT HolonAbstractWindow : public QObject, public QLoaderSettings

{
    Q_OBJECT

    friend class HolonAbstractWindowPrivate;
    friend class HolonDesktopPrivate;
    const QScopedPointer<HolonAbstractWindowPrivate> d_ptr;

protected:
    HolonAbstractWindow(QLoaderSettings *settings, HolonAbstractTask *parent);
    HolonAbstractWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonAbstractWindow(QLoaderSettings *settings, HolonWindowArea *parent);

public:
    ~HolonAbstractWindow();

    enum Area
    {
        Central,
        Sidebar
    };

    enum Attribute
    {
        WindowMinimizeButtonHint = 0x0001,
        WindowMaximizeButtonHint = 0x0002,
        WindowMinMaxButtonsHint = WindowMinimizeButtonHint | WindowMaximizeButtonHint,
        WindowCloseButtonHint = 0x0004
    };
    Q_DECLARE_FLAGS(Attributes, HolonAbstractWindow::Attribute)

    virtual HolonAbstractWindow::Area area() const = 0;
    virtual HolonAbstractWindow::Attributes attributes() const;
    HolonDesktop *desktop() const;
    QString group() const;
    virtual QIcon icon() const;
    bool isCurrent() const;
    virtual QString title() const;
    virtual QWidget *toolbar() const;
    virtual QWidget *widget(const QString &group = QString()) const = 0;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(HolonAbstractWindow::Attributes)


#endif // HOLONABSTRACTWINDOW_H
