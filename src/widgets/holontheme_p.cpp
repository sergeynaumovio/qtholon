// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontheme_p.h"
#include "holonthemecolors.h"
#include "holonthemecolors_p.h"
#include "holonthemeicons.h"
#include "holonthemeicons_p.h"
#include "holonthemestylesheets.h"
#include "holonthemestylesheets_p.h"

using namespace Qt::Literals::StringLiterals;

HolonThemePrivate::HolonThemePrivate(QLoaderSettings *settings, HolonTheme *theme)
:   HolonThemePrivate(theme,
                      new HolonThemeColors(settings, theme),
                      new HolonThemeIcons(settings, theme),
                      new HolonThemeStyleSheets(settings, theme))
{ }

HolonThemePrivate::HolonThemePrivate(HolonTheme *t,
                                     HolonThemeColors *c,
                                     HolonThemeIcons *i,
                                     HolonThemeStyleSheets *s)
:   theme(t),
    colors(c),
    icons(i),
    styleSheets(s)
{
    colors->d_ptr->icons = icons;
    colors->d_ptr->styleSheets = styleSheets;

    icons->d_ptr->colors = colors;
    icons->d_ptr->styleSheets = styleSheets;

    styleSheets->d_ptr->colors = colors;
    styleSheets->d_ptr->icons = icons;
}

void HolonThemePrivate::setCurrent(bool current)
{
    theme->setValue(u"current"_s, current);
}
