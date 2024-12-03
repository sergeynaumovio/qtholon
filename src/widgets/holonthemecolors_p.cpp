// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemecolors_p.h"
#include "holonthemecolors.h"

using namespace Qt::Literals::StringLiterals;

QColor HolonThemeColorsPrivate::color(const QString &key, QColor &value, const QColor &defaultValue)
{
    if (value.isValid())
        return value;

    if ((value = q_ptr->value(key).value<QColor>()).isValid())
        return value;

    value = defaultValue;

    return value;
}

HolonThemeColorsPrivate::HolonThemeColorsPrivate(HolonThemeColors *q, HolonTheme *t)
:   q_ptr(q),
    theme(t)
{ }

QColor HolonThemeColorsPrivate::baseColor()
{
    return color(u"baseColor"_s, base, {54, 56, 58});
}

QColor HolonThemeColorsPrivate::buttonHoveredColor()
{
    return color(u"buttonHoveredColor"_s, buttonHovered, {74, 76, 78});
}

QColor HolonThemeColorsPrivate::buttonPressedColor()
{
    return color(u"buttonPressedColor"_s, buttonPressed, {44, 46, 48});
}

QColor HolonThemeColorsPrivate::buttonPressedHoveredColor()
{
    return color(u"buttonPressedHoveredColor"_s, buttonPressedHovered, {24, 26, 28});
}

QColor HolonThemeColorsPrivate::iconsBaseColor()
{
    return color(u"iconsBaseColor"_s, iconsBase, {200, 200, 200});
}

QColor HolonThemeColorsPrivate::iconsDisabledColor()
{
    return color(u"iconsDisabledColor"_s, iconsDisabled, {100, 100, 100});
}

QColor HolonThemeColorsPrivate::mainWindowSeparatorColor()
{
    return color(u"mainWindowSeparatorColor"_s, mainWindowSeparator, {34, 36, 38});
}

QColor HolonThemeColorsPrivate::panelTextColorDark()
{
    return color(u"panelTextColorDark"_s, panelTextDark, {50, 50, 50});
}

QColor HolonThemeColorsPrivate::panelTextColorLight()
{
    return color(u"panelTextColorLight"_s, panelTextLight, {255, 255, 255});
}

QColor HolonThemeColorsPrivate::toolButtonSeparatorColor()
{
    return color(u"toolButtonSeparatorColor"_s, toolButtonSeparator, {200, 200, 200, 60});
}
