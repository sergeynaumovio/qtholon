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
    QColor iconsBase;
    QColor iconsDisabled;
    QColor panelTextDark;
    QColor panelTextLight;

public:
    HolonThemeColors *const q_ptr;
    HolonTheme *const theme;
    HolonThemeIcons *icons;
    HolonThemeStyleSheets *styleSheets;

    HolonThemeColorsPrivate(HolonThemeColors *q, HolonTheme *t);

    QColor iconsBaseColor();
    QColor iconsDisabledColor();
    QColor panelTextColorDark();
    QColor panelTextColorLight();
};

#endif // HOLONTHEMECOLORS_P_H
