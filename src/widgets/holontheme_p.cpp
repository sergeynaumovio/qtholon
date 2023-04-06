// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontheme_p.h"
#include "holonthemecolors.h"
#include "holonthemeicons.h"
#include "holonthemesizehints.h"
#include "holonthemestylesheets.h"

using namespace Qt::Literals::StringLiterals;

HolonThemePrivate::HolonThemePrivate(QLoaderSettings *settings,
                                     HolonTheme *theme,
                                     HolonDesktop *desktop)
:   HolonThemePrivate(theme,
                      new HolonThemeColors(settings, theme),
                      new HolonThemeIcons(settings, theme),
                      new HolonThemeSizeHints(settings, theme),
                      new HolonThemeStyleSheets(settings, theme),
                      desktop)
{ }

HolonThemePrivate::HolonThemePrivate(HolonTheme *t,
                                     HolonThemeColors *c,
                                     HolonThemeIcons *i,
                                     HolonThemeSizeHints *h,
                                     HolonThemeStyleSheets *s,
                                     HolonDesktop *d)
:   theme(t),
    colors(c),
    icons(i),
    sizeHints(h),
    styleSheets(s),
    desktop(d)
{ }

void HolonThemePrivate::setCurrent(bool current)
{
    theme->setValue(u"current"_s, current);
}
