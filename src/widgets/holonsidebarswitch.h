// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSIDEBARSWITCH_H
#define HOLONSIDEBARSWITCH_H

#include <QWidget>

class HolonDesktopPrivate;
class HolonSidebar;
class HolonSidebarSwitchPrivate;
class HolonTaskbar;

class HolonSidebarSwitch : public QWidget
{
    Q_OBJECT

    HolonSidebarSwitchPrivate &d;
    std::aligned_storage_t<24, sizeof (ptrdiff_t)> d_storage;

public:
    HolonSidebarSwitch(HolonDesktopPrivate &desktop_d, HolonTaskbar *parent);
    ~HolonSidebarSwitch();

    void addSidebar(HolonSidebar *sidebar);
};

#endif // HOLONSIDEBARSWITCH_H
