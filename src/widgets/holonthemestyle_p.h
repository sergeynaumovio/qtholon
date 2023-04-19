// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMESTYLE_P_H
#define HOLONTHEMESTYLE_P_H

class HolonDesktopPrivate;
class HolonTheme;
class HolonThemeStyle;

class HolonThemeStylePrivate
{
    HolonThemeStyle *const q_ptr;

public:
    HolonTheme *const theme;
    HolonDesktopPrivate *desktop_d;

    HolonThemeStylePrivate(HolonThemeStyle *q, HolonTheme *t);
};

#endif // HOLONTHEMESTYLE_P_H
