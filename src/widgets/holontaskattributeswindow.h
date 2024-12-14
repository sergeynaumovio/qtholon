// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKATTRIBUTESWINDOW_H
#define HOLONTASKATTRIBUTESWINDOW_H

#include "holonabstractwindow.h"

class HolonDesktop;
class HolonSidebar;
class HolonTaskAttributesWindowPrivate;

class Q_HOLON_EXPORT HolonTaskAttributesWindow : public HolonAbstractWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonTaskAttributesWindow)

protected:
    HolonTaskAttributesWindow(HolonTaskAttributesWindowPrivate &d, QLoaderSettings *settings, QObject *parent);

public:
    HolonTaskAttributesWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonTaskAttributesWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonTaskAttributesWindow();

    QWidget *centralWidget() const override;
};

#endif // HOLONTASKATTRIBUTESWINDOW_H
