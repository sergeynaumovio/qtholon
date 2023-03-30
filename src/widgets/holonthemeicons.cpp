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

QIcon HolonThemeIcons::closeBackgroundIcon() const
{
    return d_ptr->closeBackgroundIcon();
}

QIcon HolonThemeIcons::closeForegroundIcon() const
{
    return d_ptr->closeForegroundIcon();
}

QIcon HolonThemeIcons::dirIcon() const
{
    return d_ptr->dirIcon();
}

QIcon HolonThemeIcons::dirOpenIcon() const
{
    return d_ptr->dirOpenIcon();
}

QIcon HolonThemeIcons::maximizeIcon() const
{
    return d_ptr->maximizeIcon();
}

QIcon HolonThemeIcons::minimizeIcon() const
{
    return d_ptr->minimizeIcon();
}

QIcon HolonThemeIcons::splitButtonCloseBottomIcon() const
{
    return d_ptr->splitButtonCloseBottomIcon();
}

QIcon HolonThemeIcons::splitButtonCloseLeftIcon() const
{
    return d_ptr->splitButtonCloseLeftIcon();
}

QIcon HolonThemeIcons::splitButtonCloseRightIcon() const
{
    return d_ptr->splitButtonCloseRightIcon();
}

QIcon HolonThemeIcons::splitButtonCloseTopIcon() const
{
    return d_ptr->splitButtonCloseTopIcon();
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
