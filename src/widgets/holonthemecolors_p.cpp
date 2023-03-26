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

    iconsDisabled = QColor(50, 50, 50);

    return iconsDisabled;
}
