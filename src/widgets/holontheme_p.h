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
    HolonThemeStyle *const style;
    HolonDesktop *const desktop;
    HolonDesktopPrivate *desktop_d;

    HolonThemePrivate(HolonTheme *theme,
                      QLoaderSettings *settings,
                      HolonDesktop *desktop);

    HolonThemePrivate(HolonTheme *theme,
                      HolonThemeColors *colors,
                      HolonThemeIcons *icons,
                      HolonThemeStyle *style,
                      HolonDesktop *desktop);

    void setCurrent(bool current);
};

#endif // HOLONTHEME_P_H
