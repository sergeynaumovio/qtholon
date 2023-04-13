// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMECOLORS_P_H
#define HOLONTHEMECOLORS_P_H

#include <QColor>

class HolonTheme;
class HolonThemeColors;
class HolonThemeIcons;
class HolonThemeStyleSheets;

class HolonThemeColorsPrivate
{
    HolonThemeColors *const q_ptr;

    QColor iconsBase;
    QColor iconsDisabled;
    QColor mainWindowSeparator;
    QColor panelTextDark;
    QColor panelTextLight;

public:
    HolonTheme *const theme;

    HolonThemeColorsPrivate(HolonThemeColors *q, HolonTheme *t);

    QColor iconsBaseColor();
    QColor iconsDisabledColor();
    QColor mainWindowSeparatorColor();
    QColor panelTextColorDark();
    QColor panelTextColorLight();
};

#endif // HOLONTHEMECOLORS_P_H
