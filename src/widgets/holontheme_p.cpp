// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontheme_p.h"
#include "holonthemecolors.h"
#include "holonthemecolors_p.h"
#include "holonthemeicons.h"
#include "holonthemeicons_p.h"

using namespace Qt::Literals::StringLiterals;

HolonThemePrivate::HolonThemePrivate(QLoaderSettings *settings, HolonTheme *theme)
:   HolonThemePrivate(theme, new HolonThemeColors(settings, theme), new HolonThemeIcons(settings, theme))
{ }

HolonThemePrivate::HolonThemePrivate(HolonTheme *t, HolonThemeColors *c, HolonThemeIcons *i)
:   theme(t),
    colors(c),
    icons(i)
{
    colors->d_ptr->icons = icons;
    icons->d_ptr->colors = colors;
}

void HolonThemePrivate::setCurrent(bool current)
{
    theme->setValue(u"current"_s, current);
}
