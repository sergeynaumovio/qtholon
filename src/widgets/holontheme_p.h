// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEME_P_H
#define HOLONTHEME_P_H

class HolonTheme;
class HolonThemeColors;
class HolonThemeIcons;
class QLoaderSettings;

class HolonThemePrivate
{
public:
    HolonTheme *const theme;
    HolonThemeColors *const colors;
    HolonThemeIcons *const icons;

    HolonThemePrivate(QLoaderSettings *settings, HolonTheme *theme);
    HolonThemePrivate(HolonTheme *theme, HolonThemeColors *colors, HolonThemeIcons *icons);

    void setCurrent(bool current);
};

#endif // HOLONTHEME_P_H
