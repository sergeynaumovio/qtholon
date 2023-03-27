// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemeicons.h"
#include "holontheme.h"
#include "holonthemecolors.h"
#include "holonthemeicons_p.h"
#include <QLoaderSettings>

using namespace Qt::Literals::StringLiterals;

HolonThemeIcons::HolonThemeIcons(QLoaderSettings *settings, HolonTheme *theme)
:   QObject(theme),
    QLoaderSettings(settings),
    d_ptr(new HolonThemeIconsPrivate(this, theme))
{ }

HolonThemeIcons::~HolonThemeIcons()
{ }

HolonThemeColors *HolonThemeIcons::colors() const
{
    return d_ptr->colors;
}

QIcon HolonThemeIcons::splitButtonCloseBottomIcon() const
{
    return d_ptr->splitButtonCloseBottomIcon();
}

QIcon HolonThemeIcons::splitButtonHorizontalIcon() const
{
    return d_ptr->splitButtonHorizontalIcon();
}

QIcon HolonThemeIcons::splitButtonVerticalIcon() const
{
    return d_ptr->splitButtonVerticalIcon();
}

HolonTheme *HolonThemeIcons::theme() const
{
    return d_ptr->theme;
}
