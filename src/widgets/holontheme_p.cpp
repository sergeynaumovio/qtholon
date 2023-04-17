// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontheme_p.h"
#include "holondesktop.h"
#include "holonthemecolors.h"
#include "holonthemeicons.h"
#include "holonthemestyle.h"
#include <QApplication>

using namespace Qt::Literals::StringLiterals;

HolonThemePrivate::HolonThemePrivate(QLoaderSettings *settings,
                                     HolonTheme *theme,
                                     HolonDesktop *desktop)
:   HolonThemePrivate(theme,
                      new HolonThemeColors(settings, theme),
                      new HolonThemeIcons(settings, theme),
                      desktop)
{
    QApplication::setStyle(new HolonThemeStyle(theme));
}

HolonThemePrivate::HolonThemePrivate(HolonTheme *t,
                                     HolonThemeColors *c,
                                     HolonThemeIcons *i,
                                     HolonDesktop *d)
:   theme(t),
    colors(c),
    icons(i),
    desktop(d)
{ }

void HolonThemePrivate::setCurrent(bool current)
{
    theme->setValue(u"current"_s, current);
}
