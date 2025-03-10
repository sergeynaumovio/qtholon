// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSETTINGSWINDOW_H
#define HOLONSETTINGSWINDOW_H

#include "holontaskwindowattributeswindow.h"

class HolonDesktop;
class HolonSettingsWindowPrivate;
class HolonSidebar;

class HolonSettingsWindow : public HolonTaskWindowAttributesWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonSettingsWindow)

public:
    HolonSettingsWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonSettingsWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonSettingsWindow();

    QIcon icon() const override;
    bool isCopyable(QStringView to) const override;
    QString title() const override;
    QWidget *toolbarWidget() override;
};

#endif // HOLONSETTINGSWINDOW_H
