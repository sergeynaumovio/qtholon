// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemecolors_p.h"
#include "holonthemecolors.h"

using namespace Qt::Literals::StringLiterals;

HolonThemeColorsPrivate::HolonThemeColorsPrivate(HolonThemeColors *q, HolonTheme *t)
:   q_ptr(q),
    theme(t)
{ }

QColor HolonThemeColorsPrivate::baseColor()
{
    if (base.isValid())
        return base;

    if ((base = q_ptr->value(u"baseColor"_s).value<QColor>()).isValid())
        return base;

    base = QColor(54, 56, 58);

    return base;
}

QColor HolonThemeColorsPrivate::buttonHoveredColor()
{
    if (buttonHovered.isValid())
        return buttonHovered;

    if ((buttonHovered = q_ptr->value(u"buttonHoveredColor"_s).value<QColor>()).isValid())
        return buttonHovered;

    buttonHovered = QColor(74, 76, 78);

    return buttonHovered;
}

QColor HolonThemeColorsPrivate::buttonPressedColor()
{
    if (buttonPressed.isValid())
        return buttonPressed;

    if ((buttonPressed = q_ptr->value(u"buttonPressedColor"_s).value<QColor>()).isValid())
        return buttonPressed;

    buttonPressed = QColor(44, 46, 48);

    return buttonPressed;
}

QColor HolonThemeColorsPrivate::buttonPressedHoveredColor()
{
    if (buttonPressedHovered.isValid())
        return buttonPressedHovered;

    if ((buttonPressedHovered = q_ptr->value(u"buttonPressedHoveredColor"_s).value<QColor>()).isValid())
        return buttonPressedHovered;

    buttonPressedHovered = QColor(24, 26, 28);

    return buttonPressedHovered;
}

QColor HolonThemeColorsPrivate::iconsBaseColor()
{
    if (iconsBase.isValid())
        return iconsBase;

    if ((iconsBase = q_ptr->value(u"iconsBaseColor"_s).value<QColor>()).isValid())
        return iconsBase;

    iconsBase = QColor(200, 200, 200);

    return iconsBase;
}

QColor HolonThemeColorsPrivate::iconsDisabledColor()
{
    if (iconsDisabled.isValid())
        return iconsDisabled;

    if ((iconsDisabled = q_ptr->value(u"iconsDisabledColor"_s).value<QColor>()).isValid())
        return iconsDisabled;

    iconsDisabled = QColor(100, 100, 100);

    return iconsDisabled;
}

QColor HolonThemeColorsPrivate::mainWindowSeparatorColor()
{
    if (mainWindowSeparator.isValid())
        return mainWindowSeparator;

    if ((mainWindowSeparator = q_ptr->value(u"mainWindowSeparatorColor"_s).value<QColor>()).isValid())
        return mainWindowSeparator;

    mainWindowSeparator = QColor(34, 36, 38);

    return mainWindowSeparator;
}

QColor HolonThemeColorsPrivate::panelTextColorDark()
{
    if (panelTextDark.isValid())
        return panelTextDark;

    if ((panelTextDark = q_ptr->value(u"panelTextColorDark"_s).value<QColor>()).isValid())
        return panelTextDark;

    panelTextDark = QColor(50, 50, 50);

    return panelTextDark;
}

QColor HolonThemeColorsPrivate::panelTextColorLight()
{
    if (panelTextLight.isValid())
        return panelTextLight;

    if ((panelTextLight = q_ptr->value(u"panelTextColorLight"_s).value<QColor>()).isValid())
        return panelTextLight;

    panelTextLight = QColor(255, 255, 255);

    return panelTextLight;
}

QColor HolonThemeColorsPrivate::toolButtonSeparatorColor()
{
    if (toolButtonSeparator.isValid())
        return toolButtonSeparator;

    if ((toolButtonSeparator = q_ptr->value(u"toolButtonSeparatorColor"_s).value<QColor>()).isValid())
        return toolButtonSeparator;

    toolButtonSeparator = QColor(200, 200, 200, 60);

    return toolButtonSeparator;
}
