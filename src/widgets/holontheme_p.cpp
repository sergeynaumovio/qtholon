// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontheme_p.h"
#include "holondesktop.h"
#include "holonthemecolors.h"
#include "holonthemeicons.h"
#include "holonthemestyle.h"
#include <QApplication>

using namespace Qt::Literals::StringLiterals;

HolonThemePrivate::HolonThemePrivate(HolonTheme *theme,
                                     QLoaderSettings *settings,
                                     HolonDesktop *desktop)
:   HolonThemePrivate(theme,
                      new HolonThemeColors(settings, theme),
                      new HolonThemeIcons(settings, theme),
                      new HolonThemeStyle(theme),
                      desktop)
{
    QApplication::setStyle(new HolonThemeStyle(theme));
}

HolonThemePrivate::HolonThemePrivate(HolonTheme *t,
                                     HolonThemeColors *c,
                                     HolonThemeIcons *i,
                                     HolonThemeStyle *s,
                                     HolonDesktop *d)
:   theme(t),
    colors(c),
    icons(i),
    style(s),
    desktop(d)
{ }

void HolonThemePrivate::setCurrent(bool current)
{
    theme->setValue(u"current"_s, current);
}
