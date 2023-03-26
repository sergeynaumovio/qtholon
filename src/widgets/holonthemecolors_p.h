// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMECOLORS_P_H
#define HOLONTHEMECOLORS_P_H

#include <QColor>

class HolonTheme;
class HolonThemeColors;
class HolonThemeIcons;

class HolonThemeColorsPrivate
{
    QColor iconsBase;
    QColor iconsDisabled;

public:
    HolonThemeColors *const q_ptr;
    HolonTheme *const theme;
    HolonThemeIcons *icons;

    HolonThemeColorsPrivate(HolonThemeColors *q, HolonTheme *t);

    QColor iconsBaseColor();
    QColor iconsDisabledColor();
};

#endif // HOLONTHEMECOLORS_P_H
