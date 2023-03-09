// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWAREASWITCH_H
#define HOLONWINDOWAREASWITCH_H

#include <QScopedStorage>
#include <QWidget>

class HolonDesktopPrivate;
class HolonSidebar;
class HolonTaskbar;
class HolonWindowArea;
class HolonWindowAreaSwitchPrivate;

class HolonWindowAreaSwitch : public QWidget
{
    Q_OBJECT

    const QScopedStorage<HolonWindowAreaSwitchPrivate, 24> d_ptr;

public:
    HolonWindowAreaSwitch(HolonDesktopPrivate &desktop_d, HolonTaskbar *parent);
    ~HolonWindowAreaSwitch();

    void addSidebar(HolonSidebar *sidebar);
    void addWindowArea(HolonWindowArea *windowArea);
};

#endif // HOLONWINDOWAREASWITCH_H
