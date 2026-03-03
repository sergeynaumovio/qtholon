// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTATTRIBUTESWINDOW_H
#define HOLONABSTRACTATTRIBUTESWINDOW_H

#include "holonabstractwindow.h"

class HolonDesktop;
class HolonSidebar;
class HolonAbstractAttributesWindowPrivate;

class Q_HOLON_EXPORT HolonAbstractAttributesWindow : public HolonAbstractWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonAbstractAttributesWindow)

protected:
    HolonAbstractAttributesWindow(HolonAbstractAttributesWindowPrivate &d, QLoaderSettings *settings, QObject *parent);

public:
    HolonAbstractAttributesWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonAbstractAttributesWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonAbstractAttributesWindow();

    QWidget *centralWidget() override;
};

#endif // HOLONABSTRACTATTRIBUTESWINDOW_H
