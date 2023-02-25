// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWAREASWITCH_H
#define HOLONWINDOWAREASWITCH_H

#include <QWidget>

class HolonDesktopPrivate;
class HolonSidebar;
class HolonTaskbar;
class HolonWindowArea;
class HolonWindowAreaSwitchPrivate;

class HolonWindowAreaSwitch : public QWidget
{
    Q_OBJECT

    HolonWindowAreaSwitchPrivate &d;
    std::aligned_storage_t<24, sizeof (ptrdiff_t)> d_storage;

public:
    HolonWindowAreaSwitch(HolonDesktopPrivate &desktop_d, HolonTaskbar *parent);
    ~HolonWindowAreaSwitch();

    void addSidebar(HolonSidebar *sidebar);
    void addWindowArea(HolonWindowArea *windowArea);
};

#endif // HOLONWINDOWAREASWITCH_H
