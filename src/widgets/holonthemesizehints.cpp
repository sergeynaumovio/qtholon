// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemesizehints.h"
#include "holontheme.h"
#include "holonthemesizehints_p.h"
#include <QLoaderSettings>

HolonThemeSizeHints::HolonThemeSizeHints(QLoaderSettings *settings, HolonTheme *theme)
:   QObject(theme),
    QLoaderSettings(settings),
    d_ptr(new HolonThemeSizeHintsPrivate(this, theme))
{ }

HolonThemeSizeHints::~HolonThemeSizeHints()
{ }

QSize HolonThemeSizeHints::iconsSizeHint() const
{
    return d_ptr->iconsSizeHint();
}

QSize HolonThemeSizeHints::sidebarSwitchButtonSizeHint() const
{
    return d_ptr->sidebarSwitchButtonSizeHint();
}

HolonTheme *HolonThemeSizeHints::theme() const
{
    return d_ptr->theme;
}

QSize HolonThemeSizeHints::taskbarSizeHint() const
{
    return d_ptr->taskbarSizeHint();
}

QSize HolonThemeSizeHints::titleBarSizeHint() const
{
    return d_ptr->titleBarSizeHint();
}
