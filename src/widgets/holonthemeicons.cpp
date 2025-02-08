// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemeicons.h"
#include "holontheme.h"
#include "holonthemeicons_p.h"
#include <QLoaderSettings>

HolonThemeIcons::HolonThemeIcons(QLoaderSettings *settings, HolonTheme *theme)
:   QObject(theme),
    QLoaderSettings(this, settings),
    d_ptr(new HolonThemeIconsPrivate(theme))
{ }

HolonThemeIcons::~HolonThemeIcons()
{ }

QIcon HolonThemeIcons::clearIcon() const
{
    return d_ptr->clearIcon();
}

QIcon HolonThemeIcons::closeBackgroundIcon() const
{
    return d_ptr->closeBackgroundIcon();
}

QIcon HolonThemeIcons::closeForegroundIcon() const
{
    return d_ptr->closeForegroundIcon();
}

QIcon HolonThemeIcons::dirClosedIcon() const
{
    return d_ptr->dirClosedIcon();
}

QIcon HolonThemeIcons::dirOpenIcon() const
{
    return d_ptr->dirOpenIcon();
}

QIcon HolonThemeIcons::expandIcon() const
{
    return d_ptr->expandIcon();
}

QIcon HolonThemeIcons::indicatorArrowDownIcon() const
{
    return d_ptr->indicatorArrowDownIcon();
}

QIcon HolonThemeIcons::indicatorArrowUpIcon() const
{
    return d_ptr->indicatorArrowUpIcon();
}

QIcon HolonThemeIcons::maximizeIcon() const
{
    return d_ptr->maximizeIcon();
}

QIcon HolonThemeIcons::minimizeIcon() const
{
    return d_ptr->minimizeIcon();
}

QIcon HolonThemeIcons::runTaskIcon() const
{
    return d_ptr->runTaskIcon();
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

QIcon HolonThemeIcons::splitButtonHorizontalMenuIcon() const
{
    return d_ptr->splitButtonHorizontalMenuIcon();
}

QIcon HolonThemeIcons::splitButtonVerticalIcon() const
{
    return d_ptr->splitButtonVerticalIcon();
}

QIcon HolonThemeIcons::splitButtonVerticalMenuIcon() const
{
    return d_ptr->splitButtonVerticalMenuIcon();
}

QIcon HolonThemeIcons::stopIcon() const
{
    return d_ptr->stopIcon();
}

QIcon HolonThemeIcons::taskIcon() const
{
    return d_ptr->taskIcon();
}

HolonTheme *HolonThemeIcons::theme() const
{
    return d_ptr->theme;
}
