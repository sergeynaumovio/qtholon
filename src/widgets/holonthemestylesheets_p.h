// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMESTYLESHEETS_P_H
#define HOLONTHEMESTYLESHEETS_P_H

#include <QColor>

class HolonTheme;
class HolonThemeColors;
class HolonThemeIcons;
class HolonThemeStyleSheets;

class HolonThemeStyleSheetsPrivate
{
    QString button;
    QString titleBar;

public:
    HolonThemeStyleSheets *const q_ptr;
    HolonTheme *const theme;
    HolonThemeColors *colors;
    HolonThemeIcons *icons;

    HolonThemeStyleSheetsPrivate(HolonThemeStyleSheets *q, HolonTheme *t);

    QString buttonStyleSheet();
    QString titleBarStyleSheet();
};

#endif // HOLONTHEMESTYLESHEETS_P_H