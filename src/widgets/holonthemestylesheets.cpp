// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemestylesheets.h"
#include "holontheme.h"
#include "holonthemestylesheets_p.h"
#include <QLoaderSettings>

HolonThemeStyleSheets::HolonThemeStyleSheets(QLoaderSettings *settings, HolonTheme *theme)
:   QObject(theme),
    QLoaderSettings(settings),
    d_ptr(new HolonThemeStyleSheetsPrivate(this, theme))
{ }

HolonThemeStyleSheets::~HolonThemeStyleSheets()
{ }

HolonThemeColors *HolonThemeStyleSheets::colors() const
{
    return d_ptr->colors;
}

HolonThemeIcons *HolonThemeStyleSheets::icons() const
{
    return d_ptr->icons;
}

QString HolonThemeStyleSheets::mainWindowStyleSheet() const
{
    return d_ptr->mainWindowStyleSheet();
}

QString HolonThemeStyleSheets::pushButtonStyleSheet() const
{
    return d_ptr->pushButtonStyleSheet();
}

QString HolonThemeStyleSheets::taskbarStyleSheet() const
{
    return d_ptr->taskbarStyleSheet();
}

HolonTheme *HolonThemeStyleSheets::theme() const
{
    return d_ptr->theme;
}

QString HolonThemeStyleSheets::titleBarStyleSheet() const
{
    return d_ptr->titleBarStyleSheet();
}
