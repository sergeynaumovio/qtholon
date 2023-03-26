// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemecolors.h"
#include "holontheme.h"
#include "holonthemecolors_p.h"
#include <QLoaderSettings>

using namespace Qt::Literals::StringLiterals;

HolonThemeColors::HolonThemeColors(QLoaderSettings *settings, HolonTheme *theme)
:   QObject(theme),
    QLoaderSettings(settings),
    d_ptr(new HolonThemeColorsPrivate(this, theme))
{ }

HolonThemeColors::~HolonThemeColors()
{ }

HolonThemeIcons *HolonThemeColors::icons() const
{
    return d_ptr->icons;
}

QColor HolonThemeColors::iconsBaseColor() const
{
    return d_ptr->iconsBaseColor();
}

QColor HolonThemeColors::iconsDisabledColor() const
{
    return d_ptr->iconsDisabledColor();
}

HolonTheme *HolonThemeColors::theme() const
{
    return d_ptr->theme;
}

