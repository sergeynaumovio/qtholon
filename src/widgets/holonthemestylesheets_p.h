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
    HolonThemeStyleSheets *const q_ptr;

    QString mainWindow;
    QString pushButton;
    QString taskbar;
    QString titleBar;

public:
    HolonTheme *const theme;

    HolonThemeStyleSheetsPrivate(HolonThemeStyleSheets *q, HolonTheme *t);

    QString mainWindowStyleSheet();
    QString pushButtonStyleSheet();
    QString taskbarStyleSheet();
    QString titleBarStyleSheet();
};

#endif // HOLONTHEMESTYLESHEETS_P_H
