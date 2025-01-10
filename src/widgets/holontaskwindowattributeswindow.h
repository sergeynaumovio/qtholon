// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKWINDOWATTRIBUTESWINDOW_H
#define HOLONTASKWINDOWATTRIBUTESWINDOW_H

#include "holonabstractwindow.h"

class HolonDesktop;
class HolonSidebar;
class HolonTaskWindowAttributesWindowPrivate;

class Q_HOLON_EXPORT HolonTaskWindowAttributesWindow : public HolonAbstractWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonTaskWindowAttributesWindow)

protected:
    HolonTaskWindowAttributesWindow(HolonTaskWindowAttributesWindowPrivate &d, QLoaderSettings *settings, QObject *parent);

public:
    HolonTaskWindowAttributesWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonTaskWindowAttributesWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonTaskWindowAttributesWindow();

    QWidget *centralWidget() override;
};

#endif // HOLONTASKWINDOWATTRIBUTESWINDOW_H
