// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPARAMETERSWINDOW_H
#define HOLONPARAMETERSWINDOW_H

#include "holonabstractwindow.h"
#include <QScopedStorage>

class HolonDesktop;
class HolonParametersWindowPrivate;
class HolonSidebar;

class HolonParametersWindow : public HolonAbstractWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonParametersWindow)

    friend class HolonDesktopPrivateData;

public:
    HolonParametersWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonParametersWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonParametersWindow();

    Holon::WindowFlags flags() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    int role() const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(int role = Holon::NoRole) const override;
};

#endif // HOLONPARAMETERSWINDOW_H
