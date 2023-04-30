// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemecolors.h"
#include "holontheme.h"
#include "holonthemecolors_p.h"
#include <QLoaderSettings>

HolonThemeColors::HolonThemeColors(QLoaderSettings *settings, HolonTheme *theme)
:   QObject(theme),
    QLoaderSettings(this, settings),
    d_ptr(new HolonThemeColorsPrivate(this, theme))
{ }

HolonThemeColors::~HolonThemeColors()
{ }

QColor HolonThemeColors::baseColor() const
{
    return d_ptr->baseColor();
}

QColor HolonThemeColors::buttonHoveredColor() const
{
    return d_ptr->buttonHoveredColor();
}

QColor HolonThemeColors::buttonPressedColor() const
{
    return d_ptr->buttonPressedColor();
}

QColor HolonThemeColors::buttonPressedHoveredColor() const
{
    return d_ptr->buttonPressedHoveredColor();
}

QColor HolonThemeColors::iconsBaseColor() const
{
    return d_ptr->iconsBaseColor();
}

QColor HolonThemeColors::iconsDisabledColor() const
{
    return d_ptr->iconsDisabledColor();
}

QColor HolonThemeColors::mainWindowSeparatorColor() const
{
    return d_ptr->mainWindowSeparatorColor();
}

QColor HolonThemeColors::panelTextColorDark() const
{
    return d_ptr->panelTextColorDark();
}

QColor HolonThemeColors::panelTextColorLight() const
{
    return d_ptr->panelTextColorLight();
}

HolonTheme *HolonThemeColors::theme() const
{
    return d_ptr->theme;
}

QColor HolonThemeColors::toolButtonSeparatorColor() const
{
    return d_ptr->toolButtonSeparatorColor();
}
