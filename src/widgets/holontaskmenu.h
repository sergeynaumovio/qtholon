// Copyright (C) 2022 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKMENU_H
#define HOLONTASKMENU_H

#include <QPushButton>

class HolonDesktopPrivate;
class HolonTaskbar;

class HolonTaskMenu : public QPushButton
{
    Q_OBJECT

public:
    HolonTaskMenu(HolonDesktopPrivate &desktop_d, HolonTaskbar *parent);
};

#endif // HOLONTASKMENU_H
