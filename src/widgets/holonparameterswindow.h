// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPARAMETERSWINDOW_H
#define HOLONPARAMETERSWINDOW_H

#include "holontaskattributeswindow.h"

class HolonDesktop;
class HolonParametersWindowPrivate;
class HolonSidebar;

class HolonParametersWindow : public HolonTaskAttributesWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonParametersWindow)

    friend class HolonDesktopPrivateData;

public:
    HolonParametersWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonParametersWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonParametersWindow();

    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbarWidget() override;
};

#endif // HOLONPARAMETERSWINDOW_H
