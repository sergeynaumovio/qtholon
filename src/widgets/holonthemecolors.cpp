// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
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

QColor HolonThemeColors::iconsMenuColor() const
{
    return d_ptr->iconsMenuColor();
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

QColor HolonThemeColors::panelTextColorMid() const
{
    return d_ptr->panelTextColorMid();
}

QColor HolonThemeColors::runColor() const
{
    return d_ptr->runColor();
}

QColor HolonThemeColors::stopColor() const
{
    return d_ptr->stopColor();
}

HolonTheme *HolonThemeColors::theme() const
{
    return d_ptr->theme;
}

QColor HolonThemeColors::toolButtonSeparatorColor() const
{
    return d_ptr->toolButtonSeparatorColor();
}
