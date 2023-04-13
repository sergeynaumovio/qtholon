// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemecolors_p.h"
#include "holonthemecolors.h"

using namespace Qt::Literals::StringLiterals;

HolonThemeColorsPrivate::HolonThemeColorsPrivate(HolonThemeColors *q, HolonTheme *t)
:   q_ptr(q),
    theme(t)
{ }

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

    if ((mainWindowSeparator = q_ptr->value(u"mainWindowSeparator"_s).value<QColor>()).isValid())
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
