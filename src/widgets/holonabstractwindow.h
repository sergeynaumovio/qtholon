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
    enum Area
    {
        Central = 0x0001,
        Sidebar = 0x0002,
        Left = 0x0004,
        Right = 0x0008,
        Top = 0x0010,
        Bottom = 0x0020
    };
    Q_DECLARE_FLAGS(Areas, HolonAbstractWindow::Area)

    enum Attribute
    {
        WindowSplitButtonHint = 0x0001,
        WindowMinMaxButtonsHint = 0x0002,
        WindowCloseButtonHint = 0x0004,
        WindowAllButtonsHint = WindowSplitButtonHint |
                               WindowMinMaxButtonsHint |
                               WindowCloseButtonHint
    };
    Q_DECLARE_FLAGS(Attributes, HolonAbstractWindow::Attribute)

    ~HolonAbstractWindow();

    virtual HolonAbstractWindow::Areas areas() const;
    virtual HolonAbstractWindow::Attributes attributes() const;
    HolonDesktop *desktop() const;
    QString group() const;
    virtual QIcon icon() const;
    bool isCurrent() const;
    HolonAbstractTask *task() const;
    virtual QString title() const;
    virtual QWidget *toolbar() const;
    virtual QWidget *widget(const QString &group = QString()) const = 0;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(HolonAbstractWindow::Areas)
Q_DECLARE_OPERATORS_FOR_FLAGS(HolonAbstractWindow::Attributes)

#endif // HOLONABSTRACTWINDOW_H
