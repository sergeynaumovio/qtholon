// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEME_P_H
#define HOLONTHEME_P_H

class HolonDesktop;
class HolonDesktopPrivate;
class HolonTheme;
class HolonThemeColors;
class HolonThemeIcons;
class HolonThemeStyle;
class QLoaderSettings;

class HolonThemePrivate
{
public:
    HolonTheme *const theme;
    HolonThemeColors *const colors;
    HolonThemeIcons *const icons;
    HolonDesktop *const desktop;
    HolonDesktopPrivate *desktop_d;

    HolonThemePrivate(QLoaderSettings *settings,
                      HolonTheme *theme,
                      HolonDesktop *desktop);

    HolonThemePrivate(HolonTheme *theme,
                      HolonThemeColors *colors,
                      HolonThemeIcons *icons,
                      HolonDesktop *desktop);

    void setCurrent(bool current);
};

#endif // HOLONTHEME_P_H
