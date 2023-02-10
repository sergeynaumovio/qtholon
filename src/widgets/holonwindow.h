// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOW_H
#define HOLONWINDOW_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QLoaderIconInterface>

class HolonDesktop;
class HolonSidebar;
class HolonWindowPrivate;

class Q_HOLON_EXPORT HolonWindow : public QObject, public QLoaderSettings,
                                                   public QLoaderIconInterface

{
    Q_OBJECT
    Q_INTERFACES(QLoaderIconInterface)

    const QScopedPointer<HolonWindowPrivate> d_ptr;

protected:
    HolonWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonWindow(QLoaderSettings *settings, HolonSidebar *parent);

public:
    ~HolonWindow();

    enum Area
    {
        Central = 0x00000001,
        Sidebar = 0x00000002
    };
    Q_DECLARE_FLAGS(Areas, HolonWindow::Area)

    enum Attribute
    {
        WindowMinimizeButtonHint = 0x00000001,
        WindowMaximizeButtonHint = 0x00000002,
        WindowMinMaxButtonsHint = WindowMinimizeButtonHint | WindowMaximizeButtonHint,
        WindowCloseButtonHint = 0x00000004
    };
    Q_DECLARE_FLAGS(Attributes, HolonWindow::Attribute)

    virtual HolonWindow::Areas areas() const;
    virtual HolonWindow::Attributes attributes() const;
    void close();
    QIcon icon() const override;
    virtual QString title() const;
    virtual QWidget *toolbar() const;
    virtual QWidget *widget() const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(HolonWindow::Areas)
Q_DECLARE_OPERATORS_FOR_FLAGS(HolonWindow::Attributes)


#endif // HOLONWINDOW_H
