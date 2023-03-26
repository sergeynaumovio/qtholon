// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontheme.h"
#include "holondesktop.h"
#include "holontheme_p.h"
#include "holonthemecolors.h"
#include "holonthemeicons.h"
#include <QLoaderSettings>

using namespace Qt::Literals::StringLiterals;

HolonTheme::HolonTheme(QLoaderSettings *settings,
                       HolonDesktop *desktop,
                       HolonThemeColors *colors,
                       HolonThemeIcons *icons)
:   QObject(desktop),
    QLoaderSettings(settings),
    d_ptr(new HolonThemePrivate(this, colors, icons))
{
    desktop->addTheme(this);
}

HolonTheme::HolonTheme(QLoaderSettings *settings, HolonDesktop *desktop)
:   QObject(desktop),
    QLoaderSettings(settings),
    d_ptr(new HolonThemePrivate(settings, this))
{
    desktop->addTheme(this);
}

HolonTheme::~HolonTheme()
{ }

HolonThemeColors *HolonTheme::colors() const
{
    return d_ptr->colors;
}

HolonThemeIcons *HolonTheme::icons() const
{
    return d_ptr->icons;
}

bool HolonTheme::isCurrent() const
{
    return value(u"current"_s).toBool();
}
