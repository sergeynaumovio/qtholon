// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
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

    QColor base;
    QColor buttonHovered;
    QColor buttonPressed;
    QColor buttonPressedHovered;
    QColor iconsBase;
    QColor iconsDisabled;
    QColor iconsMenu;
    QColor mainWindowSeparator;
    QColor panelTextDark;
    QColor panelTextLight;
    QColor panelTextMid;
    QColor run;
    QColor stop;
    QColor toolButtonSeparator;

    QColor color(const QString &key, QColor &value, const QColor &defaultValue);

public:
    HolonTheme *const theme;

    HolonThemeColorsPrivate(HolonThemeColors *q, HolonTheme *t);

    QColor baseColor();
    QColor buttonHoveredColor();
    QColor buttonPressedColor();
    QColor buttonPressedHoveredColor();
    QColor iconsBaseColor();
    QColor iconsDisabledColor();
    QColor iconsMenuColor();
    QColor mainWindowSeparatorColor();
    QColor panelTextColorDark();
    QColor panelTextColorLight();
    QColor panelTextColorMid();
    QColor runColor();
    QColor stopColor();
    QColor toolButtonSeparatorColor();
};

#endif // HOLONTHEMECOLORS_P_H
