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

QString HolonThemeStyleSheets::pushButtonStyleSheet() const
{
    return d_ptr->pushButtonStyleSheet();
}

HolonTheme *HolonThemeStyleSheets::theme() const
{
    return d_ptr->theme;
}
